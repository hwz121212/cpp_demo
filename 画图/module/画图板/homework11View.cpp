// homework11View.cpp : implementation of the CHomework11View class
//

#include "stdafx.h"
#include "homework11.h"

#include "homework11Doc.h"
#include "homework11View.h"
#include "MainFrm.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHomework11View

IMPLEMENT_DYNCREATE(CHomework11View, CScrollView)

BEGIN_MESSAGE_MAP(CHomework11View, CScrollView)
	//{{AFX_MSG_MAP(CHomework11View)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CScrollView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHomework11View construction/destruction

CHomework11View::CHomework11View()
{
	upordown=0;
	m_down4=0;
}

CHomework11View::~CHomework11View()
{
}

BOOL CHomework11View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CScrollView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CHomework11View drawing

void CHomework11View::OnDraw(CDC* pDC)
{
	CHomework11Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	bool bFirst=true;
	int a,b;
	POSITION ps=pDoc->m_List.GetHeadPosition();
	POSITION pc=pDoc->m_colour.GetHeadPosition();
	POSITION pw=pDoc->m_width.GetHeadPosition();	
	CPoint pt,be;int i=1;
	while(ps)
	{
		pt=pDoc->m_List.GetNext(ps);
	if(bFirst)
	{
		pDC->MoveTo(pt);
		bFirst=false;
	}else if(pt.x!=-2&&pt.y!=-2)
	{	
		a=pDoc->m_colour.GetNext(pc);
		b=pDoc->m_width.GetNext(pw);
		CPen  newpen(PS_SOLID,b,a);
		CPen *oldpen=pDC->SelectObject(&newpen);
		if(i==0)pDC->MoveTo(pt);
	else	pDC->LineTo(pt);
		 pDC->SelectObject(oldpen);
		 newpen.DeleteObject();
		 i=1;
	}else 
	{
		i=0;
		continue;
	}
	}
	POSITION PP=m_point.GetHeadPosition();
	POSITION PC=m_colour.GetHeadPosition();
	POSITION PCP=m_cpoint.GetHeadPosition();
	POSITION PS=m_shape.GetHeadPosition();
	int m_c,m_s;
	CPoint Cpoint,Point;
	while(PS)
	{

	pDC->SelectStockObject(HOLLOW_BRUSH);
	m_s=m_shape.GetNext(PS);
	m_c=m_colour.GetNext(PC);
	CPen newpen(PS_SOLID,1,m_c);
	pDC->SelectObject(&newpen);
	Cpoint=m_cpoint.GetNext(PCP);
	Point=m_point.GetNext(PP);
	if(m_s==1)
	{
	pDC->Ellipse(Cpoint.x,Cpoint.y,Point.x,Point.y);
	}
	else if(m_s==2)
	{
	double r=pow((Point.x-Cpoint.x)*(Point.x-Cpoint.x)+(Point.y-Cpoint.y)*(Point.y-Cpoint.y),0.5);
	pDC->Ellipse((Point.x+Cpoint.x)/2-r,(Point.y+Cpoint.y)/2-r,(Point.x+Cpoint.x)/2+r,(Point.y+Cpoint.y)/2+r);
	}
	else if(m_s==3)
	{
	pDC->Rectangle(Cpoint.x,Cpoint.y,Point.x,Point.y);
	}
	else if(m_s==4)
	{
    pDC->MoveTo(Cpoint);
	pDC->LineTo(Point);
	}
	newpen.DeleteObject();
	}

}

void CHomework11View::OnInitialUpdate()
{
	//CScrollView::OnInitialUpdate();

	CSize sizeTotal(3000,2000);
	CSize sizePage(50,50);
	CSize sizeLine(10,10);
	SetScrollSizes(MM_TEXT, sizeTotal,sizePage,sizeLine);
}

/////////////////////////////////////////////////////////////////////////////
// CHomework11View printing

BOOL CHomework11View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CHomework11View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}
int CHomework11View::getcolour(){CMainFrame a;return a.Colour;}
int CHomework11View::getshape(){CMainFrame a;return a.shape;}
double CHomework11View::getsize(){CMainFrame a;return a.size;}
int CHomework11View::getstype(){CMainFrame a;return a.kind;}
void CHomework11View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CHomework11View diagnostics

#ifdef _DEBUG
void CHomework11View::AssertValid() const
{
	CScrollView::AssertValid();
}

