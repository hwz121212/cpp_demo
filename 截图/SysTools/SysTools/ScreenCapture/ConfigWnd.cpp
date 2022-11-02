#include "StdAfx.h"
#include "TSButton.h"
#include "ConfigWnd.h"
#include "CaptureHelper.h"


CConfigWnd::CConfigWnd(void):
m_bShowSizeRect(FALSE),
m_bShowTextRect(FALSE)
{
}

CConfigWnd::~CConfigWnd(void)
{
}

BEGIN_MESSAGE_MAP(CConfigWnd, CWnd)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_BN_CLICKED(BTN_SIZE1, OnSize1)
	ON_BN_CLICKED(BTN_SIZE2, OnSize2)
	ON_BN_CLICKED(BTN_SIZE3, OnSize3)
	ON_BN_CLICKED(BTN_COLOR00, OnColor00)
	ON_BN_CLICKED(BTN_COLOR01, OnColor01)
	ON_BN_CLICKED(BTN_COLOR02, OnColor02)
	ON_BN_CLICKED(BTN_COLOR03, OnColor03)
	ON_BN_CLICKED(BTN_COLOR04, OnColor04)
	ON_BN_CLICKED(BTN_COLOR05, OnColor05)
	ON_BN_CLICKED(BTN_COLOR06, OnColor06)
	ON_BN_CLICKED(BTN_COLOR07, OnColor07)

	ON_BN_CLICKED(BTN_COLOR10, OnColor10)
	ON_BN_CLICKED(BTN_COLOR11, OnColor11)
	ON_BN_CLICKED(BTN_COLOR12, OnColor12)
	ON_BN_CLICKED(BTN_COLOR13, OnColor13)
	ON_BN_CLICKED(BTN_COLOR14, OnColor14)
	ON_BN_CLICKED(BTN_COLOR15, OnColor15)
	ON_BN_CLICKED(BTN_COLOR16, OnColor16)
	ON_BN_CLICKED(BTN_COLOR17, OnColor17)
END_MESSAGE_MAP()


int CConfigWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// get image dir
	GetModuleFileName(NULL,m_imageDir.GetBufferSetLength(MAX_PATH+1),MAX_PATH);
	m_imageDir.ReleaseBuffer();
	int pos = m_imageDir.ReverseFind('\\');
	m_imageDir = m_imageDir.Left(pos);

	//
	m_hBmpBk = LoadFile(ImagePath(_T("bg_config.png")));

	RECT rt = {0};
	m_pBtnSize1 = new CTSButton();
	m_pBtnSize1->Create(_T(""), WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON ,rt, this, BTN_SIZE1);
	m_pBtnSize1->LoadImage(ImagePath(_T("size1_nor.png")), ImagePath(_T("size1_push.png")), ImagePath(_T("size1_hover.png")), ImagePath(_T("size1_dis.png")));
	m_pBtnSize1->SetCheck(BST_CHECKED);

	m_pBtnSize2 = new CTSButton();
	m_pBtnSize2->Create(_T(""), WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON , rt, this, BTN_SIZE2);
	m_pBtnSize2->LoadImage(ImagePath(_T("size2_nor.png")), ImagePath(_T("size2_push.png")), ImagePath(_T("size2_hover.png")), ImagePath(_T("size2_dis.png")));

	m_pBtnSize3 = new CTSButton();
	m_pBtnSize3->Create(_T(""), WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON , rt, this, BTN_SIZE3);
	m_pBtnSize3->LoadImage(ImagePath(_T("size3_nor.png")), ImagePath(_T("size3_push.png")), ImagePath(_T("size3_hover.png")), ImagePath(_T("size3_dis.png")));

	m_pBtnSelColor = new CTSButton();
	m_pBtnSelColor->Create(_T(""), WS_CHILD | WS_VISIBLE, rt, this, BTN_COLORSEL);
	m_pBtnSelColor->SetBkColor(BKCOLOR00);
	m_pBtnSelColor->SetBorderColor(BORDER_COLOR);
	m_pBtnSelColor->SetEnabelHover(FALSE);

	InitBtn(m_pBtnColor00, BTN_COLOR00, BKCOLOR00);
	InitBtn(m_pBtnColor01, BTN_COLOR01, BKCOLOR01);
	InitBtn(m_pBtnColor02, BTN_COLOR02, BKCOLOR02);
	InitBtn(m_pBtnColor03, BTN_COLOR03, BKCOLOR03);
	InitBtn(m_pBtnColor04, BTN_COLOR04, BKCOLOR04);
	InitBtn(m_pBtnColor05, BTN_COLOR05, BKCOLOR05);
	InitBtn(m_pBtnColor06, BTN_COLOR06, BKCOLOR06);
	InitBtn(m_pBtnColor07, BTN_COLOR07, BKCOLOR07);

	InitBtn(m_pBtnColor10, BTN_COLOR10, BKCOLOR10);
	InitBtn(m_pBtnColor11, BTN_COLOR11, BKCOLOR11);
	InitBtn(m_pBtnColor12, BTN_COLOR12, BKCOLOR12);
	InitBtn(m_pBtnColor13, BTN_COLOR13, BKCOLOR13);
	InitBtn(m_pBtnColor14, BTN_COLOR14, BKCOLOR14);
	InitBtn(m_pBtnColor15, BTN_COLOR15, BKCOLOR15);
	InitBtn(m_pBtnColor16, BTN_COLOR16, BKCOLOR16);
	InitBtn(m_pBtnColor17, BTN_COLOR17, BKCOLOR17);

	return 0;
}

