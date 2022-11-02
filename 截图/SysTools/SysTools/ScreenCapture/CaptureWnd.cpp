#include "stdafx.h"
#include "CaptureWnd.h"
#include "CaptureHelper.h"
#include "AlphaBlend.h"
#include <math.h>
#include <GdiPlus.h>

#define ANCHOR_WIDTH	4
#define ANCHOR_HEIGHT	5

#define SIZETIP_WIDTH	70
#define SIZETIP_HEIGHT	20

#define CONFIGWND_WIDTH		248
#define CONFIGWND_HEIGHT	37

#define ZOOM_WIDTH	120
#define ZOOM_HEIGHT	124

#define RE_MIN_WIDTH	60
#define RE_MIN_HEIGHT	20

#define RGB_BORDER	0x0000AEFF
#define RGB_WHITE	0xFFFFFFFF
#define RGB_BLACK	0xFF000000
#define ARGB_MASK_SCREEN	0x66000000
#define ARGB_MASK_SIZETIP	0x85000000
#define ARGB_MASK_ZOOM		0xCE000000

#define ID_RICHEDITCTRL		3000

LPALPHABLEND lpAlphaBlend = GetAlphaBlendFunc();

CCaptureWnd* CCaptureWnd::m_pCapWnd = NULL;

CCaptureWnd::CCaptureWnd(void):
m_state(STATE_SPY_WND),
m_mouseType(MOUSE_UNCLICK),
m_pToolBarWnd(NULL),
m_pConfigWnd(NULL),
m_showToolBarWnd(FALSE),
m_showConfigWnd(FALSE),
m_showSizeTip(FALSE),
m_showZoom(FALSE),
m_drawItem(NULL),
m_drawItemType(DRAWITEM_NONE),
m_drawItemSize(1),
m_drawItemColor(0x00000000),
m_pRE(NULL),
m_pRegisterWnd(NULL),
m_pREPoint(NULL)
{
	InitGDIPlus();
}

CCaptureWnd::~CCaptureWnd(void)
{
	DeInitGDIPlus();
	::DeleteObject(m_hBmpScreen);
	::DeleteObject(m_hBmpScreenMask);
	::DeleteObject(m_hBmpSizeTip);
	::DeleteObject(m_hBmpZoom);

	if (m_pToolBarWnd)
	{
		delete m_pToolBarWnd;
	}

	if (m_pConfigWnd)
	{
		delete m_pConfigWnd;
	}

	std::vector<DrawItem*>::iterator iter = m_drawItems.begin();
	for (;iter != m_drawItems.end(); iter++)
	{
		DrawItem* pItem = *iter;
		delete pItem;
	}
	m_drawItems.clear();

	if (m_drawItem)
	{
		delete m_drawItem;
	}
}

BEGIN_MESSAGE_MAP(CCaptureWnd,CWnd)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_EN_CHANGE(ID_RICHEDITCTRL, OnEnChange)
END_MESSAGE_MAP()

void CCaptureWnd::PostNcDestroy()
{
	::PostMessage(m_pRegisterWnd, WM_USER + 506, NULL, NULL);
	delete this;
}

void CCaptureWnd::OnPaint()
{
	HDC hDC = GetDC()->GetSafeHdc();
	HDC hDCScreen = CreateCompatibleDC(hDC);
	HBITMAP hBmpScreen = CreateCompatibleBitmap(hDC, m_ScreenWidth, m_ScreenHeight);
	HGDIOBJ hOldBmp = ::SelectObject(hDCScreen, hBmpScreen);

	DoPaint(hDCScreen);
	::BitBlt(hDC, 0, 0, m_ScreenWidth, m_ScreenHeight, hDCScreen, 0, 0, SRCCOPY);

	::SelectObject(hDCScreen, hOldBmp);
	::DeleteObject(hBmpScreen);
	::DeleteDC(hDCScreen);
	::ReleaseDC(NULL,hDC);

	__super::OnPaint();
}

WNDPROC oldProc;
LRESULT CALLBACK HookWndProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	LRESULT rc = CallWindowProc( oldProc, hWnd, uMsg, wParam, lParam );

	CWnd *pWnd = CWnd::FromHandle(hWnd);
	CCaptureWnd* pCaptureWnd = static_cast<CCaptureWnd*>(pWnd);
	pCaptureWnd->FilterMessage(uMsg, wParam, lParam);

	return rc;
}

int CCaptureWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	//
	oldProc=(WNDPROC)SetWindowLong(m_hWnd,GWL_WNDPROC,(LONG)HookWndProc); 

	//
	CRect rt;
	::GetWindowRect(GetDesktopWindow()->m_hWnd,&rt);
	m_ScreenWidth = rt.Width();
	m_ScreenHeight = rt.Height();
	MoveWindow(rt.left, rt.top, m_ScreenWidth, m_ScreenHeight);

	//
	m_hBmpScreen = CaptureDesktop();
	CreateMaskBitmap(m_hBmpScreenMask, m_ScreenWidth, m_ScreenHeight, ARGB_MASK_SCREEN);
	CreateMaskBitmap(m_hBmpSizeTip, SIZETIP_WIDTH, SIZETIP_HEIGHT, ARGB_MASK_SIZETIP);
	CreateMaskBitmap(m_hBmpZoom, ZOOM_WIDTH, ZOOM_HEIGHT, ARGB_MASK_ZOOM);

	//
	if (!m_pToolBarWnd)
	{
		RECT rt = {0};
		m_pToolBarWnd = new CToolBarWnd();
		m_pToolBarWnd->Create(NULL, _T("toolbar"), WS_VISIBLE, rt, this, 1000);
		m_pToolBarWnd->ShowWindow(SW_HIDE);
	}

	if (!m_pConfigWnd)
	{
		RECT rt = {0};
		m_pConfigWnd = new CConfigWnd();
		m_pConfigWnd->Create(NULL, _T("config"), WS_VISIBLE, rt, this, 1001);
		m_pConfigWnd->ShowWindow(SW_HIDE);
	}

	return 0;
}

