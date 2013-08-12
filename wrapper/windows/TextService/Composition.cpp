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

HRESULT CTextService::_CommitSelectedCandidate(TfEditCookie ec, _In_ ITfContext *pContext, _In_ WCHAR *pchText)
{
	ITfRange *pRangeInsert = nullptr;
	_InsertAtSelection(ec, pContext, pchText, &pRangeInsert);

	//
    // set the display attribute to the composition range.
    //
    _SetCompositionDisplayAttributes(ec, pContext, _gaDisplayAttributeInput);

	_TerminateComposition(ec, pContext);

	_EndCandidateList(ec, pContext);

	return S_OK;
}
