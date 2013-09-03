#ifndef LANGBAR_BUTTON_H
#define LANGBAR_BUTTON_H

#include "LangBarItem.h"
#include "LangBarItemInfo.h"

class CLangBarButton : public CLangBarItem
{
public:
	
	CLangBarButton(CTextService *pTextService, SunPinyinEngine *pEngine, REFGUID guidLangBar, LPCWSTR description);
	~CLangBarButton() {}

	// ITfLangBarItem
    virtual STDMETHODIMP GetStatus(DWORD *pdwStatus);
    virtual STDMETHODIMP Show(BOOL fShow);
    virtual STDMETHODIMP GetTooltipString(BSTR *pbstrToolTip);

    // ITfLangBarItemButton
    virtual STDMETHODIMP OnClick(TfLBIClick click, POINT pt, const RECT *prcArea);
    virtual STDMETHODIMP GetIcon(HICON *phIcon);
    virtual STDMETHODIMP GetText(BSTR *pbstrText);


	void SetupInfo(CLangBarItemInfo &tInfo, CLangBarItemInfo &fInfo) {
		_lbiTrueInfo = tInfo;
		_lbiFalseInfo = fInfo;
	}

	void SetId(UINT id) { _uiID = id; }
	UINT GetId() { return _uiID; }

	// called by engine to set LangBarButton status
	virtual void SetState(BOOL state);
	// update engine status
	virtual bool Update();

private:
	
	BOOL _bState;
	CLangBarItemInfo _lbiTrueInfo;
	CLangBarItemInfo _lbiFalseInfo;

	UINT _uiID;
};



#endif

