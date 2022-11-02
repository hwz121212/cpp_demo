/******************************************************************************
*
* PROJECT:		ExeScanner
* FILE NAME:	ExeTree.cpp
*
* DEVELOPER:	Vishalsinh Jhala
* DATE:			2-Apr-2005
*
* DESCRIPTION:	Defination file for class CExeTree.
*
* NOTES:		Encapsulates left hand tree view. Responsible for drawing 
*				round borders, and a open icon.
*
******************************************************************************/
#include "StdAfx.h"
#include "exetree.h"
#include "childview.h"
#include "resource.h"

CExeTree::CExeTree(void)
{
	m_brBorder.CreateSolidBrush(RGB(225,225,225));
	m_brWhite.CreateSolidBrush(RGB(255,255,255));
	m_penGrey.CreatePen(PS_SOLID,1,RGB(225,225,225));

	m_fntVerdana.CreateFont(
		16,                        // nHeight
		0,                         // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		FW_BOLD,                 // nWeight
		FALSE,                     // bItalic
		FALSE,                     // bUnderline
		0,                         // cStrikeOut
		ANSI_CHARSET,              // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		DEFAULT_QUALITY,           // nQuality
		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		"Verdana");                 // lpszFacename

	m_dcOpen.CreateCompatibleDC(CDC::FromHandle(::GetDC(NULL)));
	int i = m_bmpOpen.LoadBitmap(MAKEINTRESOURCE(IDB_BITMAP_RIGHT));
	m_dcOpen.SelectObject(&m_bmpOpen);
	
	m_rgnOpen.CreateRoundRectRgn(210,5,242,32,5,5);

	m_bHighlight = FALSE;
	m_bStateChange = FALSE;
	
}

CExeTree::~CExeTree(void)
{
}

BOOL CExeTree::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext)
{
	int iRet = CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
	
	return iRet;
}
BEGIN_MESSAGE_MAP(CExeTree, CWnd)
	ON_WM_PAINT()
	ON_NOTIFY(TVN_SELCHANGED,0x1005,OnSelection)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

void CExeTree::OnPaint()
{
	CPaintDC dc(this);
	//When ever cursor reaches the icon area, or moves out of it,
	// a state change is generated
	//In that case we only redraw the embossed icon. Other wise it produces
	//flickering effect.

	if(!m_bStateChange)
	{
		dc.SelectObject(m_penGrey);
		dc.SelectObject(m_brBorder);
		
		CRect rect;
		GetClientRect(rect);

		dc.RoundRect(2,2,rect.right-2,rect.bottom-2,22,22);
		dc.SelectObject(m_brWhite);
		dc.RoundRect(10,39,rect.right-10,rect.bottom-10,22,22);
		dc.SelectObject(m_fntVerdana);
		dc.SetTextColor(RGB(25,25,25));
		dc.SetBkMode(TRANSPARENT);
		dc.TextOut(20,10,"PE OBJECT HIERARCHY");
		if(m_bHighlight)
			dc.BitBlt(210,5,32,32,&m_dcOpen,32,0,SRCCOPY);
		else
			dc.BitBlt(210,5,32,32,&m_dcOpen,0,0,SRCCOPY);
	}
	else
	{
		if(m_bHighlight)
			dc.BitBlt(210,5,32,32,&m_dcOpen,32,0,SRCCOPY);
		else
			dc.BitBlt(210,5,32,32,&m_dcOpen,0,0,SRCCOPY);
	
		m_bStateChange = FALSE;
	}
	

}
void CExeTree::OnSelection(NMHDR *pNMHDR, LRESULT* pResult)
{
	LPNMTREEVIEW pTV = (LPNMTREEVIEW)pNMHDR;

	((CChildView*)GetParent())->OnSelection(pTV);

	*pResult = 0;
}
void CExeTree::OnMouseMove(UINT nFlags, CPoint point)
{
	if(m_rgnOpen.PtInRegion(point) && !m_bHighlight)
	{
		m_bHighlight = TRUE;
		m_bStateChange = TRUE;
		Invalidate(FALSE);
	} 
	if(	!m_rgnOpen.PtInRegion(point) && m_bHighlight)
	{
		m_bHighlight = FALSE;
		m_bStateChange = TRUE;
		Invalidate(FALSE);
	}

	CWnd::OnMouseMove(nFlags, point);
}

void CExeTree::OnLButtonDown(UINT nFlags, CPoint point)
{
	if( m_rgnOpen.PtInRegion(point) )
	{
		((CChildView*)GetParent())->OnFileScanpe();
	}

	CWnd::OnLButtonDown(nFlags, point);
}
