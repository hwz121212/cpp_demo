// WC.h : main header file for the WC application
//

#if !defined(AFX_WC_H__2962FCD7_C087_46D1_A971_1673F6F486A2__INCLUDED_)
#define AFX_WC_H__2962FCD7_C087_46D1_A971_1673F6F486A2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CWCApp:
// See WC.cpp for the implementation of this class
//

class CWCApp : public CWinApp
{
public:
	CWCApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWCApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CWCApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WC_H__2962FCD7_C087_46D1_A971_1673F6F486A2__INCLUDED_)
