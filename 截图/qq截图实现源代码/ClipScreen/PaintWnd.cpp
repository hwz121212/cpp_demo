#include "stdafx.h"
#include "PaintWnd.h"
#include <math.h>
#include <GdiPlus.h>
#include "resource.h"

extern HICON g_hUpIco;
extern HICON g_hDownIco;

CPaintWnd::CPaintWnd(void)
{
	m_pedit = NULL;
	m_nCurSelect = -1;
	m_isBegPaint = FALSE;
	m_IsEnableMoveLine = FALSE;
	m_nDrawWay = 0;
	m_nWidth = 1;
	m_clr = RGB(255,0,0);
	m_pParentWnd = NULL;
	m_vDrawItem.clear();
	m_nPointNum = 0;
	m_hFont = CreateFont(-14,0,0,0,FW_NORMAL,FALSE,FALSE,FALSE,0,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH|FF_ROMAN,_T("微软雅黑"));
}

CPaintWnd::~CPaintWnd(void)
{
	DeleteObject(m_hFont);
}

void CPaintWnd::SetParent(CWnd *pParentWnd)
{
	m_pParentWnd = pParentWnd;
}
void CPaintWnd::OnMouseMove(UINT nFlags, CPoint point,BOOL &bHandle)
{
	bHandle = FALSE;
	if(m_pParentWnd == NULL) return;
	if(m_pedit) return;

	if(!m_IsEnableMoveLine)
		IsSelect(point);

	if(!m_isBegPaint && m_nSelect.index != -1)
	{
		SetCursor(LoadCursor(NULL,IDC_HAND));
		bHandle = TRUE;
		if(m_IsEnableMoveLine)
		{
			MoveLine(point - m_PtStartMove);
			m_PtStartMove = point;
		}
		m_pParentWnd->RedrawWindow();
		return ;
	}
	if(!m_isBegPaint) return;
	m_vDrawItem[m_vDrawItem.size()-1].pt[m_nPointNum] = point;
	m_pParentWnd->RedrawWindow();
}

