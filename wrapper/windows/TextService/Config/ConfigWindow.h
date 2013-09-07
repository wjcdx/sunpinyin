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

#ifndef CANDIDATEWINDOW_H
#define CANDIDATEWINDOW_H

#include <portability.h>

class SunPinyinConfig;

#define NUM 8

//+---------------------------------------------------------------------------
//
// CCandidateWindow
//
//----------------------------------------------------------------------------

class CConfigWindow 
{
public:

	struct ButtonInfo
	{
		int iStyle ;
		TCHAR *szText ;
		TCHAR *szKey;
	};

    CConfigWindow(SunPinyinConfig *config);

    static BOOL _InitWindowClass();
    static void _UninitWindowClass();

    BOOL _Create();
    void _Destroy();
	void _InvalidateRect();
	HRESULT _Update();

    void _Move(int x, int y);
    void _Show();
    void _Hide();

	void _InitWindowItems() {}

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

	static void _OnConfigChanged(int id);

    static ATOM _atomWndClass;
	static ButtonInfo _buttons[NUM];
	static HWND _hwndButtons[NUM];
    HWND _hwnd;
	SunPinyinConfig *_pConfig;
};

#endif // CANDIDATEWINDOW_H
