#include "LangBarMenuItem.h"
#include "../sunpinyin_engine.h"

CLangBarMenuItem::CLangBarMenuItem(SunPinyinEngine *engine)
	: _uiIndex(0), _bState(true), _uiId(0), _dwFlags(0), _pEngine(engine)
{
}

CLangBarMenuItem::~CLangBarMenuItem()
{
}

bool CLangBarMenuItem::Update()
{
	if (_pEngine)
	{
		return _pEngine->update_prop_status(_uiId, _bState);
	}
	return false;
}

std::string CLangBarMenuItem::GetText()
{
	if (_bState)
	{
		return _lbmiiTrueInfo.GetText();
	}
	else
	{
		return _lbmiiFalseInfo.GetText();
	}
}

UINT CLangBarMenuItem::GetBmp()
{
	if (_bState)
	{
		return _lbmiiTrueInfo.GetBmp();
	}
	else
	{
		return _lbmiiFalseInfo.GetBmp();
	}
}