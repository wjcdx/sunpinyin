#ifndef CONFIG_BUTTON_H
#define CONFIG_BUTTON_H

#include <portability.h>
#include "ConfigItem.h"
#include "../GVariant.h"

//+---------------------------------------------------------------------------
//
// CConfigButton
//
//----------------------------------------------------------------------------

class CConfigButton : public CConfigItem
{
public:
	CConfigButton()
		: CConfigItem(), _bString(false)
	{}
	CConfigButton(SunPinyinConfig *config, int style, TCHAR *txt, TCHAR *key)
		: CConfigItem(config, TEXT("BUTTON"), style, txt, key), _bString(false)
	{}
	virtual ~CConfigButton() {}

	void SetupString(bool bStr, TCHAR *yes, TCHAR *no) {
		_bString = bStr;
		_trueString = std::string(yes);
		_falseString = std::string(no);
	}

	void Init();
	void OnConfigChanged();

private:

	GVariant PrepareValue(bool state);
	bool GetConfigValue();

	bool _bString;
	std::string _trueString;
	std::string _falseString;
};

#endif
