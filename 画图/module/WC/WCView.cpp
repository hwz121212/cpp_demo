// WCView.cpp : implementation of the CWCView class
//

#include "stdafx.h"
#include "WC.h"

#include "WCDoc.h"
#include "WCView.h"
#include "FontD.h"
#include "LinewidthDlg.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWCView

IMPLEMENT_DYNCREATE(CWCView, CView)

BEGIN_MESSAGE_MAP(CWCView, CView)
	//{{AFX_MSG_MAP(CWCView)
	ON_COMMAND(ID_RECTANGLE, OnRectangle)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_COMMAND(ID_COLORPEN, OnColorpen)
	ON_COMMAND(ID_FILLRECTANGLE, OnFillrectangle)
	ON_COMMAND(ID_COLORBRUSH, OnColorbrush)
	ON_COMMAND(ID_FONT1, OnFont1)
	ON_COMMAND(ID_LINEWIDTHDLG, OnLinewidthdlg)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWCView construction/destruction

CWCView::CWCView()
{
	// TODO: add construction code here
	m_isbuttondown=false;
	width=1;
	colorpen=RGB(255,0,0);
	colorbrush=RGB(255,255,0);
}

CWCView::~CWCView()
{
}

BOOL CWCView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CWCView drawing

void CWCView::OnDraw(CDC* pDC)
{
	CWCDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
	static int N=0;
	if(N==0)
	{
		N++;
		dcmomory.CreateCompatibleDC(pDC);
		bmp.LoadBitmap(IDB_BITMAP1);	
		dcmomory.SelectObject(&bmp);
	}
	CRect rect;
	GetClientRect(&rect);
	pDC->BitBlt(0,0,rect.right,rect.bottom,&dcmomory,0,0,SRCCOPY);
}

/////////////////////////////////////////////////////////////////////////////
// CWCView printing

BOOL CWCView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CWCView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CWCView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CWCView diagnostics

#ifdef _DEBUG
void CWCView::AssertValid() const
{
	CView::AssertValid();
}

void CWCView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CWCDoc* CWCView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CWCDoc)));
	return (CWCDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CWCView message handlers

void CWCView::OnRectangle() 
{
	// TODO: Add your command handler code here
	flag="rectangle";

}

void CWCView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	x1=point.x;
	y1=point.y;
	m_isbuttondown=true;
	CView::OnLButtonDown(nFlags, point);
}

void CWCView::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	m_isbuttondown=false;
	CPen pen;
		pen.CreatePen(PS_SOLID,width,colorpen);  
		dcmomory.SelectObject(&pen);
	if(flag=="rectangle")
	{	
		CBrush *pbrush=CBrush::FromHandle(   (HBRUSH)GetStockObject(NULL_BRUSH)   );
		dcmomory.SelectObject(pbrush);
		dcmomory.Rectangle(x1,y1,x2,y2);
		Invalidate();
	}
	if (flag=="font")
	{
		x1=point.x;
		y1=point.y;
		CFontD dlg;
		if(dlg.DoModal()==IDOK)
		{
			dcmomory.SelectObject(&fontstyle);
			dcmomory.SetTextColor(fontcolor);
			dcmomory.SetBkMode(TRANSPARENT);
			dcmomory.TextOut(x1,y1,dlg.m_string);
			Invalidate(false);
		}
		
	}

	CView::OnLButtonUp(nFlags, point);
}

void CWCView::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CClientDC dc(this);
	CPen pen;
	pen.CreatePen(PS_SOLID,width,colorpen);
	CBrush brush(colorbrush);
	dc.SelectObject(&brush);
	dc.SelectObject(&pen);
	if (m_isbuttondown)
	{
		if (flag=="rectangle")
		{
			CBrush *pbrush=CBrush::FromHandle(   (HBRUSH)GetStockObject(NULL_BRUSH)   );
			dc.SelectObject(pbrush);
			dc.SetROP2(R2_NOTXORPEN);
			dc.Rectangle(x1,y1,x2,y2);
			dc.Rectangle(x1,y1,point.x,point.y);
		}
		if (flag=="fillrectangle")
		{
			dc.SetROP2(R2_NOTXORPEN);
			dc.Rectangle(x1,y1,x2,y2);
			dc.Rectangle(x1,y1,point.x,point.y);
		}
	}
	x2=point.x;
	y2=point.y;
	CView::OnMouseMove(nFlags, point);
}

void CWCView::OnColorpen() 
{
	// TODO: Add your command handler code here
	CColorDialog dlg;
	if(dlg.DoModal()==IDOK)
	{
		colorpen=dlg.GetColor();
		
	}
}

void CWCView::OnFillrectangle() 
{
	// TODO: Add your command handler code here
	flag="fillrectangle";
}

void CWCView::OnColorbrush() 
{
	// TODO: Add your command handler code here
	CColorDialog dlg;
	if(dlg.DoModal()==IDOK)
	{
		colorbrush=dlg.GetColor();
		
	}
}





void CWCView::OnFont1() 
{
	// TODO: Add your command handler code here
	CFontDialog dlg;
	if(dlg.DoModal()==IDOK)
	{
		flag="font";
		LOGFONT temp;
		dlg.GetCurrentFont(&temp);
		fontstyle.DeleteObject();
		fontstyle.CreateFontIndirect(&temp);
		fontcolor=dlg.GetColor();
	}	
}

void CWCView::OnLinewidthdlg() 
{
	// TODO: Add your command handler code here
	CLinewidthDlg dlg;
	if(dlg.DoModal()==IDOK)
	{
		// TODO: Add your command handler code here
		if(!dlg.m_radio1)
			width=1;
		else if(!dlg.m_radio2)
			width=2;
		else if(!dlg.m_radio3)
			width=3;
		else if(!dlg.m_radio4)
			width=4;	
		else if(!dlg.m_radio5)
			width=5;
	}
}
