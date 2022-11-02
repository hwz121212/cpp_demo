
#pragma once

#include "afxwin.h"
#define WM_GETCOMMAND WM_USER + 1988
#include "BaseNewButton.h"


// CDlgBar 对话框
class CDlgBar : public CDialog
{
	DECLARE_DYNAMIC(CDlgBar)

public:
	CDlgBar(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgBar();

// 对话框数据
	enum { IDD = IDD_DIALOG_BAR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	afx_msg void OnOutPutStatusButtonUp(UINT);

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	void MoveButton(UINT nID,int nCount);
	BaseNewButton m_btnRect;
	BaseNewButton m_arrow;
	BaseNewButton m_btnEllipse;
	BaseNewButton m_btnLine;
	BaseNewButton m_btnStr;
	BaseNewButton m_btnBack;
	BaseNewButton m_btnSave;
	BaseNewButton m_btnEsc;
	BaseNewButton m_btnOk;
	CToolTipCtrl  m_toolTip;         //提示信息
};
