/******************************************************************************
*
* PROJECT:		ExeScanner
* FILE NAME:	ExeView.cpp
*
* DEVELOPER:	Vishalsinh Jhala
* DATE:			2-Apr-2005
*
* DESCRIPTION:	Defination file for class ExeView.
*
* NOTES:		Encapsulates right hand object view. Responsible for drawing 
*				round borders, and a action icon.
*
******************************************************************************/
#include "StdAfx.h"
#include "exeview.h"
#include "ChildView.h"
#include "Resource.h"
#include "DlgBinaryEdit.h"
#include "DlgStringEdit.h"
#include "PEResBinary.h"
#include "PEResString.h"
#include "portableexecutable.h"

CExeView::CExeView(void)
{
	m_brBorder.CreateSolidBrush(RGB(225,225,225));
	m_brWhite.CreateSolidBrush(RGB(255,255,255));
	m_penGrey.CreatePen(PS_SOLID,1,RGB(225,225,225));

	m_dcOpen.CreateCompatibleDC(CDC::FromHandle(::GetDC(NULL)));
	int i = m_bmpOpen.LoadBitmap(MAKEINTRESOURCE(IDB_BITMAP_LEFT));
	m_dcOpen.SelectObject(&m_bmpOpen);
	
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
	m_fntVerdanaSmall.CreateFont(
		12,                        // nHeight
		0,                         // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		FW_NORMAL,                 // nWeight
		FALSE,                     // bItalic
		FALSE,                     // bUnderline
		0,                         // cStrikeOut
		ANSI_CHARSET,              // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		DEFAULT_QUALITY,           // nQuality
		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		"Verdana");                 // lpszFacename

	m_iHighlight = 0;
	m_bStateChange = FALSE;
	m_CRefPrev = 0;
}

CExeView::~CExeView(void)
{
}

BOOL CExeView::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext)
{
	return CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}
BEGIN_MESSAGE_MAP(CExeView, CWnd)
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
	ON_WM_SIZE()
	ON_WM_LBUTTONDOWN()
	
//	ON_NOTIFY(WM_NOTIFY,0,OnEditLabel)
END_MESSAGE_MAP()

