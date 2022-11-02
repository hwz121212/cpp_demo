#pragma once

class CTSButton;
class CConfigWnd :
	public CWnd
{
public:
#define WM_DRAWITEM_SIZE1	WM_USER+1100
#define WM_DRAWITEM_SIZE2	WM_USER+1101
#define WM_DRAWITEM_SIZE3	WM_USER+1102
#define WM_DRAWITEM_COLOR	WM_USER+1103

public:
	CConfigWnd(void);
	~CConfigWnd(void);

public:
	DECLARE_MESSAGE_MAP()
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC );
	afx_msg void OnSize1();
	afx_msg void OnSize2();
	afx_msg void OnSize3();

	afx_msg void OnColor00();
	afx_msg void OnColor01();
	afx_msg void OnColor02();
	afx_msg void OnColor03();
	afx_msg void OnColor04();
	afx_msg void OnColor05();
	afx_msg void OnColor06();
	afx_msg void OnColor07();

	afx_msg void OnColor10();
	afx_msg void OnColor11();
	afx_msg void OnColor12();
	afx_msg void OnColor13();
	afx_msg void OnColor14();
	afx_msg void OnColor15();
	afx_msg void OnColor16();
	afx_msg void OnColor17();

public:
	void SetRectState(BOOL showSizeRect, BOOL showTextRect);

private:
	void InitBtn(CTSButton *&pBtn, int ID, DWORD color);
	void SelColor(DWORD color);
	CString ImagePath(CString name);
public:
	const static int WIDTH = 248;
	const static int HEIGHT = 37;

private:
	BOOL m_bShowSizeRect;
	BOOL m_bShowTextRect;
	CTSButton* m_pBtnSize1;
	CTSButton* m_pBtnSize2;
	CTSButton* m_pBtnSize3;

	CComboBox* m_pComboText;

	CTSButton* m_pBtnSelColor;
	HBITMAP m_hBmpSelColor;
	CTSButton* m_pBtnColor00;
	CTSButton* m_pBtnColor01;
	CTSButton* m_pBtnColor02;
	CTSButton* m_pBtnColor03;
	CTSButton* m_pBtnColor04;
	CTSButton* m_pBtnColor05;
	CTSButton* m_pBtnColor06;
	CTSButton* m_pBtnColor07;
	CTSButton* m_pBtnColor10;
	CTSButton* m_pBtnColor11;
	CTSButton* m_pBtnColor12;
	CTSButton* m_pBtnColor13;
	CTSButton* m_pBtnColor14;
	CTSButton* m_pBtnColor15;
	CTSButton* m_pBtnColor16;
	CTSButton* m_pBtnColor17;

	HBITMAP m_hBmpBk;
	CString m_imageDir;

private:
	const static DWORD BKCOLOR00 = 0xFF000000;
	const static DWORD BKCOLOR01 = 0xFF808080;
	const static DWORD BKCOLOR02 = 0xFF800000;
	const static DWORD BKCOLOR03 = 0xFF808000;
	const static DWORD BKCOLOR04 = 0xFF1D8400;
	const static DWORD BKCOLOR05 = 0xFF000080;
	const static DWORD BKCOLOR06 = 0xFF800080;
	const static DWORD BKCOLOR07 = 0xFF008080;
	const static DWORD BKCOLOR10 = 0xFFFFFFFF;
	const static DWORD BKCOLOR11 = 0xFFC0C0C0;
	const static DWORD BKCOLOR12 = 0xFFFF0000;
	const static DWORD BKCOLOR13 = 0xFFFFFF00;
	const static DWORD BKCOLOR14 = 0xFF00FF00;
	const static DWORD BKCOLOR15 = 0xFF0000FF;
	const static DWORD BKCOLOR16 = 0xFFFF00FF;
	const static DWORD BKCOLOR17 = 0xFF00FFFF;
	const static DWORD BORDER_COLOR = 0xFF335B91;

};

