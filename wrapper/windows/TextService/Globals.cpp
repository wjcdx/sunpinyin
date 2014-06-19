//////////////////////////////////////////////////////////////////////
//
//  THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
//  ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED
//  TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
//  PARTICULAR PURPOSE.
//
//  Copyright (C) 2003  Microsoft Corporation.  All rights reserved.
//
//  Globals.cpp
//
//          Global variables
//
//////////////////////////////////////////////////////////////////////

#include "globals.h"


HINSTANCE g_hInst;

LONG g_cRefDll = -1; // -1 /w no refs, for win95 InterlockedIncrement/Decrement compat

CRITICAL_SECTION g_cs;

/* e7ea138e-69f8-11d7-a6ea-00065b84435c */
const CLSID c_clsidTextService = {
    0xe7ea138e,
    0x69f8,
    0x11d7,
    {0xa6, 0xea, 0x00, 0x06, 0x5b, 0x84, 0x43, 0x5c}
  };
/* e7ea138f-69f8-11d7-a6ea-00065b84435c */
const GUID c_guidProfile = {
    0xe7ea138f,
    0x69f8,
    0x11d7,
    {0xa6, 0xea, 0x00, 0x06, 0x5b, 0x84, 0x43, 0x5c}
  };
/* 41f46e67-86d5-49fb-a1d9-3dc0941a66a3 */
const GUID c_guidLangBarItemButton = {
    0x41f46e67,
    0x86d5,
    0x49fb,
    {0xa1, 0xd9, 0x3d, 0xc0, 0x94, 0x1a, 0x66, 0xa3}
  };

// {9103FF05-665F-49AE-B95D-DB5A3D14336F}
const GUID c_guidLangBarItemMenu = 
{ 0x9103ff05, 0x665f, 0x49ae, { 0xb9, 0x5d, 0xdb, 0x5a, 0x3d, 0x14, 0x33, 0x6f } };


//
//  define two guids for display attribute info. This textservice has
//  two display attribute. One is for input text and the other is for the
//  converted text.
//
//      c_guidDisplayAttributeInput 
//      c_guidDisplayAttributeConverted
//
/* 4e1aa3fe-6c7f-11d7-a6ec-00065b84435c */
const GUID c_guidDisplayAttributeInput = { 
    0x4e1aa3fe,
    0x6c7f,
    0x11d7,
    {0xa6, 0xec, 0x00, 0x06, 0x5b, 0x84, 0x43, 0x5c}
  };
/* 4e1aa3ff-6c7f-11d7-a6ec-00065b84435c */
const GUID c_guidDisplayAttributeConverted = { 
    0x4e1aa3ff,
    0x6c7f,
    0x11d7,
    {0xa6, 0xec, 0x00, 0x06, 0x5b, 0x84, 0x43, 0x5c}
  };

std::string g_szHomePath;

//+---------------------------------------------------------------------------
//
// ConvertVKey
//
//----------------------------------------------------------------------------

WCHAR ConvertVKey(UINT code)
{
    //
    // Map virtual key to scan code
    //
    UINT scanCode = 0;
    scanCode = MapVirtualKey(code, 0);

    //
    // Keyboard state
    //
    BYTE abKbdState[256] = {'\0'};
    if (!GetKeyboardState(abKbdState))
    {
        return 0;
    }

    //
    // Map virtual key to character code
    //
    WCHAR wch = '\0';
    if (ToUnicode(code, scanCode, abKbdState, &wch, 1, 0) == 1)
    {
        return wch;
    }

    return 0;
}

int CharToWchar(WCHAR *wchStr, const char *chStr)
{
	int i = 0;
	for (; chStr[i] != '\0'; i++)
	{
		wchStr[i] = chStr[i];
	}
	return i ? (i + 1) : 0;
}

BOOL GetHomePath(std::string &path)
{
	HKEY hKey;
	DWORD dwRet;
	TCHAR tcPath[MAX_PATH] = {0};
	DWORD dwLen = MAX_PATH * sizeof(TCHAR);
	LPCTSTR lpszSubKey = "SOFTWARE\\XHua";

	dwRet = RegOpenKey(HKEY_LOCAL_MACHINE, lpszSubKey, &hKey);
	if(dwRet != ERROR_SUCCESS)
	{
		return FALSE;
	}
	dwRet = RegQueryValueEx(hKey, TEXT("Path"), NULL, NULL, (BYTE *)tcPath, &dwLen);
	if (dwRet == ERROR_SUCCESS) {
		path = tcPath;
	}
	RegCloseKey(hKey);
	return (dwRet == ERROR_SUCCESS);
}