void CPaintWnd::MoveLine(CPoint ptoffset)
{
	if(m_nSelect.index == -1) return;
	DRAWITEM &drawitem = m_vDrawItem[m_nSelect.index];
	switch(drawitem.nWay)
	{
	case ID_TOOL_XD:
	case ID_TOOL_ARROW:
	case ID_TOOL_ZX:
	case ID_TOOL_PXX:
	case ID_TOOL_TEXT:
		{
			if(m_nSelect.nCenter == 0)
			{
				drawitem.pt[0].x += ptoffset.x;
				drawitem.pt[0].y += ptoffset.y;
			}
			else if(m_nSelect.nCenter == 1)
			{
				drawitem.pt[1].x += ptoffset.x;
				drawitem.pt[1].y += ptoffset.y;
			}
			else if(m_nSelect.nCenter == 2)
			{
				drawitem.pt[2].x += ptoffset.x;
				drawitem.pt[2].y += ptoffset.y;
			}
			else
			{
				drawitem.pt[0].x += ptoffset.x;
				drawitem.pt[0].y += ptoffset.y;
				drawitem.pt[1].x += ptoffset.x;
				drawitem.pt[1].y += ptoffset.y;
				drawitem.pt[2].x += ptoffset.x;
				drawitem.pt[2].y += ptoffset.y;
			}
		}
		break;
	case ID_TOOL_ELLIPSE:
		{
			if(m_nSelect.nCenter == 0)
			{
				drawitem.pt[0].x += ptoffset.x;
			}
			else if(m_nSelect.nCenter == 1)
			{
				drawitem.pt[0].y += ptoffset.y;
			}
			else if(m_nSelect.nCenter == 2)
			{
				drawitem.pt[1].x += ptoffset.x;
			}
			else if(m_nSelect.nCenter == 3)
			{
				drawitem.pt[1].y += ptoffset.y;
			}
			else
			{
				drawitem.pt[0].x += ptoffset.x;
				drawitem.pt[0].y += ptoffset.y;
				drawitem.pt[1].x += ptoffset.x;
				drawitem.pt[1].y += ptoffset.y;
			}
		}
		break;
	case ID_TOOL_UP:
	case ID_TOOL_DOWN:
	case ID_TOOL_RIGHT:
	case ID_TOOL_WRONG:
		{
			drawitem.pt[0].x += ptoffset.x;
			drawitem.pt[0].y += ptoffset.y;
		}
		break;
	default:
		break;
	}
}
void CPaintWnd::OnLButtonUp(UINT nFlags, CPoint point)
{
	if(m_pParentWnd == NULL) return;
	if(m_IsEnableMoveLine)
	{
		DRAWITEM &drawItem = m_vDrawItem[m_nCurSelect];
		if(drawItem.nWay == ID_TOOL_ELLIPSE)
		{
			if(drawItem.pt[0].x > drawItem.pt[1].x || drawItem.pt[0].y > drawItem.pt[1].y)
			{
				POINT pt = drawItem.pt[0];
				memcpy(drawItem.pt,drawItem.pt+1,sizeof(POINT));
				memcpy(drawItem.pt + 1,&pt,sizeof(POINT));
			}
		}
	}

	m_IsEnableMoveLine = FALSE;
	if(!m_isBegPaint) return;

	DRAWITEM &ditem = m_vDrawItem[m_vDrawItem.size()-1];
	if(point ==ditem.pt[0] && !(ditem.nWay == ID_TOOL_UP || ditem.nWay == ID_TOOL_DOWN
		|| ditem.nWay == ID_TOOL_WRONG || ditem.nWay == ID_TOOL_RIGHT))//表示没移动，不需要记载并且绘画
	{
		m_vDrawItem.erase(m_vDrawItem.end() - 1);
		Reset();
		return;
	}

	++m_nPointNum;
	if(IsResetSelefLine())
	{
		m_vDrawItem[m_vDrawItem.size()-1].pt[m_nPointNum-1] = point;
		Reset();
	}
	else
	{
		m_vDrawItem[m_vDrawItem.size()-1].pt[m_nPointNum-1] = point;
	}
}
void CPaintWnd::OnLButtonDown(UINT nFlags, CPoint point)
{
	if(m_pParentWnd == NULL) return;
	if(m_nSelect.index != -1)
	{
		m_IsEnableMoveLine = TRUE;
		m_nCurSelect = m_nSelect.index;
		m_PtStartMove = point;
		return;
	}
	m_nCurSelect = -1;

	if(m_nDrawWay == ID_TOOL_TEXT)
	{
		if(m_pedit)
		{

			if(!m_vDrawItem.empty()&&m_vDrawItem[m_vDrawItem.size()-1].nWay == ID_TOOL_TEXT)
			{
				m_pedit->GetWindowText(m_vDrawItem[m_vDrawItem.size()-1].strtext);
				if(m_vDrawItem[m_vDrawItem.size()-1].strtext == "")
					m_vDrawItem.erase(m_vDrawItem.end() - 1);
				else
				{
					POINT ptEnd = GetTextEndPoint(m_vDrawItem[m_vDrawItem.size()-1].pt[0],m_vDrawItem[m_vDrawItem.size()-1].strtext,m_hFont);
					m_vDrawItem[m_vDrawItem.size()-1].pt[1] = ptEnd;
				}
				Reset();
			}
			m_pedit->DestroyWindow();
			m_pedit = NULL;
			m_pParentWnd->SetFocus();
			return;
		}
		m_pedit = new CTransEdit(m_clr);
		RECT rt = {point.x,point.y,point.x + 200,point.y + 20};
		m_pedit->Create(WS_CHILD|WS_VISIBLE|ES_AUTOHSCROLL,rt,m_pParentWnd,0x010);
		m_pedit->LimitText(255);
		m_pedit->SetFocus();
	}
	++m_nPointNum;
	if(m_nPointNum == 1)
	{
		m_isBegPaint = TRUE;
		m_item.nWay = m_nDrawWay;
		m_item.clr = m_clr;
		m_item.nWidth = m_nWidth;
		m_item.pt[m_nPointNum-1] = point;
		m_vDrawItem.push_back(m_item);
	}
	else if(m_isBegPaint)
	{
		m_vDrawItem[m_vDrawItem.size()-1].pt[m_nPointNum-1] = point;
	}
}
void CPaintWnd::OnDraw(CDC *dc,const CRect &rtDraw)
{
	if(m_pParentWnd == NULL) return;
	int nSize = m_vDrawItem.size();
	m_rtDraw = rtDraw;
	for (int i = 0;i < nSize;++i)
	{
		BOOL bSelect = FALSE;
		if(i == m_nCurSelect) 
			bSelect = TRUE;

		DrawItem(dc,rtDraw,m_vDrawItem[i],bSelect);
	}
}

