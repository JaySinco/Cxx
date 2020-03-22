#include "common/utility/base.h"
#include "common/utility/string_helper.h"
#include <iostream>
#include <map>
#include <string>
#include <uiautomation.h>
#include <vector>

#include <Psapi.h>

#define CLEAN_UP_IF_FAILED(x)              \
    if (FAILED(x)) {                       \
        LOG(ERROR) << "failed op: " << #x; \
        goto cleanup;                      \
    }

#define RELEASE_IF_NOT_NULL(x) \
    if (x != NULL)             \
        x->Release();

using namespace cxx;

IUIAutomation* g_pAutomation;

struct UIAInitializer {
    UIAInitializer()
    {
        CoInitializeEx(
            NULL,
            COINIT_MULTITHREADED);

        CoCreateInstance(
            CLSID_CUIAutomation,
            NULL,
            CLSCTX_INPROC_SERVER,
            IID_IUIAutomation, reinterpret_cast<void**>(&g_pAutomation));
    }

    ~UIAInitializer()
    {
        CoUninitialize();
    }
};

std::wstring b2w(BSTR bstr)
{
    if (bstr == nullptr)
        return {};
    return std::wstring(bstr, SysStringLen(bstr));
}

BSTR w2b(const std::wstring& wstr)
{
    if (wstr.empty())
        return nullptr;
    return SysAllocStringLen(wstr.data(), (int)wstr.size());
}

struct ElementMeta {
    std::wstring name;
    RECT rect;
    std::wstring className;
    std::wstring moduleName;
    std::wstring automationId;
    std::wstring itemType;
};

typedef std::shared_ptr<IUIAutomationElement> UIAElemPtr;
typedef std::map<PROPERTYID, std::wstring> PropCondition;

UIAElemPtr makeSmartElemPtr(IUIAutomationElement* pElement)
{
    return std::shared_ptr<IUIAutomationElement>(
        pElement,
        [](IUIAutomationElement* p) {
            RELEASE_IF_NOT_NULL(p);
        });
}

ElementMeta getElementMeta(UIAElemPtr pElement)
{
    int pid;
    BSTR className, name, id, itemType;
    ElementMeta meta;
    if (SUCCEEDED(pElement->get_CachedName(&name))) {
        meta.name = b2w(name);
        SysFreeString(name);
    }
    pElement->get_CachedBoundingRectangle(&meta.rect);
    if (SUCCEEDED(pElement->get_CachedClassName(&className))) {
        meta.className = b2w(className);
        SysFreeString(className);
    }
    if (SUCCEEDED(pElement->get_CachedProcessId(&pid))) {
        HANDLE hProcess = OpenProcess(
            PROCESS_QUERY_INFORMATION | PROCESS_VM_READ,
            FALSE,
            pid);
        if (hProcess) {
            wchar_t buf[1000];
            GetModuleBaseNameW(hProcess, NULL, buf, 1000);
            meta.moduleName = std::wstring(buf);
        }
    }
    if (SUCCEEDED(pElement->get_CachedAutomationId(&id))) {
        meta.automationId = b2w(id);
        SysFreeString(id);
    }
    if (SUCCEEDED(pElement->get_CachedItemType(&itemType))) {
        meta.itemType = b2w(itemType);
        SysFreeString(itemType);
    }
    return meta;
}

