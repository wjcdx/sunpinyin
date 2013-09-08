#include "ConfigEdit.h"
#include "../sunpinyin_config.h"

void
CConfigEdit::Init()
{
	int iStart = 0, iEnd = 0;
	std::string val = GetConfigValue();
	Edit_SetText(_hwnd, val.c_str());
}

void
CConfigEdit::OnConfigChanged(int code)
{
	TCHAR szBuffer[512] = { 0 };

	if (code != EN_KILLFOCUS)
		return;

	Edit_GetText(_hwnd, szBuffer, 512);
	_pConfig->on_config_value_changed(_szKey, PrepareValue(std::string(szBuffer)));
}

GVariant
CConfigEdit::PrepareValue(std::string &val)
{
	return GVariant(val);
}

std::string
CConfigEdit::GetConfigValue()
{
	GVariant gvar = _pConfig->config_get_value(_szKey);
	return gvar.GetString();
}
