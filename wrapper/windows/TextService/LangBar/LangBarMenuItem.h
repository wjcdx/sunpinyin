#ifndef LANGBAR_MENUITEM_H
#define LANGBAR_MENUITEM_H

#include "../Globals.h"
#include "LangBarMenuItemInfo.h"

class SunPinyinEngine;

class CLangBarMenuItem
{
public:
	
	CLangBarMenuItem(SunPinyinEngine *engine);
	~CLangBarMenuItem();

	void SetIndex(UINT index) { _uiIndex = index; }
	UINT GetIndex() { return _uiIndex; }

	void SetId(UINT id) { _uiId = id; }
	UINT GetId() { return _uiId; }

	std::string GetText();

	UINT GetBmp();

	void Selected()
	{
		_bState = !_bState;
		Update();
	}

	void SetInfo(CLangBarMenuItemInfo &trueInfo, CLangBarMenuItemInfo &falseInfo)
	{
		_lbmiiTrueInfo = trueInfo;
		_lbmiiFalseInfo = falseInfo;
	}

	// called by engine to set LangBarButton status
	virtual void SetState(BOOL state) { _bState = state; }
	// update engine status
	virtual bool Update();
	

private:
	UINT _uiIndex;

	BOOL _bState;
	UINT _uiId;
	DWORD _dwFlags;

	SunPinyinEngine *_pEngine;
	CLangBarMenuItemInfo _lbmiiTrueInfo;
	CLangBarMenuItemInfo _lbmiiFalseInfo;
};



#endif

