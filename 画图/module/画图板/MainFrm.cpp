// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "homework11.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame
#include"set.h"
IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_COMMAND(ID_pencil, Onpencil)
	ON_COMMAND(ID_brush, Onbrush)
	ON_COMMAND(erase, Onerase)
	ON_COMMAND(ID_setwidth, Onsetwidth)
	ON_COMMAND(ID_colour, Oncolour)
	ON_COMMAND(circle, Oncircle)
	ON_COMMAND(rect, Onrect)
	ON_COMMAND(duobianxing, Onduobianxing)
	ON_COMMAND(tuoyuan, Ontuoyuan)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction
int  CMainFrame::Colour=0;
int CMainFrame::kind=1;//1 pencil 2 brush
double CMainFrame::size=1; 
int  CMainFrame::shape=0;//1 tuoyuan 2 circle 3 rect 4 duobianxing
CMainFrame::CMainFrame()
{  
	// TODO: add member initialization code here
	
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_LEFT
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	// TODO: Delete these three lines if you don't want the toolbar to
	//  be dockable
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers


void CMainFrame::Onpencil() 
{
	shape=0;
	kind=1;
}

void CMainFrame::Onbrush() 
{
	kind=2;
	shape=0;	
}

void CMainFrame::Onerase() 
{
	kind=3;
	shape=0;
}

void CMainFrame::Onsetwidth() 
{		
	set a;
	a.DoModal();
	size=a.width;

	
}

void CMainFrame::Oncolour() 
{
	CColorDialog   a;
	a.DoModal();
    Colour=a.GetColor();
}

void CMainFrame::Oncircle() 
{
	// TODO: Add your command handler code here
	shape=2;
	SetCursor(AfxGetApp()->LoadStandardCursor(IDC_CROSS));   	
}

void CMainFrame::Onrect() 
{
	// TODO: Add your command handler code here
	shape=3;	
}

void CMainFrame::Onduobianxing() 
{
	// TODO: Add your command handler code here
	shape=4;	
}

void CMainFrame::Ontuoyuan() 
{
	shape=1;
	
}
