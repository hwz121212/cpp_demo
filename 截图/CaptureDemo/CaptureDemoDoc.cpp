// CaptureDemoDoc.cpp : implementation of the CCaptureDemoDoc class
//

#include "stdafx.h"
#include "CaptureDemo.h"

#include "CaptureDemoDoc.h"
#include "ScreenSelectionDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CCaptureDemoDoc

IMPLEMENT_DYNCREATE(CCaptureDemoDoc, CDocument)

BEGIN_MESSAGE_MAP(CCaptureDemoDoc, CDocument)
   ON_COMMAND(ID_CAPTURE_SCREEN, &CCaptureDemoDoc::OnCaptureScreen)
   ON_COMMAND(ID_CAPTURE_FOREGROUNDWINDOW, &CCaptureDemoDoc::OnCaptureForegroundwindow)
   ON_COMMAND(ID_CAPTURE_RECTANGLE, &CCaptureDemoDoc::OnCaptureRectangle)
END_MESSAGE_MAP()

// CCaptureDemoDoc construction/destruction

CCaptureDemoDoc::CCaptureDemoDoc()
{
	// TODO: add one-time construction code here

}

CCaptureDemoDoc::~CCaptureDemoDoc()
{
}


BOOL CCaptureDemoDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	return TRUE;
}


// CCaptureDemoDoc serialization
void CCaptureDemoDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{

	}
	else
	{
		// TODO: add loading code here
	}
}


// CCaptureDemoDoc diagnostics

#ifdef _DEBUG
void CCaptureDemoDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CCaptureDemoDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CCaptureDemoDoc commands

void CCaptureDemoDoc::OnCaptureScreen()
{
    CWnd* pWnd = AfxGetMainWnd();
    pWnd->ShowWindow(SW_HIDE);
    ::Sleep(333);

    m_image.CaptureScreen();

    pWnd->ShowWindow(SW_SHOW);
    UpdateAllViews(NULL);
}

void CCaptureDemoDoc::OnCaptureForegroundwindow()
{
    CWnd* pWnd = AfxGetMainWnd();
    pWnd->ShowWindow(SW_HIDE);
    ::Sleep(111);

    HWND hWnd = ::GetForegroundWindow();
    m_image.CaptureWindow(hWnd);

    pWnd->ShowWindow(SW_SHOW);
    UpdateAllViews(NULL);
}

void CCaptureDemoDoc::OnCaptureRectangle()
{
   CRect rect;
   CScreenSelectionDialog dlg(rect);
   dlg.DoModal();
   
   m_image.CaptureRect(rect);

   AfxGetMainWnd()->ShowWindow(SW_SHOW);
   UpdateAllViews(NULL);

}