namespace Global {
//+---------------------------------------------------------------------------
//
// CheckModifiers
//
//----------------------------------------------------------------------------


BOOL CheckModifiers(UINT modCurrent, UINT mod)
{
    mod &= ~TF_MOD_ON_KEYUP;

    if (mod & TF_MOD_IGNORE_ALL_MODIFIER)
    {
        return TRUE;
    }

    if (modCurrent == mod)
    {
        return TRUE;
    }

    if (modCurrent && !mod)
    {
        return FALSE;
    }

    CheckMod(modCurrent, mod, ALT);
    CheckMod(modCurrent, mod, SHIFT);
    CheckMod(modCurrent, mod, CONTROL);

    return TRUE;
}

//+---------------------------------------------------------------------------
//
// UpdateModifiers
//
//    wParam - virtual-key code
//    lParam - [0-15]  Repeat count
//  [16-23] Scan code
//  [24]    Extended key
//  [25-28] Reserved
//  [29]    Context code
//  [30]    Previous key state
//  [31]    Transition state
//----------------------------------------------------------------------------

USHORT ModifiersValue = 0;
BOOL   IsShiftKeyDownOnly = FALSE;
BOOL   IsControlKeyDownOnly = FALSE;
BOOL   IsAltKeyDownOnly = FALSE;

BOOL UpdateModifiers(WPARAM wParam, LPARAM lParam)
{
    // high-order bit : key down
    // low-order bit  : toggled
    SHORT sksMenu = GetKeyState(VK_MENU);
    SHORT sksCtrl = GetKeyState(VK_CONTROL);
    SHORT sksShft = GetKeyState(VK_SHIFT);

    switch (wParam & 0xff)
    {
    case VK_MENU:
        // is VK_MENU down?
        if (sksMenu & 0x8000)
        {
            // is extended key?
            if (lParam & 0x01000000)
            {
                ModifiersValue |= (TF_MOD_RALT | TF_MOD_ALT);
            }
            else
            {
                ModifiersValue |= (TF_MOD_LALT | TF_MOD_ALT);
            }

            // is previous key state up?
            if (!(lParam & 0x40000000))
            {
                // is VK_CONTROL and VK_SHIFT up?
                if (!(sksCtrl & 0x8000) && !(sksShft & 0x8000))
                {
                    IsAltKeyDownOnly = TRUE;
                }
                else
                {
                    IsShiftKeyDownOnly = FALSE;
                    IsControlKeyDownOnly = FALSE;
                    IsAltKeyDownOnly = FALSE;
                }
            }
        }
        break;

    case VK_CONTROL:
        // is VK_CONTROL down?
        if (sksCtrl & 0x8000)    
        {
            // is extended key?
            if (lParam & 0x01000000)
            {
                ModifiersValue |= (TF_MOD_RCONTROL | TF_MOD_CONTROL);
            }
            else
            {
                ModifiersValue |= (TF_MOD_LCONTROL | TF_MOD_CONTROL);
            }

            // is previous key state up?
            if (!(lParam & 0x40000000))
            {
                // is VK_SHIFT and VK_MENU up?
                if (!(sksShft & 0x8000) && !(sksMenu & 0x8000))
                {
                    IsControlKeyDownOnly = TRUE;
                }
                else
                {
                    IsShiftKeyDownOnly = FALSE;
                    IsControlKeyDownOnly = FALSE;
                    IsAltKeyDownOnly = FALSE;
                }
            }
        }
        break;

    case VK_SHIFT:
        // is VK_SHIFT down?
        if (sksShft & 0x8000)    
        {
            // is scan code 0x36(right shift)?
            if (((lParam >> 16) & 0x00ff) == 0x36)
            {
                ModifiersValue |= (TF_MOD_RSHIFT | TF_MOD_SHIFT);
            }
            else
            {
                ModifiersValue |= (TF_MOD_LSHIFT | TF_MOD_SHIFT);
            }

            // is previous key state up?
            if (!(lParam & 0x40000000))
            {
                // is VK_MENU and VK_CONTROL up?
                if (!(sksMenu & 0x8000) && !(sksCtrl & 0x8000))
                {
                    IsShiftKeyDownOnly = TRUE;
                }
                else
                {
                    IsShiftKeyDownOnly = FALSE;
                    IsControlKeyDownOnly = FALSE;
                    IsAltKeyDownOnly = FALSE;
                }
            }
        }
        break;

    default:
        IsShiftKeyDownOnly = FALSE;
        IsControlKeyDownOnly = FALSE;
        IsAltKeyDownOnly = FALSE;
        break;
    }

    if (!(sksMenu & 0x8000))
    {
        ModifiersValue &= ~TF_MOD_ALLALT;
    }
    if (!(sksCtrl & 0x8000))
    {
        ModifiersValue &= ~TF_MOD_ALLCONTROL;
    }
    if (!(sksShft & 0x8000))
    {
        ModifiersValue &= ~TF_MOD_ALLSHIFT;
    }

    return TRUE;
}
};
