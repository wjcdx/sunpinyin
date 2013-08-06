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

#include "Globals.h"
#include "TextService.h"
#include "CandidateWindow.h"

#define CAND_WIDTH     200
#define CAND_HEIGHT    50

ATOM CCandidateWindow::_atomWndClass = 0;
TCHAR CCandidateWindow::_rgPreeditString[128] = { 0 };
TCHAR CCandidateWindow::_rgCandidatesString[512] = { 0 };

const TCHAR c_szCandidateDescription[] = TEXT("Dummy Candidate Window");

//+---------------------------------------------------------------------------
//
// ctor
//
//----------------------------------------------------------------------------

CCandidateWindow::CCandidateWindow()
{
    _hwnd = NULL;
	memset(_rgPreeditString, 0, sizeof(_rgPreeditString));
	memset(_rgCandidatesString, 0, sizeof(_rgCandidatesString));
}

//+---------------------------------------------------------------------------
//
// _InitWindowClass
//
//----------------------------------------------------------------------------

/* static */
BOOL CCandidateWindow::_InitWindowClass()
{
    WNDCLASS wc;

    wc.style = 0;
    wc.lpfnWndProc = CCandidateWindow::_WindowProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = g_hInst;
    wc.hIcon = NULL;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)GetStockObject(LTGRAY_BRUSH);
    wc.lpszMenuName = NULL;
    wc.lpszClassName = TEXT("TextServiceCandidateWindow");

    _atomWndClass = RegisterClass(&wc);

    return (_atomWndClass != 0);
}


//+---------------------------------------------------------------------------
//
// _UninitClass
//
//----------------------------------------------------------------------------

/* static */
void CCandidateWindow::_UninitWindowClass()
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

BOOL CCandidateWindow::_Create()
{
    _hwnd = CreateWindowEx(WS_EX_TOPMOST | WS_EX_TOOLWINDOW,
                           (LPCTSTR)_atomWndClass,
                           TEXT("TextService Candidate Window"),
                           WS_BORDER | WS_DISABLED | WS_POPUP,
                           0, 0,
                           CAND_WIDTH, CAND_HEIGHT,
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

void CCandidateWindow::_Destroy()
{
    if (_hwnd != NULL)
    {
        DestroyWindow(_hwnd);
        _hwnd = NULL;
    }
}

void CCandidateWindow::_InvalidateRect()
{
    if (_hwnd != NULL)
    {
        InvalidateRect(_hwnd, NULL, TRUE);
    }
}

HRESULT CCandidateWindow::_Update(TCHAR *preedit, TCHAR *candidates)
{
	_InvalidateRect();
	return S_OK;
}

//+---------------------------------------------------------------------------
//
// _Move
//
//----------------------------------------------------------------------------

void CCandidateWindow::_Move(int x, int y)
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

void CCandidateWindow::_Show()
{
    ShowWindow(_hwnd, SW_SHOWNA);
}

//+---------------------------------------------------------------------------
//
// _Hide
//
//----------------------------------------------------------------------------

void CCandidateWindow::_Hide()
{
    ShowWindow(_hwnd, SW_HIDE);
}

//+---------------------------------------------------------------------------
//
// _OnKeyDown
//
//----------------------------------------------------------------------------

HRESULT CCandidateWindow::_OnKeyDown(UINT uVKey)
{
    return S_OK;
}

//+---------------------------------------------------------------------------
//
// _OnKeyUp
//
//----------------------------------------------------------------------------

HRESULT CCandidateWindow::_OnKeyUp(UINT uVKey)
{
    return S_OK;
}

//+---------------------------------------------------------------------------
//
// _WindowProc
//
// Cand window proc.
//----------------------------------------------------------------------------

LRESULT CALLBACK CCandidateWindow::_WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    HDC hdc;
    PAINTSTRUCT ps;
	TEXTMETRIC tm;

    switch (uMsg)
    {
        case WM_CREATE:
            _SetThis(hwnd, lParam);
            return 0;

        case WM_PAINT:
            hdc = BeginPaint(hwnd, &ps);
            SetBkMode(hdc, TRANSPARENT);

			GetTextMetrics(hdc, &tm);
			int x = tm.tmAveCharWidth;
			int y = tm.tmHeight + tm.tmExternalLeading;

			TextOut(hdc, 0, 0, _rgPreeditString, lstrlen(_rgPreeditString));
			TextOut(hdc, 0, y, _rgCandidatesString, lstrlen(_rgCandidatesString));

            EndPaint(hwnd, &ps);
            return 0;
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

