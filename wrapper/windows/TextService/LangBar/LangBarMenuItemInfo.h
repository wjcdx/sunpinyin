#ifndef LANGBAR_MENUITEM_INFO_H
#define LANGBAR_MENUITEM_INFO_H



class CLangBarMenuItemInfo
{
public:
	
	CLangBarMenuItemInfo() {}
	~CLangBarMenuItemInfo() {}

	void SetText(std::string text) { _Text = text; }
	std::string GetText() { return _Text; }

	void SetBmp(UINT bmp) { _uiBmp = bmp; }
	UINT GetBmp() { return _uiBmp; }

private:
	
	std::string _Text;
	UINT _uiBmp;
};

#endif
