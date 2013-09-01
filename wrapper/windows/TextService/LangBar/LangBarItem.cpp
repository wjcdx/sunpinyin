#include "LangBarItem.h"
#include "../TextService.h"

//
// The cookie for the sink to CLangBarItemButton.
//
#define TEXTSERVICE_LANGBARITEMSINK_COOKIE 0x0fab0fab

//
// The ids of the menu item of the language bar button.
//
#define MENUITEM_INDEX_0 0
#define MENUITEM_INDEX_1 1
#define MENUITEM_INDEX_OPENCLOSE 2

//
// The descriptions of the menu item of the language bar button.
//
static WCHAR c_szMenuItemDescription0[] = L"Menu Item Description 0";
static WCHAR c_szMenuItemDescription1[] = L"Menu Item Description 1";
static WCHAR c_szMenuItemDescriptionOpenClose[] = L"Open";

//+---------------------------------------------------------------------------
//
// ctor
//
//----------------------------------------------------------------------------

CLangBarItem::CLangBarItem(CTextService *pTextService, SunPinyinEngine *pEngine)
{
    DllAddRef();

    // Initialize the sink pointer to NULL.
    _pLangBarItemSink = NULL;

    _pTextService = pTextService;
    _pTextService->AddRef();

	_pEngine = pEngine;

    _cRef = 1;
}

//+---------------------------------------------------------------------------
//
// dtor
//
//----------------------------------------------------------------------------

CLangBarItem::~CLangBarItem()
{
    DllRelease();
    _pTextService->Release();
}

//+---------------------------------------------------------------------------
//
// QueryInterface
//
//----------------------------------------------------------------------------

STDAPI CLangBarItem::QueryInterface(REFIID riid, void **ppvObj)
{
    if (ppvObj == NULL)
        return E_INVALIDARG;

    *ppvObj = NULL;

    if (IsEqualIID(riid, IID_IUnknown) ||
        IsEqualIID(riid, IID_ITfLangBarItem) ||
        IsEqualIID(riid, IID_ITfLangBarItemButton))
    {
        *ppvObj = (ITfLangBarItemButton *)this;
    }
    else if (IsEqualIID(riid, IID_ITfSource))
    {
        *ppvObj = (ITfSource *)this;
    }

    if (*ppvObj)
    {
        AddRef();
        return S_OK;
    }

    return E_NOINTERFACE;
}


//+---------------------------------------------------------------------------
//
// AddRef
//
//----------------------------------------------------------------------------

STDAPI_(ULONG) CLangBarItem::AddRef()
{
    return ++_cRef;
}

//+---------------------------------------------------------------------------
//
// Release
//
//----------------------------------------------------------------------------

STDAPI_(ULONG) CLangBarItem::Release()
{
    LONG cr = --_cRef;

    assert(_cRef >= 0);

    if (_cRef == 0)
    {
        delete this;
    }

    return cr;
}

//+---------------------------------------------------------------------------
//
// GetInfo
//
//----------------------------------------------------------------------------

STDAPI CLangBarItem::GetInfo(TF_LANGBARITEMINFO *pInfo)
{
	_tfLangBarItemInfo.dwStyle |= TF_LBI_STYLE_SHOWNINTRAY;
    *pInfo = _tfLangBarItemInfo;
    return S_OK;
}

//+---------------------------------------------------------------------------
//
// GetStatus
//
//----------------------------------------------------------------------------

STDAPI CLangBarItem::GetStatus(DWORD *pdwStatus)
{
    *pdwStatus = 0;
    return S_OK;
}

//+---------------------------------------------------------------------------
//
// Show
//
//----------------------------------------------------------------------------

STDAPI CLangBarItem::Show(BOOL fShow)
{
    return E_NOTIMPL;
}

//+---------------------------------------------------------------------------
//
// GetTooltipString
//
//----------------------------------------------------------------------------

STDAPI CLangBarItem::GetTooltipString(BSTR *pbstrToolTip)
{
    return E_NOTIMPL;
}

//+---------------------------------------------------------------------------
//
// OnClick
//
//----------------------------------------------------------------------------

STDAPI CLangBarItem::OnClick(TfLBIClick click, POINT pt, const RECT *prcArea)
{
    return S_OK;
}

//+---------------------------------------------------------------------------
//
// InitMenu
//
//----------------------------------------------------------------------------