void CPaintWnd::SetDrawWay( UINT nWay )
{
	BOOL bSetAttr = FALSE;
	if(m_pedit)//edit需要特殊处理
	{
		if(!m_vDrawItem.empty()&&m_vDrawItem[m_vDrawItem.size()-1].nWay == ID_TOOL_TEXT)
		{
			m_pedit->GetWindowText(m_vDrawItem[m_vDrawItem.size()-1].strtext);
			if(m_vDrawItem[m_vDrawItem.size()-1].strtext == "")
				m_vDrawItem.erase(m_vDrawItem.end() - 1);
			Reset();
		}
		m_pedit->DestroyWindow();
		m_pedit = NULL;
		m_pParentWnd->SetFocus();
	}

	switch(nWay)
	{
	case ID_TOOL_CLR_RED:m_clr = RGB(255,0,0);bSetAttr = TRUE;break;
	case ID_TOOL_CLR_YELLOW:m_clr = RGB(255,255,0);bSetAttr = TRUE;break;
	case ID_TOOL_CLR_GREEN:m_clr = RGB(0,255,0);bSetAttr = TRUE;break;
	case ID_TOOL_CLR_BLUE:m_clr = RGB(0,0,255);bSetAttr = TRUE;break;
	case ID_TOOL_1PIX:m_nWidth = 1;bSetAttr = TRUE;break;
	case ID_TOOL_3PIX:m_nWidth = 3;bSetAttr = TRUE;break;
	case ID_TOOL_SAVE:break;
	case ID_TOOL_DEL:OnDel();break;
	default:
		{
			m_isBegPaint = FALSE;
			m_nPointNum = 0;
			m_nDrawWay = nWay;
		}
		break;
	}

	if(bSetAttr && m_nCurSelect != -1)
	{
		m_vDrawItem[m_nCurSelect].clr = m_clr;
		m_vDrawItem[m_nCurSelect].nWidth = m_nWidth;
		m_pParentWnd->RedrawWindow();
	}
}