void CExeView::OnPaint()
{
	CPaintDC dc(this);
	CRect rect;
	GetClientRect(rect);

	//When user moves in or out of icon area, a state change is generated and only
	//icons are drawn. this ensures non flickering window
	if(!m_bStateChange)
	{
		CChildView* pParent = (CChildView*)GetParent();

		dc.SelectObject(m_penGrey);
		dc.SelectObject(m_brBorder);

		dc.RoundRect(2,2,rect.right-2,rect.bottom-2,22,22);
		dc.SelectObject(m_brWhite);
		dc.RoundRect(10,39,rect.right-10,rect.bottom-10,22,22);
		
		dc.SelectObject(&m_fntVerdana);
		dc.SetTextColor(RGB(25,25,25));
		dc.SetBkMode(TRANSPARENT);
		dc.TextOut(20,10,"OBJECT VIEW");
		
		if(pParent->IsDCValid())
		{
			int w=rect.right-56,h=rect.bottom-91;
			if(pParent->m_iWidth<=w && pParent->m_iHeight<=h)
			{	
				int dispx = (w-pParent->m_iWidth)/2;
				int dispy = (h-pParent->m_iHeight)/2;
				dc.BitBlt(28+dispx,55+dispy,pParent->m_iWidth,pParent->m_iHeight,pParent->m_pDC,0,0,SRCCOPY);
			}
			else if(pParent->m_iWidth>w && pParent->m_iHeight<=h)
			{
				int dispy = (h-pParent->m_iHeight)/2;
				dc.StretchBlt(28,55+dispy,w,pParent->m_iHeight,pParent->m_pDC,0,0,pParent->m_iWidth,pParent->m_iHeight,SRCCOPY);
			}
			else if(pParent->m_iWidth<=w && pParent->m_iHeight>h)
			{
				int dispx = (w-pParent->m_iWidth)/2;
				dc.StretchBlt(28+dispx,55,pParent->m_iWidth,h,pParent->m_pDC,0,0,pParent->m_iWidth,pParent->m_iHeight,SRCCOPY);
			}
			else
			{
				dc.StretchBlt(28,55,w,h,pParent->m_pDC,0,0,pParent->m_iWidth,pParent->m_iHeight,SRCCOPY);
			}

		}
		switch(m_iHighlight)
		{
		case 0:
			dc.BitBlt(rect.right-128,5,32,32,&m_dcOpen,0,0,SRCCOPY);
			dc.BitBlt(rect.right-96,5,32,32,&m_dcOpen,64,0,SRCCOPY);
			dc.BitBlt(rect.right-64,5,32,32,&m_dcOpen,128,0,SRCCOPY);
			break;
		case 1:
			dc.BitBlt(rect.right-128,5,32,32,&m_dcOpen,32,0,SRCCOPY);
			dc.BitBlt(rect.right-96,5,32,32,&m_dcOpen,64,0,SRCCOPY);
			dc.BitBlt(rect.right-64,5,32,32,&m_dcOpen,128,0,SRCCOPY);
			break;
		case 2:
			dc.BitBlt(rect.right-128,5,32,32,&m_dcOpen,0,0,SRCCOPY);
			dc.BitBlt(rect.right-96,5,32,32,&m_dcOpen,96,0,SRCCOPY);
			dc.BitBlt(rect.right-64,5,32,32,&m_dcOpen,128,0,SRCCOPY);
			break;
		case 3:
			dc.BitBlt(rect.right-128,5,32,32,&m_dcOpen,0,0,SRCCOPY);
			dc.BitBlt(rect.right-96,5,32,32,&m_dcOpen,64,0,SRCCOPY);
			dc.BitBlt(rect.right-64,5,32,32,&m_dcOpen,160,0,SRCCOPY);
			break;
		}
	}
	else
	{
		switch(m_iHighlight)
		{
		case 0:
			dc.BitBlt(rect.right-128,5,32,32,&m_dcOpen,0,0,SRCCOPY);
			dc.BitBlt(rect.right-96,5,32,32,&m_dcOpen,64,0,SRCCOPY);
			dc.BitBlt(rect.right-64,5,32,32,&m_dcOpen,128,0,SRCCOPY);
			break;
		case 1:
			dc.BitBlt(rect.right-128,5,32,32,&m_dcOpen,32,0,SRCCOPY);
			dc.BitBlt(rect.right-96,5,32,32,&m_dcOpen,64,0,SRCCOPY);
			dc.BitBlt(rect.right-64,5,32,32,&m_dcOpen,128,0,SRCCOPY);
			break;
		case 2:
			dc.BitBlt(rect.right-128,5,32,32,&m_dcOpen,0,0,SRCCOPY);
			dc.BitBlt(rect.right-96,5,32,32,&m_dcOpen,96,0,SRCCOPY);
			dc.BitBlt(rect.right-64,5,32,32,&m_dcOpen,128,0,SRCCOPY);
			break;
		case 3:
			dc.BitBlt(rect.right-128,5,32,32,&m_dcOpen,0,0,SRCCOPY);
			dc.BitBlt(rect.right-96,5,32,32,&m_dcOpen,64,0,SRCCOPY);
			dc.BitBlt(rect.right-64,5,32,32,&m_dcOpen,160,0,SRCCOPY);
			break;
		}
		m_bStateChange = FALSE;
	}

}

void CExeView::OnMouseMove(UINT nFlags, CPoint point)
{
	if( m_rgnModify.PtInRegion(point) && m_iHighlight!=1 )
	{
		m_iHighlight = 1;
		m_bStateChange = TRUE;
		Invalidate(FALSE);
	} 
	else if(m_rgnExtract.PtInRegion(point) && m_iHighlight!=2 )
	{
		m_iHighlight = 2;
		m_bStateChange = TRUE;
		Invalidate(FALSE);
	}
	else if(m_rgnLegend.PtInRegion(point) && m_iHighlight!=3 )
	{
		m_iHighlight = 3;
		m_bStateChange = TRUE;
		Invalidate(FALSE);
	}
	if( !m_rgnModify.PtInRegion(point) &&
		!m_rgnExtract.PtInRegion(point) &&
		!m_rgnLegend.PtInRegion(point) &&
		 m_iHighlight !=0)
	{
		m_iHighlight = 0;
		m_bStateChange = TRUE;
		Invalidate(FALSE);
	}
	//if( ((CChildView*)GetParent())->m_bPEHeaderShow )
	//	ShowTip(point);

	CWnd::OnMouseMove(nFlags, point);
}

void CExeView::OnSize(UINT nType, int cx, int cy)
{
	m_rgnModify.DeleteObject();
	m_rgnExtract.DeleteObject();
	m_rgnLegend.DeleteObject();
	
	m_rgnModify.CreateRectRgn(cx-128,5,cx-96,37);
	m_rgnExtract.CreateRectRgn(cx-96,5,cx-64,37);
	m_rgnLegend.CreateRectRgn(cx-64,5,cx-32,37);

	CWnd::OnSize(nType, cx, cy);

}

void CExeView::OnLButtonDown(UINT nFlags, CPoint point)
{
	if( m_rgnModify.PtInRegion(point) )
	{
		((CChildView*)GetParent())->OnPortableexecutableImport();
	}
	else if( m_rgnExtract.PtInRegion(point) )
	{
		((CChildView*)GetParent())->OnPortableexecutableExport();
	}
	else if( m_rgnLegend.PtInRegion(point) )
	{
		((CChildView*)GetParent())->OnPortableexecutableProperties();
	}

	CWnd::OnLButtonDown(nFlags, point);

}

