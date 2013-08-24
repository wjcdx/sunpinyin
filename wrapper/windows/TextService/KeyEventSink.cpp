//////////////////////////////////////////////////////////////////////
//
//  THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
//  ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED
//  TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
//  PARTICULAR PURPOSE.
//
//  Copyright (C) 2003  Microsoft Corporation.  All rights reserved.
//
//  KeyEventSink.cpp
//
//          ITfKeyEventSink implementation.
//
//////////////////////////////////////////////////////////////////////

#include "Globals.h"
#include "TextService.h"
#include "CandidateList.h"

//
// GUID for the preserved keys.
//
/* 6a0bde41-6adf-11d7-a6ea-00065b84435c */
static const GUID GUID_PRESERVEDKEY_ONOFF = { 
    0x6a0bde41,
    0x6adf,
    0x11d7,
    {0xa6, 0xea, 0x00, 0x06, 0x5b, 0x84, 0x43, 0x5c}
  };

/* 6a0bde42-6adf-11d7-a6ea-00065b84435c */
static const GUID GUID_PRESERVEDKEY_F6 = { 
    0x6a0bde42,
    0x6adf,
    0x11d7,
    {0xa6, 0xea, 0x00, 0x06, 0x5b, 0x84, 0x43, 0x5c}
  };


//
// the preserved keys declaration
//
// VK_KANJI is the virtual key for Kanji key, which is available in 106
// Japanese keyboard.
//
static const TF_PRESERVEDKEY c_pkeyOnOff0 = { 0xC0, TF_MOD_ALT };
static const TF_PRESERVEDKEY c_pkeyOnOff1 = { VK_KANJI, TF_MOD_IGNORE_ALL_MODIFIER };
static const TF_PRESERVEDKEY c_pkeyF6 =   { VK_F6, TF_MOD_ON_KEYUP };

//
// the description for the preserved keys
//
static const WCHAR c_szPKeyOnOff[] = L"OnOff";
static const WCHAR c_szPKeyF6[]    = L"Function 6";

//+---------------------------------------------------------------------------
//
// _IsKeyEaten
//
//----------------------------------------------------------------------------

BOOL CTextService::_IsKeyEaten(ITfContext *pContext, WPARAM wParam)
{
    // if the keyboard is disabled, keys are not consumed.
    if (_IsKeyboardDisabled())
        return FALSE;

    // if the keyboard is closed, keys are not consumed.
    if (!_IsKeyboardOpen())
        return FALSE;

	wParam &= 0xff;
    switch (wParam)
    {
	case VK_HOME:
	case VK_END:
	case VK_LEFT:
    case VK_RIGHT:
	case VK_UP:
	case VK_DOWN:
	case VK_PRIOR:
	case VK_NEXT:
	case VK_DELETE:
	case VK_BACK:
    case VK_RETURN:
	case VK_ESCAPE:
	case VK_LSHIFT:
	case VK_RSHIFT:
	case VK_LCONTROL:
	case VK_RCONTROL:
	case VK_LMENU:
	case VK_RMENU:
	case VK_SPACE:
		if (!_IsComposing())
			return FALSE;
	}

	return TRUE;
}