void CCaptureWnd::OnLButtonDown(UINT nFlags, CPoint point)
{
	CAPTURE_STATE state = GetState();
	if (STATE_SPY_WND == state)
	{
		SetState(STATE_CAPTURE_RECT);
		SetMouseType(MOUSE_CLICK_SIZING_RECT_N_S_W_E);
		m_ptFix = point;
		m_showSizeTip = TRUE;
		m_showZoom = TRUE;
		MoveChildWnd(FALSE, FALSE);
	}
	else if (STATE_CAPTURE_RECT == state)
	{
		if (m_rcCap.PtInRect(point))
		{
			SetMouseType(MOUSE_CLICK_MOVING_RECT);
		}
		else if (m_rcNW.PtInRect(point)
			|| m_rcNE.PtInRect(point)
			|| m_rcSW.PtInRect(point)
			|| m_rcSE.PtInRect(point))
		{
			SetMouseType(MOUSE_CLICK_SIZING_RECT_N_S_W_E);

			if (m_rcNW.PtInRect(point)) m_ptFix = CPoint(m_rcCap.right, m_rcCap.bottom);
			if (m_rcNE.PtInRect(point)) m_ptFix = CPoint(m_rcCap.left, m_rcCap.bottom);
			if (m_rcSW.PtInRect(point)) m_ptFix = CPoint(m_rcCap.right, m_rcCap.top);
			if (m_rcSE.PtInRect(point)) m_ptFix = CPoint(m_rcCap.left, m_rcCap.top);

			m_showZoom = true;
			m_showToolBarWnd = FALSE;
			MoveChildWnd(FALSE, FALSE);
		}
		else if (m_rcWest.PtInRect(point)
			|| m_rcEast.PtInRect(point))
		{
			SetMouseType(MOUSE_CLICK_SIZING_RECT_W_E);

			CPoint center;
			center.x = (m_rcCap.right - m_rcCap.left)/2;
			center.y = (m_rcCap.bottom - m_rcCap.top)/2;
			if (m_rcWest.PtInRect(point)) m_ptFix = CPoint(m_rcCap.right, center.y);
			if (m_rcEast.PtInRect(point)) m_ptFix = CPoint(m_rcCap.left, center.y);

			m_showZoom = true;
			m_showToolBarWnd = FALSE;
			MoveChildWnd(FALSE, FALSE);
		}
		else if (m_rcNorth.PtInRect(point)
			|| m_rcSouth.PtInRect(point))
		{
			SetMouseType(MOUSE_CLICK_SIZING_RECT_N_S);

			CPoint center;
			center.x = (m_rcCap.right - m_rcCap.left)/2;
			center.y = (m_rcCap.bottom - m_rcCap.top)/2;
			if (m_rcNorth.PtInRect(point)) m_ptFix = CPoint(center.x, m_rcCap.bottom);
			if (m_rcSouth.PtInRect(point)) m_ptFix = CPoint(center.y, m_rcCap.top);

			m_showZoom = true;
			m_showToolBarWnd = FALSE;
			MoveChildWnd(FALSE, FALSE);
		}
	}
	else if (STATE_EDIT_RECT == state)
	{
		if (m_rcCap.PtInRect(point))
		{
			if (m_drawItemType == DRAWITEM_RECT
				|| m_drawItemType == DRAWITEM_ELLAPSE)
			{
				m_drawItem = NewDrawItem(m_drawItemType);
				((DrawItemRect*)m_drawItem)->rt.left = point.x;
				((DrawItemRect*)m_drawItem)->rt.top = point.y;
			}
			else if (m_drawItemType == DRAWITEM_ARROW)
			{
				m_drawItem = NewDrawItem(m_drawItemType);
				((DrawItemArrow*)m_drawItem)->begin = point;
			}
			else if (m_drawItemType == DRAWITEM_BRUSH)
			{
				m_drawItem = NewDrawItem(m_drawItemType);
				CPoint *pPt = new CPoint(point);
				((DrawItemBrush*)m_drawItem)->points.push_back(pPt);
				Invalidate();
			}
			else if (m_drawItemType == DRAWITEM_TEXT)
			{
				//OutputDebugStringA("DRAWITEM_TEXT \n ");
				if (m_pRE)
				{
					//OutputDebugStringA("m_pRE \n");
					CRect rc;
					::GetWindowRect(m_pRE->m_hWnd, &rc);
					if (!rc.PtInRect(point))
					{
						m_drawItem = NewDrawItem(DRAWITEM_TEXT);
						DrawItemText* pText = static_cast<DrawItemText*>(m_drawItem);
						m_drawItem->type = m_drawItemType;
						pText->hBmp = PrintRichEdit(pText->rt);
						m_drawItems.push_back(m_drawItem);

						m_pRE->DestroyWindow();
						delete m_pRE;
						m_pRE = NULL;
						m_drawItem = NULL;
						//m_pREPoint = new CPoint(point);
					}
				}
				else
				{
					//OutputDebugStringA("m_pREPoint \n ");
					m_pREPoint = new CPoint(point);
				}
			}
		}
		else
		{
			// do nothing
		}
	}
	
	m_lastPoint = point;
	CWnd::OnLButtonDown(nFlags, point);
}

void CCaptureWnd::OnLButtonUp(UINT nFlags, CPoint point)
{
	CAPTURE_STATE state = GetState();
	if (STATE_SPY_WND == state)
	{
		// do nothing
	}
	else if (STATE_CAPTURE_RECT== state)
	{
		MOUSE_TYPE type = GetMouseType();
		if (MOUSE_CLICK_MOVING_RECT == type)
		{
		}
		else if (MOUSE_CLICK_SIZING_RECT_N_S_W_E == type
			|| MOUSE_CLICK_SIZING_RECT_W_E == type
			|| MOUSE_CLICK_SIZING_RECT_N_S == type)
		{
			m_showZoom = false;
			MoveChildWnd(TRUE, FALSE);
		}
	}
	else if (STATE_EDIT_RECT == state)
	{
		if (m_drawItem)
		{
			if (m_drawItemType == DRAWITEM_RECT
				|| m_drawItemType == DRAWITEM_ELLAPSE)
			{
				CPoint pt = NormalizePoint(m_rcCap, point);
				((DrawItemRect*)m_drawItem)->rt.right = pt.x;
				((DrawItemRect*)m_drawItem)->rt.bottom = pt.y;
				m_drawItems.push_back(m_drawItem);
				m_drawItem = NULL;
			}
			else if (m_drawItemType == DRAWITEM_ARROW)
			{
				CPoint pt = NormalizePoint(m_rcCap, point);
				((DrawItemArrow*)m_drawItem)->end = pt;
				m_drawItems.push_back(m_drawItem);
				m_drawItem = NULL;
			}
			else if (m_drawItemType == DRAWITEM_BRUSH)
			{
				CPoint *pPt = new CPoint(point);
				((DrawItemBrush*)m_drawItem)->points.push_back(pPt);
				m_drawItems.push_back(m_drawItem);
				m_drawItem = NULL;
			}
		}
		else
		{
			if (m_drawItemType == DRAWITEM_TEXT)
			{
				if (m_pREPoint) // Text Item not exist
				{
					if (!m_pRE)
					{
						m_pRE = new CRichEditCtrl();
						RECT rt;
						m_pRE->CreateEx(0, ES_MULTILINE|ES_WANTRETURN, rt, this, ID_RICHEDITCTRL);
						m_pRE->SetEventMask(ENM_CHANGE);
						CHARFORMAT cf;
						ZeroMemory(&cf, sizeof(CHARFORMAT));
						cf.cbSize = sizeof(CHARFORMAT);
						cf.dwMask = CFM_COLOR;
						cf.dwEffects &=~CFE_AUTOCOLOR;
						cf.crTextColor = RGB(GetBValue(m_drawItemColor), GetGValue(m_drawItemColor), GetRValue(m_drawItemColor));
						m_pRE->SetDefaultCharFormat(cf);

						CRect rcREBorder = m_rcCap;
						rcREBorder.DeflateRect(1,1,1,1);
						if (rcREBorder.right - m_pREPoint->x < RE_MIN_WIDTH)
						{
							m_pREPoint->x = rcREBorder.right - RE_MIN_WIDTH;
						}
						if (rcREBorder.bottom - m_pREPoint->y < RE_MIN_HEIGHT)
						{
							m_pREPoint->y = rcREBorder.bottom - RE_MIN_HEIGHT;
						}
						m_pRE->MoveWindow(m_pREPoint->x,m_pREPoint->y, RE_MIN_WIDTH, RE_MIN_HEIGHT);
						delete m_pREPoint;
						m_pREPoint = NULL;
						m_pRE->ShowWindow(SW_SHOW);
						m_pRE->SetFocus();
						Invalidate();
					}
				}
			}
		}
	}

	SetMouseType(MOUSE_UNCLICK);
	m_lastPoint = point;
	CWnd::OnLButtonUp(nFlags, point);
}

