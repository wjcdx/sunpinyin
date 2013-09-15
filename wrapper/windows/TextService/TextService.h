//////////////////////////////////////////////////////////////////////
//
//  THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
//  ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED
//  TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
//  PARTICULAR PURPOSE.
//
//  Copyright (C) 2003  Microsoft Corporation.  All rights reserved.
//
//  TextService.h
//
//          CTextService declaration.
//
//////////////////////////////////////////////////////////////////////

#ifndef TEXTSERVICE_H
#define TEXTSERVICE_H

#include "sunpinyin_engine.h"
#include "CandidateList.h"

class CLangBarItem;

class CTextService : public ITfTextInputProcessor,
                     public ITfThreadMgrEventSink,
                     public ITfTextEditSink,
                     public ITfKeyEventSink,
					 public ITfThreadFocusSink,
                     public ITfCompositionSink,
                     public ITfDisplayAttributeProvider
{
public:
    CTextService();
    ~CTextService();

    // IUnknown
    STDMETHODIMP QueryInterface(REFIID riid, void **ppvObj);
    STDMETHODIMP_(ULONG) AddRef(void);
    STDMETHODIMP_(ULONG) Release(void);

    // ITfTextInputProcessor
    STDMETHODIMP Activate(ITfThreadMgr *pThreadMgr, TfClientId tfClientId);
    STDMETHODIMP Deactivate();

    // ITfThreadMgrEventSink
    STDMETHODIMP OnInitDocumentMgr(ITfDocumentMgr *pDocMgr);
    STDMETHODIMP OnUninitDocumentMgr(ITfDocumentMgr *pDocMgr);
    STDMETHODIMP OnSetFocus(ITfDocumentMgr *pDocMgrFocus, ITfDocumentMgr *pDocMgrPrevFocus);
    STDMETHODIMP OnPushContext(ITfContext *pContext);
    STDMETHODIMP OnPopContext(ITfContext *pContext);

    // ITfTextEditSink
    STDMETHODIMP OnEndEdit(ITfContext *pContext, TfEditCookie ecReadOnly, ITfEditRecord *pEditRecord);

    // ITfKeyEventSink
    STDMETHODIMP OnSetFocus(BOOL fForeground);
    STDMETHODIMP OnTestKeyDown(ITfContext *pContext, WPARAM wParam, LPARAM lParam, BOOL *pfEaten);
    STDMETHODIMP OnKeyDown(ITfContext *pContext, WPARAM wParam, LPARAM lParam, BOOL *pfEaten);
    STDMETHODIMP OnTestKeyUp(ITfContext *pContext, WPARAM wParam, LPARAM lParam, BOOL *pfEaten);
    STDMETHODIMP OnKeyUp(ITfContext *pContext, WPARAM wParam, LPARAM lParam, BOOL *pfEaten);
    STDMETHODIMP OnPreservedKey(ITfContext *pContext, REFGUID rguid, BOOL *pfEaten);

	// ITfThreadFocusSink
    STDMETHODIMP OnSetThreadFocus();
    STDMETHODIMP OnKillThreadFocus();

    // ITfCompositionSink
    STDMETHODIMP OnCompositionTerminated(TfEditCookie ecWrite, ITfComposition *pComposition);

    // ITfDisplayAttributeProvider
    STDMETHODIMP EnumDisplayAttributeInfo(IEnumTfDisplayAttributeInfo **ppEnum);
    STDMETHODIMP GetDisplayAttributeInfo(REFGUID guidInfo, ITfDisplayAttributeInfo **ppInfo);

    // CClassFactory factory callback
    static HRESULT CreateInstance(IUnknown *pUnkOuter, REFIID riid, void **ppvObj);

    ITfThreadMgr *_GetThreadMgr() { return _pThreadMgr; }
    TfClientId _GetClientId() { return _tfClientId; }

    // utility function for compartment
    BOOL _IsKeyboardDisabled();
    BOOL _IsKeyboardOpen();
    HRESULT _SetKeyboardOpen(BOOL fOpen);

    // functions for the composition object.
    void _StartComposition(ITfContext *pContext);
    void _EndComposition(ITfContext *pContext);
    void _TerminateComposition(TfEditCookie ec, ITfContext *pContext);
    BOOL _IsComposing();
    void _SetComposition(ITfComposition *pComposition);
	HRESULT _CommitSelectedCandidate(TfEditCookie ec, _In_ ITfContext *pContext,
		_In_ const WCHAR *pchText, int length);
	HRESULT CTextService::_HandleComposition(TfEditCookie ec, ITfContext *pContext);

	// key event handlers for composition/candidate/phrase common objects.
    HRESULT _HandleComplete(TfEditCookie ec, _In_ ITfContext *pContext);
    HRESULT _HandleCancel(TfEditCookie ec, _In_ ITfContext *pContext);

    // key event handlers.
	HRESULT _DispatchKeyEvent(TfEditCookie ec, ITfContext *pContext, CKeyEvent &oEvent);
    HRESULT _InvokeKeyHandler(ITfContext *pContext, CKeyEvent &oEvent);

	HRESULT _StartCandidateList(TfEditCookie ec, ITfContext *pContext);
	HRESULT _UpdateCandidateList(TfEditCookie ec, ITfContext *pContext);
	HRESULT _EndCandidateList(TfEditCookie ec, ITfContext *pContext);

    void _ClearCompositionDisplayAttributes(TfEditCookie ec, ITfContext *pContext);
    BOOL _SetCompositionDisplayAttributes(TfEditCookie ec, ITfContext *pContext, TfGuidAtom gaDisplayAttribute);
    BOOL _InitDisplayAttributeGuidAtom();

private:
    // initialize and uninitialize ThreadMgrEventSink.
    BOOL _InitThreadMgrEventSink();
    void _UninitThreadMgrEventSink();

    // initialize TextEditSink.
    BOOL _InitTextEditSink(ITfDocumentMgr *pDocMgr);

    // initialize and uninitialize LanguageBar Item.
    BOOL _InitLanguageBar();
    void _UninitLanguageBar();

    // initialize and uninitialize KeyEventSink.
    BOOL _InitKeyEventSink();
    void _UninitKeyEventSink();

	BOOL _InitThreadFocusSink();
    void _UninitThreadFocusSink();

    // initialize and uninitialize PreservedKey.
    BOOL _InitPreservedKey();
    void _UninitPreservedKey();
	BOOL CheckShiftKeyOnly(_In_ const TF_PRESERVEDKEY *ptfPsvKey);

    // utility function for KeyEventSink
	BOOL _IsKeyEaten(ITfContext *pContext, WPARAM wParam, CKeyEvent &event);
	BOOL PrepareKeyEvent(CKeyEvent &oEvent, WPARAM wParam);

    //
    // state
    //
    ITfThreadMgr *_pThreadMgr;
    TfClientId _tfClientId;

    // The cookie of ThreadMgrEventSink
    DWORD _dwThreadMgrEventSinkCookie;

	// The cookie of ThreadFocusSink
    DWORD _dwThreadFocusSinkCookie;

	ITfDocumentMgr* _pDocMgrLastFocused;

    //
    // private variables for TextEditSink
    //
    ITfContext   *_pTextEditSinkContext;
    DWORD _dwTextEditSinkCookie;

    CLangBarItem *_pLangBarItem;

    // the current composition object.
    ITfComposition *_pComposition;

    // guidatom for the display attibute.
    TfGuidAtom _gaDisplayAttributeInput;
    TfGuidAtom _gaDisplayAttributeConverted;

    // the candidate list object.
    ImeWrapper::CCandidateList *_pCandidateList;

	// sunpinyin engine
	SunPinyinEngine *_pEngine;
	BOOL _bKeyEaten;

    LONG _cRef;     // COM ref count
};


#endif // TEXTSERVICE_H
