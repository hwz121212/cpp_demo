#if !defined(AFX_LINEWIDTHDLG_H__A137E825_5890_428C_97C4_4CFDED198B4C__INCLUDED_)
#define AFX_LINEWIDTHDLG_H__A137E825_5890_428C_97C4_4CFDED198B4C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LinewidthDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CLinewidthDlg dialog

class CLinewidthDlg : public CDialog
{
// Construction
public:
	CLinewidthDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CLinewidthDlg)
	enum { IDD = IDD_WIDTHDLG };
	int		m_radio1;
	int		m_radio2;
	int		m_radio3;
	int		m_radio4;
	int		m_radio5;
	//}}AFX_DATA
public:
	CBrush m_brush;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLinewidthDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CLinewidthDlg)
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL OnInitDialog();
	afx_msg void OnRadio1();
	afx_msg void OnRadio2();
	afx_msg void OnRadio3();
	afx_msg void OnRadio4();
	afx_msg void OnRadio5();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LINEWIDTHDLG_H__A137E825_5890_428C_97C4_4CFDED198B4C__INCLUDED_)
