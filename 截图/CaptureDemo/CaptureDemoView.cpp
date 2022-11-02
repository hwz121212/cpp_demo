// CaptureDemoView.cpp : implementation of the CCaptureDemoView class
//

#include "stdafx.h"
#include "CaptureDemo.h"

#include "CaptureDemoDoc.h"
#include "CaptureDemoView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CCaptureDemoView

IMPLEMENT_DYNCREATE(CCaptureDemoView, CScrollView)

BEGIN_MESSAGE_MAP(CCaptureDemoView, CScrollView)
END_MESSAGE_MAP()

// CCaptureDemoView construction/destruction

CCaptureDemoView::CCaptureDemoView()
{

}

CCaptureDemoView::~CCaptureDemoView()
{
}

BOOL CCaptureDemoView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CScrollView::PreCreateWindow(cs);
}

// CCaptureDemoView drawing

void CCaptureDemoView::OnDraw(CDC* pDC)
{
   CCaptureDemoDoc* pDoc = GetDocument();
   ASSERT_VALID(pDoc);
   if (!pDoc)
      return;


   CScreenImage& image = pDoc->GetImage();
   if(!image.IsNull())
   {
      CBrush brush(::GetSysColor(COLOR_APPWORKSPACE));
      image.Draw(pDC->GetSafeHdc(), 0, 0);
      FillOutsideRect(pDC, &brush);
   }
   else
   {
      CRect rect;
      pDC->GetClipBox(rect);
      pDC->FillSolidRect(rect, ::GetSysColor(COLOR_APPWORKSPACE));
   }



}

void CCaptureDemoView::OnInitialUpdate()
{
	CSize size(100, 100);
	SetScrollSizes(MM_TEXT, size);
   
}


// CCaptureDemoView diagnostics

#ifdef _DEBUG
void CCaptureDemoView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CCaptureDemoView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CCaptureDemoDoc* CCaptureDemoView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CCaptureDemoDoc)));
	return (CCaptureDemoDoc*)m_pDocument;
}
#endif //_DEBUG


// CCaptureDemoView message handlers

void CCaptureDemoView::OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/)
{
   CCaptureDemoDoc* pDoc = GetDocument();
   CScreenImage& image = pDoc->GetImage();

   if(!image.IsNull())
   {
      CSize size(image.GetWidth(), image.GetHeight());
      SetScrollSizes(MM_TEXT, size);
      ResizeParentToFit();
   }
}