BOOL CConfigWnd::OnEraseBkgnd(CDC* pDC )
{
	RECT rect;
	GetClientRect(&rect);   
	CDC  dcMem;
	dcMem.CreateCompatibleDC(pDC);
	dcMem.SelectObject(m_hBmpBk);
	pDC->StretchBlt(0,0,248,37,&dcMem,0,0, 248,37,SRCCOPY);

	dcMem.DeleteDC();

	m_pBtnSize1->InvalidateBk();
	m_pBtnSize2->InvalidateBk();
	m_pBtnSize3->InvalidateBk();
	m_pBtnSelColor->InvalidateBk();
	m_pBtnColor00->InvalidateBk();
	m_pBtnColor01->InvalidateBk();
	m_pBtnColor02->InvalidateBk();
	m_pBtnColor03->InvalidateBk();
	m_pBtnColor04->InvalidateBk();
	m_pBtnColor05->InvalidateBk();
	m_pBtnColor06->InvalidateBk();
	m_pBtnColor07->InvalidateBk();
	m_pBtnColor10->InvalidateBk();
	m_pBtnColor11->InvalidateBk();
	m_pBtnColor12->InvalidateBk();
	m_pBtnColor13->InvalidateBk();
	m_pBtnColor14->InvalidateBk();
	m_pBtnColor15->InvalidateBk();
	m_pBtnColor16->InvalidateBk();
	m_pBtnColor17->InvalidateBk();
	return CWnd::OnEraseBkgnd(pDC);
}

void CConfigWnd::OnPaint()
{
	CPaintDC dc(this);
	CWnd::OnPaint();
}

void CConfigWnd::OnSize1()
{
	if (BST_CHECKED == m_pBtnSize1->GetCheck())
	{
		m_pBtnSize2->SetCheckAndInvalid(BST_UNCHECKED);
		m_pBtnSize3->SetCheckAndInvalid(BST_UNCHECKED);
	}
	GetParent()->PostMessage(WM_DRAWITEM_SIZE1, NULL, NULL);
}

