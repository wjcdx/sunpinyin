
// win_standalone.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// Cwin_standaloneApp:
// �йش����ʵ�֣������ win_standalone.cpp
//

class Cwin_standaloneApp : public CWinApp
{
public:
	Cwin_standaloneApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern Cwin_standaloneApp theApp;