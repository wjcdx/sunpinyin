
// win_standaloneDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "win_standalone.h"
#include "win_standaloneDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Cwin_standaloneDlg 对话框



Cwin_standaloneDlg::Cwin_standaloneDlg(CWnd* pParent /*=NULL*/)
	: CDialog(Cwin_standaloneDlg::IDD, pParent)
	, InputString(_T(""))
	, CandidateListVar(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

Cwin_standaloneDlg::~Cwin_standaloneDlg()
{
	finit_sunpinyin(pview, pWinHandler);
	/*if (pview != NULL) {
		delete pview;
	}
	if (pWinHandler != NULL) {
		delete pWinHandler;
	}*/
}

void Cwin_standaloneDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, InputString);
	DDX_Text(pDX, CandidateList, CandidateListVar);
}

BEGIN_MESSAGE_MAP(Cwin_standaloneDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &Cwin_standaloneDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &Cwin_standaloneDlg::OnBnClickedCancel)
	ON_EN_CHANGE(IDC_EDIT1, &Cwin_standaloneDlg::OnEnChangeEdit1)
END_MESSAGE_MAP()


// Cwin_standaloneDlg 消息处理程序

BOOL Cwin_standaloneDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	pWinHandler = new CWinHandler(&CandidateListVar, NULL);
	pview = init_sunpinyin(pWinHandler);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void Cwin_standaloneDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR Cwin_standaloneDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void Cwin_standaloneDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialog::OnOK();
}


void Cwin_standaloneDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialog::OnCancel();
}


void Cwin_standaloneDlg::OnEnChangeEdit1()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	CKeyEvent *keyEvent = NULL;
	int prvLen = InputString.GetLength();
	UpdateData(true);

	int thsLen = InputString.GetLength();
	if (thsLen > prvLen) {
		char c = InputString.GetAt(thsLen - 1);
		keyEvent = new CKeyEvent(c, c, 0);
	} else {
		keyEvent = new CKeyEvent(IM_VK_BACK_SPACE, IM_VK_BACK_SPACE, 0);
	}

	key_press_cb(keyEvent, pview);
	delete keyEvent;

	UpdateData(false);
}

