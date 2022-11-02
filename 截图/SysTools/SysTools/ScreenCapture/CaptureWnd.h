#pragma once
#include <vector>
#include "ToolBarWnd.h"
#include "ConfigWnd.h"

class CCaptureWnd 
	:public CWnd
{
public:
	enum CAPTURE_STATE
	{
		STATE_SPY_WND,
		STATE_CAPTURE_RECT,
		STATE_EDIT_RECT,
		STATE_RECT_READY,
	};

	enum MOUSE_TYPE
	{
		MOUSE_UNCLICK,
		MOUSE_CLICK_MOVING_RECT,
		MOUSE_CLICK_SIZING_RECT_N_S_W_E,
		MOUSE_CLICK_SIZING_RECT_W_E,
		MOUSE_CLICK_SIZING_RECT_N_S,
	};

	enum DrawItemType
	{
		DRAWITEM_NONE,
		DRAWITEM_RECT,
		DRAWITEM_ELLAPSE,
		DRAWITEM_ARROW,
		DRAWITEM_BRUSH,
		DRAWITEM_TEXT,
	};

	struct DrawItem
	{
		DrawItemType type;
		int size;
		DWORD color;

		DrawItem()
		{
			size = 1;
			color = 0xFFFF0000;
		}
		~DrawItem()
		{
		}
	};

	struct DrawItemRect : public DrawItem
	{
		RECT rt;
	};

	struct DrawItemArrow : public DrawItem
	{
		CPoint begin;
		CPoint end;
	};

	struct DrawItemBrush: public DrawItem
	{
		std::vector<CPoint*> points;
	};

	struct DrawItemText : public DrawItem
	{
		HBITMAP hBmp;
		RECT rt;
		DrawItemText()
		{
			hBmp = NULL;
		}
		~DrawItemText()
		{
			::DeleteObject(hBmp);
		}
	};

public:
	CCaptureWnd(void);
	~CCaptureWnd(void);
	DECLARE_MESSAGE_MAP()

public:
	virtual void PostNcDestroy();
	afx_msg void OnPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnEnChange();
	BOOL FilterMessage(UINT uMsg,WPARAM wParam,LPARAM lParam);

public:
	static CCaptureWnd* GetInstance();
	void EventRegister(HWND hWnd);

private:
	void DoPaint(HDC &hDC);
	void PaintScreen(HDC &hDC);
	void PaintMask(HDC &hDC);
	void PaintCaptureRect(HDC &hDC);
	void PaintSizeTip(HDC &hDC);
	void PaintZoom(HDC &hDC);
	void PaintBorder(HDC &hDC);
	void PaintItems(HDC &hDC);
	void PaintItem(HDC &hDC, DrawItem *pItem);
	void PaintArrow(HDC &hDC, DrawItemArrow* pArrow);
	void PaintBrush(HDC &hDC, DrawItemBrush* pBrush);
	void PaintText(HDC &hDC, DrawItemText* pText);

	//void CreateMaskBitmap(HBITMAP &hBitmap, int width, int height, DWORD argb);
	void FullScreen();
	void MoveCapture(CPoint point);
	void SizeCapture(MOUSE_TYPE type, CPoint point);
	void SizeCaptureRect(MOUSE_TYPE type, CPoint point);
	void LimitCaptureRect();
	void SetAnchor();
	void SetCursor(CPoint point);
	void MoveChildWnd(BOOL showToolbar, BOOL showConfigWnd);
	void MoveZoom();

	CAPTURE_STATE GetState() {return m_state;}
	void SetState(CAPTURE_STATE state) {m_state = state;}
	MOUSE_TYPE GetMouseType() {return m_mouseType;}
	void SetMouseType(MOUSE_TYPE type) {m_mouseType = type;}

	DrawItem* NewDrawItem(DrawItemType type);
	HBITMAP PaintDrawItems();
	HBITMAP PrintRichEdit(RECT &rt);

	void EmptyEdit();

private:
	void CopyBitmapToClipBoard(HBITMAP hBitmap);

	CPoint NormalizePoint(CRect rc, CPoint point);
private:
	static CCaptureWnd *m_pCapWnd;
	HWND m_pRegisterWnd;
private:
	CAPTURE_STATE m_state;
	MOUSE_TYPE m_mouseType;
	
	CRect m_rcCap;
	CRect m_rcNW;
	CRect m_rcNE;
	CRect m_rcSW;
	CRect m_rcSE;
	CRect m_rcWest;
	CRect m_rcNorth;
	CRect m_rcEast;
	CRect m_rcSouth;

	CPoint m_ptFix;
	CPoint m_lastPoint;


private:
	std::vector<DrawItem*> m_drawItems;
	DrawItemType m_drawItemType;
	int m_drawItemSize;
	DWORD m_drawItemColor;
	DrawItem* m_drawItem;
	CRichEditCtrl* m_pRE;
	CPoint *m_pREPoint;

private:
	CToolBarWnd* m_pToolBarWnd;
	CConfigWnd *m_pConfigWnd;
	CRect m_rcSizeTip;
	CRect m_rcZoom;
	CPoint m_ptZoom;

	BOOL m_showToolBarWnd;
	BOOL m_showConfigWnd;
	BOOL m_showSizeTip;
	BOOL m_showZoom;

	HBITMAP m_hBmpScreen;
	HBITMAP m_hBmpScreenMask;
	HBITMAP m_hBmpSizeTip;
	HBITMAP m_hBmpZoom;
	int m_ScreenWidth;
	int m_ScreenHeight;

};