STDAPI CLangBarItem::InitMenu(ITfMenu *pMenu)
{
    // 
    // Add the fisrt menu item.
    // 
    //pMenu->AddMenuItem(MENUITEM_INDEX_0,
    //                   0, 
    //                   NULL, 
    //                   NULL, 
    //                   c_szMenuItemDescription0, 
    //                   (ULONG)wcslen(c_szMenuItemDescription0), 
    //                   NULL);

    //// 
    //// Add the second menu item.
    //// 
    //pMenu->AddMenuItem(MENUITEM_INDEX_1,
    //                   0, 
    //                   NULL, 
    //                   NULL, 
    //                   c_szMenuItemDescription1, 
    //                   (ULONG)wcslen(c_szMenuItemDescription1), 
    //                   NULL);

    //// 
    //// Add the keyboard open close item.
    //// 
    //DWORD dwFlags = 0;
    //if (_pTextService->_IsKeyboardDisabled())
    //    dwFlags |= TF_LBMENUF_GRAYED;
    //else if (_pTextService->_IsKeyboardOpen())
    //    dwFlags |= TF_LBMENUF_CHECKED;

    //pMenu->AddMenuItem(MENUITEM_INDEX_OPENCLOSE,
    //                   dwFlags, 
    //                   NULL, 
    //                   NULL, 
    //                   c_szMenuItemDescriptionOpenClose, 
    //                   (ULONG)wcslen(c_szMenuItemDescriptionOpenClose), 
    //                   NULL);

    return S_OK;
}

//+---------------------------------------------------------------------------
//
// OnMenuSelect
//
//----------------------------------------------------------------------------

STDAPI CLangBarItem::OnMenuSelect(UINT wID)
{
    BOOL fOpen;

    //
    // This is callback when the menu item is selected.
    //
    /*switch (wID)
    {
        case MENUITEM_INDEX_0:
            break;

        case MENUITEM_INDEX_1:
            break;

        case MENUITEM_INDEX_OPENCLOSE:
            fOpen = _pTextService->_IsKeyboardOpen();
            _pTextService->_SetKeyboardOpen(fOpen ? FALSE : TRUE);
            break;
    }*/

    return S_OK;
}

//+---------------------------------------------------------------------------
//
// GetIcon
//
//----------------------------------------------------------------------------

STDAPI CLangBarItem::GetIcon(HICON *phIcon)
{
    return E_NOTIMPL;
}

//+---------------------------------------------------------------------------
//
// GetText
//
//----------------------------------------------------------------------------

STDAPI CLangBarItem::GetText(BSTR *pbstrText)
{
    return E_NOTIMPL;
}

//+---------------------------------------------------------------------------
//
// AdviseSink
//
//----------------------------------------------------------------------------

STDAPI CLangBarItem::AdviseSink(REFIID riid, IUnknown *punk, DWORD *pdwCookie)
{
    //
    // We allow only ITfLangBarItemSink interface.
    //
    if (!IsEqualIID(IID_ITfLangBarItemSink, riid))
        return CONNECT_E_CANNOTCONNECT;

    //
    // We support only one sink.
    //
    if (_pLangBarItemSink != NULL)
        return CONNECT_E_ADVISELIMIT;

    //
    // Query the ITfLangBarItemSink interface and store it into _pLangBarItemSink.
    //
    if (punk->QueryInterface(IID_ITfLangBarItemSink, (void **)&_pLangBarItemSink) != S_OK)
    {
        _pLangBarItemSink = NULL;
        return E_NOINTERFACE;
    }

    //
    // return our cookie.
    //
    *pdwCookie = TEXTSERVICE_LANGBARITEMSINK_COOKIE;
    return S_OK;
}

//+---------------------------------------------------------------------------
//
// UnadviseSink
//
//----------------------------------------------------------------------------

STDAPI CLangBarItem::UnadviseSink(DWORD dwCookie)
{
    // 
    // Check the given cookie.
    // 
    if (dwCookie != TEXTSERVICE_LANGBARITEMSINK_COOKIE)
        return CONNECT_E_NOCONNECTION;

    //
    // If there is nno connected sink, the method fails.
    //
    if (_pLangBarItemSink == NULL)
        return CONNECT_E_NOCONNECTION;

    _pLangBarItemSink->Release();
    _pLangBarItemSink = NULL;

    return S_OK;
}


//+---------------------------------------------------------------------------
//
// _InitLanguageBar
//
//----------------------------------------------------------------------------

BOOL CTextService::_InitLanguageBar()
{
    ITfLangBarItemMgr *pLangBarItemMgr;
    BOOL fRet;

    if (_pThreadMgr->QueryInterface(IID_ITfLangBarItemMgr, (void **)&pLangBarItemMgr) != S_OK)
        return FALSE;

    fRet = FALSE;

    if (_pEngine)
	{
		fRet = _pEngine->init_language_bar(pLangBarItemMgr);
	}

Exit:
    pLangBarItemMgr->Release();
    return fRet;
}

//+---------------------------------------------------------------------------
//
// _UninitLanguageBar
//
//----------------------------------------------------------------------------

void CTextService::_UninitLanguageBar()
{
    ITfLangBarItemMgr *pLangBarItemMgr;

    if (_pThreadMgr->QueryInterface(IID_ITfLangBarItemMgr, (void **)&pLangBarItemMgr) == S_OK)
    {
        if (_pEngine)
		{
			_pEngine->uninit_language_bar(pLangBarItemMgr);
		}
    }
}