void CPaintWnd::DrawItem(CDC *dc,const CRect &rtDraw,const DRAWITEM &item,BOOL bSelect)
{
	int nSave = dc->SaveDC();

	switch(item.nWay)
	{
	case ID_TOOL_XD://线段
		{
			HPEN hPen = CreatePen(PS_SOLID,item.nWidth,item.clr);
			dc->SelectObject(hPen);
			dc->IntersectClipRect(&rtDraw);
			dc->MoveTo(item.pt[0]);
			dc->LineTo(item.pt[1]);

			if(bSelect)
			{
				DrawFillPoint(dc,item.pt[0]);
				DrawFillPoint(dc,item.pt[1]);
			}
			DeleteObject(hPen);
		}
		break;
	case ID_TOOL_ZX://直线
		{
			HPEN hPen = CreatePen(PS_SOLID,item.nWidth,item.clr);
			dc->SelectObject(hPen);
			dc->IntersectClipRect(&rtDraw);
			DrawZX(dc,rtDraw,item.pt[0],item.pt[1]);
			if(bSelect)
			{
				DrawFillPoint(dc,item.pt[0]);
				DrawFillPoint(dc,item.pt[1]);
			}
			DeleteObject(hPen);
		}
		break;
	case ID_TOOL_ARROW://箭头
		{
			HPEN hPen = CreatePen(PS_SOLID,item.nWidth,item.clr);
			dc->SelectObject(hPen);
			dc->IntersectClipRect(&rtDraw);
			DrawArrow(dc,item.clr,item.nWidth,item.pt[0],item.pt[1]);
			if(bSelect)
			{
				DrawFillPoint(dc,item.pt[0]);
				DrawFillPoint(dc,item.pt[1]);
			}
			DeleteObject(hPen);
		}
		break;
	case ID_TOOL_PXX://平行线
		{
			HPEN hPen = CreatePen(PS_SOLID,item.nWidth,item.clr);
			dc->SelectObject(hPen);
			dc->IntersectClipRect(&rtDraw);
			DrawPXX(dc,rtDraw,item.pt[0],item.pt[1],item.pt[2]);

			if(bSelect)
			{
				DrawFillPoint(dc,item.pt[0]);
				DrawFillPoint(dc,item.pt[1]);
				DrawFillPoint(dc,item.pt[2]);
			}
			DeleteObject(hPen);
		}
		break;
	case ID_TOOL_ELLIPSE://椭圆
		{
			HPEN hPen = CreatePen(PS_SOLID,item.nWidth,item.clr);
			dc->SelectObject(hPen);
			dc->IntersectClipRect(&rtDraw);
			DrawEllipse(dc,item.pt[0],item.pt[1]);

			if(bSelect)
			{
				CRect rt(min(item.pt[0].x,item.pt[1].x),min(item.pt[0].y,item.pt[1].y),max(item.pt[0].x,item.pt[1].x),max(item.pt[0].y,item.pt[1].y));
				POINT pt0 = {rt.left,rt.top + rt.Height()/2};
				POINT pt1 = {rt.left + rt.Width()/2,rt.top};
				POINT pt2 = {rt.right,rt.top + rt.Height()/2};
				POINT pt3 = {rt.left+ rt.Width()/2,rt.bottom};

				DrawFillPoint(dc,pt0);
				DrawFillPoint(dc,pt1);
				DrawFillPoint(dc,pt2);
				DrawFillPoint(dc,pt3);
			}
			DeleteObject(hPen);
		}
		break;
	case ID_TOOL_TEXT:
		{
			dc->IntersectClipRect(&rtDraw);
			dc->SetBkMode(TRANSPARENT);
			dc->SelectObject(m_hFont);
			dc->SetTextColor(item.clr);
			CRect rt(min(item.pt[0].x,item.pt[1].x),min(item.pt[0].y,item.pt[1].y),max(item.pt[0].x,item.pt[1].x),max(item.pt[0].y,item.pt[1].y));
			if(bSelect)
			{
				CPen dotpen(PS_DOT,1,RGB(255,0,0));
				dc->SelectObject(dotpen);
				//dc->FillRect(&rt,CBrush::FromHandle((HBRUSH)GetStockObject(NULL_BRUSH)));
				dc->MoveTo(rt.TopLeft());
				dc->LineTo(rt.right-1,rt.top);
				dc->LineTo(rt.right-1,rt.bottom - 1);
				dc->LineTo(rt.left,rt.bottom - 1);
				dc->LineTo(rt.TopLeft());
			}
			dc->DrawText(item.strtext,&rt,DT_LEFT|DT_VCENTER|DT_SINGLELINE);
			//dc->TextOut(item.pt[0].x,item.pt[0].y,item.strtext);
		}
		break;
	case ID_TOOL_UP:
		{
			dc->IntersectClipRect(&rtDraw);
			POINT pt = item.pt[0];
			pt.x -= 16;
			pt.y -= 16;
			dc->DrawIcon(pt,g_hUpIco);

			if(bSelect)
			{
				CPen dotpen(PS_DOT,1,RGB(255,0,0));
				dc->SelectObject(dotpen);
				CRect rt(pt.x,pt.y,pt.x + 32,pt.y + 32);
				dc->MoveTo(rt.TopLeft());
				dc->LineTo(rt.right-1,rt.top);
				dc->LineTo(rt.right-1,rt.bottom - 1);
				dc->LineTo(rt.left,rt.bottom - 1);
				dc->LineTo(rt.TopLeft());
			}
		}
		break;
	case ID_TOOL_DOWN:
		{
			dc->IntersectClipRect(&rtDraw);
			POINT pt = item.pt[0];
			pt.x -= 16;
			pt.y -= 16;
			dc->DrawIcon(pt,g_hDownIco);
			if(bSelect)
			{
				CPen dotpen(PS_DOT,1,RGB(255,0,0));
				dc->SelectObject(dotpen);
				CRect rt(pt.x,pt.y,pt.x + 32,pt.y + 32);
				dc->MoveTo(rt.TopLeft());
				dc->LineTo(rt.right-1,rt.top);
				dc->LineTo(rt.right-1,rt.bottom - 1);
				dc->LineTo(rt.left,rt.bottom - 1);
				dc->LineTo(rt.TopLeft());
			}
		}
		break;
	case ID_TOOL_RIGHT:
		{
			dc->IntersectClipRect(&rtDraw);
			DrawPng(dc,item.pt[0],IDB_PNG_RIGHT);
			if(bSelect)
			{
				DrawFillPoint(dc,item.pt[0]);
			}
		}
		break;
	case ID_TOOL_WRONG:
		{
			dc->IntersectClipRect(&rtDraw);
			DrawPng(dc,item.pt[0],IDB_PNG_WRONG);
			if(bSelect)
			{
				DrawFillPoint(dc,item.pt[0]);
			}
		}
		break;
	default:
		break;
	}
	dc->RestoreDC(nSave);
}

