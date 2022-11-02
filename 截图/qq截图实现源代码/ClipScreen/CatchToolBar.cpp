#include "stdafx.h"
#include "CatchToolBar.h"
#include "resource.h"

#define TOOLBAR_HEIGHT 21
HBITMAP CCatchToolBar::m_hBkImg = NULL;
HBITMAP CCatchToolBar::m_hHotBkImg = NULL;
UINT g_ToolBar[18] = {0,1,4,5,6,7,8,9,10,11,22,23,24,25,26,27,28,29};
UINT g_ToolGroup[18] = {1,1,2,2,2,2,3,3,3,3,3,3,3,3,3,3,-1,-1};
BOOL g_ToolGroupSel[18] = {TRUE,FALSE,TRUE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE};

UINT g_ToolBarID[18] = 
{
	ID_TOOL_1PIX,ID_TOOL_3PIX,
	ID_TOOL_CLR_RED,ID_TOOL_CLR_YELLOW,
	ID_TOOL_CLR_GREEN,ID_TOOL_CLR_BLUE,
	ID_TOOL_XD,ID_TOOL_ZX,
	ID_TOOL_ARROW,ID_TOOL_PXX,
	ID_TOOL_ELLIPSE,ID_TOOL_TEXT,
	ID_TOOL_DOWN,ID_TOOL_UP,
	ID_TOOL_RIGHT,ID_TOOL_WRONG,
	ID_TOOL_SAVE,ID_TOOL_DEL
};


CCatchToolBar::CCatchToolBar(void)
{
	m_hBkImg = LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_BITMAP_TOOL));
	m_hHotBkImg = LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_BITMAP_HOTTOOL));
	m_fclick = NULL;
	SetRect(0,0,0,TOOLBAR_HEIGHT);
	m_bShow = FALSE;
	m_pWnd = NULL;
}

CCatchToolBar::~CCatchToolBar(void)
{
	for (int i = 0;i < m_vItem.size();++i)
	{
		delete m_vItem[i];
		m_vItem[i] = NULL;
	}
	m_vItem.clear();

	DeleteObject(m_hBkImg);
	DeleteObject(m_hHotBkImg);
}

void CCatchToolBar::SetParent(CWnd *pWnd)
{
	m_pWnd = pWnd;
}

void CCatchToolBar::SetPos(POINT pt)
{
	POINT ptrb = {pt.x +Width(),pt.y + Height() };
	SetRect(pt,ptrb);
	CalcItem();
}

void CCatchToolBar::Show(BOOL bShow)
{
	if(m_bShow != bShow)
	{
		m_bShow = bShow;
		ReDraw();
	}
}

void CCatchToolBar::AddItem(CToolItem *pItem)
{
	if(pItem != NULL)
	{
		m_vItem.push_back(pItem);
		POINT pt1 = TopLeft();
		POINT pt2 = BottomRight();
		POINT pt3 = {right,top};
		pItem->SetPos(pt3);
		pt2.x += pItem->Width();
		SetRect(pt1,pt2);
	}
}
void CCatchToolBar::CalcItem()
{
	POINT pt = TopLeft();
	for (int i = 0;i < m_vItem.size();++i)
	{
		if(m_vItem[i])
		{
			m_vItem[i]->SetPos(pt);
			pt.x+= m_vItem[i]->Width();
		}
	}
}
void CCatchToolBar::Draw(HDC hdc)
{
	if(!m_bShow) return;
	if(IsRectEmpty()) return;

	HDC hMemDC = CreateCompatibleDC(hdc);
	HBITMAP hMembitmap = CreateCompatibleBitmap(hdc,Width(),Height());
	SelectObject(hMemDC,hMembitmap);
	DrawBk(hMemDC);
	BitBlt(hdc,left,top,Width(),Height(),hMemDC,0,0,SRCCOPY);
	DeleteObject(hMembitmap);
	DeleteDC(hMemDC);

	//Item
	for (int i = 0;i < m_vItem.size();++i)
	{
		if(m_vItem[i])
			m_vItem[i]->Draw(hdc);
	}
}

