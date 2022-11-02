#include "StdAfx.h"
#include "TSButton.h"
#include "CaptureHelper.h"

CTSButton::CTSButton(void):
m_hImageNor(NULL),
m_hImagePush(NULL),
m_hImageHover(NULL),
m_hImageDisable(NULL),
m_hBkBmp(NULL),
m_dwBkColor(0x00000000),
m_dwBorderColor(0x00000000),
m_bEnableHover(TRUE)
{
	m_bIsTracking = FALSE;
	m_bIsHovering = FALSE;
}

CTSButton::~CTSButton(void)
{
	if (m_hImageNor) delete m_hImageNor;
	if (m_hImagePush) delete m_hImagePush;
	if (m_hImageHover) delete m_hImageHover;
	if (m_hImageDisable) delete m_hImageDisable;
}

BEGIN_MESSAGE_MAP(CTSButton, CButton)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSELEAVE()
	ON_WM_MOUSEHOVER()
END_MESSAGE_MAP()

wchar_t* CTSButton::CharToWchar(const char* c)
{
	int len = MultiByteToWideChar(CP_ACP,0,c,strlen(c),NULL,0);
	wchar_t* m_wchar=new wchar_t[len+1];
	MultiByteToWideChar(CP_ACP,0,c,strlen(c),m_wchar,len);
	m_wchar[len]='\0';
	return m_wchar;
}

void CTSButton::LoadImage(LPCTSTR lpszNor, LPCTSTR lpszPush,LPCTSTR lpszHover,LPCTSTR lpszDisable)
{
#ifdef _UNICODE
	wchar_t* lpwNor = lpszNor;
	wchar_t* lpwPush = lpszPush;
	wchar_t* lpwHover = lpszHover;
	wchar_t* lpwDis = lpszDisable;
#else
	wchar_t* lpwNor = CharToWchar(lpszNor);
	wchar_t* lpwPush = CharToWchar(lpszPush);
	wchar_t* lpwHover = CharToWchar(lpszHover);
	wchar_t* lpwDis = CharToWchar(lpszDisable);
#endif
	m_hImageNor = Gdiplus::Image::FromFile(lpwNor);
	m_hImagePush = Gdiplus::Image::FromFile(lpwPush);
	m_hImageHover = Gdiplus::Image::FromFile(lpwHover);
	m_hImageDisable = Gdiplus::Image::FromFile(lpwDis);

#ifdef _UNICODE
#else
	delete []lpwNor;
	delete []lpwPush;
	delete []lpwHover;
	delete []lpwDis;
#endif

	Invalidate();
}

//////////////////////////////////////////////////////////////////////////
// invalidate the background image of TSButton, 
// must call in OnEraseBkgnd function of parent window
//////////////////////////////////////////////////////////////////////////
void CTSButton::InvalidateBk()
{
	if (m_hBkBmp) ::DeleteObject(m_hBkBmp);

	HDC hDC = ::GetDC(m_hWnd);
	CRect rc;  
	GetClientRect(rc);

	HDC hParentDC = ::GetDC(::GetParent(m_hWnd));
	CPoint pt(0,0);
	MapWindowPoints(GetParent(),&pt,1);

	HDC hMemDC = ::CreateCompatibleDC(hDC);
	m_hBkBmp = ::CreateCompatibleBitmap(hDC, rc.Width(), rc.Width());
	HGDIOBJ hOldBmp = ::SelectObject(hMemDC, m_hBkBmp);

	::BitBlt(hMemDC,rc.left,rc.top,rc.Width(),rc.Height(),hParentDC,pt.x,pt.y,SRCCOPY);
	::SelectObject(hMemDC, hOldBmp);
	::DeleteDC(hMemDC);
}

void CTSButton::SetCheckAndInvalid(int nChecked)
{
	CButton::SetCheck(nChecked);
	Invalidate();
}

void CTSButton::OnMouseHover(UINT nFlags, CPoint point) 
{
	m_bIsHovering = TRUE;
	Invalidate();
}

void CTSButton::OnMouseLeave(void)
{
	m_bIsHovering = FALSE;
	m_bIsTracking = FALSE;
	Invalidate();
}

void CTSButton::OnPaint()
{
	CPaintDC dc(this); 		
	CRect rc;  
	GetClientRect(rc);
	
	HDC hMemDC = ::CreateCompatibleDC(dc);
	HBITMAP hBmp = ::CreateCompatibleBitmap(dc, rc.Width(), rc.Height());
	HGDIOBJ hOldBmp = ::SelectObject(hMemDC, hBmp);
	if (m_hBkBmp)
	{
		HDC hBkDC = ::CreateCompatibleDC(dc);
		HGDIOBJ hOldBkBmp = ::SelectObject(hBkDC,m_hBkBmp);		
		::BitBlt(hMemDC,rc.left,rc.top,rc.Width(),rc.Height(),hBkDC, 0, 0,SRCCOPY);
		::SelectObject(hBkDC, hOldBkBmp);
		::DeleteDC(hBkDC);
	}

	if (m_hImageNor)
	{
		Gdiplus::Image *m_hImageCur = m_hImageNor;
		if (BST_CHECKED == GetCheck())
		{
			m_hImageCur = m_hImagePush;
		}
		else
		{
			if (m_bIsHovering)
			{
				m_hImageCur = m_hImageHover;
			}
		}

		Gdiplus::Graphics graphics(hMemDC);
		graphics.DrawImage(m_hImageCur, 0, 0, rc.Width(), rc.Height());
	}
	else
	{
		RECT rcBorder = rc;
		RECT rcColor = rc;
		rcColor.left+=2;
		rcColor.right-=2;
		rcColor.top+=2;
		rcColor.bottom-=2;
		
		if (m_bEnableHover && !m_bIsHovering)
		{
			rcBorder.left+=1;
			rcBorder.right-=1;
			rcBorder.top+=1;
			rcBorder.bottom-=1;
		}

		::FillRect(hMemDC, rcBorder, 0xFFFFFFFF);
		::DrawRect(hMemDC, rcBorder, 1, m_dwBorderColor);
		::FillRect(hMemDC, rcColor, m_dwBkColor);
	}

	::BitBlt(dc,rc.left,rc.top,rc.Width(),rc.Height(),hMemDC, 0, 0,SRCCOPY);
	::SelectObject(hMemDC, hOldBmp);
	::DeleteObject(hBmp);
	::DeleteDC(hMemDC);
}

void CTSButton::OnLButtonUp(UINT nFlags, CPoint point)
{
	Invalidate();
	CButton::OnLButtonUp(nFlags, point);
}

void CTSButton::OnLButtonDown(UINT nFlags, CPoint point)
{
	Invalidate();
	CButton::OnLButtonDown(nFlags, point);
}

void CTSButton::OnMouseMove(UINT nFlags, CPoint point) 
{
	if (!m_bIsTracking)
	{
		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof(tme);
		tme.hwndTrack = m_hWnd;
		tme.dwFlags = TME_LEAVE|TME_HOVER;
		tme.dwHoverTime = 1;
		m_bIsTracking = _TrackMouseEvent(&tme);
	}

	CButton::OnMouseMove(nFlags, point);
}

//=============================================================================
// overide the erase function
//=============================================================================
BOOL CTSButton::OnEraseBkgnd(CDC* pDC) 
{
	return TRUE;
}
