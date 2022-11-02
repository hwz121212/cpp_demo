#include "StdAfx.h"
#include "TSButton.h"
#include "ConfigWnd.h"
#include "ToolBarWnd.h"
#include "CaptureHelper.h"

CToolBarWnd::CToolBarWnd(void)
{
	m_pRectBtn = NULL;
	m_pEllipseBtn = NULL;
	m_pArrowBtn = NULL;
	m_pBrushBtn = NULL;
	m_pTextBtn = NULL;
	m_pFilterBtn = NULL;
	m_pUndoBtn = NULL;
	m_pSaveBtn = NULL;
	m_pCancelBtn = NULL;
	m_pOKBtn = NULL;
}

CToolBarWnd::~CToolBarWnd(void)
{
	delete m_pRectBtn;
	delete m_pEllipseBtn;
	delete m_pArrowBtn;
	delete m_pBrushBtn;
	delete m_pTextBtn;
	delete m_pFilterBtn;
	delete m_pUndoBtn;
	delete m_pSaveBtn;
	delete m_pCancelBtn;
	delete m_pOKBtn;
}

BEGIN_MESSAGE_MAP(CToolBarWnd, CWnd)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_BN_CLICKED(BTN_RECT, OnRectBtn)
	ON_BN_CLICKED(BTN_ELLIPSE, OnEllipseBtn)
	ON_BN_CLICKED(BTN_ARROW, OnArrowBtn)
	ON_BN_CLICKED(BTN_BRUSH, OnBrushBtn)
	ON_BN_CLICKED(BTN_TEXT, OnTextwBtn)
	ON_BN_CLICKED(BTN_UNDO, OnUndoBtn)
	ON_BN_CLICKED(BTN_SAVE, OnSaveBtn)
	ON_BN_CLICKED(BTN_CANCEL, OnCancelBtn)
	ON_BN_CLICKED(BTN_OK, OnOKBtn)
END_MESSAGE_MAP()


int CToolBarWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// get image directory
	GetModuleFileName(NULL, m_imageDir.GetBufferSetLength(MAX_PATH+1),MAX_PATH);
	m_imageDir.ReleaseBuffer();
	int pos = m_imageDir.ReverseFind('\\');
	m_imageDir = m_imageDir.Left(pos);

	// create buttons
	const int BTN_WIDTH = 22;
	const int BTN_HEIGHT = 22;
	const int BTN_MARGIN = 4;
	const int LEFT_MARGIN = 7;
	const int TOP_MARGIN = 2;
	CRect rt;
	rt.SetRect(LEFT_MARGIN, TOP_MARGIN, LEFT_MARGIN+BTN_WIDTH, TOP_MARGIN+BTN_HEIGHT);
	m_pRectBtn = new CTSButton();
	m_pRectBtn->Create(_T(""), WS_CHILD|WS_VISIBLE|BS_AUTOCHECKBOX , rt, this,  BTN_RECT);
	m_pRectBtn->MoveWindow(&rt);

	rt.OffsetRect(BTN_MARGIN+BTN_WIDTH,0);
	m_pEllipseBtn = new CTSButton();	
	m_pEllipseBtn->Create(_T(""), WS_CHILD | WS_VISIBLE|BS_AUTOCHECKBOX, rt, this, BTN_ELLIPSE);
	m_pEllipseBtn->MoveWindow(&rt);

	rt.OffsetRect(BTN_MARGIN+BTN_WIDTH,0);
	m_pArrowBtn = new CTSButton();
	m_pArrowBtn->Create(_T(""), WS_CHILD | WS_VISIBLE|BS_AUTOCHECKBOX, rt, this, BTN_ARROW);
	m_pArrowBtn->MoveWindow(&rt);

	rt.OffsetRect(BTN_MARGIN+BTN_WIDTH,0);
	m_pBrushBtn = new CTSButton();
	m_pBrushBtn->Create(_T(""), WS_CHILD | WS_VISIBLE|BS_AUTOCHECKBOX, rt, this, BTN_BRUSH);
	m_pBrushBtn->MoveWindow(&rt);

	rt.OffsetRect(BTN_MARGIN+BTN_WIDTH,0);
	m_pTextBtn = new CTSButton();
	m_pTextBtn->Create(_T(""), WS_CHILD | WS_VISIBLE| BS_AUTOCHECKBOX, rt, this, BTN_TEXT);
	m_pTextBtn->MoveWindow(&rt);

	rt.OffsetRect(BTN_WIDTH + 16,0);
	m_pUndoBtn = new CTSButton();
	m_pUndoBtn->Create(_T(""), WS_CHILD | WS_VISIBLE , rt, this, BTN_UNDO);
	m_pUndoBtn->MoveWindow(&rt);

	rt.OffsetRect(BTN_MARGIN+BTN_WIDTH,0);
	m_pSaveBtn = new CTSButton();
	m_pSaveBtn->Create(_T(""), WS_CHILD | WS_VISIBLE , rt, this, BTN_SAVE);
	m_pSaveBtn->MoveWindow(&rt);

	rt.OffsetRect(BTN_WIDTH + 16,0);
	m_pCancelBtn = new CTSButton();
	m_pCancelBtn->Create(_T(""), WS_CHILD | WS_VISIBLE , rt, this, BTN_CANCEL);
	m_pCancelBtn->MoveWindow(&rt);

	rt.OffsetRect(BTN_MARGIN+BTN_WIDTH,0);
	m_pOKBtn = new CTSButton();
	m_pOKBtn->Create(_T(""), WS_CHILD | WS_VISIBLE , rt, this,  BTN_OK);
	m_pOKBtn->MoveWindow(&rt);	

	m_hBmpBk = LoadFile(ImagePath(_T("bk.png")));

	m_pRectBtn->LoadImage(ImagePath(_T("rect_nor.png")), ImagePath(_T("rect_push.png")),ImagePath(_T("rect_hover.png")), ImagePath(_T("rect_dis.png")));
	m_pEllipseBtn->LoadImage(ImagePath(_T("ellipse_nor.png")), ImagePath(_T("ellipse_push.png")),ImagePath(_T("ellipse_hover.png")), ImagePath(_T("ellipse_dis.png")));
	m_pArrowBtn->LoadImage(ImagePath(_T("arrow_nor.png")), ImagePath(_T("arrow_push.png")),ImagePath(_T("arrow_hover.png")), ImagePath(_T("arrow_dis.png")));
	m_pBrushBtn->LoadImage(ImagePath(_T("brush_nor.png")), ImagePath(_T("brush_push.png")),ImagePath(_T("brush_hover.png")), ImagePath(_T("brush_dis.png")));
	m_pTextBtn->LoadImage(ImagePath(_T("text_nor.png")), ImagePath(_T("text_push.png")),ImagePath(_T("text_hover.png")), ImagePath(_T("text_dis.png")));
	m_pUndoBtn->LoadImage(ImagePath(_T("undo_nor.png")), ImagePath(_T("undo_push.png")),ImagePath(_T("undo_hover.png")), ImagePath(_T("undo_dis.png")));
	m_pSaveBtn->LoadImage(ImagePath(_T("save_nor.png")), ImagePath(_T("save_push.png")),ImagePath(_T("save_hover.png")), ImagePath(_T("save_dis.png")));
	m_pCancelBtn->LoadImage(ImagePath(_T("cancel_nor.png")), ImagePath(_T("cancel_push.png")),ImagePath(_T("cancel_hover.png")), ImagePath(_T("cancel_dis.png")));
	m_pOKBtn->LoadImage(ImagePath(_T("ok_nor.png")), ImagePath(_T("ok_push.png")),ImagePath(_T("ok_hover.png")), ImagePath(_T("ok_dis.png")));

	return 0;
}

BOOL CToolBarWnd::OnEraseBkgnd(CDC* pDC )
{
	RECT rect;
	GetClientRect(&rect);   
	CDC  dcMem;
	dcMem.CreateCompatibleDC(pDC);
	dcMem.SelectObject(m_hBmpBk);
	pDC->StretchBlt(0,0,WIDTH,HEIGHT,&dcMem,0,0,   
		WIDTH,HEIGHT,SRCCOPY);

	dcMem.DeleteDC();

	m_pRectBtn->InvalidateBk();
	m_pEllipseBtn->InvalidateBk();
	m_pArrowBtn->InvalidateBk();
	m_pBrushBtn->InvalidateBk();
	m_pTextBtn->InvalidateBk();
	m_pUndoBtn->InvalidateBk();
	m_pSaveBtn->InvalidateBk();
	m_pCancelBtn->InvalidateBk();
	m_pOKBtn->InvalidateBk();
	return CWnd::OnEraseBkgnd(pDC);
}

void CToolBarWnd::OnPaint()
{
	CPaintDC dc(this);
	CWnd::OnPaint();
}

