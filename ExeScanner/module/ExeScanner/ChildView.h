/******************************************************************************
*
* PROJECT:		ExeScanner
* FILE NAME:	ChildView.h
*
* DEVELOPER:	Vishalsinh Jhala
* DATE:			2-Apr-2005
*
* DESCRIPTION:	Declaration file for class CChildView.
*
* NOTES:		FrameWnd instanciates this class. It is responsible for 
*				handling all user commands and creating child views like
*				left tree view and right object view.
*
******************************************************************************/


#pragma once

#include "ExeTree.h"
#include "ExeView.h"
#include "ExeLegend.h"
#include "PEFile.h"
#include "PortableExecutable.h"

// CChildView window

class CChildView : public CWnd
{
// Construction
public:
	CChildView();

// Attributes
public:

// Operations
public:

// Overrides
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// Implementation
public:
	virtual ~CChildView();
	BOOL IsDCValid(){return m_bMemDC;}
	// Generated message map functions
protected:
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()

private:
	CExeTree m_wndTree;//LeftPane that will hold tree control
	CExeLegend m_wndLegend;//Legend (shows properties)


	PEFile *m_PEFile;//PE File
	PortableExecutable *m_PE;//PE Root Object
	PETreeList *m_pTreeList;//List of PE Objects in doubly linked list


	BOOL m_bMemDC;//Is Resource DC Valid
	CString m_strInfo;//Legend	
	CFont m_fntVerdana;
	CImageList m_imglstRes;//Image List for custom toolbar

	void DeletePEObjects();
	void PopulateTreeCtrl();
	void SetImageFromList(HTREEITEM hItem,char *pszName);
	void Load(PEResource *pR);
public:
	BOOL m_bPEHeaderShow;//Initially shows PE Header in GUI.
	int m_iWidth,m_iHeight;//Width and height of DC

	CDC *m_pDC;//Resource DC
	CBitmap *m_pBmp;//Resource Bitmap
	CWnd *m_pWndRes;//Resource window

	CTreeCtrl m_treeExe;//Tree control
	CExeView m_wndView;//Right pane view

public:
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnFileScanpe();
	void OnSelection(LPNMTREEVIEW pTV);
	
	void OnPortableexecutableExport();
	void OnPortableexecutableImport();
	void OnPortableexecutableProperties();

	afx_msg void OnHelpHelp();
	void OnMoveWnd();
	afx_msg void OnToolsDos();
};

