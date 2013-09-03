#ifndef LANGBAR_MENU_H
#define LANGBAR_MENU_H

#include "LangBarItem.h"
#include "LangBarItemInfo.h"
#include "LangBarMenuItem.h"
#include <vector>

class CLangBarMenu : public CLangBarItem
{
public:
	
	CLangBarMenu(CTextService *pTextService, SunPinyinEngine *pEngine, REFGUID guidLangBar, LPCWSTR description);
	~CLangBarMenu() {}

    // ITfLangBarItemButton
    virtual STDMETHODIMP InitMenu(ITfMenu *pMenu);
    virtual STDMETHODIMP OnMenuSelect(UINT wID);
    virtual STDMETHODIMP GetIcon(HICON *phIcon);
    virtual STDMETHODIMP GetText(BSTR *pbstrText);

	void SetItemState(UINT id, BOOL state);
	bool AddItem(CLangBarMenuItem &item) {
		_lbmMenuItems.push_back(item);
		return true;
	}

	void SetText(std::string text) { _sText = text; }
	void SetIcon(UINT icon) { _uiIcon = icon; }

private:
	std::string _sText;
	UINT _uiIcon;
	CLangBarItemInfo _lbiInfo;
	std::vector<CLangBarMenuItem> _lbmMenuItems;
};



#endif
