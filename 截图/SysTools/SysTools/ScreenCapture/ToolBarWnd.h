#pragma once

class CTSButton;
class CConfigWnd;
class CToolBarWnd :
	public CWnd
{
public:

#define WM_CAPTURE_RECT		WM_USER+1000
#define WM_CAPTURE_ELLIPSE	WM_USER+1001
#define WM_CAPTURE_ARROW	WM_USER+1002
#define WM_CAPTURE_BRUSH	WM_USER+1003
#define WM_CAPTURE_TEXT		WM_USER+1004
#define WM_CAPTURE_UNDO		WM_USER+1005
#define WM_CAPTURE_SAVE		WM_USER+1006
#define WM_CAPTURE_CANCEL	WM_USER+1007
#define WM_CAPTURE_OK		WM_USER+1008

public:
	CToolBarWnd(void);
	~CToolBarWnd(void);

private:
	CTSButton* m_pRectBtn;
	CTSButton *m_pEllipseBtn;
	CTSButton* m_pArrowBtn;
	CTSButton* m_pBrushBtn;
	CTSButton* m_pTextBtn;
	CTSButton* m_pFilterBtn;
	CTSButton* m_pUndoBtn;
	CTSButton* m_pSaveBtn;
	CTSButton* m_pCancelBtn;
	CTSButton* m_pOKBtn;

	DWORD m_RectColor;
	int m_RectBorderSize;

	HBITMAP m_hBmpBk;

public:
	DECLARE_MESSAGE_MAP()
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC );

	afx_msg void OnRectBtn();
	afx_msg void OnEllipseBtn();
	afx_msg void OnArrowBtn();
	afx_msg void OnBrushBtn();
	afx_msg void OnTextwBtn();
	afx_msg void OnUndoBtn();
	afx_msg void OnSaveBtn();
	afx_msg void OnOKBtn();
	afx_msg void OnCancelBtn();

private:
	CString ImagePath(CString name);
	DWORD GetRectColor() {return m_RectColor;}
	void SetRectColor(DWORD color) {m_RectColor = color;}

public:
	const static int WIDTH = 267;
	const static int HEIGHT = 26;

private:
	CString m_imageDir;
};

