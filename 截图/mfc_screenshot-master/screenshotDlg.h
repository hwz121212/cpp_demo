
// screenshotDlg.h : 头文件
//

#pragma once
#include "afxwin.h"



// CscreenshotDlg 对话框
class CscreenshotDlg : public CDialogEx
{
// 构造
public:
	CscreenshotDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_SCREENSHOT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	
	BOOL PreTranslateMessage(MSG* pMsg);

	afx_msg void OnBnClickedBtnModify();
	CEdit m_edit_ctrl;
	CEdit m_edit_alt;
	CEdit m_edit_key;

	CScreenshotToolDlg* dlg;
	CButton m_cbx_max_width;
	CButton m_cbx_max_height;
	CEdit m_edit_max_width;
	CEdit m_edit_max_height;
	CModelConfig *config;
	afx_msg void OnDestroy();


	afx_msg void OnBnClickedCheckMaxWidth();
	afx_msg void OnBnClickedCheckMaxHeight();
};