void CCaptureWnd::OnMouseMove(UINT nFlags, CPoint point)
{
	CAPTURE_STATE state = GetState();
	if (STATE_SPY_WND == state)
	{
		// do nothing
	}
	else if (STATE_CAPTURE_RECT == state)
	{
		MOUSE_TYPE type = GetMouseType();
		if (MOUSE_CLICK_MOVING_RECT == type)
		{
			MoveCapture(point);			
			m_showToolBarWnd = TRUE;
			MoveChildWnd(TRUE, FALSE);
		}
		else if (MOUSE_CLICK_SIZING_RECT_N_S_W_E == type
			||MOUSE_CLICK_SIZING_RECT_N_S == type
			||MOUSE_CLICK_SIZING_RECT_W_E == type)
		{
			SizeCapture(type, point);
			MoveChildWnd(FALSE, FALSE);
			Invalidate();
		}
	}
	else if (STATE_EDIT_RECT == state)
	{
		if (m_drawItem)
		{
			if (m_drawItemType == DRAWITEM_RECT
				|| m_drawItemType == DRAWITEM_ELLAPSE)
			{
				CPoint pt = NormalizePoint(m_rcCap, point);
				((DrawItemRect*)m_drawItem)->rt.right = pt.x;
				((DrawItemRect*)m_drawItem)->rt.bottom = pt.y;
				Invalidate();
			}
			else if (m_drawItemType == DRAWITEM_ARROW)
			{
				CPoint pt = NormalizePoint(m_rcCap, point);
				((DrawItemArrow*)m_drawItem)->end = pt;
				
				Invalidate();
			}
			else if (m_drawItemType == DRAWITEM_BRUSH)
			{
				DrawItemBrush* pBrush = static_cast<DrawItemBrush*>(m_drawItem);
				if (pBrush)
				{
					CPoint *pPt = new CPoint(point);
					pBrush->points.push_back(pPt);
					Invalidate();
				}
			}
		}
	}
	
	SetCursor(point);

	m_lastPoint = point;
	CWnd::OnMouseMove(nFlags, point);
}

CCaptureWnd* CCaptureWnd::GetInstance()
{
	if (!m_pCapWnd)
	{
		m_pCapWnd = new CCaptureWnd();
		CString csWndClass = AfxRegisterWndClass(CS_OWNDC, 0, 0, 0);
		DWORD dwExStyle;
#ifdef DEBUG
		dwExStyle = WS_EX_TOOLWINDOW;
#else
		dwExStyle = WS_EX_TOOLWINDOW|WS_EX_TOPMOST;
#endif
		m_pCapWnd->CreateEx(dwExStyle, csWndClass, _T("Capture_Wnd"), WS_POPUP| WS_CLIPCHILDREN |WS_VISIBLE ,0,20,0,20,NULL,NULL);
	}
	return m_pCapWnd;
}

void CCaptureWnd::EventRegister(HWND hWnd)
{
	m_pRegisterWnd = hWnd;
}

void CCaptureWnd::OnEnChange()
{
	CRect rc;
	m_pRE->GetWindowRect(&rc);
	CRect rcREBorder = m_rcCap;
	rcREBorder.DeflateRect(1,1,1,1);
	int maxWidth = rcREBorder.right - rc.left;

	CSize m_dimRtf = EstimateTextSize(m_pRE, CSize(maxWidth, rc.Height()));
	if (m_dimRtf.cx < RE_MIN_WIDTH)
	{
		m_dimRtf.cx = RE_MIN_WIDTH;
	}
	if (m_dimRtf.cy > rcREBorder.bottom - rc.top)
	{
		m_dimRtf.cy = rcREBorder.bottom - rc.top;
		int n = m_pRE->GetTextLength();
		if (n>0)
		{
			m_pRE->SetSel(n-1,n);
			m_pRE->ReplaceSel(_T(""));
		}
	}

	m_pRE->MoveWindow(rc.left,rc.top,m_dimRtf.cx, m_dimRtf.cy );
}