BOOL CPaintWnd::IsResetSelefLine()
{
	BOOL bRet = FALSE;
	switch(m_nDrawWay)
	{
	case ID_TOOL_XD:
	case ID_TOOL_ZX:
	case ID_TOOL_ELLIPSE:
	case ID_TOOL_ARROW:
		{
			if(m_nPointNum >= 2) bRet =  TRUE;
		}
		break;
	case ID_TOOL_PXX:
		{
			if(m_nPointNum >= 3) bRet =  TRUE;
		}
		break;
	case ID_TOOL_TEXT:
	case ID_TOOL_UP:
	case ID_TOOL_DOWN:
	case ID_TOOL_RIGHT:
	case ID_TOOL_WRONG:
		{
			if(m_nPointNum >= 1) bRet =  TRUE;
		}
		break;
	default:
		break;
	}	
	return bRet;
}

void CPaintWnd::Reset()
{
	m_isBegPaint = FALSE;
	m_nPointNum = 0;
}

int CPaintWnd::IsSelect(POINT pt)
{
	int nSize = m_vDrawItem.size();
	for (int i = 0;i < nSize;++i)
	{
		switch(m_vDrawItem[i].nWay)
		{
		case ID_TOOL_ARROW:
		case ID_TOOL_XD:
			{
				if(ptInLine(pt,m_vDrawItem[i].pt[0],m_vDrawItem[i].pt[1],5))
				{
					m_nSelect.index = i;
					RECT rt = {m_vDrawItem[i].pt[0].x - 5,m_vDrawItem[i].pt[0].y - 5,m_vDrawItem[i].pt[0].x + 5,m_vDrawItem[i].pt[0].y + 5};
					RECT rt2 = {m_vDrawItem[i].pt[1].x - 5,m_vDrawItem[i].pt[1].y - 5,m_vDrawItem[i].pt[1].x + 5,m_vDrawItem[i].pt[1].y + 5};

					if(::PtInRect(&rt,pt))
					{
						m_nSelect.nCenter = 0;//第一个点为基准移动
					}
					else if(::PtInRect(&rt2,pt))
					{
						m_nSelect.nCenter = 1;//第二个点为基准移动
					}
					else
					{
						m_nSelect.nCenter = -1;//整体移动
					}
					return i;
				}
			}
			break;
		case ID_TOOL_ZX:
			{
				if(ptInLine(pt,m_rtDraw,m_vDrawItem[i].pt[0],m_vDrawItem[i].pt[1],5))
				{
					m_nSelect.index = i;
					RECT rt = {m_vDrawItem[i].pt[0].x - 5,m_vDrawItem[i].pt[0].y - 5,m_vDrawItem[i].pt[0].x + 5,m_vDrawItem[i].pt[0].y + 5};
					RECT rt2 = {m_vDrawItem[i].pt[1].x - 5,m_vDrawItem[i].pt[1].y - 5,m_vDrawItem[i].pt[1].x + 5,m_vDrawItem[i].pt[1].y + 5};

					if(::PtInRect(&rt,pt))
					{
						m_nSelect.nCenter = 0;//第一个点为基准移动
					}
					else if(::PtInRect(&rt2,pt))
					{
						m_nSelect.nCenter = 1;//第二个点为基准移动
					}
					else
					{
						m_nSelect.nCenter = -1;//整体移动
					}
					return i;
				}
			}
			break;
		case ID_TOOL_PXX:
			{
				if(PtInPXX(pt,m_rtDraw,m_vDrawItem[i].pt[0],m_vDrawItem[i].pt[1],m_vDrawItem[i].pt[2],5))
				{
					m_nSelect.index = i;
					RECT rt = {m_vDrawItem[i].pt[0].x - 5,m_vDrawItem[i].pt[0].y - 5,m_vDrawItem[i].pt[0].x + 5,m_vDrawItem[i].pt[0].y + 5};
					RECT rt2 = {m_vDrawItem[i].pt[1].x - 5,m_vDrawItem[i].pt[1].y - 5,m_vDrawItem[i].pt[1].x + 5,m_vDrawItem[i].pt[1].y + 5};
					RECT rt3 = {m_vDrawItem[i].pt[2].x - 5,m_vDrawItem[i].pt[2].y - 5,m_vDrawItem[i].pt[2].x + 5,m_vDrawItem[i].pt[2].y + 5};

					if(::PtInRect(&rt,pt))
					{
						m_nSelect.nCenter = 0;//第一个点为基准移动
					}
					else if(::PtInRect(&rt2,pt))
					{
						m_nSelect.nCenter = 1;//第二个点为基准移动
					}
					else if(::PtInRect(&rt3,pt))
					{
						m_nSelect.nCenter = 2;//第二个点为基准移动
					}
					else
					{
						m_nSelect.nCenter = -1;//整体移动
					}
					return i;
				}
			}
			break;
		case ID_TOOL_ELLIPSE:
			{
				if(PtInEllipse(pt,m_vDrawItem[i].pt[0],m_vDrawItem[i].pt[1],5))
				{
					CRect rt(min(m_vDrawItem[i].pt[0].x,m_vDrawItem[i].pt[1].x),min(m_vDrawItem[i].pt[0].y,m_vDrawItem[i].pt[1].y),max(m_vDrawItem[i].pt[0].x,m_vDrawItem[i].pt[1].x),max(m_vDrawItem[i].pt[0].y,m_vDrawItem[i].pt[1].y));
					m_nSelect.index = i;

					POINT pt0 = {rt.left,rt.top + rt.Height()/2};
					POINT pt1 = {rt.left + rt.Width()/2,rt.top};
					POINT pt2 = {rt.right,rt.top + rt.Height()/2};
					POINT pt3 = {rt.left+ rt.Width()/2,rt.bottom};

					RECT rt0 = {pt0.x - 5,pt0.y - 5,pt0.x + 5,pt0.y + 5};
					RECT rt1 = {pt1.x - 5,pt1.y - 5,pt1.x + 5,pt1.y + 5};
					RECT rt2 = {pt2.x - 5,pt2.y - 5,pt2.x + 5,pt2.y + 5};
					RECT rt3 = {pt3.x - 5,pt3.y - 5,pt3.x + 5,pt3.y + 5};

					if(::PtInRect(&rt0,pt))
					{
						m_nSelect.nCenter = 0;//第一个点为基准移动
					}
					else if(::PtInRect(&rt1,pt))
					{
						m_nSelect.nCenter = 1;//第二个点为基准移动
					}
					else if(::PtInRect(&rt2,pt))
					{
						m_nSelect.nCenter = 2;//第二个点为基准移动
					}
					else if(::PtInRect(&rt3,pt))
					{
						m_nSelect.nCenter = 3;//第二个点为基准移动
					}
					else
					{
						m_nSelect.nCenter = -1;//整体移动
					}
					return i;
				}
			}
			break;
		case ID_TOOL_TEXT:
			{
				if(PtInText(pt,m_vDrawItem[i].pt[0],m_vDrawItem[i].pt[1]))
				{
					m_nSelect.index = i;
					m_nSelect.nCenter = -1;//整体移动

					return i;
				}
			}
			break;
		case ID_TOOL_UP:
		case ID_TOOL_DOWN:
			{
				POINT pt1 = m_vDrawItem[i].pt[0];

				RECT rt = {pt1.x - 16,pt1.y - 16,pt1.x + 16,pt1.y + 16};
				if(::PtInRect(&rt,pt))
				{
					m_nSelect.index = i;
					m_nSelect.nCenter = -1;//整体移动

					return i;
				}
			}
			break;
		case ID_TOOL_RIGHT:
		case ID_TOOL_WRONG:
			{
				POINT pt1 = m_vDrawItem[i].pt[0];

				RECT rt = {pt1.x - 5,pt1.y - 5,pt1.x + 5,pt1.y + 5};
				if(::PtInRect(&rt,pt))
				{
					m_nSelect.index = i;
					m_nSelect.nCenter = -1;//整体移动

					return i;
				}
			}
			break;
		default:
			{
				m_nSelect.index = -1;
				m_nSelect.nCenter = -1;
			}
			break;
		}
	}

	m_nSelect.index = -1;
	m_nSelect.nCenter = -1;
	return m_nSelect.index;
}