void CHomework11View::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CHomework11Doc* CHomework11View::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CHomework11Doc)));
	return (CHomework11Doc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CHomework11View message handlers

void CHomework11View::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CClientDC pDC(this);
	CHomework11Doc*p=GetDocument();
		upordown=1;
	if(getshape()==4&&m_down4)
	{
	CPen Pen(PS_SOLID,1,getcolour());
	pDC.SelectObject(&Pen);	
	pDC.MoveTo(cpoint);
	pDC.LineTo(point.x,point.y);
	bpoint=point;
	Pen.DeleteObject(); 
	p->m_colour.AddTail(getcolour());
	p->m_width.AddTail(1);
	m_colour.AddTail(getcolour());
	 m_cpoint.AddTail(cpoint);
	 m_point.AddTail(point);
	 m_shape.AddTail(4);
	}
	cpoint=point;
	CScrollView::OnLButtonDown(nFlags, point);
}

void CHomework11View::OnLButtonUp(UINT nFlags, CPoint point) 
{    CClientDC pDC(this);
		upordown=0;	
	CHomework11Doc*p=GetDocument();
	 if(getshape()==1)
	 {
		 m_colour.AddTail(getcolour());
		 m_cpoint.AddTail(cpoint);
		 m_point.AddTail(point);
		 m_shape.AddTail(1);
		pDC.SelectStockObject(HOLLOW_BRUSH);
	    CPen Pen(PS_SOLID,1,getcolour());
		pDC.SelectObject(&Pen);
		pDC.Ellipse(cpoint.x,cpoint.y,point.x,point.y);	
		Pen.DeleteObject(); 
	 }
	 else  if(getshape()==2)
	 {
		 m_colour.AddTail(getcolour());
		 m_cpoint.AddTail(cpoint);
		 m_point.AddTail(point);
		 m_shape.AddTail(2);
		pDC.SelectStockObject(HOLLOW_BRUSH);   
		double r=pow((point.x-cpoint.x)*(point.x-cpoint.x)+(point.y-cpoint.y)*(point.y-cpoint.y),0.5);
		CPoint m=point+cpoint;
		m.x/=2;
		m.y/=2;
		CPen Pen(PS_SOLID,1,getcolour());
		pDC.SelectObject(&Pen);
		pDC.Ellipse(m.x-r,m.y-r,m.x+r,m.y+r);
		Pen.DeleteObject(); 	
	 } 
	 else if(getshape()==3)
	 {
	 m_colour.AddTail(getcolour());
	 m_cpoint.AddTail(cpoint);
	 m_point.AddTail(point);
	 m_shape.AddTail(3);
		pDC.SelectStockObject(HOLLOW_BRUSH);	
		CPen Pen(PS_SOLID,1,getcolour());
		pDC.SelectObject(&Pen);
		pDC.Rectangle(cpoint.x,cpoint.y,point.x,point.y);
		Pen.DeleteObject(); 
	 } else if(getshape()==4)
	 {cpoint=point;	
	 m_colour.AddTail(getcolour());
	 m_cpoint.AddTail(cpoint);
	 m_point.AddTail(point);
	 m_shape.AddTail(4);
	CPen Pen(PS_SOLID,1,getcolour());
	pDC.SelectObject(&Pen);
	pDC.MoveTo(cpoint);
	pDC.LineTo(point.x,point.y);
	Pen.DeleteObject(); 
	}
	 CPoint pt(-2,-2);
	 p->m_List.AddTail(pt);
	CScrollView::OnLButtonUp(nFlags, point);
}

