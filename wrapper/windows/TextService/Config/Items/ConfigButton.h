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
		: CConfigItem()
	{}
	CConfigButton(SunPinyinConfig *config, int style, TCHAR *txt, TCHAR *key)
		: CConfigItem(config, TEXT("BUTTON"), style, txt, key)
	{}
	virtual ~CConfigButton() {}

	void Init();
	void OnConfigChanged(int code);

private:

	GVariant PrepareValue(bool state);
	bool GetConfigValue();
};

#endif
