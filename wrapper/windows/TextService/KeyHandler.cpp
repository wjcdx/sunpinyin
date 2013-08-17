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

#include "ime-core/imi_keys.h"

//+---------------------------------------------------------------------------
//
// CKeyHandlerEditSession
//
//----------------------------------------------------------------------------

class CKeyHandlerEditSession : public CEditSessionBase
{
public:
    CKeyHandlerEditSession(CTextService *pTextService, ITfContext *pContext, WPARAM wParam) : CEditSessionBase(pTextService, pContext)
    {
        _wParam = wParam;
    }

    // ITfEditSession
    STDMETHODIMP DoEditSession(TfEditCookie ec);

private:
    WPARAM _wParam;
};

//+---------------------------------------------------------------------------
//
// DoEditSession
//
//----------------------------------------------------------------------------

STDAPI CKeyHandlerEditSession::DoEditSession(TfEditCookie ec)
{

    switch (_wParam)
    {
        case VK_LEFT:
        case VK_RIGHT:
            return _pTextService->_HandleArrowKey(ec, _pContext, _wParam);

        case VK_RETURN:
            return _pTextService->_HandleReturnKey(ec, _pContext);

        case VK_SPACE:
            return _pTextService->_HandleSpaceKey(ec, _pContext);
		case VK_BACK:
			return _pTextService->_HandleBackSpaceKey(ec, _pContext, VK_BACK);
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
			//MessageBox(NULL, "Number inputed.", NULL, MB_OK);
			return _pTextService->_HandleNumberKey(ec, _pContext, _wParam);



        default:
            if (_wParam >= 'A' && _wParam <= 'Z') {
				char chr = _wParam + 'a' - 'A';
                return _pTextService->_HandleCharacterKey(ec, _pContext, chr);
			}
            break;
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

HRESULT CTextService::_HandleCharacterKey(TfEditCookie ec, ITfContext *pContext, WPARAM wParam)
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
    ch = (WCHAR)wParam;

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
	CKeyEvent event(ch, ch, 0);
	_pEngine->process_key_event(ec, pContext, event);

Exit:
    tfSelection.range->Release();
    return S_OK;
}


HRESULT CTextService::_HandleNumberKey(TfEditCookie ec, ITfContext *pContext, WPARAM wParam)
{
    WCHAR ch;

    // Start the new compositon if there is no composition.
    if (!_IsComposing()) {
        //_StartComposition(pContext);
		return S_OK;
	}

    //
    // Assign VK_ value to the char. So the inserted the character is always
    // uppercase.
    //
    ch = (WCHAR)wParam;

	// SUNPINYIN HOOK POINT
	CKeyEvent event(ch, ch, 0);
	_pEngine->process_key_event(ec, pContext, event);

    return S_OK;
}

//+---------------------------------------------------------------------------
//
// _HandleReturnKey
//
//----------------------------------------------------------------------------

HRESULT CTextService::_HandleReturnKey(TfEditCookie ec, ITfContext *pContext)
{
    // just terminate the composition
	// SUNPINYIN HOOK POINT
	CKeyEvent event(IM_VK_ENTER, IM_VK_ENTER, 0);
	_pEngine->process_key_event(ec, pContext, event);

    return S_OK;
}

//+---------------------------------------------------------------------------
//
// _HandleSpaceKey
//
//----------------------------------------------------------------------------

HRESULT CTextService::_HandleSpaceKey(TfEditCookie ec, ITfContext *pContext)
{
	CKeyEvent event(IM_VK_SPACE, IM_VK_SPACE, 0);
	_pEngine->process_key_event(ec, pContext, event);

    return S_OK;
}

HRESULT CTextService::_HandleBackSpaceKey(TfEditCookie ec, ITfContext *pContext, WPARAM wParam)
{
	ITfRange *pOriginRangle;
    ITfRange *pRangeComposition;
    TF_SELECTION tfSelection;
    ULONG cFetched;
    WCHAR ch;
    BOOL fCovered;
	ULONG ulGot = 0;

    //
    // Assign VK_ value to the char. So the inserted the character is always
    // uppercase.
    //
    ch = (WCHAR)wParam;

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
		ret = pRangeComposition->Clone(&pOriginRangle);

		WCHAR text[512] = { 0 };
		pRangeComposition->GetText(ec, 0, text, 512, &ulGot);
		if (ulGot <= 0)
			goto Exit;

		LONG lShifted;
		ret = pRangeComposition->Collapse(ec, TF_ANCHOR_END);
		ret = pRangeComposition->ShiftStart(ec, -1, &lShifted, NULL);

		ret = pOriginRangle->SetText(ec, 0, text, ulGot - 1);
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
	CKeyEvent event(IM_VK_BACK_SPACE, IM_VK_BACK_SPACE, 0);
	_pEngine->process_key_event(ec, pContext, event);

	if (ulGot == 1) {
		_HandleCancel(ec, pContext);
	}

Exit:
	pOriginRangle->Release();
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

HRESULT CTextService::_HandleArrowKey(TfEditCookie ec, ITfContext *pContext, WPARAM wParam)
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
    if (wParam == VK_LEFT)
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
        // VK_RIGHT
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

HRESULT CTextService::_InvokeKeyHandler(ITfContext *pContext, WPARAM wParam, LPARAM lParam)
{
    CKeyHandlerEditSession *pEditSession;
    HRESULT hr = E_FAIL;

    // Insert a char in place of this keystroke
    if ((pEditSession = new CKeyHandlerEditSession(this, pContext, wParam)) == NULL)
        goto Exit;

    // a lock is required
    // nb: this method is one of the few places where it is legal to use
    // the TF_ES_SYNC flag
    hr = pContext->RequestEditSession(_tfClientId, pEditSession, TF_ES_SYNC | TF_ES_READWRITE, &hr);

    pEditSession->Release();

Exit:
    return hr;
}

