//////////////////////////////////////////////////////////////////////
//
//  THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
//  ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED
//  TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
//  PARTICULAR PURPOSE.
//
//  Copyright (C) 2003  Microsoft Corporation.  All rights reserved.
//
//  CandidateWindow.h
//
//          CCandidateWindow declaration.
//
//////////////////////////////////////////////////////////////////////

#ifndef CONFIG_ITEM_H
#define CONFIG_ITEM_H

#include <portability.h>
class SunPinyinConfig;

//+---------------------------------------------------------------------------
//
// CConfigItem
//
//----------------------------------------------------------------------------

class CConfigItem
{
public:
	CConfigItem();
	CConfigItem(SunPinyinConfig *config, TCHAR *clazz, int style, TCHAR *txt, TCHAR *key);
	virtual ~CConfigItem();

	virtual void Init() {}
	virtual void OnConfigChanged();

	void SetHwnd(HWND hwnd) { _hwnd = hwnd; }

	TCHAR *GetClass() { return _szClassName; }
	int GetStyle() { return _iStyle; }
	TCHAR *GetText() { return _szText; }

protected:

    HWND _hwnd;
	SunPinyinConfig *_pConfig;

	TCHAR *_szClassName;
	int _iStyle;
	TCHAR *_szText;
	TCHAR *_szKey;
};

#endif // CONFIG_ITEM_H