void CHomework11View::OnMouseMove(UINT nFlags, CPoint point) 
{

	CClientDC pDC(this);
	CHomework11Doc*p=GetDocument();

	if(getshape()!=0)
	{
		HCURSOR	hCursor   =   SetCursor(LoadCursor(NULL, IDC_CROSS));
	}
	else if(getstype()==1)
	{
		HCURSOR	hCursor   =  SetCursor(AfxGetApp()->LoadCursor(IDC_CURSOR1));   
	}else if(getstype()==2)
	{
		 HCURSOR	hCursor   =  SetCursor(AfxGetApp()->LoadCursor(IDC_CURSOR2));   
	}
	else if(getstype()==3)
	{
		 HCURSOR	hCursor   =  SetCursor(AfxGetApp()->LoadCursor(IDC_CURSOR3));   
	}
	if(upordown==1){
		if(getshape()==0){m_down4=0;
	CPen newPen(PS_SOLID,1,getcolour());
	CPen*old=pDC.SelectObject(&newPen);
	{
		if(getstype()==1){
	pDC.MoveTo(cpoint);
	pDC.LineTo(point.x,point.y);
	cpoint=point;
	p->m_colour.AddTail(getcolour());
	p->m_List.AddTail(point);
	p->m_width.AddTail(getsize());
	}
	else if(getstype()==2)
	{
	CPen  newpen(PS_SOLID,getsize()+5,getcolour());
	CPen *oldpen=pDC.SelectObject(&newpen);
	pDC.MoveTo(cpoint);
	pDC.LineTo(point.x,point.y);
	cpoint=point;
	p->m_List.AddTail(point);
	p->m_colour.AddTail(getcolour());
	p->m_width.AddTail(getsize()+5);
	pDC.SelectObject(oldpen);
	}
	else if(getstype()==3)
	{
	int clr=RGB(255,255,255);
	CPen  newpen(PS_SOLID,getsize()+5,clr);
	CPen *oldpen=pDC.SelectObject(&newpen);
	pDC.MoveTo(cpoint);
	pDC.LineTo(point.x,point.y);
	cpoint=point; 
	pDC.SelectObject(oldpen);
	p->m_colour.AddTail(clr);
	p->m_width.AddTail(getsize()+5);	
	p->m_List.AddTail(point);
	}
	}
	newPen.DeleteObject(); 
		}
	else if(getshape()==1)
	{  
		m_down4=0;
		pDC.SetROP2(R2_NOT);
		pDC.SelectStockObject(HOLLOW_BRUSH);
		pDC.Ellipse(cpoint.x,cpoint.y,bpoint.x,bpoint.y);
	   CPen Pen(PS_SOLID,1,getcolour());
		pDC.SelectObject(&Pen);
		pDC.Ellipse(cpoint.x,cpoint.y,point.x,point.y);	
		Pen.DeleteObject(); 
	}else if(getshape()==2)
	{ 
		m_down4=0;
		pDC.SetROP2(R2_NOT);
		pDC.SelectStockObject(HOLLOW_BRUSH);   
		int a=(cpoint.x)<(cpoint.y)?cpoint.y:cpoint.x;
		int b=(point.x)>(point.y)?point.y:point.x;
		int c=(bpoint.x)>(bpoint.y)?bpoint.y:bpoint.x;
		double r=pow((point.x-cpoint.x)*(point.x-cpoint.x)+(point.y-cpoint.y)*(point.y-cpoint.y),0.5);
		double r0=pow((cpoint.x-bpoint.x)*(cpoint.x-bpoint.x)+(cpoint.y-bpoint.y)*(cpoint.y-bpoint.y),0.5);
		CPoint m=point+cpoint;
		CPoint m0=cpoint+bpoint;
		m.x/=2;
		m.y/=2;
		m0.x/=2;
		m0.y/=2;
		pDC.Ellipse(m0.x-r0,m0.y-r0,m0.x+r0,m0.y+r0);
		CPen Pen(PS_SOLID,1,getcolour());
		pDC.SelectObject(&Pen);
		pDC.Ellipse(m.x-r,m.y-r,m.x+r,m.y+r);
		Pen.DeleteObject(); 	
	}
	else if(getshape()==3)
	{
		m_down4=0;
		pDC.SetROP2(R2_NOT);
		pDC.SelectStockObject(HOLLOW_BRUSH);
		pDC.Rectangle(cpoint.x,cpoint.y,bpoint.x,bpoint.y);
		CPen Pen(PS_SOLID,1,getcolour());
		pDC.SelectObject(&Pen);
		pDC.Rectangle(cpoint.x,cpoint.y,point.x,point.y);
		Pen.DeleteObject(); 
	}else if(getshape()==4)
	{ 
		m_down4=1;
	pDC.SetROP2(R2_NOT);
	pDC.MoveTo(cpoint);
	pDC.LineTo(bpoint);
	CPen Pen(PS_SOLID,1,getcolour());
	pDC.SelectObject(&Pen);
	pDC.MoveTo(cpoint);
	pDC.LineTo(point.x,point.y);
	Pen.DeleteObject(); 
	}
	}
bpoint=point;
	CScrollView::OnMouseMove(nFlags, point);
}

void CHomework11View::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{

	
	
}