BOOL CTextService::PrepareKeyEvent(CKeyEvent &oEvent, WPARAM wParam, LPARAM lParam)
{
 	WCHAR wch = ConvertVKey((UINT)wParam);

	wParam &= 0xff;
    switch (wParam)
    {
	case VK_HOME:
		oEvent.code = IM_VK_HOME;
		break;
	case VK_END:
		oEvent.code = IM_VK_END;
		break;
	case VK_LEFT:
		oEvent.code = IM_VK_LEFT;
		break;
    case VK_RIGHT:
		oEvent.code = IM_VK_RIGHT;
		break;
	case VK_UP:
		oEvent.code = IM_VK_UP;
		break;
	case VK_DOWN:
		oEvent.code = IM_VK_DOWN;
		break;
	case VK_PRIOR:
		oEvent.code = IM_VK_PAGE_UP;
		break;
	case VK_NEXT:
		oEvent.code = IM_VK_PAGE_DOWN;
		break;
	case VK_DELETE:
		oEvent.code = IM_VK_DELETE;
		break;
	case VK_BACK:
		oEvent.code = IM_VK_BACK_SPACE;
		break;
    case VK_RETURN:
        oEvent.code = IM_VK_ENTER;
		break;
	case VK_ESCAPE:
		oEvent.code = IM_VK_ESCAPE;
		break;
	case VK_LSHIFT:
		oEvent.code = IM_VK_SHIFT_L;
		break;
	case VK_RSHIFT:
		oEvent.code = IM_VK_SHIFT_R;
		break;
	case VK_LCONTROL:
		oEvent.code = IM_VK_CONTROL_L;
		break;
	case VK_RCONTROL:
		oEvent.code = IM_VK_CONTROL_R;
		break;
	case VK_LMENU:
	case VK_RMENU:
		oEvent.code = IM_VK_ALT;
		break;
	case VK_SPACE:
		oEvent.code = IM_VK_SPACE;

    default:
		oEvent.code = wch;
        break;
	}
	oEvent.value = oEvent.code;

	// high-order bit : key down
    // low-order bit  : toggled
    SHORT sksMenu = GetKeyState(VK_MENU);
    SHORT sksCtrl = GetKeyState(VK_CONTROL);
    SHORT sksShft = GetKeyState(VK_SHIFT);

	if (sksMenu & 0x8000) oEvent.modifiers |= IM_ALT_MASK;
	if (sksCtrl & 0x8000) oEvent.modifiers |= IM_CTRL_MASK;
	if (sksShft & 0x8000) oEvent.modifiers |= IM_SHIFT_MASK;

	if (wch > 0x20 && wch < 0x7f // isprint(key_val) && !isspace(key_val)
		&& !(oEvent.modifiers & IM_CTRL_MASK)) {
		// we only care about key_val here
		oEvent.value = oEvent.code;
    } else {
        // what matters is key_code
		oEvent.value = 0;
    }

    return TRUE;
}


//+---------------------------------------------------------------------------
//
// OnSetFocus
//
// Called by the system whenever this service gets the keystroke device focus.
//----------------------------------------------------------------------------

STDAPI CTextService::OnSetFocus(BOOL fForeground)
{
    return S_OK;
}

//+---------------------------------------------------------------------------
//
// OnTestKeyDown
//
// Called by the system to query this service wants a potential keystroke.
//----------------------------------------------------------------------------

STDAPI CTextService::OnTestKeyDown(ITfContext *pContext, WPARAM wParam, LPARAM lParam, BOOL *pfEaten)
{
    *pfEaten = _IsKeyEaten(pContext, wParam);
    return S_OK;
}

//+---------------------------------------------------------------------------
//
// OnKeyDown
//
// Called by the system to offer this service a keystroke.  If *pfEaten == TRUE
// on exit, the application will not handle the keystroke.
//----------------------------------------------------------------------------

STDAPI CTextService::OnKeyDown(ITfContext *pContext, WPARAM wParam, LPARAM lParam, BOOL *pfEaten)
{
    _bKeyEaten = _IsKeyEaten(pContext, wParam);

    if (_bKeyEaten)
    {
		CKeyEvent event(0, 0, 0);
		PrepareKeyEvent(event, wParam, lParam);
		// let the handler judge if it's eaten.
        _InvokeKeyHandler(pContext, event);
    }
	*pfEaten = _bKeyEaten;
    return S_OK;
}

//+---------------------------------------------------------------------------
//
// OnTestKeyUp
//
// Called by the system to query this service wants a potential keystroke.
//----------------------------------------------------------------------------

STDAPI CTextService::OnTestKeyUp(ITfContext *pContext, WPARAM wParam, LPARAM lParam, BOOL *pfEaten)
{
    *pfEaten = _IsKeyEaten(pContext, wParam);
    return S_OK;
}

//+---------------------------------------------------------------------------
//
// OnKeyUp
//
// Called by the system to offer this service a keystroke.  If *pfEaten == TRUE
// on exit, the application will not handle the keystroke.
//----------------------------------------------------------------------------

STDAPI CTextService::OnKeyUp(ITfContext *pContext, WPARAM wParam, LPARAM lParam, BOOL *pfEaten)
{
    *pfEaten = _IsKeyEaten(pContext, wParam);
    return S_OK;
}

