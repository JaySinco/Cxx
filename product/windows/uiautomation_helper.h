#include "common/utility/logging.h"
#include <map>
#include <string>
#include <uiautomation.h>
#include <vector>
#define CLEAN_UP_IF_FAILED(x)              \
    if (FAILED(x)) {                       \
        LOG(ERROR) << "failed op: " << #x; \
        goto cleanup;                      \
    }

#define RELEASE_IF_NOT_NULL(x) \
    if (x != NULL)             \
        x->Release();

extern IUIAutomation* g_pAutomation;

struct UIAInitializer {
    UIAInitializer();
    ~UIAInitializer();
};

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
UIAElemPtr make_uia_elem(IUIAutomationElement* pElement);
ElementMeta GetElementMeta(UIAElemPtr pElement);

std::vector<UIAElemPtr> findElementByProp(
    PropCondition cond,
    UIAElemPtr root = nullptr,
    TreeScope scope = TreeScope_Children,
    bool findAll = true);

std::ostream& operator<<(std::ostream& out, const UIAElemPtr& pElement);

inline std::wstring b2w(BSTR bstr)
{
    if (bstr == nullptr)
        return {};
    return std::wstring(bstr, SysStringLen(bstr));
}

inline BSTR w2b(const std::wstring& wstr)
{
    if (wstr.empty())
        return nullptr;
    return SysAllocStringLen(wstr.data(), (int)wstr.size());
}