void CPaintWnd::DrawZX(CDC *dc,const CRect &rtDraw,POINT pt1,POINT pt2)
{
	if(pt1.x == pt2.x && pt1.y == pt2.y ) return;

	if(pt1.x == pt2.x)
	{
		pt1.y = rtDraw.top;
		pt2.y = rtDraw.bottom;		
	}
	else
	{
		float k = float(pt2.y - pt1.y)/float(pt2.x - pt1.x);
		float b = pt1.y - k*pt1.x;
		pt1.x = rtDraw.left;
		pt1.y = k*pt1.x + b;

		pt2.x = rtDraw.right;
		pt2.y = k*pt2.x + b;
	}
	dc->MoveTo(pt1);
	dc->LineTo(pt2);
}

void CPaintWnd::DrawPXX(CDC *dc,const CRect &rtDraw,POINT pt1,POINT pt2,POINT pt3)
{
	DrawZX(dc,rtDraw,pt1,pt2);
	POINT ptTemp={0,0};
	if(pt3.x != -1 || pt3.y != -1)
	{
		if(pt1.x == pt2.x)
		{
			pt1.x = pt2.x = pt3.x;
			pt1.y = rtDraw.top;
			pt2.y = rtDraw.bottom;		
		}
		else
		{
			float k = float(pt2.y - pt1.y)/float(pt2.x - pt1.x);
			float b = pt3.y - k*pt3.x;
			pt1.x = rtDraw.left;
			pt1.y = k*pt1.x + b;

			pt2.x = rtDraw.right;
			pt2.y = k*pt2.x + b;
		}
		dc->MoveTo(pt1);
		dc->LineTo(pt2);
	}
}

