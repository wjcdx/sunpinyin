#ifndef CONFIG_TEXT_EDIT_H
#define CONFIG_TEXT_EDIT_H

#include <portability.h>
#include "ConfigItem.h"
#include "../GVariant.h"

//+---------------------------------------------------------------------------
//
// CConfigButton
//
//----------------------------------------------------------------------------

class CConfigEdit : public CConfigItem
{
public:
	CConfigEdit()
		: CConfigItem()
	{}
	CConfigEdit(SunPinyinConfig *config, int style, TCHAR *txt, TCHAR *key)
		: CConfigItem(config, TEXT("EDIT"), style, txt, key)
	{
		_iStyle = WS_BORDER | ES_LEFT;
	}
	virtual ~CConfigEdit() {}

	void Init();
	void OnConfigChanged();

private:

	GVariant PrepareValue(std::string &val);
	std::string GetConfigValue();
};

#endif