void CConfigWnd::OnSize2()
{
	if (BST_CHECKED == m_pBtnSize2->GetCheck())
	{
		m_pBtnSize1->SetCheckAndInvalid(BST_UNCHECKED);
		m_pBtnSize3->SetCheckAndInvalid(BST_UNCHECKED);
	}

	GetParent()->PostMessage(WM_DRAWITEM_SIZE2, NULL, NULL);
}

void CConfigWnd::OnSize3()
{
	if (BST_CHECKED == m_pBtnSize3->GetCheck())
	{
		m_pBtnSize1->SetCheckAndInvalid(BST_UNCHECKED);
		m_pBtnSize2->SetCheckAndInvalid(BST_UNCHECKED);
	}

	GetParent()->PostMessage(WM_DRAWITEM_SIZE3, NULL, NULL);
}

void CConfigWnd::OnColor00()
{
	SelColor(BKCOLOR00);
}
void CConfigWnd::OnColor01()
{
	SelColor(BKCOLOR01);
}
void CConfigWnd::OnColor02()
{
	SelColor(BKCOLOR02);
}
void CConfigWnd::OnColor03()
{
	SelColor(BKCOLOR03);
}
void CConfigWnd::OnColor04()
{
	SelColor(BKCOLOR04);
}
void CConfigWnd::OnColor05()
{
	SelColor(BKCOLOR05);
}
void CConfigWnd::OnColor06()
{
	SelColor(BKCOLOR06);
}
void CConfigWnd::OnColor07()
{
	SelColor(BKCOLOR07);
}
void CConfigWnd::OnColor10()
{
	SelColor(BKCOLOR10);
}
void CConfigWnd::OnColor11()
{
	SelColor(BKCOLOR11);
}
void CConfigWnd::OnColor12()
{
	SelColor(BKCOLOR12);
}
void CConfigWnd::OnColor13()
{
	SelColor(BKCOLOR13);
}
void CConfigWnd::OnColor14()
{
	SelColor(BKCOLOR14);
}
void CConfigWnd::OnColor15()
{
	SelColor(BKCOLOR15);
}
void CConfigWnd::OnColor16()
{
	SelColor(BKCOLOR16);
}
void CConfigWnd::OnColor17()
{
	SelColor(BKCOLOR17);
}

