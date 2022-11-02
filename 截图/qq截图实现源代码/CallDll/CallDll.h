// CallDll.h : main header file for the CALLDLL application
//

#if !defined(AFX_CALLDLL_H__90B1D07F_F1DC_45C8_902E_61FF8CC70C10__INCLUDED_)
#define AFX_CALLDLL_H__90B1D07F_F1DC_45C8_902E_61FF8CC70C10__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CCallDllApp:
// See CallDll.cpp for the implementation of this class
//

class CCallDllApp : public CWinApp
{
public:
	CCallDllApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCallDllApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CCallDllApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CALLDLL_H__90B1D07F_F1DC_45C8_902E_61FF8CC70C10__INCLUDED_)
