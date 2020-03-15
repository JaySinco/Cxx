#include "ui_assistant.h"
#include "common/utility/string_helper.h"
#include <psapi.h>

using namespace cxx;

IUIAutomation* g_pAutomation;

UIAElemPtr make_uia_elem(IUIAutomationElement* pElement)
{
    return std::shared_ptr<IUIAutomationElement>(
        pElement,
        [](IUIAutomationElement* p) {
            if (p != NULL)
                p->Release();
        });
}

UIAInitializer::UIAInitializer()
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

UIAInitializer::~UIAInitializer()
{
    CoUninitialize();
}

std::vector<UIAElemPtr> findElementByProp(
    const std::wstring& value,
    UIAElemPtr root,
    PROPERTYID propId,
    TreeScope scope,
    bool findAll)
{
    IUIAutomationCondition* pCondition = NULL;
    IUIAutomationElementArray* pFound = NULL;
    VARIANT varProp;
    int length;
    std::vector<UIAElemPtr> res;
    if (value.size() <= 0) {
        if (FAILED(g_pAutomation->CreateTrueCondition(&pCondition)))
            goto cleanup;
    } else {
        varProp.vt = VT_BSTR;
        varProp.bstrVal = SysAllocStringLen(value.c_str(), (int)value.size());
        if (varProp.bstrVal == NULL) {
            goto cleanup;
        }
        if (FAILED(g_pAutomation->CreatePropertyCondition(propId, varProp, &pCondition)))
            goto cleanup;
    }
    if (root == nullptr) {
        IUIAutomationElement* pRoot = NULL;
        if (FAILED(g_pAutomation->GetRootElement(&pRoot)))
            goto cleanup;
        root = make_uia_elem(pRoot);
    }
    if (findAll) {
        if (FAILED(root->FindAll(scope, pCondition, &pFound)))
            goto cleanup;
        if (FAILED(pFound->get_Length(&length)))
            goto cleanup;
        for (int i = 0; i < length; ++i) {
            IUIAutomationElement* pElement;
            if (FAILED(pFound->GetElement(i, &pElement)))
                continue;
            res.push_back(make_uia_elem(pElement));
        }
    } else {
        IUIAutomationElement* pElement;
        if (FAILED(root->FindFirst(scope, pCondition, &pElement))) {
            goto cleanup;
        }
        res.push_back(make_uia_elem(pElement));
    }

cleanup:
    if (pCondition != NULL)
        pCondition->Release();
    if (pFound != NULL)
        pFound->Release();
    VariantClear(&varProp);
    return res;
}

ElementMeta GetElementMeta(UIAElemPtr pElement)
{
    int pid;
    BSTR className, name, id, itemType;
    ElementMeta meta;
    pElement->get_CurrentBoundingRectangle(&meta.rect);
    if (!FAILED(pElement->get_CurrentAutomationId(&id)) && id != nullptr) {
        meta.automationId = std::wstring(id, SysStringLen(id));
    }
    if (!FAILED(pElement->get_CurrentClassName(&className)) && className != nullptr) {
        meta.className = std::wstring(className, SysStringLen(className));
    }
    if (!FAILED(pElement->get_CurrentName(&name)) && name != nullptr) {
        meta.name = std::wstring(name, SysStringLen(name));
    }
    if (!FAILED(pElement->get_CurrentItemType(&itemType)) && itemType != nullptr) {
        meta.itemType = std::wstring(itemType, SysStringLen(itemType));
    };
    if (!FAILED(pElement->get_CurrentProcessId(&pid))) {
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
    return meta;
}

std::ostream& operator<<(std::ostream& out, const UIAElemPtr& pElement)
{
    ElementMeta meta = GetElementMeta(pElement);
    out << "[" << encodeAnsi(meta.moduleName) << "] "
        << encodeAnsi(meta.name);
    return out;
}