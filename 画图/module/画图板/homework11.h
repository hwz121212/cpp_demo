// homework11.h : main header file for the HOMEWORK11 application
//

#if !defined(AFX_HOMEWORK11_H__D8473943_6DCA_4854_8423_1604B9518EE8__INCLUDED_)
#define AFX_HOMEWORK11_H__D8473943_6DCA_4854_8423_1604B9518EE8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CHomework11App:
// See homework11.cpp for the implementation of this class
//

class CHomework11App : public CWinApp
{
public:
	CHomework11App();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHomework11App)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CHomework11App)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HOMEWORK11_H__D8473943_6DCA_4854_8423_1604B9518EE8__INCLUDED_)
