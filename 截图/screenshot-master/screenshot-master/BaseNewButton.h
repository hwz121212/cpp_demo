
#pragma once
#include <atlimage.h>  


// BaseNewButton
class BaseNewButton : public CButton
{
	DECLARE_DYNAMIC(BaseNewButton)

public:
	BaseNewButton();
	virtual ~BaseNewButton();

	typedef enum state
	{
		NORMAL,
		HOVER,
		DOWN,
		DISABLE,
		FLASH,
	}state;

public:
	void SetWindowText(LPCTSTR lpszString);
	void SetImage(UINT nNormalID, int nImageWidth, int nImageHeight);
	void SetImage(CString strNormal, int nImageWidth, int nImageHeight);
	void SetTextColor(COLORREF fgcolor,COLORREF bgcolor);
	void SetTextPos(CPoint point);
	void SetStretchBlt(BOOL bStretchBlt=FALSE);
	void SetFlashButton();

protected:
	virtual void PreSubclassWindow();
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDIS);
	virtual void DrawButton();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnEnable(BOOL bEnable);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);

	DECLARE_MESSAGE_MAP()


private:
	state    m_state;
	COLORREF m_fg;
	COLORREF m_bg;
	COLORREF m_bgDraw;
	CPoint   m_textPos;
	BOOL     m_bMouseOver;
	BOOL     m_bEnabled;
	BOOL     m_bStretchBlt;
	int      m_nImageWidth;
	int      m_nImageHeight;
	CImage   m_imgNormal;
	int      m_nFlashIndex;
	int      m_nFlashCount;
};

