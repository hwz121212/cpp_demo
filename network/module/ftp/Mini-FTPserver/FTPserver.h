// FTPserver.h : main header file for the FTPSERVER application
//

#if !defined(AFX_FTPSERVER_H__CA25D189_9924_412A_BCD1_0E77D9D7C4F8__INCLUDED_)
#define AFX_FTPSERVER_H__CA25D189_9924_412A_BCD1_0E77D9D7C4F8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols
#include "SystemTray.h"

/////////////////////////////////////////////////////////////////////////////
// CFTPserverApp:
// See FTPserver.cpp for the implementation of this class
//

#define WM_ICON_NOTIFY WM_USER+111

class CFTPserverApp : public CWinApp
{
public:
	CFTPserverApp();
	CSystemTray m_TrayIcon;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFTPserverApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CFTPserverApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	afx_msg void OnAppShow();
	afx_msg void OnAppExit();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FTPSERVER_H__CA25D189_9924_412A_BCD1_0E77D9D7C4F8__INCLUDED_)
