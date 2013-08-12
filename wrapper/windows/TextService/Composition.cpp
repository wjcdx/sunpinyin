//////////////////////////////////////////////////////////////////////
//
//  THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
//  ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED
//  TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
//  PARTICULAR PURPOSE.
//
//  Copyright (C) 2003  Microsoft Corporation.  All rights reserved.
//
//  Composition.cpp
//
//          ITfCompositionSink interface and compositon utilities
//
//////////////////////////////////////////////////////////////////////

#include "Globals.h"
#include "TextService.h"

//+---------------------------------------------------------------------------
//
// OnCompositionTerminated
//
// Callback for ITfCompositionSink.  The system calls this method whenever
// someone other than this service ends a composition.
//----------------------------------------------------------------------------

STDAPI CTextService::OnCompositionTerminated(TfEditCookie ecWrite, ITfComposition *pComposition)
{

    // releae our cached composition
    if (_pComposition != NULL)
    {
        _pComposition->Release();
        _pComposition = NULL;
    }

    return S_OK;
}

//+---------------------------------------------------------------------------
//
// _IsComposing
//
//----------------------------------------------------------------------------

BOOL CTextService::_IsComposing()
{
    return _pComposition != NULL;
}

//+---------------------------------------------------------------------------
//
// _SetComposition
//
//----------------------------------------------------------------------------

void CTextService::_SetComposition(ITfComposition *pComposition)
{
    _pComposition = pComposition;
}

//+---------------------------------------------------------------------------
//
// _InsertAtSelection
//
//----------------------------------------------------------------------------

HRESULT CTextService::_InsertAtSelection(TfEditCookie ec, _In_ ITfContext *pContext, _In_ WCHAR *pchText, _Outptr_ ITfRange **ppCompRange)
{
    ITfRange* rangeInsert = nullptr;
    ITfInsertAtSelection* pias = nullptr;
    HRESULT hr = S_OK;

    if (ppCompRange == nullptr)
    {
        hr = E_INVALIDARG;
        goto Exit;
    }

    *ppCompRange = nullptr;

    hr = pContext->QueryInterface(IID_ITfInsertAtSelection, (void **)&pias);
    if (FAILED(hr))
    {
        goto Exit;
    }

    hr = pias->InsertTextAtSelection(ec, TF_IAS_QUERYONLY, pchText, (LONG)lstrlenW(pchText), &rangeInsert);

    if ( FAILED(hr) || rangeInsert == nullptr)
    {
        rangeInsert = nullptr;
        pias->Release();
        goto Exit;
    }

    *ppCompRange = rangeInsert;
    pias->Release();
    hr = S_OK;

Exit:
    return hr;
}


BOOL IsRangeCovered(TfEditCookie ec, ITfRange *pRangeTest, ITfRange *pRangeCover);
HRESULT CTextService::_CommitSelectedCandidate(TfEditCookie ec, _In_ ITfContext *pContext, _In_ WCHAR *pchText)
{
    ITfRange *pRangeComposition;
    TF_SELECTION tfSelection;
    ULONG cFetched;
    WCHAR ch;
    BOOL fCovered;

	//ITfRange *pRangeInsert = nullptr;
	//_InsertAtSelection(ec, pContext, pchText, &pRangeInsert);

	// first, test where a keystroke would go in the document if an insert is done
    if (pContext->GetSelection(ec, TF_DEFAULT_SELECTION, 1, &tfSelection, &cFetched) != S_OK || cFetched != 1)
        return S_FALSE;

    // is the insertion point covered by a composition?
    if (_pComposition->GetRange(&pRangeComposition) == S_OK)
    {
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
    if (tfSelection.range->SetText(ec, 0, pchText, lstrlenW(pchText)) != S_OK)
        goto Exit;

    // update the selection, and make it an insertion point just past
    // the inserted text.
    tfSelection.range->Collapse(ec, TF_ANCHOR_END);
    pContext->SetSelection(ec, 1, &tfSelection);



	//
    // set the display attribute to the composition range.
    //
    _SetCompositionDisplayAttributes(ec, pContext, _gaDisplayAttributeInput);

	_TerminateComposition(ec, pContext);

	_EndCandidateList(ec, pContext);

	return S_OK;

Exit:
    tfSelection.range->Release();
    return S_OK;
}
