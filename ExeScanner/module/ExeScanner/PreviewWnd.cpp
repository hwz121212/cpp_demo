/******************************************************************************
*
* PROJECT:		ExeScanner
* FILE NAME:	PreviewWnd.cpp
*
* DEVELOPER:	Vishalsinh Jhala
* DATE:			2-Apr-2005
*
* DESCRIPTION:	Defination file for class CPreviewWnd.
*
* NOTES:		Preview window for the resource Images.
*
******************************************************************************/

#include "stdafx.h"
#include "ExeScanner.h"
#include "PreviewWnd.h"
#include "ImageImportDlg.h"


// CPreviewWnd

IMPLEMENT_DYNAMIC(CPreviewWnd, CWnd)
CPreviewWnd::CPreviewWnd()
{
}

CPreviewWnd::~CPreviewWnd()
{
}


BEGIN_MESSAGE_MAP(CPreviewWnd, CWnd)
	ON_WM_PAINT()
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_WM_SIZE()
END_MESSAGE_MAP()



// CPreviewWnd message handlers
void CPreviewWnd::SetDlg(CImageImportDlg * pDlg, CDC* pDC,int iWidth,int iHeight)
{
	m_pDlg = pDlg;
	m_iWidth = iWidth;
	m_iHeight = iHeight;
	m_pDC = pDC;

	CRect rect;
	GetClientRect(rect);
	m_DispW = rect.right;
	m_DispH = rect.bottom;

	if(m_DispW<m_iWidth)
	{
		SetScrollRange(SB_HORZ,0,m_iWidth);
		m_movX = (float)(m_iWidth-m_DispW)/m_iWidth;
	}
	else
	{
		m_DispW = m_iWidth;
		m_movX = 0;
	}
	if(m_DispH<m_iHeight)
	{
		SetScrollRange(SB_VERT,0,m_iHeight);
		m_movY = (float)(m_iHeight-m_DispH)/m_iHeight;
	}
	else
	{
		m_DispH = m_iHeight;
		m_movY = 0;
	}
	m_iScrollX = m_iScrollY = 0;

}

void CPreviewWnd::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	
	if(m_pDlg)
		dc.BitBlt(0,0,m_DispW,m_DispH,m_pDC,(int)m_iScrollX*m_movX,(int)m_iScrollY*m_movY,SRCCOPY);

}
void CPreviewWnd::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
   // Get the minimum and maximum scroll-bar positions.
   int minpos = 0;
   int maxpos = m_iWidth;

   // Get the current position of scroll box.
   int curpos = GetScrollPos(SB_HORZ);

   // Determine the new position of scroll box.
   switch (nSBCode)
   {
   case SB_LEFT:      // Scroll to far left.
      curpos = minpos;
      break;

   case SB_RIGHT:      // Scroll to far right.
      curpos = maxpos;
      break;

   case SB_ENDSCROLL:   // End scroll.
      break;

   case SB_LINELEFT:      // Scroll left.
      if (curpos > minpos)
         curpos--;
      break;

   case SB_LINERIGHT:   // Scroll right.
      if (curpos < maxpos)
         curpos++;
      break;

   case SB_PAGELEFT:    // Scroll one page left.
   {
      // Get the page size. 
      /*SCROLLINFO   info;
      pScrollBar->GetScrollInfo(&info, SIF_ALL);
   
      if (curpos > minpos)
      curpos = max(minpos, curpos - (int) info.nPage);*/
   }
      break;

   case SB_PAGERIGHT:      // Scroll one page right.
   {
      // Get the page size. 
      /*SCROLLINFO   info;
      pScrollBar->GetScrollInfo(&info, SIF_ALL);

      if (curpos < maxpos)
         curpos = min(maxpos, curpos + (int) info.nPage);*/
   }
      break;

   case SB_THUMBPOSITION: // Scroll to absolute position. nPos is the position
      curpos = nPos;      // of the scroll box at the end of the drag operation.
      break;

   case SB_THUMBTRACK:   // Drag scroll box to specified position. nPos is the
      curpos = nPos;     // position that the scroll box has been dragged to.
      break;
   }

   // Set the new position of the thumb (scroll box).
   SetScrollPos(SB_HORZ,curpos);
	m_iScrollX = curpos;

	CWnd::OnHScroll(nSBCode, nPos, pScrollBar);
	this->Invalidate(FALSE);
}

void CPreviewWnd::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
   // Get the minimum and maximum scroll-bar positions.
   int minpos = 0;
   int maxpos = m_iHeight;
   
   // Get the current position of scroll box.
   int curpos = GetScrollPos(SB_VERT);

   // Determine the new position of scroll box.
   switch (nSBCode)
   {
   case SB_TOP:      // Scroll to far left.
      curpos = minpos;
      break;

   case SB_BOTTOM:      // Scroll to far right.
      curpos = maxpos;
      break;

   case SB_ENDSCROLL:   // End scroll.
      break;

   case SB_LINEUP:      // Scroll left.
      if (curpos > minpos)
         curpos--;
      break;

   case SB_LINEDOWN:   // Scroll right.
      if (curpos < maxpos)
         curpos++;
      break;

   case SB_PAGEUP:    // Scroll one page left.
   {
      // Get the page size. 
      /*SCROLLINFO   info;
      pScrollBar->GetScrollInfo(&info, SIF_ALL);
   
      if (curpos > minpos)
      curpos = max(minpos, curpos - (int) info.nPage);*/
   }
      break;

   case SB_PAGEDOWN:      // Scroll one page right.
   {
      // Get the page size. 
      /*SCROLLINFO   info;
      pScrollBar->GetScrollInfo(&info, SIF_ALL);

      if (curpos < maxpos)
         curpos = min(maxpos, curpos + (int) info.nPage);*/
   }
      break;

   case SB_THUMBPOSITION: // Scroll to absolute position. nPos is the position
      curpos = nPos;      // of the scroll box at the end of the drag operation.
      break;

   case SB_THUMBTRACK:   // Drag scroll box to specified position. nPos is the
      curpos = nPos;     // position that the scroll box has been dragged to.
      break;
   }

   // Set the new position of the thumb (scroll box).
   SetScrollPos(SB_VERT,curpos);
	m_iScrollY = curpos;

	CWnd::OnVScroll(nSBCode, nPos, pScrollBar);
	this->Invalidate(FALSE);
}

void CPreviewWnd::OnSize(UINT nType, int cx, int cy)
{
	CRect rect;
	GetClientRect(rect);
	m_DispW = rect.right;
	m_DispH = rect.bottom;

	if(m_DispW<m_iWidth)
	{
		SetScrollRange(SB_HORZ,0,m_iWidth);
		m_movX = (float)(m_iWidth-m_DispW)/m_iWidth;
	}
	else
	{
		SetScrollRange(SB_HORZ,0,0);
		m_DispW = m_iWidth;
		m_movX = 0;
	}
	if(m_DispH<m_iHeight)
	{
		SetScrollRange(SB_VERT,0,m_iHeight);
		m_movY = (float)(m_iHeight-m_DispH)/m_iHeight;
	}
	else
	{
		SetScrollRange(SB_VERT,0,0);
		m_DispH = m_iHeight;
		m_movY = 0;
	}
	m_iScrollX = m_iScrollY = 0;
}
