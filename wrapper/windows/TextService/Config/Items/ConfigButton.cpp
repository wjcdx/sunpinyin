#include "ConfigButton.h"
#include "../sunpinyin_config.h"

void
CConfigButton::Init()
{
	bool state = GetConfigValue();
	SendMessage (_hwnd, BM_SETCHECK, state, 0);
}

void
CConfigButton::OnConfigChanged()
{
	bool state = SendMessage (_hwnd, BM_GETCHECK, 0, 0);
	_pConfig->on_config_value_changed(_szKey, PrepareValue(state));
}

GVariant
CConfigButton::PrepareValue(bool state)
{
	if (!_bString)
		return GVariant(state);

	if (state) {
		return GVariant(_trueString);
	}
	else {
		return GVariant(_falseString);
	}
}

bool CConfigButton::GetConfigValue()
{
	bool state = false;
	GVariant gvar = _pConfig->config_get_value(_szKey);

	if (!_bString)
		return gvar.GetBool();

	if (_trueString == gvar.GetString())
		return true;
	return false;
}
