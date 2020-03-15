#include "common/utility/logging.h"
#include <string>
#include <uiautomation.h>
#include <vector>

extern IUIAutomation* g_pAutomation;

using UIAElemPtr = std::shared_ptr<IUIAutomationElement>;

struct UIAInitializer {
    UIAInitializer();
    ~UIAInitializer();
};

struct ElementMeta {
    RECT rect;
    std::wstring automationId;
    std::wstring name;
    std::wstring className;
    std::wstring itemType;
    std::wstring moduleName;
};

UIAElemPtr make_uia_elem(IUIAutomationElement* pElement);
ElementMeta GetElementMeta(UIAElemPtr pElement);

std::vector<UIAElemPtr> findElementByProp(
    const std::wstring& value,
    UIAElemPtr root = nullptr,
    PROPERTYID propId = UIA_ClassNamePropertyId,
    TreeScope scope = TreeScope_Children,
    bool findAll = true);

std::ostream& operator<<(std::ostream& out, const UIAElemPtr& pElement);
