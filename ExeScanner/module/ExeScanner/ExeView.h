/******************************************************************************
*
* PROJECT:		ExeScanner
* FILE NAME:	ExeView.h
*
* DEVELOPER:	Vishalsinh Jhala
* DATE:			2-Apr-2005
*
* DESCRIPTION:	Declaration file for class ExeView.
*
* NOTES:		Encapsulates right hand object view. Responsible for drawing 
*				round borders, and a action icon.
*
******************************************************************************/
#pragma once

class CExeView : public CWnd
{
public:
	CExeView(void);
	~CExeView(void);
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	DECLARE_MESSAGE_MAP()
	afx_msg void OnPaint();

private:
	CBrush m_brBorder,m_brWhite;
	CPen m_penGrey;
	CFont m_fntVerdana,m_fntVerdanaSmall ;

	CDC m_dcOpen;
	CBitmap m_bmpOpen;
	CRgn m_rgnModify,m_rgnExtract,m_rgnLegend;
	BOOL m_bStateChange;
	int m_iHighlight;
	//void ShowTip(CPoint pt);
	COLORREF m_CRefPrev;

public:
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	BYTE CExeView::AlphaHexToInt(char ch);
protected:
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
};
