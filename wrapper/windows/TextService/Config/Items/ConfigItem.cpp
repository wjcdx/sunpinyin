#include "ConfigItem.h"
#include "../sunpinyin_config.h"

CConfigItem::CConfigItem()
	: _hwnd(NULL), _pConfig(NULL), _szClassName(NULL), _iStyle(0), _szText(NULL), _szKey(NULL)
{
}

CConfigItem::CConfigItem(SunPinyinConfig *config, TCHAR *clazz, int style, TCHAR *txt, TCHAR *key)
	: _hwnd(NULL), _pConfig(config), _szClassName(clazz), _iStyle(style), _szText(txt), _szKey(key)
{
}

CConfigItem::~CConfigItem()
{
}

void
CConfigItem::OnConfigChanged(int code)
{
}