BOOL CCaptureWnd::FilterMessage(UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	switch(uMsg)
	{
	case WM_CAPTURE_OK:
		{
			HBITMAP hBmpPainted = PaintDrawItems();
			HBITMAP hBmp = ChopBitmap(hBmpPainted, m_rcCap.left, m_rcCap.top,
				m_rcCap.right - m_rcCap.left,
				m_rcCap.bottom - m_rcCap.top);
			::DeleteObject(hBmpPainted);

			CopyBitmapToClipBoard(hBmp);
			if (m_pRegisterWnd)
			{
				::PostMessage(m_pRegisterWnd, WM_USER + 504, (WPARAM)hBmp, NULL);
			}
			m_pCapWnd = NULL;
			PostMessage(WM_CLOSE, NULL, NULL);
		}
		break;
	case WM_KEYDOWN:
		{
			if(VK_ESCAPE == wParam) 
			{
				m_pCapWnd = NULL;
				PostMessage(WM_CLOSE, NULL, NULL);
			}
		}
		break;
	case WM_CAPTURE_CANCEL:
		{
			ShowWindow(SW_HIDE);
			m_pCapWnd = NULL;
			PostMessage(WM_CLOSE, NULL, NULL);

		}
		break;
	case WM_CAPTURE_RECT:
		{
			SetState(STATE_EDIT_RECT);
			m_pConfigWnd->SetRectState(TRUE, FALSE);
			MoveChildWnd(TRUE, (BOOL)wParam);

			EmptyEdit();
			m_drawItemType = DRAWITEM_RECT;
		}
		break;
	case WM_CAPTURE_ELLIPSE:
		{
			SetState(STATE_EDIT_RECT);
			m_pConfigWnd->SetRectState(TRUE, FALSE);
			MoveChildWnd(TRUE, (BOOL)wParam);

			EmptyEdit();
			m_drawItemType = DRAWITEM_ELLAPSE;
			
		}
		break;
	case WM_CAPTURE_ARROW:
		{
			SetState(STATE_EDIT_RECT);
			m_pConfigWnd->SetRectState(FALSE, TRUE);
			MoveChildWnd(TRUE, (BOOL)wParam);

			EmptyEdit();
			m_drawItemType = DRAWITEM_ARROW;
		}
		break;
	case WM_CAPTURE_BRUSH:
		{
			SetState(STATE_EDIT_RECT);
			m_pConfigWnd->SetRectState(TRUE, FALSE);
			MoveChildWnd(TRUE, (BOOL)wParam);

			EmptyEdit();
			m_drawItemType = DRAWITEM_BRUSH;
		}
		break;
	case WM_CAPTURE_TEXT:
		{
			SetState(STATE_EDIT_RECT);
			m_pConfigWnd->SetRectState(FALSE, TRUE);
			MoveChildWnd(TRUE, (BOOL)wParam);

			m_drawItemType = DRAWITEM_TEXT;
		}
		break;
	case WM_CAPTURE_UNDO:
		{
			EmptyEdit();
			if (m_drawItems.size() >=1)
			{
				DrawItem* item = m_drawItems.at(m_drawItems.size()-1);
				delete item;
				m_drawItems.pop_back();
				Invalidate();
			}
		}
		break;
	case WM_CAPTURE_SAVE:
		{
			HBITMAP hBmpPainted = PaintDrawItems();
			HBITMAP hBmp = ChopBitmap(hBmpPainted, m_rcCap.left, m_rcCap.top,
				m_rcCap.right - m_rcCap.left,
				m_rcCap.bottom - m_rcCap.top);
			::DeleteObject(hBmpPainted);

			SYSTEMTIME st;
			GetSystemTime(&st);
			CString defaultName;
			defaultName.Format(_T("screen_capture_%d%d%d%d%d%d"), st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute,st.wSecond);
			CFileDialog fd(FALSE, _T(""), defaultName, OFN_FILEMUSTEXIST|OFN_HIDEREADONLY, _T("文件类型(*.png)|*.png||”"), this);
			if(fd.DoModal() == IDOK)
			{
				CString szGetName = fd.GetPathName();
				SaveFile(szGetName, hBmp);
			}

			::DeleteObject(hBmp);
			m_pCapWnd = NULL;
			PostMessage(WM_CLOSE, NULL, NULL);
		}
		break;
	case WM_DRAWITEM_SIZE1:
		{
			m_drawItemSize = 1;
		}
		break;
	case WM_DRAWITEM_SIZE2:
		{
			m_drawItemSize = 3;
		}
		break;
	case WM_DRAWITEM_SIZE3:
		{
			m_drawItemSize = 5;
		}
		break;
		
	case WM_DRAWITEM_COLOR:
		{
			m_drawItemColor = (DWORD)wParam;
			if (m_pRE && m_drawItemType == DRAWITEM_TEXT)
			{
				CHARFORMAT cf;
				ZeroMemory(&cf, sizeof(CHARFORMAT));
				cf.cbSize = sizeof(CHARFORMAT);
				cf.dwMask = CFM_COLOR;
				cf.dwEffects &=~CFE_AUTOCOLOR;
				cf.crTextColor = RGB(GetBValue(m_drawItemColor), GetGValue(m_drawItemColor), GetRValue(m_drawItemColor));
				m_pRE->SetDefaultCharFormat(cf);
				int nsel = m_pRE->GetWindowTextLength();
				m_pRE->SetSel(0, nsel);
				m_pRE->SetSelectionCharFormat(cf);
			}
		}
		break;
	default:
		break;
	}

	return TRUE;
}

void CCaptureWnd::EmptyEdit()
{
	if (m_pRE)
	{
		m_drawItem = NewDrawItem(DRAWITEM_TEXT);
		DrawItemText* pText = static_cast<DrawItemText*>(m_drawItem);
		m_drawItem->type = m_drawItemType;
		pText->hBmp = PrintRichEdit(pText->rt);
		m_drawItems.push_back(m_drawItem);
		m_drawItem = NULL;

		m_pRE->DestroyWindow();
		delete m_pRE;
		m_pRE = NULL;
	}
}

void CCaptureWnd::DoPaint(HDC &hDC)
{
	PaintScreen(hDC);
	PaintMask(hDC);
	PaintCaptureRect(hDC);
	PaintBorder(hDC);
	PaintSizeTip(hDC);
	PaintZoom(hDC);
}

void CCaptureWnd::PaintScreen(HDC &hDC)
{
	HDC hMemDC = CreateCompatibleDC(hDC);
	HGDIOBJ hOldBmp = ::SelectObject(hMemDC,m_hBmpScreen);
	::BitBlt(hDC,
		0,
		0,
		m_ScreenWidth,
		m_ScreenHeight,
		hMemDC,
		0,
		0,
		SRCCOPY);

	::SelectObject(hMemDC,hOldBmp);
	::DeleteDC(hMemDC);
}

void CCaptureWnd::PaintMask(HDC &hDC)
{
	HDC hMemDC = CreateCompatibleDC(hDC);

	HGDIOBJ hOldBmp = ::SelectObject(hMemDC, m_hBmpScreenMask);

	BLENDFUNCTION bf = { AC_SRC_OVER, 0, 0xFF, AC_SRC_ALPHA };

	lpAlphaBlend(hDC, 0,0,m_ScreenWidth, m_ScreenHeight,
		hMemDC,0,0, m_ScreenWidth, m_ScreenHeight, bf);

	::SelectObject(hMemDC,hOldBmp);
	::DeleteDC(hMemDC);
}

