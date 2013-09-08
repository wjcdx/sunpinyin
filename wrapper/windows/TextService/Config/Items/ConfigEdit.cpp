#include "ConfigEdit.h"
#include "../sunpinyin_config.h"

void
CConfigEdit::Init()
{
	int iStart = 0, iEnd = 0;
	std::string val = GetConfigValue();
	
	SendMessage (_hwnd, EM_GETSEL, (WPARAM) &iStart, (LPARAM) &iEnd);
	SendMessage (_hwnd, EM_SETSEL, iStart, iEnd);
	SendMessage (_hwnd, EM_REPLACESEL, 0, (LPARAM) val.c_str());
}

void
CConfigEdit::OnConfigChanged()
{
	TCHAR szBuffer[512] = { 0 };
	int iLength = SendMessage (_hwnd, EM_GETLINE, 0, (LPARAM) szBuffer);
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
