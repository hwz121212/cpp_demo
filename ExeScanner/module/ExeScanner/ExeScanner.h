/******************************************************************************
*
* PROJECT:		ExeScanner
* FILE NAME:	ExeScanner.h
*
* DEVELOPER:	Vishalsinh Jhala
* DATE:			2-Apr-2005
*
* DESCRIPTION:	Declaration file for class CExeScannerApp.
*
* NOTES:		Holds the main app object. Machine generated.
*
******************************************************************************/
//
#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols


// CExeScannerApp:
// See ExeScanner.cpp for the implementation of this class
//

class CExeScannerApp : public CWinApp
{
public:
	CExeScannerApp();


// Overrides
public:
	virtual BOOL InitInstance();
	
// Implementation

public:
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CExeScannerApp theApp;