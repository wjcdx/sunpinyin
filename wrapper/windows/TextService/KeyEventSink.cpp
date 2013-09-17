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
static const GUID GUID_PRESERVEDKEY_STATUS_SWITCH = { 
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
static const TF_PRESERVEDKEY c_pkeyStatusSwitch =   { VK_SHIFT, TF_MOD_ON_KEYUP };

//
// the description for the preserved keys
//
static const WCHAR c_szPKeyOnOff[] = L"OnOff";
static const WCHAR c_szPKeyStatusSwitch[]    = L"Function 6";

//+---------------------------------------------------------------------------
//
// _IsKeyEaten
//
//----------------------------------------------------------------------------

BOOL CTextService::_IsKeyEaten(ITfContext *pContext, WPARAM wParam, CKeyEvent &event)
{
    // if the keyboard is disabled, keys are not consumed.
    if (_IsKeyboardDisabled()) {
		MessageBox(NULL, "KeyboardDisabled!", NULL, MB_OK);
        return FALSE;
	}

    // if the keyboard is closed, keys are not consumed.
    if (!_IsKeyboardOpen()) {
		MessageBox(NULL, "KeyboardClosed!", NULL, MB_OK);
        return FALSE;
	}

	// TODO: use Preserved Keys to enable some function keys.
	// we block all CTRL events to enable CTRL+A/X/C/V and other edit functions.
	if (event.modifiers & IM_CTRL_MASK)
		return FALSE;

	//if (!_pEngine->is_chinese_mode() && !_pEngine->is_mode_switch_key(event))
	//	return FALSE;

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
	case VK_SPACE:
		if (!_IsComposing())
			return FALSE;
	}

	return TRUE;
}

BOOL CTextService::PrepareKeyEvent(CKeyEvent &oEvent, WPARAM wParam)
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
	case VK_SPACE:
		oEvent.code = IM_VK_SPACE;
		break;
	case VK_MENU:
	case VK_SHIFT:
	case VK_CONTROL:
    default:
		oEvent.code = wch;
        break;
	}

	if (Global::ModifiersValue & TF_MOD_ALLALT) {
		oEvent.modifiers |= IM_ALT_MASK;
	}

	if (Global::ModifiersValue & TF_MOD_ALLCONTROL) {
		oEvent.modifiers |= IM_CTRL_MASK;
	}

	if (Global::ModifiersValue & TF_MOD_ALLSHIFT) {
		oEvent.modifiers |= IM_SHIFT_MASK;
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

static bool IsKeyDownHandled(CKeyEvent &event)
{
	if (event.modifiers) {
		// handle SHIFT+A
		if (event.modifiers & IM_SHIFT_MASK 
			&& event.code != 0) {
			return TRUE;
		}
		return FALSE;
	}
	return TRUE;
}

static bool IsKeyDownEaten(CKeyEvent &event)
{
	if (event.modifiers) {
		// handle SHIFT+A
		if (event.modifiers & IM_SHIFT_MASK 
			&& event.code != 0) {
			return TRUE;
		}
		return FALSE;
	}
	return TRUE;
}

static bool IsKeyUpHandled(CKeyEvent &event)
{
	return FALSE;
}

static bool IsKeyUpEaten(CKeyEvent &event)
{
	if (event.modifiers) {
		return FALSE;
	}
	return TRUE;
}

//+---------------------------------------------------------------------------
//
// OnTestKeyDown
//
// Called by the system to query this service wants a potential keystroke.
//----------------------------------------------------------------------------

STDAPI CTextService::OnTestKeyDown(ITfContext *pContext, WPARAM wParam, LPARAM lParam, BOOL *pfEaten)
{
	Global::UpdateModifiers(wParam, lParam);

	CKeyEvent event(0, 0, 0);
	PrepareKeyEvent(event, wParam);
    _bKeyEaten = _IsKeyEaten(pContext, wParam, event);
	
	if (_bKeyEaten) {
		_bKeyEaten = IsKeyDownEaten(event);
	}

	*pfEaten = _bKeyEaten;
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
	Global::UpdateModifiers(wParam, lParam);

	CKeyEvent event(0, 0, 0);
	PrepareKeyEvent(event, wParam);
    _bKeyEaten = _IsKeyEaten(pContext, wParam, event);

	if (_bKeyEaten) {
		_bKeyEaten = IsKeyDownEaten(event);
	}

	if (_bKeyEaten && IsKeyDownHandled(event))
    {
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
	Global::UpdateModifiers(wParam, lParam);

	CKeyEvent event(0, 0, 0);
	PrepareKeyEvent(event, wParam);
	_bKeyEaten = _IsKeyEaten(pContext, wParam, event);
	
	if (_bKeyEaten) {
		_bKeyEaten = IsKeyUpEaten(event);
	}

	*pfEaten = _bKeyEaten;
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
	Global::UpdateModifiers(wParam, lParam);

	CKeyEvent event(0, 0, 0);
	PrepareKeyEvent(event, wParam);
	_bKeyEaten = _IsKeyEaten(pContext, wParam, event);

	if (_bKeyEaten) {
		_bKeyEaten = IsKeyUpEaten(event);
	}

	if (_bKeyEaten && IsKeyUpHandled(event))
    {
		// let the handler judge if it's eaten.
        _InvokeKeyHandler(pContext, event);
    }

	*pfEaten = _bKeyEaten;
    return S_OK;
}

//+---------------------------------------------------------------------------
//
// CheckShiftKeyOnly
//
//----------------------------------------------------------------------------

BOOL CTextService::CheckShiftKeyOnly(_In_ const TF_PRESERVEDKEY *ptfPsvKey)
{
    if ((/*(ptfPsvKey->uModifiers & (_TF_MOD_ON_KEYUP_SHIFT_ONLY & 0xffff0000)) && */!Global::IsShiftKeyDownOnly)/* ||
        ((ptfPsvKey->uModifiers & (_TF_MOD_ON_KEYUP_CONTROL_ONLY & 0xffff0000)) && !Global::IsControlKeyDownOnly) ||
        ((ptfPsvKey->uModifiers & (_TF_MOD_ON_KEYUP_ALT_ONLY & 0xffff0000)) && !Global::IsAltKeyDownOnly)*/)
    {
        return FALSE;
    }

    return TRUE;
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
    else if (IsEqualGUID(rguid, GUID_PRESERVEDKEY_STATUS_SWITCH))
	{
		if (!CheckShiftKeyOnly(&c_pkeyStatusSwitch))
        {
            *pfEaten = FALSE;
            return S_OK;
        }

		CKeyEvent event(0, 0, IM_SHIFT_MASK);
		_InvokeKeyHandler(pContext, event);
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

    // register SHIFT key
    hr = pKeystrokeMgr->PreserveKey(_tfClientId, 
                                    GUID_PRESERVEDKEY_STATUS_SWITCH,
                                    &c_pkeyStatusSwitch,
                                    c_szPKeyStatusSwitch,
                                    wcslen(c_szPKeyStatusSwitch));

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
	pKeystrokeMgr->UnpreserveKey(GUID_PRESERVEDKEY_STATUS_SWITCH, &c_pkeyStatusSwitch);

    pKeystrokeMgr->Release();
}

