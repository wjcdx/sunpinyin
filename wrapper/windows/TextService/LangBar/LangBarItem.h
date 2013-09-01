
#ifndef LANGBAR_ITEM_H
#define LANGBAR_ITEM_H

#include "../Globals.h"
#include "../Resource.h"

class SunPinyinEngine;
class CTextService;

//+---------------------------------------------------------------------------
//
// CLangBarItemButton class
//
//----------------------------------------------------------------------------

class CLangBarItem : public ITfLangBarItemButton,
                           public ITfSource
{
public:
    CLangBarItem(CTextService *pTextService, SunPinyinEngine *pEngine);
	virtual ~CLangBarItem();

    // IUnknown
    virtual STDMETHODIMP QueryInterface(REFIID riid, void **ppvObj);
    virtual STDMETHODIMP_(ULONG) AddRef(void);
    virtual STDMETHODIMP_(ULONG) Release(void);

    // ITfLangBarItem
    virtual STDMETHODIMP GetInfo(TF_LANGBARITEMINFO *pInfo);
    virtual STDMETHODIMP GetStatus(DWORD *pdwStatus);
    virtual STDMETHODIMP Show(BOOL fShow);
    virtual STDMETHODIMP GetTooltipString(BSTR *pbstrToolTip);

    // ITfLangBarItemButton
    virtual STDMETHODIMP OnClick(TfLBIClick click, POINT pt, const RECT *prcArea);
    virtual STDMETHODIMP InitMenu(ITfMenu *pMenu);
    virtual STDMETHODIMP OnMenuSelect(UINT wID);
    virtual STDMETHODIMP GetIcon(HICON *phIcon);
    virtual STDMETHODIMP GetText(BSTR *pbstrText);

    // ITfSource
    virtual STDMETHODIMP AdviseSink(REFIID riid, IUnknown *punk, DWORD *pdwCookie);
    virtual STDMETHODIMP UnadviseSink(DWORD dwCookie);


protected:
    ITfLangBarItemSink *_pLangBarItemSink;
    TF_LANGBARITEMINFO _tfLangBarItemInfo;

    CTextService *_pTextService;
    LONG _cRef;

	SunPinyinEngine *_pEngine;
};

#endif
