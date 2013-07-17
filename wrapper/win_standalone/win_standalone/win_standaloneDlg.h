
// win_standaloneDlg.h : 头文件
//

#pragma once

#include "sunpinyin.h"
#include "imi_gtkwin.h"

// Cwin_standaloneDlg 对话框
class Cwin_standaloneDlg : public CDialog
{
// 构造
public:
	Cwin_standaloneDlg(CWnd* pParent = NULL);	// 标准构造函数
	Cwin_standaloneDlg::~Cwin_standaloneDlg();

// 对话框数据
	enum { IDD = IDD_WIN_STANDALONE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
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
