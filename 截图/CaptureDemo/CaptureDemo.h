// CaptureDemo.h : main header file for the CaptureDemo application
//
#pragma once

#include "resource.h" 


// CCaptureDemoApp:
// See CaptureDemo.cpp for the implementation of this class
//

class CCaptureDemoApp : public CWinApp
{
public:
	CCaptureDemoApp();


// Overrides
public:
	virtual BOOL InitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CCaptureDemoApp theApp;