void CCaptureWnd::PaintCaptureRect(HDC &hDC)
{
	HDC hMemDC = CreateCompatibleDC(hDC);
	HBITMAP hBitmap = CopyBitmap(m_hBmpScreen);
	HGDIOBJ hOldBmp = ::SelectObject(hMemDC,hBitmap);

	PaintItems(hMemDC);
	PaintItem(hMemDC, m_drawItem);

	::BitBlt(hDC,
		m_rcCap.left,
		m_rcCap.top,
		m_rcCap.right-m_rcCap.left,
		m_rcCap.bottom-m_rcCap.top,
		hMemDC,
		m_rcCap.left,
		m_rcCap.top,
		SRCCOPY);

	::SelectObject(hMemDC,hOldBmp);
	::DeleteObject(hBitmap);
	::DeleteDC(hMemDC);
}

void CCaptureWnd::PaintSizeTip(HDC &hDC)
{
	if (!m_showSizeTip) return;

	HDC hMemDC = CreateCompatibleDC(hDC);
	HGDIOBJ hOldBmp = ::SelectObject(hMemDC, m_hBmpSizeTip);

	BLENDFUNCTION bf = { AC_SRC_OVER, 0, 0xFF, AC_SRC_ALPHA };

	lpAlphaBlend(hDC, m_rcSizeTip.left, m_rcSizeTip.top, SIZETIP_WIDTH, SIZETIP_HEIGHT,
		hMemDC, 0, 0, SIZETIP_WIDTH, SIZETIP_HEIGHT, bf);
	::SelectObject(hMemDC,hOldBmp);
	::DeleteDC(hMemDC);

	CRect rc = m_rcSizeTip;
	rc.top +=2;
	CString str;
	int width = m_rcCap.right - m_rcCap.left;
	int height = m_rcCap.bottom - m_rcCap.top;
	if (width == 0) width=1;
	if (height == 0) height=1;
	str.Format(_T("%d x %d"), width, height);
	DrawText(hDC, (RECT)rc, str.GetBuffer(), RGB_WHITE, DT_CENTER|DT_VCENTER);
	str.ReleaseBuffer();
}

void CCaptureWnd::PaintZoom(HDC &hDC)
{
#define ZOOM_SRC_WIDTH	40
#define ZOOM_SRC_HEIGHT	40
#define ZOOM_DST_BORDER	1
#define ZOOM_DST_WIDTH	ZOOM_WIDTH-2
#define ZOOM_DST_HEIGHT	88
#define ZOOM_CROSS_THICK	2
#define TEXT_OFFSET_X	5
#define TEXT_HEIGHT		17

	if (!m_showZoom) return;
	
	HDC hMemDC = CreateCompatibleDC(hDC);
	HGDIOBJ hOldBmp = ::SelectObject(hMemDC, m_hBmpZoom);

	BLENDFUNCTION bf = { AC_SRC_OVER, 0, 0xFF, AC_SRC_ALPHA };

	lpAlphaBlend(hDC, m_rcZoom.left, m_rcZoom.top, ZOOM_WIDTH, ZOOM_HEIGHT,
		hMemDC, 0, 0, ZOOM_WIDTH, ZOOM_HEIGHT, bf);

	::SelectObject(hMemDC, m_hBmpScreen);
	StretchBlt(hDC, 
		m_rcZoom.left+ZOOM_DST_BORDER, 
		m_rcZoom.top+ZOOM_DST_BORDER, 
		ZOOM_DST_WIDTH, 
		ZOOM_DST_HEIGHT,
		hMemDC,
		m_ptZoom.x - ZOOM_SRC_WIDTH/2, 
		m_ptZoom.y - ZOOM_SRC_HEIGHT/2, 
		ZOOM_SRC_WIDTH, 
		ZOOM_SRC_HEIGHT, 
		SRCCOPY);
	
	::SelectObject(hMemDC, hOldBmp);
	::DeleteDC(hMemDC);

	//
	CRect rcVer;
	rcVer.SetRect(m_rcZoom.left + ZOOM_WIDTH/2, m_rcZoom.top+ZOOM_DST_BORDER, m_rcZoom.left+ZOOM_WIDTH/2+ZOOM_CROSS_THICK,m_rcZoom.top+ZOOM_DST_HEIGHT);
	FillRect(hDC, (RECT)rcVer, RGB_BORDER);

	CRect rcHor;
	rcHor.SetRect(m_rcZoom.left+ZOOM_DST_BORDER, m_rcZoom.top+ZOOM_DST_HEIGHT/2 + ZOOM_DST_BORDER,m_rcZoom.right-ZOOM_DST_BORDER, m_rcZoom.top+ZOOM_DST_HEIGHT/2+ ZOOM_DST_BORDER + ZOOM_CROSS_THICK);
	FillRect(hDC, rcHor, RGB_BORDER);

	//
	CRect rcSize;
	rcSize.SetRect(m_rcZoom.left + TEXT_OFFSET_X, m_rcZoom.top + ZOOM_DST_HEIGHT + ZOOM_DST_BORDER, m_rcZoom.right, m_rcZoom.top+ZOOM_DST_HEIGHT+TEXT_HEIGHT);

	CString strSize;
	strSize.Format(_T("%d x %d"), m_rcCap.right-m_rcCap.left, m_rcCap.bottom-m_rcCap.top);
	DrawText(hDC, (RECT)rcSize, strSize.GetBuffer(), RGB_WHITE, 0);
	strSize.ReleaseBuffer();
	//
	CRect rcRGB = rcSize;
	rcRGB.OffsetRect(0, TEXT_HEIGHT);

	CString strRGB;
	DWORD color = GetRGB(m_hBmpScreen, m_ptZoom.x, m_ptZoom.y);
	strRGB.Format(_T("RGB:(%d,%d,%d)"), GetRValue(color), GetRValue(color), GetRValue(color));
	DrawText(hDC, (RECT)rcRGB, strRGB.GetBuffer(), RGB_WHITE, 0);
	strRGB.ReleaseBuffer();
}

void CCaptureWnd::PaintBorder(HDC &hDC)
{
	DrawRect(hDC, m_rcCap, 1, RGB_BORDER);
	FillRect(hDC, m_rcWest, RGB_BORDER);
	FillRect(hDC, m_rcNorth, RGB_BORDER);
	FillRect(hDC, m_rcEast, RGB_BORDER);
	FillRect(hDC, m_rcSouth, RGB_BORDER);
	FillRect(hDC, m_rcNW, RGB_BORDER);
	FillRect(hDC, m_rcSW, RGB_BORDER);
	FillRect(hDC, m_rcNE, RGB_BORDER);
	FillRect(hDC, m_rcSE, RGB_BORDER);
}

void CCaptureWnd::PaintItems(HDC &hDC)
{
	std::vector<DrawItem*>::iterator iter = m_drawItems.begin();
	for (;iter != m_drawItems.end(); iter++)
	{
		PaintItem(hDC, static_cast<DrawItem*>(*iter));		
	}
}

