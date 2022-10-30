
// MonitorDemoDlg.h : ͷ�ļ�
//

#pragma once
#include "OScopeCtrl.h"
#include "afxcmn.h"

// CMonitorDemoDlg �Ի���
class CMonitorDemoDlg : public CDialog
{
// ����
public:
	CMonitorDemoDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_MONITORDEMO_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
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
	// ���ɵ���Ϣӳ�亯��
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
