/******************************************************************************
*
* PROJECT:		ExeScanner
* FILE NAME:	ExeLegend.h
*
* DEVELOPER:	Vishalsinh Jhala
* DATE:			2-Apr-2005
*
* DESCRIPTION:	Declaration file for class ExeLegend.
*
* NOTES:		Represents property window. Its a semi-transparent, popup window
*				with non-standard GUI. Allows users to view the properties of
*				each object.
*
******************************************************************************/
#pragma once


// CExeLegend

class CExeLegend : public CWnd
{
	DECLARE_DYNAMIC(CExeLegend)

public:
	CExeLegend();
	virtual ~CExeLegend();

protected:
	DECLARE_MESSAGE_MAP()
	CBrush m_brGrey,m_brDarkGrey;
	CPen m_penWhite;
	CDC m_dcPointer;
	CBitmap m_bmpPointer;
	CRgn m_rgnRoundRect,m_rgnBig;

	CListCtrl m_lstProperty;
	void Populate();
	void Type(int i, int j,CString str);

	void Transparent();

public:
	virtual BOOL CreateEx(DWORD dwExStyle, LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, LPVOID lpParam = NULL);
	afx_msg void OnPaint();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	void Animate();
	BOOL m_bMaximize;//Stores the current state of window. Animate fn. does exactly opposite
					 //of this variable. i.e. Minimize if its value is true.
	CString strInfo;//This stores the property. Each column/row are seperated by \n

	virtual BOOL PreTranslateMessage(MSG* pMsg);
};