void CCaptureWnd::PaintItem(HDC &hDC, DrawItem *pItem)
{
	if (!pItem) return;

	switch (pItem->type)
	{
	case DRAWITEM_RECT:
		{
			DrawItemRect* pRect = static_cast<DrawItemRect*>(pItem);
			DrawRect(hDC, pRect->rt, pRect->size, pRect->color);
		}
		break;
	case DRAWITEM_ELLAPSE:
		{
			DrawItemRect* pRect = static_cast<DrawItemRect*>(pItem);
			DrawRoundRect(hDC, pRect->rt, pRect->size, 
				pRect->rt.right - pRect->rt.left,
				pRect->rt.bottom - pRect->rt.top,
				pRect->color);
		}
		break;
	case DRAWITEM_ARROW:
		{
			PaintArrow(hDC, (DrawItemArrow*)pItem);
		}
		break;
	case DRAWITEM_BRUSH:
		{
			PaintBrush(hDC, (DrawItemBrush*)pItem);
		}
		break;
	case DRAWITEM_TEXT:
		{
			PaintText(hDC, (DrawItemText*)pItem);
		}
		break;
	default:
		break;
	}
}

void CCaptureWnd::PaintArrow(HDC &hDC, DrawItemArrow* pArrow)
{
    // Create the outline for our custom end cap. 
	Gdiplus::GraphicsPath path;
    path.AddLine(Gdiplus::Point(0, -1), Gdiplus::Point(2, -1));
    path.AddLine(Gdiplus::Point(2, -1), Gdiplus::Point(0, 4));
    path.AddLine(Gdiplus::Point(0, 4), Gdiplus::Point(-2, -1));
    path.CloseFigure();
 
    // Create a pen and set end custom start and end 
    // caps to the hook cap. 
	Gdiplus::Color color(255, GetBValue(pArrow->color), GetGValue(pArrow->color), GetRValue(pArrow->color));
	const float ARROW_SIZE = 1.0f;
    Gdiplus::Pen pen(color,ARROW_SIZE);
    Gdiplus::CustomLineCap custCap(&path, NULL);
 	pen.SetCustomEndCap(&custCap);
   
    // Draw the lines. 
	Gdiplus::Graphics g(hDC);
    g.DrawLine(&pen,pArrow->begin.x, pArrow->begin.y, pArrow->end.x, pArrow->end.y);
}

void CCaptureWnd::PaintBrush(HDC &hDC, DrawItemBrush* pBrush)
{
	std::vector<CPoint*>::iterator iter;
	for (iter = pBrush->points.begin(); iter != pBrush->points.end(); iter++)
	{
		RECT rt;
		rt.left = (*iter)->x;
		rt.right = (*iter)->x;
		rt.top = (*iter)->y;
		rt.bottom = (*iter)->y;
		DrawLine(hDC, *(*iter), *(*iter), pBrush->size, pBrush->color, 0);

		if (iter != pBrush->points.begin())
		{
			CPoint *end = *iter;
			CPoint *begin = *(iter-1);
			if (end != begin)
			{
				RECT rt;
				rt.left = min(begin->x, end->x);
				rt.right = max(begin->x, end->x);
				rt.top = min(begin->y, end->y);
				rt.bottom = max(begin->y, end->y);

				DrawLine(hDC, *begin, *end, pBrush->size, pBrush->color, 0);
			}
		}
	}
}

void CCaptureWnd::PaintText(HDC &hDC, DrawItemText* pText)
{
	if (!pText->hBmp)
	{
		if (m_pRE)
		{
			RECT rt;
			m_pRE->GetClientRect(&rt);
			POINT ptLeftTop,ptRightBottom;
			ptLeftTop.x = rt.left;
			ptLeftTop.y = rt.top;
			ptRightBottom.x = rt.right;
			ptRightBottom.y = rt.bottom;
			::ClientToScreen(m_pRE->m_hWnd, &ptLeftTop);
			::ClientToScreen(m_pRE->m_hWnd, &ptRightBottom);
			ptLeftTop.x -=1;
			ptLeftTop.y -=1;
			ptRightBottom.x +=1;
			ptRightBottom.y +=1;

			POINT ptLeftBottom,ptRightTop;
			ptRightTop = ptLeftTop;
			ptRightTop.x = ptRightBottom.x;
			ptLeftBottom = ptRightBottom;
			ptLeftBottom.x = ptLeftTop.x;

			DrawLine(hDC, ptLeftTop, ptRightTop, 1, RGB_BLACK, PS_DASHDOT);
			DrawLine(hDC, ptLeftTop, ptLeftBottom, 1, RGB_BLACK, PS_DASHDOT);
			DrawLine(hDC, ptRightTop, ptRightBottom, 1, RGB_BLACK, PS_DASHDOT);
			DrawLine(hDC, ptLeftBottom, ptRightBottom, 1, RGB_BLACK, PS_DASHDOT);
		}
	}
	else
	{
		HDC hMemDC = CreateCompatibleDC(hDC);
		HGDIOBJ hOldBmp = ::SelectObject(hMemDC, pText->hBmp);


		BLENDFUNCTION bf = { AC_SRC_OVER, 0, 0xFF, AC_SRC_ALPHA };

		lpAlphaBlend(hDC, pText->rt.left, pText->rt.top,
			pText->rt.right - pText->rt.left,
			pText->rt.bottom - pText->rt.top,
			hMemDC,
			0,
			0,
			pText->rt.right - pText->rt.left,
			pText->rt.bottom - pText->rt.top,
			bf);

		::SelectObject(hMemDC,hOldBmp);
		::DeleteDC(hMemDC);
	}
}

void CCaptureWnd::FullScreen()
{
	
}

void CCaptureWnd::MoveCapture(CPoint point)
{
	int offsetx = 0, offsety=0;
	offsetx = point.x - m_lastPoint.x;
	offsety = point.y - m_lastPoint.y;

	if (m_rcCap.left + offsetx < 0 )
	{
		offsetx = m_rcCap.left;
	}
	if (m_rcCap.right + offsetx > m_ScreenWidth)
	{
		offsetx = m_ScreenWidth - m_rcCap.right;
	}
	
	if (m_rcCap.top + offsety < 0)
	{
		offsety = m_rcCap.top;
	}
	if (m_rcCap.bottom + offsety > m_ScreenHeight)
	{
		offsety = m_ScreenHeight - m_rcCap.bottom;
	}

	m_rcCap.OffsetRect(offsetx, offsety);
	SetAnchor();
}

