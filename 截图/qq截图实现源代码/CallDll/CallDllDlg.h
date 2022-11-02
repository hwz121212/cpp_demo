// CallDllDlg.h : header file
//

#if !defined(AFX_CALLDLLDLG_H__8255186F_BB1E_4687_9CDF_2476ADBCD9DD__INCLUDED_)
#define AFX_CALLDLLDLG_H__8255186F_BB1E_4687_9CDF_2476ADBCD9DD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CCallDllDlg dialog

class CCallDllDlg : public CDialog
{
// Construction
public:
	CCallDllDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CCallDllDlg)
	enum { IDD = IDD_CALLDLL_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCallDllDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CCallDllDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnButtonCall();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CALLDLLDLG_H__8255186F_BB1E_4687_9CDF_2476ADBCD9DD__INCLUDED_)