void CConfigWnd::SetRectState(BOOL showSizeRect, BOOL showTextRect)
{
	m_bShowSizeRect = showSizeRect;
	m_bShowTextRect = showTextRect;

	const int HEIGHT = 37;
	CRect rcSizeRect, rcTextRect, rcColorRect;
	if (m_bShowSizeRect)
	{
		const int SIZEWIDTH = 84;
		rcSizeRect.SetRect(3, 0, SIZEWIDTH, HEIGHT);

		CRect rcBtnSize = rcSizeRect;
		const int BTNSIZEWIDTH = 24;
		const int BTNTOPOFFSET = 6;
		const int BTNBOTTOMOFFSET = 7;

		rcBtnSize.SetRect(rcBtnSize.left, rcBtnSize.top+BTNTOPOFFSET, rcBtnSize.left + BTNSIZEWIDTH, rcBtnSize.bottom-BTNBOTTOMOFFSET);
		m_pBtnSize1->MoveWindow(&rcBtnSize);
		m_pBtnSize1->ShowWindow(SW_SHOW);
		rcBtnSize.SetRect(rcBtnSize.right+1, rcBtnSize.top, rcBtnSize.right+1 + BTNSIZEWIDTH, rcBtnSize.bottom);
		m_pBtnSize2->MoveWindow(&rcBtnSize);
		m_pBtnSize2->ShowWindow(SW_SHOW);
		rcBtnSize.SetRect(rcBtnSize.right+1, rcBtnSize.top, rcBtnSize.right+1 + BTNSIZEWIDTH, rcBtnSize.bottom);
		m_pBtnSize3->MoveWindow(&rcBtnSize);
		m_pBtnSize3->ShowWindow(SW_SHOW);
	}
	else
	{
		rcSizeRect.SetRect(0, 0, 0, 0);
		m_pBtnSize1->ShowWindow(SW_HIDE);
		m_pBtnSize2->ShowWindow(SW_HIDE);
		m_pBtnSize3->ShowWindow(SW_HIDE);
	}

	if (m_bShowTextRect)
	{
		const int TEXTWIDTH = 84;
		rcTextRect.SetRect(rcSizeRect.right, 0, rcSizeRect.right + TEXTWIDTH, HEIGHT);
	}
	else
	{
		rcTextRect.SetRect(rcSizeRect.right, 0, rcSizeRect.right, HEIGHT);
	}

	const int COLORWIDTH = 164;
	rcColorRect.SetRect(rcTextRect.right, 0, rcTextRect.right + COLORWIDTH, HEIGHT);
	
	// set btn
	CRect rcBtnSel = rcColorRect;
	rcBtnSel.DeflateRect(5, 4, 131, 5);
	m_pBtnSelColor->MoveWindow(&rcBtnSel);
	CRect rcBtnColor0 = rcColorRect;
	rcBtnColor0.DeflateRect(37,3,112,19);
	m_pBtnColor00->MoveWindow(rcBtnColor0);

	rcBtnColor0.OffsetRect(15,0);
	m_pBtnColor01->MoveWindow(rcBtnColor0);
	rcBtnColor0.OffsetRect(15,0);
	m_pBtnColor02->MoveWindow(rcBtnColor0);
	rcBtnColor0.OffsetRect(15,0);
	m_pBtnColor03->MoveWindow(rcBtnColor0);
	rcBtnColor0.OffsetRect(15,0);
	m_pBtnColor04->MoveWindow(rcBtnColor0);
	rcBtnColor0.OffsetRect(15,0);
	m_pBtnColor05->MoveWindow(rcBtnColor0);
	rcBtnColor0.OffsetRect(15,0);
	m_pBtnColor06->MoveWindow(rcBtnColor0);
	rcBtnColor0.OffsetRect(15,0);
	m_pBtnColor07->MoveWindow(rcBtnColor0);

	CRect rcBtnColor1 = rcColorRect;
	rcBtnColor1.DeflateRect(37,18,112,4);
	m_pBtnColor10->MoveWindow(rcBtnColor1);
	rcBtnColor1.OffsetRect(15,0);
	m_pBtnColor11->MoveWindow(rcBtnColor1);
	rcBtnColor1.OffsetRect(15,0);
	m_pBtnColor12->MoveWindow(rcBtnColor1);
	rcBtnColor1.OffsetRect(15,0);
	m_pBtnColor13->MoveWindow(rcBtnColor1);
	rcBtnColor1.OffsetRect(15,0);
	m_pBtnColor14->MoveWindow(rcBtnColor1);
	rcBtnColor1.OffsetRect(15,0);
	m_pBtnColor15->MoveWindow(rcBtnColor1);
	rcBtnColor1.OffsetRect(15,0);
	m_pBtnColor16->MoveWindow(rcBtnColor1);
	rcBtnColor1.OffsetRect(15,0);
	m_pBtnColor17->MoveWindow(rcBtnColor1);

	Invalidate(TRUE);
}

void CConfigWnd::InitBtn(CTSButton *& pBtn, int ID, DWORD color)
{
	RECT rt = {0};
	pBtn = new CTSButton();
	pBtn->Create(_T(""), WS_CHILD | WS_VISIBLE, rt, this, ID);
	pBtn->SetBkColor(color);
	pBtn->SetBorderColor(BORDER_COLOR);
}

void CConfigWnd::SelColor(DWORD color)
{
	m_pBtnSelColor->SetBkColor(color);
	GetParent()->PostMessage(WM_DRAWITEM_COLOR, color, NULL);
}

CString CConfigWnd::ImagePath(CString name)
{
	CString path = m_imageDir;
	path.Append(_T("\\skin\\capture\\"));
	path.Append(name);

	return path;
}