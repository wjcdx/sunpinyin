#include "LangBarMenu.h"

CLangBarMenu::CLangBarMenu(CTextService *pTextService, SunPinyinEngine *pEngine, REFGUID guidLangBar, LPCWSTR description)
	: CLangBarItem(pTextService, pEngine), _sText("")
{
	// initialize TF_LANGBARITEMINFO structure.
    _tfLangBarItemInfo.clsidService = c_clsidTextService;												    // This LangBarItem belongs to this TextService.
    _tfLangBarItemInfo.guidItem = guidLangBar;															        // GUID of this LangBarItem.
    _tfLangBarItemInfo.dwStyle = (TF_LBI_STYLE_BTN_MENU | TF_LBI_STYLE_SHOWNINTRAY);						    // This LangBar is a button type.
    _tfLangBarItemInfo.ulSort = 0;																			    // The position of this LangBar Item is not specified.
    SafeStringCopy(_tfLangBarItemInfo.szDescription, ARRAYSIZE(_tfLangBarItemInfo.szDescription), description);  // Set the description of this LangBar Item.
}


void CLangBarMenu::SetItemState(UINT id, BOOL state)
{
	std::vector<CLangBarMenuItem>::iterator it = _lbmMenuItems.begin();
	std::vector<CLangBarMenuItem>::iterator ite = _lbmMenuItems.end();

	for (; it != ite; it++)
	{
		if (it->GetId() == id)
		{
			it->SetState(state);
		}
	}
}


//+---------------------------------------------------------------------------
//
// InitMenu
//
//----------------------------------------------------------------------------

STDAPI CLangBarMenu::InitMenu(ITfMenu *pMenu)
{
	std::vector<CLangBarMenuItem>::iterator it = _lbmMenuItems.begin();
	std::vector<CLangBarMenuItem>::iterator ite = _lbmMenuItems.end();

	for (; it != ite; it++)
	{
		WCHAR des[512] = { 0 };
		ULONG len = CharToWchar(des, it->GetText().c_str());
		pMenu->AddMenuItem(it->GetIndex(),
		                   0,
						   (HBITMAP)LoadImage(g_hInst, MAKEINTRESOURCE(it->GetBmp()), IMAGE_BITMAP, 16, 16, 0),
		                   NULL,
		                   des,
		                   len,
		                   NULL);
	}

    return S_OK;
}

//+---------------------------------------------------------------------------
//
// OnMenuSelect
//
//----------------------------------------------------------------------------

STDAPI CLangBarMenu::OnMenuSelect(UINT wIndex)
{
	std::vector<CLangBarMenuItem>::iterator it = _lbmMenuItems.begin();
	std::vector<CLangBarMenuItem>::iterator ite = _lbmMenuItems.end();

	for (; it != ite; it++)
	{
		if (it->GetIndex() == wIndex)
		{
			it->Selected();
		}
	}

    return S_OK;
}

//+---------------------------------------------------------------------------
//
// GetIcon
//
//----------------------------------------------------------------------------

STDAPI CLangBarMenu::GetIcon(HICON *phIcon)
{
    *phIcon = (HICON)LoadImage(g_hInst, MAKEINTRESOURCE(_uiIcon), IMAGE_ICON, 16, 16, 0);
    return (*phIcon != NULL) ? S_OK : E_FAIL;
}

//+---------------------------------------------------------------------------
//
// GetText
//
//----------------------------------------------------------------------------

STDAPI CLangBarMenu::GetText(BSTR *pbstrText)
{
	WCHAR text[512] = { 0 };
	CharToWchar(text, _sText.c_str());

    *pbstrText = SysAllocString(text);
    return (*pbstrText == NULL) ? E_OUTOFMEMORY : S_OK;
}


