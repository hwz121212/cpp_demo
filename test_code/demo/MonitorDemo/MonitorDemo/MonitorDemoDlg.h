
// MonitorDemoDlg.h : 头文件
//

#pragma once
#include "OScopeCtrl.h"
#include "afxcmn.h"

// CMonitorDemoDlg 对话框
class CMonitorDemoDlg : public CDialog
{
// 构造
public:
	CMonitorDemoDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_MONITORDEMO_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;
	//================================
	typedef enum {
		NONE_EVENT = 0,
		CPU_EVENT,
		MEMORY_EVENT,
		NET_IO_EVENT
	}TIMER_EVENT_ID;
	TIMER_EVENT_ID m_timer_Event_id;
	COScopeCtrl m_OScopeCtrl;
	HTREEITEM m_hRoot;
	BOOL m_bStartTimer;
	void InitOScopeCtrl();
	void InsertPcTitleToUserTreeControl();
	void InitUserTreeControl();
	//================================
	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CTreeCtrl m_tree;
	afx_msg void OnBnClickedTimerSwitch();
	afx_msg void OnBnClickedExit();
	
	afx_msg void OnTvnSelchangedUserTree(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