void CCatchToolBar::DrawBk(HDC hdc)
{
	HBRUSH hbr = CreateSolidBrush(RGB(255,255,0));
	RECT rt = {0,0,Width(),Height()};
	FillRect(hdc,&rt,hbr);
	DeleteObject(hbr);
}
void CCatchToolBar::ReDraw()
{
	if(m_pWnd)
		m_pWnd->InvalidateRect(this);
}
BOOL CCatchToolBar::OnLbuttonDwon(UINT nFlags, CPoint point)
{
	if(!m_bShow) return FALSE;

	for (int i = 0;i < m_vItem.size();++i)
	{
		if(m_vItem[i] && m_vItem[i]->PtInRect(point))
		{
			m_vItem[i]->OnLbuttonDwon(nFlags,point);
			m_fclick(m_vItem[i],m_pWnd);
			return TRUE;
		}
	}
	return FALSE;
}
BOOL CCatchToolBar::OnMouseMove(UINT nFlags, CPoint point)
{
	if(!m_bShow) return FALSE;

	if(PtInRect(point))
	{
		::SetCursor(::LoadCursor(NULL, IDC_ARROW));
		for (int i = 0;i < m_vItem.size();++i)
		{
			if(m_vItem[i]->PtInRect(point))
			{
				m_vItem[i]->OnMouseMove(nFlags,point);
				break;
			}
		}
	}

	return FALSE;
}

void CCatchToolBar::SetClickfun(PITEMCLICK_FUN fun)
{
	m_fclick = fun;
}

void CCatchToolBar::SetSel(CToolItem *pSelTool)
{
	if(pSelTool == NULL) return;
	if(pSelTool->m_uGroupID == -1) return;
	int nsize = m_vItem.size();

	for (int i = 0;i < nsize;++i)
	{
		if(m_vItem[i] && m_vItem[i]->m_uGroupID == pSelTool->m_uGroupID)
		{
			if(m_vItem[i] != pSelTool && m_vItem[i]->IsSel())
			{
				m_vItem[i]->SetSel(FALSE);
				pSelTool->SetSel(TRUE);
				m_pWnd->InvalidateRect(m_vItem[i],TRUE);
				m_pWnd->InvalidateRect(pSelTool,TRUE);
				return;
			}
		}
	}
	if(!pSelTool->IsSel())
	{
		pSelTool->SetSel(TRUE);
		m_pWnd->InvalidateRect(pSelTool,TRUE);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CToolItem::CToolItem(CRect *pParent,int nWidht,int nHeight,UINT uID,UINT ImgIndex,UINT uGroupID,BOOL bSel)
{
	m_pParent = pParent;
	m_uGroupID = uGroupID;
	m_uID = uID;
	m_ImgIndex = ImgIndex;
	m_nWidth = nWidht;
	m_isSel = bSel;
	SetRect(0,0,nWidht,nHeight);
}
void CToolItem::Draw(HDC hdc)
{
	if(IsRectEmpty()) return;

	HDC hMemDC = CreateCompatibleDC(hdc);
	HBITMAP hMembitmap = CreateCompatibleBitmap(hdc,Width(),Height());
	SelectObject(hMemDC,hMembitmap);
	DrawBk(hMemDC);
	BitBlt(hdc,left,top,Width(),Height(),hMemDC,0,0,SRCCOPY);
	DeleteObject(hMembitmap);
	DeleteDC(hMemDC);
}
void CToolItem::DrawBk(HDC hdc)
{
	int nSave = SaveDC(hdc);
	RECT rtClient = {0,0,Width(),Height()};
	SetTextColor(hdc,RGB(255,255,255));
	HBRUSH hbr = CreateSolidBrush(RGB(255,0,255));
	HDC hMemDc = CreateCompatibleDC(hdc);
	if(!m_isSel)
		SelectObject(hMemDc,CCatchToolBar::m_hBkImg);
	else
		SelectObject(hMemDc,CCatchToolBar::m_hHotBkImg);

	BitBlt(hdc,0,0,Width(),Height(),hMemDc,m_nWidth*m_ImgIndex,0,SRCCOPY);
	DeleteDC(hMemDc);
	DeleteObject(hbr);
	RestoreDC(hdc,nSave);
}

void CToolItem::SetPos(POINT pt)
{
	POINT ptrb = {pt.x +Width(),pt.y + Height() };
	SetRect(pt,ptrb);
}
BOOL CToolItem::OnLbuttonDwon(UINT nFlags, CPoint point)
{
	return FALSE;
}
BOOL CToolItem::OnMouseMove(UINT nFlags, CPoint point)
{
	return FALSE;
}