void CCaptureWnd::SizeCapture(MOUSE_TYPE type, CPoint point)
{
	SizeCaptureRect(type, point);
	LimitCaptureRect();
	SetAnchor();
}

void CCaptureWnd::SizeCaptureRect(MOUSE_TYPE type, CPoint point)
{
	if (MOUSE_CLICK_SIZING_RECT_N_S_W_E == type)
	{
		m_rcCap.left = min(m_ptFix.x, point.x);
		m_rcCap.right = max(m_ptFix.x, point.x);
		m_rcCap.top = min(m_ptFix.y, point.y);
		m_rcCap.bottom = max(m_ptFix.y, point.y);
	}
	else if (MOUSE_CLICK_SIZING_RECT_W_E == type)
	{
		m_rcCap.left = min(m_ptFix.x, point.x);
		m_rcCap.right = max(m_ptFix.x, point.x);
	}
	else if (MOUSE_CLICK_SIZING_RECT_N_S == type)
	{
		m_rcCap.top = min(m_ptFix.y, point.y);
		m_rcCap.bottom = max(m_ptFix.y, point.y);
	}
}

void CCaptureWnd::LimitCaptureRect()
{
	if (m_rcCap.right >= m_ScreenWidth)
	{
		m_rcCap.right = m_ScreenWidth;
	}
	if (m_rcCap.bottom >= m_ScreenHeight)
	{
		m_rcCap.right = m_ScreenHeight;
	}
}

void CCaptureWnd::SetAnchor()
{
	m_rcNW = m_rcCap;
	m_rcNW.OffsetRect(-2,-2);
	m_rcNW.right = m_rcNW.left + ANCHOR_WIDTH;
	m_rcNW.bottom = m_rcNW.top + ANCHOR_HEIGHT;
	//
	m_rcNE = m_rcCap;
	m_rcNE.OffsetRect(1,-2);
	m_rcNE.left = m_rcNE.right - ANCHOR_WIDTH;
	m_rcNE.bottom = m_rcNE.top + ANCHOR_HEIGHT;
	//
	m_rcSW = m_rcCap;
	m_rcSW.OffsetRect(-2,2);
	m_rcSW.right = m_rcSW.left + ANCHOR_WIDTH;
	m_rcSW.top = m_rcSW.bottom - ANCHOR_HEIGHT;
	//
	m_rcSE = m_rcCap;
	m_rcSE.OffsetRect(1,2);
	m_rcSE.left = m_rcSE.right - ANCHOR_WIDTH;
	m_rcSE.top = m_rcSE.bottom - ANCHOR_HEIGHT;
	
	//
	CPoint center;
	center.x = (m_rcCap.right - m_rcCap.left)/2;
	center.y = (m_rcCap.bottom - m_rcCap.top)/2;

	m_rcWest = m_rcCap;
	m_rcWest.OffsetRect(-2,0);
	m_rcWest.right = m_rcWest.left + ANCHOR_WIDTH;
	m_rcWest.top = m_rcCap.top + center.y - ANCHOR_HEIGHT/2;
	m_rcWest.bottom = m_rcWest.top + ANCHOR_HEIGHT;
	//
	m_rcNorth = m_rcCap;
	m_rcNorth.OffsetRect(0,-2);
	m_rcNorth.left = m_rcCap.left + center.x - ANCHOR_WIDTH/2;
	m_rcNorth.right = m_rcNorth.left + ANCHOR_WIDTH;
	m_rcNorth.bottom = m_rcNorth.top + ANCHOR_HEIGHT;
	//
	m_rcEast = m_rcCap;
	m_rcEast.OffsetRect(1,0);
	m_rcEast.left = m_rcEast.right - ANCHOR_WIDTH;
	m_rcEast.top = m_rcCap.top + center.y - ANCHOR_HEIGHT/2;
	m_rcEast.bottom = m_rcEast.top + ANCHOR_HEIGHT;
	//
	m_rcSouth = m_rcCap;
	m_rcSouth.OffsetRect(0,2);
	m_rcSouth.left = m_rcCap.left + center.x - ANCHOR_WIDTH/2;
	m_rcSouth.right = m_rcSouth.left + ANCHOR_WIDTH;
	m_rcSouth.top =  m_rcSouth.bottom - ANCHOR_HEIGHT;
	//
}

void CCaptureWnd::SetCursor(CPoint point)
{
	CAPTURE_STATE state = GetState();
	if (STATE_SPY_WND == state)
	{
		::SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(IDC_ARROW)));
	}
	else if (STATE_CAPTURE_RECT == state)
	{
		if (m_rcCap.PtInRect(point))
		{
			::SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(IDC_SIZEALL)));
		}
		else if (m_rcWest.PtInRect(point) || m_rcEast.PtInRect(point))
		{
			::SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(IDC_SIZEWE)));
		}
		else if (m_rcNorth.PtInRect(point) || m_rcSouth.PtInRect(point))
		{
			::SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(IDC_SIZENS)));
		}
		else if (m_rcNW.PtInRect(point) || m_rcSE.PtInRect(point))
		{
			::SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(IDC_SIZENWSE)));
		}
		else if (m_rcNE.PtInRect(point) || m_rcSW.PtInRect(point))
		{
			::SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(IDC_SIZENESW)));
		}
		else
		{
			::SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(IDC_ARROW)));
		}
	}
	else if (STATE_EDIT_RECT == state)
	{
		if (m_rcCap.PtInRect(point))
		{
			::SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(IDC_CROSS)));
		}
		else
		{
			::SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(IDC_ARROW)));
		}
	}
}

void CCaptureWnd::MoveChildWnd(BOOL showToolbar, BOOL showConfigWnd)
{
	int requireHeight = 0;
	int topSizeTip = 0;
	int topToolbar = 0;
	const int offsety = 2;
	if (showToolbar)
	{
		requireHeight += offsety;
		requireHeight += CToolBarWnd::HEIGHT;
	}
	if (showConfigWnd)
	{
		ASSERT(showToolbar);
		requireHeight += offsety;
		requireHeight += CONFIGWND_HEIGHT;
	}

	if (m_ScreenHeight - m_rcCap.bottom > requireHeight)
	{
		topSizeTip = m_rcCap.top - SIZETIP_HEIGHT;

		if (topSizeTip < 0)
		{
			topSizeTip = m_rcCap.top;
		}

		topToolbar = m_rcCap.bottom;
	}
	else
	{
		topSizeTip = m_rcCap.top - (SIZETIP_HEIGHT + offsety + requireHeight + offsety);
		if (topSizeTip < 0)
		{
			topSizeTip = m_rcCap.top;
		}
		topToolbar = topSizeTip + SIZETIP_HEIGHT;
	}

	//
	int left = m_rcCap.right - CToolBarWnd::WIDTH;
	if (left < 0) left = 0;

	m_rcSizeTip.SetRect(m_rcCap.left, topSizeTip, m_rcCap.left + SIZETIP_WIDTH, topSizeTip + SIZETIP_HEIGHT);

	m_pToolBarWnd->MoveWindow(left, topToolbar + offsety, CToolBarWnd::WIDTH, CToolBarWnd::HEIGHT);
	showToolbar?m_pToolBarWnd->ShowWindow(SW_SHOW):m_pToolBarWnd->ShowWindow(SW_HIDE);

	m_pConfigWnd->MoveWindow(left, topToolbar + offsety +  CToolBarWnd::HEIGHT + offsety, CConfigWnd::WIDTH, CConfigWnd::HEIGHT);
	showConfigWnd?m_pConfigWnd->ShowWindow(SW_SHOW):m_pConfigWnd->ShowWindow(SW_HIDE);

	MoveZoom();

	Invalidate();
}

