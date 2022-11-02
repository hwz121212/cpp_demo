// ClipScreen.h : main header file for the CLIPSCREEN DLL
//

#if !defined(AFX_CLIPSCREEN_H__8CCBE62F_7EE8_4953_BF99_5E94FE8F94E7__INCLUDED_)
#define AFX_CLIPSCREEN_H__8CCBE62F_7EE8_4953_BF99_5E94FE8F94E7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CClipScreenApp
// See ClipScreen.cpp for the implementation of this class
//

class CClipScreenApp : public CWinApp
{
public:
	CClipScreenApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CClipScreenApp)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CClipScreenApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CLIPSCREEN_H__8CCBE62F_7EE8_4953_BF99_5E94FE8F94E7__INCLUDED_)
