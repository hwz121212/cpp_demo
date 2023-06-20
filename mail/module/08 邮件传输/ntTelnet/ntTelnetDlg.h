// ntTelnetDlg.h : header file
//

#if !defined(AFX_ntTelnetDLG_H__FB8CD439_433B_42C8_86D4_9B179CE9D000__INCLUDED_)
#define AFX_ntTelnetDLG_H__FB8CD439_433B_42C8_86D4_9B179CE9D000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CntTelnetDlg dialog
#include "CmdEditCtrl.h"
class CntTelnetDlg : public CDialog
{
// Construction
public:
	WSADATA ws;
	SOCKET sock;
	SOCKADDR_IN addr_in;
	BOOL bConnection;

	BOOL UrlToIp(char* szIp,char* szUrl);
	CntTelnetDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CntTelnetDlg)
	enum { IDD = IDD_ntTelnet_DIALOG };
	CCmdEditCtrl	m_edit;
	UINT	m_uPort;
	CString	m_strUrl;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CntTelnetDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CntTelnetDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnConnection();
	afx_msg void OnDisConnection();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ntTelnetDLG_H__FB8CD439_433B_42C8_86D4_9B179CE9D000__INCLUDED_)
