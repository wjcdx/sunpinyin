#include "LangBarButton.h"
#include "../sunpinyin_engine.h"

CLangBarButton::CLangBarButton(CTextService *pTextService, SunPinyinEngine *pEngine,
							   REFGUID guidLangBar, LPCWSTR description)
		: CLangBarItem(pTextService, pEngine), _bState(true)
{
	// initialize TF_LANGBARITEMINFO structure.
    _tfLangBarItemInfo.clsidService = c_clsidTextService;												    // This LangBarItem belongs to this TextService.
    _tfLangBarItemInfo.guidItem = guidLangBar;															        // GUID of this LangBarItem.
    _tfLangBarItemInfo.dwStyle = (TF_LBI_STYLE_BTN_BUTTON | TF_LBI_STYLE_SHOWNINTRAY);						    // This LangBar is a button type.
    _tfLangBarItemInfo.ulSort = 0;																			    // The position of this LangBar Item is not specified.
    SafeStringCopy(_tfLangBarItemInfo.szDescription, ARRAYSIZE(_tfLangBarItemInfo.szDescription), description);  // Set the description of this LangBar Item.
}

void CLangBarButton::SetState(BOOL state)
{
	if (_bState != state && _pLangBarItemSink) 
    {
		_bState = state;
		_pLangBarItemSink->OnUpdate(TF_LBI_TOOLTIP | TF_LBI_ICON | TF_LBI_TEXT);
    }
}

bool CLangBarButton::Update()
{
	if (_pEngine)
	{
		return _pEngine->update_prop_status(_uiID, _bState);
	}
	return false;
}

//+---------------------------------------------------------------------------
//
// GetStatus
//
//----------------------------------------------------------------------------

STDAPI CLangBarButton::GetStatus(DWORD *pdwStatus)
{
    *pdwStatus = 0;
    return S_OK;
}

//+---------------------------------------------------------------------------
//
// Show
//
//----------------------------------------------------------------------------

STDAPI CLangBarButton::Show(BOOL fShow)
{
    fShow;
    if (_pLangBarItemSink)
    {
        _pLangBarItemSink->OnUpdate(TF_LBI_STATUS);
    }
    return S_OK;
}

//+---------------------------------------------------------------------------
//
// GetTooltipString
//
//----------------------------------------------------------------------------

STDAPI CLangBarButton::GetTooltipString(BSTR *pbstrToolTip)
{
	WCHAR des[512] = { 0 };
	if (_bState)
	{
		CharToWchar(des, _lbiTrueInfo.getTooltip().c_str());
	}
	else
	{
		CharToWchar(des, _lbiFalseInfo.getTooltip().c_str());
	}
    *pbstrToolTip = SysAllocString(des);

    return (*pbstrToolTip == NULL) ? E_OUTOFMEMORY : S_OK;
}

//+---------------------------------------------------------------------------
//
// OnClick
//
//----------------------------------------------------------------------------

STDAPI CLangBarButton::OnClick(TfLBIClick click, POINT pt, const RECT *prcArea)
{
	SetState(!_bState);
	Update();
    return S_OK;
}


//+---------------------------------------------------------------------------
//
// GetIcon
//
//----------------------------------------------------------------------------

STDAPI CLangBarButton::GetIcon(HICON *phIcon)
{
	int icon = IDI_ENG;
	if (_bState)
	{
		icon = _lbiTrueInfo.getIcon();
	}
	else
	{
		icon = _lbiFalseInfo.getIcon();
	}
    //*phIcon = (HICON)LoadImage(g_hInst, icon, IMAGE_ICON, 16, 16, 0);
	*phIcon = (HICON)LoadImage(g_hInst, MAKEINTRESOURCE(icon), IMAGE_ICON, 16, 16, 0);
	//*phIcon = (HICON)LoadImage(g_hInst, TEXT("IDI_TEXTSERVICE"), IMAGE_ICON, 16, 16, 0);
    return (*phIcon != NULL) ? S_OK : E_FAIL;
}

//+---------------------------------------------------------------------------
//
// GetText
//
//----------------------------------------------------------------------------

STDAPI CLangBarButton::GetText(BSTR *pbstrText)
{
	WCHAR text[512] = { 0 };
	if (_bState)
	{
		CharToWchar(text, _lbiTrueInfo.getText().c_str());
	}
	else
	{
		CharToWchar(text, _lbiFalseInfo.getText().c_str());
	}

    *pbstrText = SysAllocString(text);
    return (*pbstrText == NULL) ? E_OUTOFMEMORY : S_OK;
}

