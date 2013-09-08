#include "ConfigButton.h"
#include "../sunpinyin_config.h"

void
CConfigButton::Init()
{
	bool state = GetConfigValue();
	Button_SetCheck(_hwnd, state ? BST_CHECKED : BST_UNCHECKED);
}

void
CConfigButton::OnConfigChanged(int code)
{
	bool state = (Button_GetCheck(_hwnd) == BST_CHECKED);
	_pConfig->on_config_value_changed(_szKey, PrepareValue(state));
}

GVariant
CConfigButton::PrepareValue(bool state)
{
	return GVariant(state);
}

bool CConfigButton::GetConfigValue()
{
	GVariant gvar = _pConfig->config_get_value(_szKey);
	return gvar.GetBool();
}
