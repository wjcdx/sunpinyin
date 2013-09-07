//////////////////////////////////////////////////////////////////////
//
//  THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
//  ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED
//  TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
//  PARTICULAR PURPOSE.
//
//  Copyright (C) 2003  Microsoft Corporation.  All rights reserved.
//
//  CandidateWindow.cpp
//
//          CCandidateWindow class
//
//////////////////////////////////////////////////////////////////////

#include "../Globals.h"
#include "ConfigWindow.h"
#include "sunpinyin_config.h"
#include "sunpinyin_config_keys.h"

#define CAND_WIDTH     400
#define CAND_HEIGHT    40
#define CAND_LENGTH    30

ATOM CConfigWindow::_atomWndClass = 0;

static const TCHAR c_szCandidateDescription[] = TEXT("Dummy Config Window");

//+---------------------------------------------------------------------------
//
// ctor
//
//----------------------------------------------------------------------------

CConfigWindow::CConfigWindow(SunPinyinConfig *config)
	: _pConfig(config)
{
    _hwnd = NULL;
	_InitWindowClass();
}

//+---------------------------------------------------------------------------
//
// _InitWindowClass
//
//----------------------------------------------------------------------------

/* static */
BOOL CConfigWindow::_InitWindowClass()
{
    WNDCLASS wc;

    wc.style = 0;
    wc.lpfnWndProc = CConfigWindow::_WindowProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = g_hInst;
    wc.hIcon = NULL;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)GetStockObject(LTGRAY_BRUSH);
    wc.lpszMenuName = NULL;
    wc.lpszClassName = TEXT("TextServiceConfigWindow");

    _atomWndClass = RegisterClass(&wc);

    return (_atomWndClass != 0);
}


//+---------------------------------------------------------------------------
//
// _UninitClass
//
//----------------------------------------------------------------------------

/* static */
void CConfigWindow::_UninitWindowClass()
{
    if (_atomWndClass != 0)
    {
        UnregisterClass((LPCTSTR)_atomWndClass, g_hInst);
    }
}


//+---------------------------------------------------------------------------
//
// _Create
//
//----------------------------------------------------------------------------

BOOL CConfigWindow::_Create()
{
    _hwnd = CreateWindowEx(0,
                           (LPCTSTR)_atomWndClass,
                           TEXT("TextService Candidate Window"),
                           WS_OVERLAPPEDWINDOW,
                           CW_USEDEFAULT, CW_USEDEFAULT,
                           CW_USEDEFAULT, CW_USEDEFAULT,
                           NULL,
                           NULL,
                           g_hInst,
                           this);

    return (_hwnd != NULL);
}

//+---------------------------------------------------------------------------
//
// _Destroy
//
//----------------------------------------------------------------------------

void CConfigWindow::_Destroy()
{
    if (_hwnd != NULL)
    {
        DestroyWindow(_hwnd);
        _hwnd = NULL;
    }
}

void CConfigWindow::_InvalidateRect()
{
    if (_hwnd != NULL)
    {
        InvalidateRect(_hwnd, NULL, TRUE);
    }
}

HRESULT CConfigWindow::_Update()
{
	_InvalidateRect();
	return S_OK;
}

//+---------------------------------------------------------------------------
//
// _Move
//
//----------------------------------------------------------------------------

void CConfigWindow::_Move(int x, int y)
{
    if (_hwnd != NULL)
    {
        RECT rc;
        GetWindowRect(_hwnd, &rc);
        MoveWindow(_hwnd, x, y, rc.right - rc.left, rc.bottom - rc.top, TRUE);
    }
}

//+---------------------------------------------------------------------------
//
// _Show
//
//----------------------------------------------------------------------------

void CConfigWindow::_Show()
{
    ShowWindow(_hwnd, SW_SHOWNA);
}

//+---------------------------------------------------------------------------
//
// _Hide
//
//----------------------------------------------------------------------------

void CConfigWindow::_Hide()
{
    ShowWindow(_hwnd, SW_HIDE);
}


CConfigWindow::ButtonInfo CConfigWindow::_buttons[] =
{
	//BS_PUSHBUTTON, TEXT ("PUSHBUTTON"),
	//BS_DEFPUSHBUTTON, TEXT ("DEFPUSHBUTTON"),
	//BS_CHECKBOX, TEXT ("CHECKBOX"),
	BS_AUTOCHECKBOX, TEXT ("Init Mode Chinese"), TEXT (CONFIG_GENERAL_INITIAL_MODE),
	//BS_RADIOBUTTON, TEXT ("RADIOBUTTON"),
	//BS_3STATE, TEXT ("3STATE"),
	//BS_AUTO3STATE, TEXT ("AUTO3STATE"),
	//BS_GROUPBOX, TEXT ("GROUPBOX"),
	//BS_AUTORADIOBUTTON, TEXT ("AUTORADIO"),
	//BS_OWNERDRAW, TEXT ("OWNERDRAW")
};

//+---------------------------------------------------------------------------
//
// _WindowProc
//
// Cand window proc.
//----------------------------------------------------------------------------

LRESULT CALLBACK CConfigWindow::_WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    HDC hdc;
	static int cxChar, cyChar;
	int i = 0;
	int id = 0;
	bool state = false;


    switch (uMsg)
    {
        case WM_CREATE:
            _SetThis(hwnd, lParam);

			cxChar = LOWORD (GetDialogBaseUnits ()) ;
			cyChar = HIWORD (GetDialogBaseUnits ()) ;
			for (i = 0 ; i < NUM ; i++) {
				_hwndButtons[i] = CreateWindow(
					TEXT("button"), _buttons[i].szText,
					WS_CHILD | WS_VISIBLE | _buttons[i].iStyle,
					cxChar, cyChar * (1 + 2 * i),
					20 * cxChar, 7 * cyChar / 4,
					hwnd, (HMENU) i,
					((LPCREATESTRUCT) lParam)->hInstance,
					NULL);
			}
			return 0 ;

   //     case WM_PAINT:

			//hdc = BeginPaint(hwnd, &ps);
   //         SetBkMode(hdc, TRANSPARENT);

			//GetTextMetrics(hdc, &tm);
			//int x = tm.tmAveCharWidth;
			//int y = tm.tmHeight + tm.tmExternalLeading;

   //         EndPaint(hwnd, &ps);
   //         return 0;
		case WM_DRAWITEM:
		case WM_COMMAND:

			id = LOWORD (wParam);
			_OnConfigChanged(id);

			break ;
		case WM_DESTROY :
			PostQuitMessage (0) ;
		return 0 ;
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

void
CConfigWindow::_OnConfigChanged(int id)
{
	bool state = false;
	state = SendMessage ((HWND) _hwndButtons[id], BM_GETCHECK, 0, 0);
	_pConfig->
}

