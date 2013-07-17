
// win_standaloneDlg.h : ͷ�ļ�
//

#pragma once

#include "sunpinyin.h"
#include "imi_gtkwin.h"

// Cwin_standaloneDlg �Ի���
class Cwin_standaloneDlg : public CDialog
{
// ����
public:
	Cwin_standaloneDlg(CWnd* pParent = NULL);	// ��׼���캯��
	Cwin_standaloneDlg::~Cwin_standaloneDlg();

// �Ի�������
	enum { IDD = IDD_WIN_STANDALONE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnEnChangeEdit1();
	afx_msg void OnStnClickedCandidatelist();
	CString InputString;
	CString CandidateListVar;

	CString Candidates;
	CString Preedit;
	CIMIWinHandler *pWinHandler;
	CIMIView *pview;
};