std::vector<UIAElemPtr> findElementByProp(
    PropCondition cond,
    UIAElemPtr root = nullptr,
    TreeScope scope = TreeScope_Children,
    bool findAll = true)
{
    IUIAutomationCondition* pCondition = NULL;
    IUIAutomationElementArray* pFound = NULL;
    IUIAutomationCacheRequest* pCacheRequest = NULL;
    int length;
    std::vector<UIAElemPtr> res;
    if (cond.size() <= 0) {
        CLEAN_UP_IF_FAILED(g_pAutomation->CreateTrueCondition(&pCondition));
    } else {
        VARIANT varProp;
        VariantInit(&varProp);
        std::vector<IUIAutomationCondition*> conditionArray;
        for (const auto& it : cond) {
            varProp.vt = VT_BSTR;
            varProp.bstrVal = w2b(it.second);
            if (varProp.bstrVal == NULL) {
                continue;
            }
            CLEAN_UP_IF_FAILED(g_pAutomation->CreatePropertyCondition(
                it.first, varProp, &pCondition));
            conditionArray.push_back(pCondition);
            VariantClear(&varProp);
        }
        if (conditionArray.size() == 0) {
            CLEAN_UP_IF_FAILED(g_pAutomation->CreateTrueCondition(&pCondition));
        } else if (conditionArray.size() == 1) {
            // skip create condition from array!
        } else {
            CLEAN_UP_IF_FAILED(g_pAutomation->CreateAndConditionFromNativeArray(
                conditionArray.data(), (int)conditionArray.size(), &pCondition));
        }
    }
    if (root == nullptr) {
        IUIAutomationElement* pRoot = NULL;
        CLEAN_UP_IF_FAILED(g_pAutomation->GetRootElement(&pRoot));
        root = makeSmartElemPtr(pRoot);
    }
    CLEAN_UP_IF_FAILED(g_pAutomation->CreateCacheRequest(&pCacheRequest));
    for (auto prop : {
             UIA_NamePropertyId,
             UIA_BoundingRectanglePropertyId,
             UIA_ClassNamePropertyId,
             UIA_ProcessIdPropertyId,
             UIA_AutomationIdPropertyId,
             UIA_ItemTypePropertyId,
         }) {
        CLEAN_UP_IF_FAILED(pCacheRequest->AddProperty(prop));
    }
    if (findAll) {
        CLEAN_UP_IF_FAILED(root->FindAllBuildCache(scope, pCondition, pCacheRequest, &pFound));
        CLEAN_UP_IF_FAILED(pFound->get_Length(&length));
        for (int i = 0; i < length; ++i) {
            IUIAutomationElement* pElement;
            if (FAILED(pFound->GetElement(i, &pElement)))
                continue;
            res.push_back(makeSmartElemPtr(pElement));
        }
    } else {
        IUIAutomationElement* pElement;
        CLEAN_UP_IF_FAILED(root->FindFirstBuildCache(scope, pCondition, pCacheRequest, &pElement));
        res.push_back(makeSmartElemPtr(pElement));
    }

cleanup:
    RELEASE_IF_NOT_NULL(pCondition);
    RELEASE_IF_NOT_NULL(pFound);
    RELEASE_IF_NOT_NULL(pCacheRequest);
    return res;
}

std::ostream& operator<<(std::ostream& out, const UIAElemPtr& pElement)
{
    ElementMeta meta = getElementMeta(pElement);
    out << "NAME\t\t\"" << encodeAnsi(meta.name) << "\"" << std::endl
        << "POS\t\t(" << meta.rect.left << ", " << meta.rect.top << ")" << std::endl
        << "SIZE\t\t(" << (meta.rect.right - meta.rect.left)
        << ", " << (meta.rect.bottom - meta.rect.top) << ")" << std::endl
        << "CLASS\t\t\"" << encodeAnsi(meta.className) << "\"" << std::endl
        << "MODULE\t\t\"" << encodeAnsi(meta.moduleName) << "\"" << std::endl
        << "ID\t\t\"" << encodeAnsi(meta.automationId) << "\"" << std::endl
        << "ITEM\t\t\"" << encodeAnsi(meta.itemType) << "\"" << std::endl;
    return out;
}

int main(int argc, char* argv[])
{
    TRY_BEGIN
    google::InitGoogleLogging(argv[0]);
    gflags::ParseCommandLineFlags(&argc, &argv, true);
    FLAGS_logtostderr = 1;
    FLAGS_minloglevel = 0;
    UIAInitializer uiaInit;
    auto elemList = findElementByProp({
        { UIA_NamePropertyId, L"Program Manager" },
        { UIA_ClassNamePropertyId, L"Progman" },
    });
    for (const auto elem : elemList) {
        std::cout << elem << std::endl;
    }
    CATCH_ALL
}