#pragma once
#include <GdiPlus.h>

class CTSButton : public CButton
{
public:
	DECLARE_MESSAGE_MAP()
public:
	CTSButton(void);
	~CTSButton(void);

public:
	void LoadImage(LPCTSTR lpszNor, LPCTSTR lpszPush,LPCTSTR lpszHover,LPCTSTR lpszDisable);
	void SetBkColor(DWORD color) { m_dwBkColor = color; Invalidate(); }
	void SetBorderColor(DWORD color) { m_dwBorderColor = color; }
	void SetEnabelHover(BOOL enable = TRUE ) { m_bEnableHover = enable; }
	void InvalidateBk();
	void SetCheckAndInvalid(int nChecked);

protected:
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnMouseLeave(void);
	afx_msg void OnMouseHover(UINT nFlags, CPoint point);

private:
	wchar_t* CTSButton::CharToWchar(const char* c);

private:
	Gdiplus::Image *m_hImageNor;
	Gdiplus::Image *m_hImagePush;
	Gdiplus::Image *m_hImageHover;
	Gdiplus::Image *m_hImageDisable;

	HBITMAP m_hBkBmp;
	BOOL	m_bIsTracking;
	BOOL	m_bIsHovering;
	
	BOOL	m_bEnableHover;
	DWORD	m_dwBkColor;
	DWORD	m_dwBorderColor;
};