void CCaptureWnd::MoveZoom()
{
#define ZOOM_OFFSET_X	5
#define ZOOM_OFFSET_Y	30

	if (m_showZoom)
	{
		m_ptZoom = m_lastPoint;
		m_rcZoom.SetRect(m_ptZoom.x + ZOOM_OFFSET_X, 
			m_ptZoom.y + ZOOM_OFFSET_Y, 
			m_ptZoom.x + ZOOM_OFFSET_X + ZOOM_WIDTH, 
			m_ptZoom.y + ZOOM_OFFSET_Y + ZOOM_HEIGHT);
	}
}

CCaptureWnd::DrawItem* CCaptureWnd::NewDrawItem(DrawItemType type)
{
	DrawItem* pItem = NULL;

	if (DRAWITEM_RECT == type ||DRAWITEM_ELLAPSE == type)
	{
		pItem = new DrawItemRect();
	}
	else if (DRAWITEM_ARROW == type)
	{
		pItem = new DrawItemArrow();
	}
	else if (DRAWITEM_BRUSH == type)
	{
		pItem = new DrawItemBrush();
	}
	else if (DRAWITEM_TEXT == type)
	{
		pItem = new DrawItemText();
	}
	pItem->type = type;
	pItem->size = m_drawItemSize;
	pItem->color = m_drawItemColor;

	return pItem;
}

HBITMAP CCaptureWnd::PaintDrawItems()
{
	HDC hSrcDC = ::CreateCompatibleDC(NULL);
	HDC hDstDC = ::CreateCompatibleDC(NULL);

	HBITMAP hbmResult = ::CreateCompatibleBitmap(CClientDC(NULL), m_ScreenWidth, m_ScreenHeight);
	HBITMAP hbmOldSource = (HBITMAP)::SelectObject(hSrcDC, m_hBmpScreen);
	HBITMAP hbmOldDest = (HBITMAP)::SelectObject(hDstDC, hbmResult);
	::BitBlt(hDstDC, 0, 0, m_ScreenWidth, m_ScreenHeight, hSrcDC, 0, 0, SRCCOPY);

	PaintItems(hDstDC);

	::SelectObject(hSrcDC, hbmOldSource);
	::SelectObject(hDstDC, hbmOldDest);

	::DeleteDC(hSrcDC);
	::DeleteDC(hDstDC);

	return hbmResult;
}

HBITMAP CCaptureWnd::PrintRichEdit(RECT &rt)
{
	if (!m_pRE) return NULL;

	m_pRE->SendMessage(WM_KILLFOCUS, NULL, NULL);

	HDC hDCClient = GetDC()->GetSafeHdc();
	HDC hDC = m_pRE->GetDC()->GetSafeHdc();
	m_pRE->GetClientRect(&rt);
	HDC hMemDC = CreateCompatibleDC(hDC);
	HBITMAP hBitmap = CreateCompatibleBitmap(hDC, rt.right-rt.left, rt.bottom-rt.top);
	HGDIOBJ hOldBmp = ::SelectObject(hMemDC, hBitmap);
	BOOL ret = BitBlt(hMemDC, 0,0,rt.right-rt.left, rt.bottom-rt.top, hDC, 0,0,SRCCOPY);

	POINT ptLeftTop, ptRightBottom;
	ptLeftTop.x = rt.left;
	ptLeftTop.y = rt.top;
	ptRightBottom.x = rt.right;
	ptRightBottom.y = rt.bottom;
	::ClientToScreen(m_pRE->m_hWnd, &ptLeftTop);
	::ClientToScreen(m_pRE->m_hWnd, &ptRightBottom);
	rt.left = ptLeftTop.x;
	rt.top = ptLeftTop.y;
	rt.right = ptRightBottom.x;
	rt.bottom = ptRightBottom.y;

	::SelectObject(hDC, hOldBmp);
	::DeleteDC(hMemDC);
	::ReleaseDC(NULL,hDCClient);
	::ReleaseDC(NULL,hDC);

	BITMAP bm;
	GetObject(hBitmap,sizeof(BITMAP),&bm);
	DWORD dwSize = bm.bmHeight * bm.bmWidthBytes;
	unsigned char* pBits = new unsigned char[dwSize];
	GetBitmapBits(hBitmap, dwSize, pBits);
	if(bm.bmBitsPixel == 32 )
	{
		for (int y=0; y<bm.bmHeight; ++y)
		{
			BYTE *pPixel= (BYTE *)pBits + bm.bmWidth * 4 * y;
			for (int x=0; x<bm.bmWidth; ++x)
			{
				if (pPixel[0] == 0xFF
					&& pPixel[1] == 0xFF
					&& pPixel[2] == 0xFF)
				{
					pPixel[3] = 0x00;
				}
				else
				{
					pPixel[3] = 0xFF;
				}

				pPixel+= 4;
			}
		}
	}

	SetBitmapBits(hBitmap, dwSize, pBits);

	return hBitmap;
}

void CCaptureWnd::CopyBitmapToClipBoard(HBITMAP hBitmap)
{
	if (::OpenClipboard (m_hWnd)) 
	{
		::EmptyClipboard ();
		::SetClipboardData (CF_BITMAP, hBitmap);
		::CloseClipboard ();
	}
}

CPoint CCaptureWnd::NormalizePoint(CRect rc, CPoint point)
{
	CPoint pt = point;
	if (pt.x < rc.left)
	{
		pt.x = rc.left;
	}
	if (pt.x > rc.right)
	{
		pt.x = rc.right;
	}
	if (pt.y < rc.top)
	{
		pt.y = rc.top;
	}
	if (pt.y > rc.bottom)
	{
		pt.y = rc.bottom;
	}
	return pt;
}