void CPaintWnd::DrawEllipse(CDC *dc,POINT pt1,POINT pt2)
{
	dc->SelectObject((HBRUSH)GetStockObject(NULL_BRUSH));
	dc->Ellipse(pt1.x,pt1.y,pt2.x,pt2.y);
}

void CPaintWnd::DrawArrow(CDC *dc,COLORREF clrtemp,UINT nWidth,POINT pt1,POINT pt2)
{
	Gdiplus::Graphics g(dc->GetSafeHdc());
	Gdiplus::Color clr(GetRValue(clrtemp),GetGValue(clrtemp),GetBValue(clrtemp));
	Gdiplus::Pen pen(clr,nWidth);
	pen.SetEndCap(Gdiplus::LineCapArrowAnchor);
	Gdiplus::AdjustableArrowCap acc(8,6,FALSE);
	pen.SetCustomEndCap(&acc);
	g.DrawLine(&pen,pt1.x,pt1.y,pt2.x,pt2.y);
}

void CPaintWnd::DrawPng( CDC *dc,POINT pt1 ,UINT uIdRes)
{
	CPngFromResource rpng(uIdRes);
	if(!rpng) return;
 	Gdiplus::Graphics g(dc->GetSafeHdc());
	Gdiplus::Image *pimg = Gdiplus::Image::FromStream(rpng);
	if(pimg == NULL) return;
	
	pt1.x -= pimg->GetWidth()/2;
	pt1.y -= pimg->GetHeight()/2;
	g.DrawImage(pimg,pt1.x,pt1.y,0,0,pimg->GetWidth(),pimg->GetHeight(), Gdiplus::UnitPixel);
}

void CPaintWnd::DrawFillPoint(CDC *dc,POINT pt,int nOffset)
{
	RECT rt = {pt.x - nOffset,pt.y - nOffset,pt.x + nOffset,pt.y + nOffset};
	dc->FillSolidRect(&rt,RGB(255,255,255));
}
BOOL CPaintWnd::ptInLine(CPoint point, CPoint lineStartPoint, CPoint lineEndPoint, double fTolerance)
{
	//叉积=0三点 （X1 Y1 ) (X2 Y2) (X3 Y3)叉积=(X1-X2)*(Y3-Y2)-(X3-X2)*(Y1-Y2)=0时 三点在一直线上
	double L,R,S;
	BOOL bResult = FALSE;

	L = sqrt((double)((lineEndPoint.x-lineStartPoint.x)*(lineEndPoint.x-lineStartPoint.x) + (lineEndPoint.y-lineStartPoint.y)*(lineEndPoint.y-lineStartPoint.y)));
	if(L != 0)
	{
		R = ((lineStartPoint.y-point.y)*(lineStartPoint.y-lineEndPoint.y)-(lineStartPoint.x-point.x)*(lineEndPoint.x-lineStartPoint.x))/(L*L);
		S = ((lineStartPoint.y-point.y)*(lineEndPoint.x-lineStartPoint.x)-(lineStartPoint.x-point.x)*(lineEndPoint.y-lineStartPoint.y))/(L*L);
		if(R>0 && R<1)
		{
			if(abs(S*L)<= fTolerance)
				bResult = TRUE;
		}
	}
	return bResult;
}