BOOL CExeView::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	CString str;
	LVHITTESTINFO hti;
	if(wParam == 0)
	{
		LPNMHDR nmhdr = (LPNMHDR)lParam;
		if(nmhdr->code == NM_CLICK)
		{
			CListCtrl *pCtrl;
			pCtrl = (CListCtrl*)CWnd::FromHandle(nmhdr->hwndFrom);
			HTREEITEM hti1 = ((CChildView*)GetParent())->m_treeExe.GetSelectedItem();
			PEResource *pB = (PEResource *)((CChildView*)GetParent())->m_treeExe.GetItemData(hti1);
			
			ZeroMemory(&hti,sizeof(LVHITTESTINFO));
			hti.pt = ((LPNMITEMACTIVATE)lParam)->ptAction;
			ListView_SubItemHitTest(nmhdr->hwndFrom,&hti);

			str = pCtrl->GetItemText(hti.iItem,hti.iSubItem);
			
			//When the right hand view is clicked, the reason
			//is user want to modify string or binary object.
			//Detect the right object and call the appropriate dialog 
			//to allow the user to do so.
			if(str.GetLength())
			if( strcmp(pB->GetName(),"STRING") == 0 && hti.iSubItem == 1 )
			{
				CString sSize = pCtrl->GetItemText(hti.iItem,2);
				int iOrgSize = atoi(sSize.GetBuffer(1024));

				CDlgStringEdit dlg(str,iOrgSize);
				if( dlg.DoModal() == IDOK)
				{
					pCtrl->SetItemText(hti.iItem,hti.iSubItem,dlg.m_strString);
					CString strAdd = pCtrl->GetItemText(hti.iItem,0);
					
					((PEResString*)pB)->Save(atoi(strAdd.GetBuffer(1024)),dlg.m_strString,iOrgSize);

				}
			}
			else if( strcmp(pB->GetName(),"BINARY") == 0 && ((hti.iSubItem>=1 && hti.iSubItem<=8)||(hti.iSubItem>=10 && hti.iSubItem<=17)))
			{
				CDlgBinaryEdit dlg(str);
				if( dlg.DoModal() == IDOK)
				{
					pCtrl->SetItemText(hti.iItem,hti.iSubItem,dlg.m_strBin);

					int j;
					if(hti.iSubItem<8)
						j = hti.iSubItem - 1;
					else
						j = hti.iSubItem - 2;
					BYTE bVal;
					str = pCtrl->GetItemText(hti.iItem,hti.iSubItem);
					bVal = AlphaHexToInt(str.GetAt(0));
					bVal = bVal<<4;
					bVal |=	AlphaHexToInt(str.GetAt(1));

					((PEResBinary*)pB)->Save(bVal,hti.iItem*16+j);
					CString strCharView = pCtrl->GetItemText(hti.iItem,18);

					if(hti.iSubItem<8)
					{	
						if(isalnum(bVal))
							strCharView.SetAt(hti.iSubItem-1,bVal);
						else
							strCharView.SetAt(hti.iSubItem-1,'.');
					}
					else
					{	
						if(isalnum(bVal))
							strCharView.SetAt(hti.iSubItem-2,bVal);
						else
							strCharView.SetAt(hti.iSubItem-2,'.');
					}
					
					pCtrl->SetItemText(hti.iItem,18,strCharView);

				}
			}
		}
	}

	return CWnd::OnNotify(wParam, lParam, pResult);
}
/******************************************************************************
* Function Name: AlphaHexToInt
*
* Note: Converts hex character to equivalent integer number and returns it.
*
******************************************************************************/
BYTE CExeView::AlphaHexToInt(char ch)
{
	switch(ch)
	{
	case '0':
		return 0X00;
		break;
	case '1':
		return 0x1;
		break;
	case '2':
		return 0x2;
		break;
	case '3':
		return 0x3;
		break;
	case '4':
		return 4;
		break;
	case '5':
		return 5;
		break;
	case '6':
		return 6;
		break;
	case '7':
		return 7;
		break;
	case '8':
		return 8;
		break;
	case '9':
		return 9;
		break;
	case 'a':
	case 'A':
		return 0xa;
		break;
	case 'b':
	case 'B':
		return 0xb;
		break;
	case 'c':
	case 'C':
		return 0xc;
		break;
	case 'd':
	case 'D':
		return 0xd;
		break;
	case 'e':
	case 'E':
		return 0xe;
		break;
	case 'f':
	case 'F':
		return 0xf;
		break;
	}
	return 0;
}
