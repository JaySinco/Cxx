#include "ui_assistant.h"

IUIAutomationElementArray* FindAndCacheListItems(IUIAutomationElement* pList)
{
    if (pList == NULL)
        return NULL;
    IUIAutomationCondition* pCondition = NULL;
    IUIAutomationCacheRequest* pCacheRequest = NULL;
    IUIAutomationElementArray* pFound = NULL;
    HRESULT hr = g_pAutomation->CreateTrueCondition(&pCondition);
    if (FAILED(hr))
        goto cleanup;
    hr = g_pAutomation->CreateCacheRequest(&pCacheRequest);
    if (FAILED(hr))
        goto cleanup;
    hr = pCacheRequest->AddPattern(UIA_SelectionItemPatternId);
    if (FAILED(hr))
        goto cleanup;
    hr = pCacheRequest->AddProperty(UIA_NamePropertyId);
    if (FAILED(hr))
        goto cleanup;
    pList->FindAllBuildCache(TreeScope_Children, pCondition, pCacheRequest, &pFound);
cleanup:
    if (pCondition != NULL)
        pCondition->Release();
    if (pCacheRequest != NULL)
        pCacheRequest->Release();
    return pFound;
}

HRESULT GetCachedListItem(IUIAutomationElement* pItem)
{
    if (pItem == NULL) {
        return E_INVALIDARG;
    }
    IUIAutomationSelectionItemPattern* pSelectionItemPattern;
    HRESULT hr = pItem->GetCachedPatternAs(UIA_SelectionItemPatternId,
        IID_IUIAutomationSelectionItemPattern, (void**)&pSelectionItemPattern);
    if (pSelectionItemPattern != NULL) {
        // ... To do: Do something with the pattern.

        pSelectionItemPattern->Release();
    }
    // Retrieve a cached property.
    BSTR bstrName;
    hr = pItem->get_CachedName(&bstrName);
    if (SUCCEEDED(hr)) {
        // ... To do: Do something with the property.

        // Clean up when done with name.
        SysFreeString(bstrName);
        bstrName = nullptr;
    }
    BOOL isControl;
    // The following returns E_INVALIDARG because the property was not cached.
    // hr = pItem->get_CachedIsControlElement(&isControl);

    // The following is valid because we have a full reference to the object, therefore
    // we can get current properties. If the cache request had been made with
    // AutomationElementMode_None, no current properties would be available from
    // this IUIAutomationElement.
    hr = pItem->get_CurrentIsControlElement(&isControl);
    return hr;
}

int main(int argc, char* argv[])
{
    TRY_BEGIN
    google::InitGoogleLogging(argv[0]);
    gflags::ParseCommandLineFlags(&argc, &argv, true);
    FLAGS_logtostderr = 1;
    FLAGS_minloglevel = 0;
    UIAInitializer uiaInit;
    auto elemList = findElementByProp(L"Chrome_WidgetWin_1");
    for (const auto elem : elemList) {
        LOG(INFO) << elem;
    }
    CATCH_ALL
}