#ifndef LANGBAR_ITEM_INFO_H
#define LANGBAR_ITEM_INFO_H

class CLangBarItemInfo
{
public:
	CLangBarItemInfo() {};
	~CLangBarItemInfo() {};

	void setIcon(int icon) { _icon = icon; };
	int getIcon() { return _icon; }

	void setText(std::string text) { _text = text; }
	std::string getText() { return _text; }

	void setTooltip(std::string tooltip) { _tooltip = tooltip; }
	std::string getTooltip() { return _tooltip; }

private:
	//std::string _icon;
	int _icon;
	std::string _text;
	std::string _tooltip;
};


#endif
