
// win_standaloneDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "win_standalone.h"
#include "win_standaloneDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Cwin_standaloneDlg �Ի���



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


// Cwin_standaloneDlg ��Ϣ�������

BOOL Cwin_standaloneDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	pWinHandler = new CWinHandler(&CandidateListVar, NULL);
	pview = init_sunpinyin(pWinHandler);

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void Cwin_standaloneDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR Cwin_standaloneDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void Cwin_standaloneDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDialog::OnOK();
}


void Cwin_standaloneDlg::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDialog::OnCancel();
}


void Cwin_standaloneDlg::OnEnChangeEdit1()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialog::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
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

