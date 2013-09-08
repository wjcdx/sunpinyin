//////////////////////////////////////////////////////////////////////
//
//  THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
//  ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED
//  TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
//  PARTICULAR PURPOSE.
//
//  Copyright (C) 2003  Microsoft Corporation.  All rights reserved.
//
//  Globals.h
//
//          Global variable declarations.
//
//////////////////////////////////////////////////////////////////////

#ifndef GLOBALS_H
#define GLOBALS_H

#include <windows.h>
#include <Windowsx.h>
#include <ole2.h>
#include <olectl.h>
#include <assert.h>
#include "msctf.h"

#include <ime-core/imi_keys.h>

void DllAddRef();
void DllRelease();

#define TEXTSERVICE_LANGID    MAKELANGID(LANG_CHINESE_SIMPLIFIED, SUBLANG_CHINESE_SIMPLIFIED)

#define TEXTSERVICE_DESC    L"XingHua Input Method"
#define TEXTSERVICE_DESC_A   "XingHua Input Method"
#define TEXTSERVICE_MODEL   TEXT("Apartment")

#define TEXTSERVICE_ICON_INDEX  0
#define LANGBAR_ITEM_DESC   L"Sample Text Service Button"
#define LANGBAR_MENU_DESC   L"Sample Text Service Menu"

//+---------------------------------------------------------------------------
//
// SafeStringCopy
//
// Copies a string from one buffer to another.  wcsncpy does not always
// null-terminate the destination buffer; this function does.
//----------------------------------------------------------------------------

inline void SafeStringCopy(WCHAR *pchDst, ULONG cchMax, const WCHAR *pchSrc)
{
    if (cchMax > 0)
    {
        wcsncpy(pchDst, pchSrc, cchMax);
        pchDst[cchMax-1] = '\0';
    }
}

extern HINSTANCE g_hInst;

extern LONG g_cRefDll;

extern CRITICAL_SECTION g_cs;

extern const CLSID c_clsidTextService;

extern const GUID c_guidProfile;

extern const GUID c_guidLangBarItemButton;
extern const GUID c_guidLangBarItemMenu;

extern const GUID c_guidDisplayAttributeInput;
extern const GUID c_guidDisplayAttributeConverted;

WCHAR ConvertVKey(UINT code);

int CharToWchar(WCHAR *wchStr, const char *chStr);

#endif // GLOBALS_H