void CToolBarWnd::OnRectBtn()
{
	if (BST_CHECKED == m_pRectBtn->GetCheck())
	{
		m_pEllipseBtn->SetCheckAndInvalid(BST_UNCHECKED);
		m_pArrowBtn->SetCheckAndInvalid(BST_UNCHECKED);
		m_pBrushBtn->SetCheckAndInvalid(BST_UNCHECKED);
		m_pTextBtn->SetCheckAndInvalid(BST_UNCHECKED);
		GetParent()->PostMessage(WM_CAPTURE_RECT, TRUE, NULL);
	}
	else
	{
		GetParent()->PostMessage(WM_CAPTURE_RECT, FALSE, NULL);
	}
}

void CToolBarWnd::OnEllipseBtn()
{
	if (BST_CHECKED == m_pEllipseBtn->GetCheck())
	{
		m_pRectBtn->SetCheckAndInvalid(BST_UNCHECKED);
		m_pArrowBtn->SetCheckAndInvalid(BST_UNCHECKED);
		m_pBrushBtn->SetCheckAndInvalid(BST_UNCHECKED);
		m_pTextBtn->SetCheckAndInvalid(BST_UNCHECKED);
		GetParent()->PostMessage(WM_CAPTURE_ELLIPSE, TRUE, NULL);
	}
	else
	{
		GetParent()->PostMessage(WM_CAPTURE_ELLIPSE, FALSE, NULL);
	}
}

void CToolBarWnd::OnArrowBtn()
{
	if (BST_CHECKED == m_pArrowBtn->GetCheck())
	{
		m_pRectBtn->SetCheckAndInvalid(BST_UNCHECKED);
		m_pEllipseBtn->SetCheckAndInvalid(BST_UNCHECKED);
		m_pBrushBtn->SetCheckAndInvalid(BST_UNCHECKED);
		m_pTextBtn->SetCheckAndInvalid(BST_UNCHECKED);
		GetParent()->PostMessage(WM_CAPTURE_ARROW, TRUE, NULL);
	}
	else
	{
		GetParent()->PostMessage(WM_CAPTURE_ARROW, FALSE, NULL);
	}
}

void CToolBarWnd::OnBrushBtn()
{
	if (BST_CHECKED == m_pBrushBtn->GetCheck())
	{
		m_pRectBtn->SetCheckAndInvalid(BST_UNCHECKED);
		m_pEllipseBtn->SetCheckAndInvalid(BST_UNCHECKED);
		m_pArrowBtn->SetCheckAndInvalid(BST_UNCHECKED);
		m_pTextBtn->SetCheckAndInvalid(BST_UNCHECKED);
		GetParent()->PostMessage(WM_CAPTURE_BRUSH, TRUE, NULL);
	}
	else
	{
		GetParent()->PostMessage(WM_CAPTURE_BRUSH, FALSE, NULL);
	}
}

void CToolBarWnd::OnTextwBtn()
{
	if (BST_CHECKED == m_pTextBtn->GetCheck())
	{
		m_pRectBtn->SetCheckAndInvalid(BST_UNCHECKED);
		m_pEllipseBtn->SetCheckAndInvalid(BST_UNCHECKED);
		m_pArrowBtn->SetCheckAndInvalid(BST_UNCHECKED);
		m_pBrushBtn->SetCheckAndInvalid(BST_UNCHECKED);
		GetParent()->PostMessage(WM_CAPTURE_TEXT, TRUE, NULL);
	}
	else
	{
		GetParent()->PostMessage(WM_CAPTURE_TEXT, FALSE, NULL);
	}
}

void CToolBarWnd::OnUndoBtn()
{
	GetParent()->PostMessage(WM_CAPTURE_UNDO, NULL,NULL);
}

void CToolBarWnd::OnSaveBtn()
{
	GetParent()->PostMessage(WM_CAPTURE_SAVE, NULL,NULL);
}

void CToolBarWnd::OnOKBtn()
{
	GetParent()->SendMessage(WM_CAPTURE_OK, NULL,NULL);
}

void CToolBarWnd::OnCancelBtn()
{
	GetParent()->PostMessage(WM_CAPTURE_CANCEL, NULL,NULL);
}

CString CToolBarWnd::ImagePath(CString name)
{
	CString path = m_imageDir;
	path.Append(_T("\\skin\\capture\\"));
	path.Append(name);

	return path;
}
