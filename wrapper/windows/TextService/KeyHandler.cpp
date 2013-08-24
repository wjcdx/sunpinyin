//////////////////////////////////////////////////////////////////////
//
//  THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
//  ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED
//  TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
//  PARTICULAR PURPOSE.
//
//  Copyright (C) 2003  Microsoft Corporation.  All rights reserved.
//
//  KeyHandler.cpp
//
//          the handler routines for key events
//
//////////////////////////////////////////////////////////////////////

#include "Globals.h"
#include "EditSession.h"
#include "TextService.h"
#include "CandidateList.h"


//+---------------------------------------------------------------------------
//
// CKeyHandlerEditSession
//
//----------------------------------------------------------------------------

class CKeyHandlerEditSession : public CEditSessionBase
{
public:
    CKeyHandlerEditSession(CTextService *pTextService, ITfContext *pContext, CKeyEvent &oEvent) : CEditSessionBase(pTextService, pContext)
    {
		_uiCode = oEvent.code;
		_uiValue = oEvent.value;
		_uiModifiers = oEvent.modifiers;
    }

    // ITfEditSession
    STDMETHODIMP DoEditSession(TfEditCookie ec);

private:
	UINT _uiCode;
	UINT _uiValue;
	UINT _uiModifiers;
};

//+---------------------------------------------------------------------------
//
// DoEditSession
//
//----------------------------------------------------------------------------

STDAPI CKeyHandlerEditSession::DoEditSession(TfEditCookie ec)
{
	CKeyEvent oEvent(_uiCode, _uiValue, _uiModifiers);

	if (_uiCode > 0x20 && _uiCode < 0x7f) {
		_pTextService->_HandleComposition(ec, _pContext);
	}

	return _pTextService->_DispatchKeyEvent(ec, _pContext, oEvent);
}

//+---------------------------------------------------------------------------
//
// IsRangeCovered
//
// Returns TRUE if pRangeTest is entirely contained within pRangeCover.
//
//----------------------------------------------------------------------------

BOOL IsRangeCovered(TfEditCookie ec, ITfRange *pRangeTest, ITfRange *pRangeCover)
{
    LONG lResult;

    if (pRangeCover->CompareStart(ec, pRangeTest, TF_ANCHOR_START, &lResult) != S_OK ||
        lResult > 0)
    {
        return FALSE;
    }

    if (pRangeCover->CompareEnd(ec, pRangeTest, TF_ANCHOR_END, &lResult) != S_OK ||
        lResult < 0)
    {
        return FALSE;
    }

    return TRUE;
}

//+---------------------------------------------------------------------------
//
// _HandleComposition
//
// If there's no a composition, start one and return S_OK.
//
//----------------------------------------------------------------------------

HRESULT CTextService::_HandleComposition(TfEditCookie ec, ITfContext *pContext)
{
	// Start the new compositon if there is no composition.
    if (!_IsComposing())
        _StartComposition(pContext);

    return S_OK;
}


//+---------------------------------------------------------------------------
//
// _DispatchKeyEvent
//
//----------------------------------------------------------------------------

HRESULT CTextService::_DispatchKeyEvent(TfEditCookie ec, ITfContext *pContext, CKeyEvent &oEvent)
{
	_bKeyEaten = _pEngine->process_key_event(ec, pContext, oEvent);
    return S_OK;
}


//+---------------------------------------------------------------------------
//
// _InvokeKeyHandler
//
// This text service is interested in handling keystrokes to demonstrate the
// use the compositions. Some apps will cancel compositions if they receive
// keystrokes while a compositions is ongoing.
//
//----------------------------------------------------------------------------

HRESULT CTextService::_InvokeKeyHandler(ITfContext *pContext, CKeyEvent &oEvent)
{
    CKeyHandlerEditSession *pEditSession;
    HRESULT hr = E_FAIL;

    // Insert a char in place of this keystroke
    if ((pEditSession = new CKeyHandlerEditSession(this, pContext, oEvent)) == NULL)
        goto Exit;

    // a lock is required
    // nb: this method is one of the few places where it is legal to use
    // the TF_ES_SYNC flag
    hr = pContext->RequestEditSession(_tfClientId, pEditSession, TF_ES_SYNC | TF_ES_READWRITE, &hr);

    pEditSession->Release();

Exit:
    return hr;
}

