/******************************************************************************
*
* PROJECT:		ExeScanner
* FILE NAME:	ExeTree.h
*
* DEVELOPER:	Vishalsinh Jhala
* DATE:			2-Apr-2005
*
* DESCRIPTION:	Declaration file for class CExeTree.
*
* NOTES:		Encapsulates left hand tree view. Responsible for drawing 
*				round borders, and a open icon.
*
******************************************************************************/
#pragma once

class CExeTree : public CWnd
{
public:
	CExeTree(void);
	~CExeTree(void);
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	DECLARE_MESSAGE_MAP()
	afx_msg void OnPaint();

	virtual void OnSelection(NMHDR *pNMHDR, LRESULT* pResult);
private:
	CBrush m_brBorder,m_brWhite;
	CPen m_penGrey;
	CFont m_fntVerdana;
	CDC m_dcOpen;
	CBitmap m_bmpOpen;
	CRgn m_rgnOpen;
	BOOL m_bHighlight,m_bStateChange;

public:
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};
