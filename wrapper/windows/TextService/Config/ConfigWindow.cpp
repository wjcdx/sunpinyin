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
#include "ConfigItem.h"
#include "ConfigButton.h"
#include "ConfigEdit.h"



ATOM CConfigWindow::_atomWndClass = 0;
SunPinyinConfig *CConfigWindow::_pConfig = NULL;
std::map<int, CConfigItem *> CConfigWindow::_ConfigItems;


//+---------------------------------------------------------------------------
//
// ctor
//
//----------------------------------------------------------------------------

CConfigWindow::CConfigWindow(SunPinyinConfig *config)
{
    _hwnd = NULL;
	_InitWindowClass();

	_pConfig = config;
	_SetupConfigItems();
}

CConfigWindow::~CConfigWindow()
{
	std::map<int, CConfigItem *>::iterator it = _ConfigItems.begin();
	std::map<int, CConfigItem *>::iterator ite = _ConfigItems.end();

	for (; it != ite;) {
		delete it->second;
		_ConfigItems.erase(it++);
	}
}


//+---------------------------------------------------------------------------
//
// _InitWindowClass
//
//----------------------------------------------------------------------------

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


//+---------------------------------------------------------------------------
//
// _WindowProc
//
// Cand window proc.
//----------------------------------------------------------------------------

LRESULT CALLBACK CConfigWindow::_WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    HDC hdc;
	HWND hwndItem;
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
			for (i = 0 ; i < _ConfigItems.size(); i++) {
				hwndItem = CreateWindow( 
					_ConfigItems[i]->GetClass(), _ConfigItems[i]->GetText(),
					WS_CHILD | WS_VISIBLE | _ConfigItems[i]->GetStyle(),
					cxChar, cyChar * (1 + 2 * i),
					20 * cxChar, 7 * cyChar / 4,
					hwnd, (HMENU) i,
					((LPCREATESTRUCT) lParam)->hInstance,
					NULL);
				_ConfigItems[i]->SetHwnd(hwndItem);
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
			_ConfigItems[id]->OnConfigChanged();

			break ;
		case WM_DESTROY :
			_pConfig->save_configs();
			PostQuitMessage (0) ;
			return 0 ;
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

void
CConfigWindow::_SetupConfigItems()
{
	CConfigButton * btn = new CConfigButton(_pConfig, BS_AUTOCHECKBOX,
		TEXT ("Init Mode Chinese"), TEXT (CONFIG_GENERAL_INITIAL_MODE));
	btn->SetupString(true, TEXT ("Chinese"), TEXT ("English"));
	_ConfigItems[0] = btn;

	CConfigEdit *edit = new CConfigEdit(_pConfig, 0, TEXT("History Power"),
		TEXT (CONFIG_GENERAL_MEMORY_POWER));
	_ConfigItems[1] = edit;
}

void
CConfigWindow::_InitConfigItems()
{
	std::map<int, CConfigItem *>::iterator it = _ConfigItems.begin();
	std::map<int, CConfigItem *>::iterator ite = _ConfigItems.end();

	for (; it != ite; it++) {
		it->second->Init();
	}
}