//+---------------------------------------------------------------------------
//
// OnPreservedKey
//
// Called when a hotkey (registered by us, or by the system) is typed.
//----------------------------------------------------------------------------

STDAPI CTextService::OnPreservedKey(ITfContext *pContext, REFGUID rguid, BOOL *pfEaten)
{

    if (IsEqualGUID(rguid, GUID_PRESERVEDKEY_ONOFF))
    {
        BOOL fOpen = _IsKeyboardOpen();
        _SetKeyboardOpen(fOpen ? FALSE : TRUE);
        *pfEaten = TRUE;
    }
    else
    {
        *pfEaten = FALSE;
    }

    return S_OK;
}

//+---------------------------------------------------------------------------
//
// _InitKeyEventSink
//
// Advise a keystroke sink.
//----------------------------------------------------------------------------

BOOL CTextService::_InitKeyEventSink()
{
    ITfKeystrokeMgr *pKeystrokeMgr;
    HRESULT hr;

    if (_pThreadMgr->QueryInterface(IID_ITfKeystrokeMgr, (void **)&pKeystrokeMgr) != S_OK)
        return FALSE;

    hr = pKeystrokeMgr->AdviseKeyEventSink(_tfClientId, (ITfKeyEventSink *)this, TRUE);

    pKeystrokeMgr->Release();

    return (hr == S_OK);
}

//+---------------------------------------------------------------------------
//
// _UninitKeyEventSink
//
// Unadvise a keystroke sink.  Assumes a sink has been advised already.
//----------------------------------------------------------------------------

void CTextService::_UninitKeyEventSink()
{
    ITfKeystrokeMgr *pKeystrokeMgr;

    if (_pThreadMgr->QueryInterface(IID_ITfKeystrokeMgr, (void **)&pKeystrokeMgr) != S_OK)
        return;

    pKeystrokeMgr->UnadviseKeyEventSink(_tfClientId);

    pKeystrokeMgr->Release();
}

//+---------------------------------------------------------------------------
//
// _InitPreservedKey
//
// Register a hot key.
//----------------------------------------------------------------------------

BOOL CTextService::_InitPreservedKey()
{
    ITfKeystrokeMgr *pKeystrokeMgr;
    HRESULT hr;

    if (_pThreadMgr->QueryInterface(IID_ITfKeystrokeMgr, (void **)&pKeystrokeMgr) != S_OK)
        return FALSE;

    // register Alt+~ key
    hr = pKeystrokeMgr->PreserveKey(_tfClientId, 
                                    GUID_PRESERVEDKEY_ONOFF,
                                    &c_pkeyOnOff0,
                                    c_szPKeyOnOff,
                                    wcslen(c_szPKeyOnOff));

    // register KANJI key
    hr = pKeystrokeMgr->PreserveKey(_tfClientId, 
                                    GUID_PRESERVEDKEY_ONOFF,
                                    &c_pkeyOnOff1,
                                    c_szPKeyOnOff,
                                    wcslen(c_szPKeyOnOff));

    // register F6 key
    hr = pKeystrokeMgr->PreserveKey(_tfClientId, 
                                    GUID_PRESERVEDKEY_F6,
                                    &c_pkeyF6,
                                    c_szPKeyF6,
                                    wcslen(c_szPKeyF6));

    pKeystrokeMgr->Release();

    return (hr == S_OK);
}

//+---------------------------------------------------------------------------
//
// _UninitPreservedKey
//
// Uninit a hot key.
//----------------------------------------------------------------------------

void CTextService::_UninitPreservedKey()
{
    ITfKeystrokeMgr *pKeystrokeMgr;

    if (_pThreadMgr->QueryInterface(IID_ITfKeystrokeMgr, (void **)&pKeystrokeMgr) != S_OK)
        return;

    pKeystrokeMgr->UnpreserveKey(GUID_PRESERVEDKEY_ONOFF, &c_pkeyOnOff0);
    pKeystrokeMgr->UnpreserveKey(GUID_PRESERVEDKEY_ONOFF, &c_pkeyOnOff1);
    pKeystrokeMgr->UnpreserveKey(GUID_PRESERVEDKEY_F6,    &c_pkeyF6);

    pKeystrokeMgr->Release();
}

