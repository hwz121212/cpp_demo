// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__74189C4D_0902_4BBE_A3C5_2A4952FC67C7__INCLUDED_)
#define AFX_MAINFRM_H__74189C4D_0902_4BBE_A3C5_2A4952FC67C7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//#include"homework11.cpp "
class CMainFrame : public CFrameWnd
{
	
protected: // create from serialization only
public:static int shape;
	static double size;	
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Attributes

static int 	 kind;
static int 	Colour;
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CStatusBar  m_wndStatusBar;
	CToolBar    m_wndToolBar;

// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void Onpencil();
	afx_msg void Onbrush();
	afx_msg void Onerase();
	afx_msg void Onsetwidth();
	afx_msg void Oncolour();
	afx_msg void Oncircle();
	afx_msg void Onrect();
	afx_msg void Onduobianxing();
	afx_msg void Ontuoyuan();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__74189C4D_0902_4BBE_A3C5_2A4952FC67C7__INCLUDED_)
