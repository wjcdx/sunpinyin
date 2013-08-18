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
		_uiModifiers = oEvent.modifiers;
    }

    // ITfEditSession
    STDMETHODIMP DoEditSession(TfEditCookie ec);

private:
	UINT _uiCode;
	UINT _uiModifiers;
};

//+---------------------------------------------------------------------------
//
// DoEditSession
//
//----------------------------------------------------------------------------

STDAPI CKeyHandlerEditSession::DoEditSession(TfEditCookie ec)
{
	CKeyEvent oEvent(_uiCode, _uiCode, _uiModifiers);
    switch (_uiCode)
    {
		case IM_VK_BACK_SPACE:
			return _pTextService->_HandleBackSpaceKey(ec, _pContext, oEvent);

		case IM_VK_LEFT:
        case IM_VK_RIGHT:
			return _pTextService->_HandleArrowKey(ec, _pContext, oEvent);
			
        case IM_VK_ENTER:
        case IM_VK_SPACE:
		case IM_VK_PAGE_UP:
		case IM_VK_PAGE_DOWN:
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
			return _pTextService->_DispatchKeyEvent(ec, _pContext, oEvent);
        default:
			if ((_uiCode >= 'A' && _uiCode <= 'Z')
			|| (_uiCode >= 'a' && _uiCode <= 'z'))
            return _pTextService->_HandleCharacterKey(ec, _pContext, oEvent);
    }

    return S_OK;
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
// _HandleCharacterKey
//
// If the keystroke happens within a composition, eat the key and return S_OK.
//
//----------------------------------------------------------------------------

HRESULT CTextService::_HandleCharacterKey(TfEditCookie ec, ITfContext *pContext, CKeyEvent &oEvent)
{
    ITfRange *pRangeComposition;
    TF_SELECTION tfSelection;
    ULONG cFetched;
    WCHAR ch;
    BOOL fCovered;

	// Start the new compositon if there is no composition.
    if (!_IsComposing())
        _StartComposition(pContext);

    //
    // Assign VK_ value to the char. So the inserted the character is always
    // uppercase.
    //
    ch = (WCHAR)oEvent.code;

    // first, test where a keystroke would go in the document if an insert is done
    if (pContext->GetSelection(ec, TF_DEFAULT_SELECTION, 1, &tfSelection, &cFetched) != S_OK || cFetched != 1)
        return S_FALSE;

	//{
	//	CHAR text[512] = {0};
	//	CHAR msg[512] = {0};
	//	ULONG ulGot = 0;
	//	tfSelection.range->GetText(ec, 0, (WCHAR *)text, 512, &ulGot);
	//	for (int i=0, j=0; i < 512; i++) {
	//		if (text[i] == 0)
	//			continue;
	//		msg[j++] = text[i];
	//	}
	//	MessageBox(NULL, msg, nullptr, MB_OK);
	//}

    // is the insertion point covered by a composition?
    if (_pComposition->GetRange(&pRangeComposition) == S_OK)
    {
		//{
		//	CHAR text[512] = {0};
		//	CHAR msg[512] = {0};
		//	ULONG ulGot = 0;
		//	pRangeComposition->GetText(ec, 0, (WCHAR *)text, 512, &ulGot);
		//	for (int i=0, j=0; i < 512; i++) {
		//		if (text[i] == 0)
		//			continue;
		//		msg[j++] = text[i];
		//	}
		//	MessageBox(NULL, msg, nullptr, MB_OK);
		//}

        fCovered = IsRangeCovered(ec, tfSelection.range, pRangeComposition);

        pRangeComposition->Release();

        if (!fCovered)
        {
            goto Exit;
        }
    }

    // insert the text
    // Use SetText here instead of InsertTextAtSelection because a composition is already started
    // Don't allow the app to adjust the insertion point inside our composition
    if (tfSelection.range->SetText(ec, 0, &ch, 1) != S_OK)
        goto Exit;

    // update the selection, and make it an insertion point just past
    // the inserted text.
    tfSelection.range->Collapse(ec, TF_ANCHOR_END);
    pContext->SetSelection(ec, 1, &tfSelection);

    //
    // set the display attribute to the composition range.
    //
    _SetCompositionDisplayAttributes(ec, pContext, _gaDisplayAttributeInput);

	// SUNPINYIN HOOK POINT
	_pEngine->process_key_event(ec, pContext, oEvent);

Exit:
    tfSelection.range->Release();
    return S_OK;
}


//+---------------------------------------------------------------------------
//
// _DispatchKeyEvent
//
//----------------------------------------------------------------------------

HRESULT CTextService::_DispatchKeyEvent(TfEditCookie ec, ITfContext *pContext, CKeyEvent &oEvent)
{
	_pEngine->process_key_event(ec, pContext, oEvent);
    return S_OK;
}


HRESULT CTextService::_HandleBackSpaceKey(TfEditCookie ec, ITfContext *pContext, CKeyEvent &oEvent)
{
	ITfRange *pOriginRange;
    ITfRange *pRangeComposition;
    TF_SELECTION tfSelection;
    ULONG cFetched;
    BOOL fCovered;
	ULONG ulGot = 0;


    // first, test where a keystroke would go in the document if an insert is done
    if (pContext->GetSelection(ec, TF_DEFAULT_SELECTION, 1, &tfSelection, &cFetched) != S_OK || cFetched != 1)
        return S_FALSE;

    // is the insertion point covered by a composition?
    if (_pComposition->GetRange(&pRangeComposition) == S_OK)
    {
        fCovered = IsRangeCovered(ec, tfSelection.range, pRangeComposition);

        if (!fCovered)
        {
            goto Exit;
        }
		HRESULT ret = S_OK;
		ret = pRangeComposition->Clone(&pOriginRange);

		WCHAR text[512] = { 0 };
		pRangeComposition->GetText(ec, 0, text, 512, &ulGot);
		if (ulGot <= 0)
			goto Exit;

		LONG lShifted;
		ret = pRangeComposition->Collapse(ec, TF_ANCHOR_END);
		ret = pRangeComposition->ShiftStart(ec, -1, &lShifted, NULL);

		ret = pOriginRange->SetText(ec, 0, text, ulGot - 1);
    }

    // update the selection, and make it an insertion point just past
    // the inserted text.
    tfSelection.range->Collapse(ec, TF_ANCHOR_END);
    pContext->SetSelection(ec, 1, &tfSelection);

    //
    // set the display attribute to the composition range.
    //
    _SetCompositionDisplayAttributes(ec, pContext, _gaDisplayAttributeInput);

Dispatch:
	// SUNPINYIN HOOK POINT
	_pEngine->process_key_event(ec, pContext, oEvent);

	if (ulGot == 1) {
		_HandleCancel(ec, pContext);
	}

Exit:
	pOriginRange->Release();
	pRangeComposition->Release();
    tfSelection.range->Release();
    return S_OK;
}


//+---------------------------------------------------------------------------
//
// _HandleArrowKey
//
// Update the selection within a composition.
//
//----------------------------------------------------------------------------

HRESULT CTextService::_HandleArrowKey(TfEditCookie ec, ITfContext *pContext, CKeyEvent &oEvent)
{
    ITfRange *pRangeComposition;
    LONG cch;
    BOOL fEqual;
    TF_SELECTION tfSelection;
    ULONG cFetched;

    // get the selection
    if (pContext->GetSelection(ec, TF_DEFAULT_SELECTION, 1, &tfSelection, &cFetched) != S_OK ||
        cFetched != 1)
    {
        // no selection?
        return S_OK; // eat the keystroke
    }

    // get the composition range
    if (_pComposition->GetRange(&pRangeComposition) != S_OK)
        goto Exit;

    // adjust the selection
    if (oEvent == IM_VK_LEFT)
    {
        if (tfSelection.range->IsEqualStart(ec, pRangeComposition, TF_ANCHOR_START, &fEqual) == S_OK &&
            !fEqual)
        {
            tfSelection.range->ShiftStart(ec, -1, &cch, NULL);
        }
        tfSelection.range->Collapse(ec, TF_ANCHOR_START);
    }
    else
    {
        // IM_VK_RIGHT
        if (tfSelection.range->IsEqualEnd(ec, pRangeComposition, TF_ANCHOR_END, &fEqual) == S_OK &&
            !fEqual)
        {
            tfSelection.range->ShiftEnd(ec, +1, &cch, NULL);
        }
        tfSelection.range->Collapse(ec, TF_ANCHOR_END);
    }

    pContext->SetSelection(ec, 1, &tfSelection);

    pRangeComposition->Release();

Exit:
    tfSelection.range->Release();
    return S_OK; // eat the keystroke
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