BOOL CPaintWnd::PtInText(CPoint point,CPoint pointTextS,CPoint pointTextE)
{
	CRect rt(min(pointTextS.x,pointTextE.x),min(pointTextS.y,pointTextE.y),max(pointTextS.x,pointTextE.x),max(pointTextS.y,pointTextE.y));
	if(rt.PtInRect(point)) return TRUE;
	return FALSE;
}
BOOL CPaintWnd::ptInLine(CPoint point,const CRect &rtDraw,CPoint lineStartPoint, CPoint lineEndPoint, double fTolerance)
{
	if(lineStartPoint.x == lineEndPoint.x && lineStartPoint.y == lineEndPoint.y ) return FALSE;

	if(lineStartPoint.x == lineEndPoint.x)
	{
		lineStartPoint.y = rtDraw.top;
		lineEndPoint.y = rtDraw.bottom;		
	}
	else
	{
		float k = float(lineEndPoint.y - lineStartPoint.y)/float(lineEndPoint.x - lineStartPoint.x);
		float b = lineStartPoint.y - k*lineStartPoint.x;
		lineStartPoint.x = rtDraw.left;
		lineStartPoint.y = k*lineStartPoint.x + b;

		lineEndPoint.x = rtDraw.right;
		lineEndPoint.y = k*lineEndPoint.x + b;
	}

	return ptInLine(point,lineStartPoint,lineEndPoint,fTolerance);
}

BOOL CPaintWnd::PtInPXX(CPoint point, const CRect &rtDraw,CPoint lineStartPoint, CPoint lineEndPoint,CPoint lineTwo, double fTolerance)
{
	BOOL bRet = FALSE;
	if(ptInLine(point,rtDraw,lineStartPoint,lineEndPoint,fTolerance)) return TRUE;

	POINT ptTemp={0,0};
	if(lineTwo.x != -1 || lineTwo.y != -1)
	{
		if(lineStartPoint.x == lineEndPoint.x)
		{
			lineStartPoint.x = lineEndPoint.x = lineTwo.x;
			lineStartPoint.y = rtDraw.top;
			lineEndPoint.y = rtDraw.bottom;		
		}
		else
		{
			float k = float(lineEndPoint.y - lineStartPoint.y)/float(lineEndPoint.x - lineStartPoint.x);
			float b = lineTwo.y - k*lineTwo.x;
			lineStartPoint.x = rtDraw.left;
			lineStartPoint.y = k*lineStartPoint.x + b;

			lineEndPoint.x = rtDraw.right;
			lineEndPoint.y = k*lineEndPoint.x + b;
		}
		
		if(ptInLine(point,rtDraw,lineStartPoint,lineEndPoint,fTolerance)) return TRUE;
	}

	return FALSE;
}

BOOL CPaintWnd::PtInEllipse(CPoint point, CPoint lineStartPoint, CPoint lineEndPoint, double fTolerance)
{

	CRect rt(min(lineStartPoint.x,lineEndPoint.x),min(lineStartPoint.y,lineEndPoint.y),max(lineStartPoint.x,lineEndPoint.x),max(lineStartPoint.y,lineEndPoint.y));
	float a = (float)rt.Width()/2;
	float b = (float)rt.Height()/2;
	if(point.x > rt.right+fTolerance || point.x < rt.left-fTolerance || point.y < rt.top - fTolerance || point.y > rt.bottom + fTolerance) return FALSE;

	float x = a - (rt.right - point.x);
	if (abs(x) >= a + fTolerance)
	{
		if(x > 0) x - fTolerance;
		else x + fTolerance;
	}
	
	if (abs(x) > a) return FALSE; 

	float y1 = 0;
	if(point.y > rt.bottom - b)
		y1 = (rt.top + rt.bottom)/2 + sqrt(1-pow(x,2)/(a*a))*b;
	else 
		y1 = (rt.top + rt.bottom)/2 - sqrt(1-pow(x,2)/(a*a))*b;

	if(abs(y1 - point.y )>= fTolerance) return FALSE;

	return TRUE;
}

void CPaintWnd::OnDel()
{
	if(m_nCurSelect == -1) return;
	if(m_nCurSelect >= m_vDrawItem.size()) return;
	m_vDrawItem.erase(m_vDrawItem.begin() + m_nCurSelect);
	m_nCurSelect = -1;
	m_pParentWnd->RedrawWindow();
}

POINT CPaintWnd::GetTextEndPoint(POINT ptStart,const CString &str,const HFONT &font)
{
	if(m_pParentWnd == NULL) return ptStart;

	CDC *pdc = m_pParentWnd->GetDC();
	pdc->SelectObject(font);
	RECT rt = {ptStart.x,ptStart.y,ptStart.x,ptStart.y + 30};
	pdc->DrawText(str,&rt,DT_LEFT|DT_VCENTER|DT_SINGLELINE|DT_CALCRECT);
	m_pParentWnd->ReleaseDC(pdc);
	ptStart.x  = rt.right;
	ptStart.y = rt.bottom;
	return  ptStart;
}