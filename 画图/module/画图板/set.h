#if !defined(AFX_SET_H__F3485309_C262_4E63_84C5_98DAC6650EBE__INCLUDED_)
#define AFX_SET_H__F3485309_C262_4E63_84C5_98DAC6650EBE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// set.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// set dialog

class set : public CDialog
{
// Construction
public:double width;
	set(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(set)
	enum { IDD = IDD_DIALOG1 };
	double	m_1;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(set)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(set)
	afx_msg void OnChangeEdit1();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SET_H__F3485309_C262_4E63_84C5_98DAC6650EBE__INCLUDED_)
