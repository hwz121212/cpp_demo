// ntTelnet.h : main header file for the ntTelnet application
//

#if !defined(AFX_ntTelnet_H__B02F4388_2122_422F_88B8_DBE69674A41B__INCLUDED_)
#define AFX_ntTelnet_H__B02F4388_2122_422F_88B8_DBE69674A41B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CntTelnetApp:
// See ntTelnet.cpp for the implementation of this class
//

class CntTelnetApp : public CWinApp
{
public:
	CntTelnetApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CntTelnetApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CntTelnetApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ntTelnet_H__B02F4388_2122_422F_88B8_DBE69674A41B__INCLUDED_)
