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

#ifndef CONFIGWINDOW_H
#define CONFIGWINDOW_H

#include <portability.h>
#include <map>

class SunPinyinConfig;
class CConfigItem;

//+---------------------------------------------------------------------------
//
// CCandidateWindow
//
//----------------------------------------------------------------------------

class CConfigWindow 
{
public:

    CConfigWindow(SunPinyinConfig *config);
	~CConfigWindow();

    static BOOL _InitWindowClass();
    static void _UninitWindowClass();

    BOOL _Create();
    void _Destroy();
	void _InvalidateRect();
	HRESULT _Update();

    void _Move(int x, int y);
    void _Show();
    void _Hide();

	void _SetupConfigItems();
	void _InitConfigItems();

private:
    static LRESULT CALLBACK _WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

    static void _SetThis(HWND hwnd, LPARAM lParam)
    {
        SetWindowLongPtr(hwnd, GWLP_USERDATA, 
                         (LONG_PTR)((CREATESTRUCT *)lParam)->lpCreateParams);
    }

    static CConfigWindow *_GetThis(HWND hwnd)
    {
        return (CConfigWindow *)GetWindowLongPtr(hwnd, GWLP_USERDATA);
    }

    static ATOM _atomWndClass;
	static std::map<int, CConfigItem *> _ConfigItems;
    HWND _hwnd;
	static SunPinyinConfig *_pConfig;
};

#endif // CONFIGWINDOW_H
