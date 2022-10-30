#if !defined(AFX_FONTD_H__3858A01A_215E_472B_B730_8D6B7EBE539D__INCLUDED_)
#define AFX_FONTD_H__3858A01A_215E_472B_B730_8D6B7EBE539D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FontD.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFontD dialog

class CFontD : public CDialog
{
// Construction
public:
	CFontD(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CFontD)
	enum { IDD = IDD_INSERTFONT };
	CString	m_string;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFontD)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CFontD)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FONTD_H__3858A01A_215E_472B_B730_8D6B7EBE539D__INCLUDED_)
