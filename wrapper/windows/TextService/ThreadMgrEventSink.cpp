//////////////////////////////////////////////////////////////////////
//
//  THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
//  ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED
//  TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
//  PARTICULAR PURPOSE.
//
//  Copyright (C) 2003  Microsoft Corporation.  All rights reserved.
//
//  ThreadMgrEventSink.cpp
//
//          ITfThreadMgrEventSink implementation.
//
//////////////////////////////////////////////////////////////////////

#include "Globals.h"
#include "TextService.h"

//+---------------------------------------------------------------------------
//
// OnInitDocumentMgr
//
// Sink called by the framework just before the first context is pushed onto
// a document.
//----------------------------------------------------------------------------

STDAPI CTextService::OnInitDocumentMgr(ITfDocumentMgr *pDocMgr)
{
	pDocMgr;
    return S_OK;
}

//+---------------------------------------------------------------------------
//
// OnUninitDocumentMgr
//
// Sink called by the framework just after the last context is popped off a
// document.
//----------------------------------------------------------------------------

STDAPI CTextService::OnUninitDocumentMgr(ITfDocumentMgr *pDocMgr)
{
	pDocMgr;
    return E_NOTIMPL;
}

#define TRACE() fprintf(fp, "%s: %d\n", __FILE__, __LINE__)

//+---------------------------------------------------------------------------
//
// OnSetFocus
//
// Sink called by the framework when focus changes from one document to
// another.  Either document may be NULL, meaning previously there was no
// focus document, or now no document holds the input focus.
//----------------------------------------------------------------------------

STDAPI CTextService::OnSetFocus(ITfDocumentMgr *pDocMgrFocus, ITfDocumentMgr *pDocMgrPrevFocus)
{
    //
    // Whenever focus is changed, initialize the TextEditSink.
    //
	pDocMgrPrevFocus;
    _InitTextEditSink(pDocMgrFocus);

	FILE *fp = fp = fopen("e:/log.txt", "a");
	if (fp) {
		if (pDocMgrFocus == NULL) {
			fprintf(fp, "DocMgrFocus is NULL!\n");
			fflush(fp);
		} else {
			fprintf(fp, "DocMgrFocus is: %p\n", pDocMgrFocus);
			fflush(fp);
		}
	}

	TRACE();
	if (_pCandidateList) {
		TRACE();
		ITfDocumentMgr* pCandidateListDocumentMgr = nullptr;
		ITfContext* pTfContext = _pCandidateList->_GetContextDocument();
        if ((nullptr != pTfContext) && SUCCEEDED(pTfContext->GetDocumentMgr(&pCandidateListDocumentMgr)))
        {
			TRACE();
            if (pCandidateListDocumentMgr != pDocMgrFocus)
            {
				TRACE();
				// Hide Candidate Window
				_pCandidateList->OnKillThreadFocus();
            }
            else 
            {
				TRACE();
				// TODO: Show Candidate Window
				_pCandidateList->OnSetThreadFocus();
            }
			TRACE();
            pCandidateListDocumentMgr->Release();
        }
	}
	TRACE();
	if (_pDocMgrLastFocused)
    {
		TRACE();
        _pDocMgrLastFocused->Release();
		_pDocMgrLastFocused = nullptr;
    }

	TRACE();
    _pDocMgrLastFocused = pDocMgrFocus;

	TRACE();
    if (_pDocMgrLastFocused)
    {
		TRACE();
        _pDocMgrLastFocused->AddRef();
    }

    return S_OK;
}

//+---------------------------------------------------------------------------
//
// OnPushContext
//
// Sink called by the framework when a context is pushed.
//----------------------------------------------------------------------------

STDAPI CTextService::OnPushContext(ITfContext *pContext)
{
    return E_NOTIMPL;
}

//+---------------------------------------------------------------------------
//
// OnPopContext
//
// Sink called by the framework when a context is popped.
//----------------------------------------------------------------------------

STDAPI CTextService::OnPopContext(ITfContext *pContext)
{
    return E_NOTIMPL;
}

//+---------------------------------------------------------------------------
//
// _InitThreadMgrEventSink
//
// Advise our sink.
//----------------------------------------------------------------------------

BOOL CTextService::_InitThreadMgrEventSink()
{
    ITfSource *pSource;
    BOOL fRet;

    if (_pThreadMgr->QueryInterface(IID_ITfSource, (void **)&pSource) != S_OK)
        return FALSE;

    fRet = FALSE;

    if (pSource->AdviseSink(IID_ITfThreadMgrEventSink, (ITfThreadMgrEventSink *)this, &_dwThreadMgrEventSinkCookie) != S_OK)
    {
        // don't try to Unadvise _dwThreadMgrEventSinkCookie later
        _dwThreadMgrEventSinkCookie = TF_INVALID_COOKIE;
        goto Exit;
    }

    fRet = TRUE;

Exit:
    pSource->Release();
    return fRet;
}

//+---------------------------------------------------------------------------
//
// _UninitThreadMgrEventSink
//
// Unadvise our sink.
//----------------------------------------------------------------------------

void CTextService::_UninitThreadMgrEventSink()
{
    ITfSource *pSource;

    if (_dwThreadMgrEventSinkCookie == TF_INVALID_COOKIE)
        return; // never Advised

    if (_pThreadMgr->QueryInterface(IID_ITfSource, (void **)&pSource) == S_OK)
    {
        pSource->UnadviseSink(_dwThreadMgrEventSinkCookie);
        pSource->Release();
    }

    _dwThreadMgrEventSinkCookie = TF_INVALID_COOKIE;
}
