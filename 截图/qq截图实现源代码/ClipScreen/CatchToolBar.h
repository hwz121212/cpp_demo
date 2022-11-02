#pragma once
#include "atltypes.h"
#include <vector>


class CCatchScreenDlg;
class CToolItem;
#define ID_BASE 0x100
#define ID_TOOL_1PIX ID_BASE+0
#define ID_TOOL_3PIX ID_BASE+1
#define ID_TOOL_CLR_RED ID_BASE+2
#define ID_TOOL_CLR_YELLOW ID_BASE+3
#define ID_TOOL_CLR_GREEN ID_BASE+4
#define ID_TOOL_CLR_BLUE ID_BASE+5
#define ID_TOOL_XD ID_BASE+6
#define ID_TOOL_ZX ID_BASE+7
#define ID_TOOL_ARROW ID_BASE+8
#define ID_TOOL_PXX ID_BASE+9
#define ID_TOOL_ELLIPSE ID_BASE+10
#define ID_TOOL_TEXT ID_BASE+11
#define ID_TOOL_DOWN ID_BASE+12
#define ID_TOOL_UP ID_BASE+13
#define ID_TOOL_RIGHT ID_BASE+14
#define ID_TOOL_WRONG ID_BASE+15
#define ID_TOOL_SAVE ID_BASE+16
#define ID_TOOL_DEL ID_BASE+17

extern UINT g_ToolBar[18];
extern UINT g_ToolBarID[18];
extern UINT g_ToolGroup[18];
extern BOOL g_ToolGroupSel[18];
typedef void (*PITEMCLICK_FUN)(CToolItem *pPtem,CWnd *pWnd);

class CToolItem:public CRect
{
public:
	CToolItem(CRect *pParent,int nWidht,int nHeight,UINT uID,UINT ImgIndex,UINT uGroupID,BOOL bSel);
	void Draw(HDC hdc);
	void DrawBk(HDC hdc);
	void SetPos(POINT pt);
	UINT GetID(){return m_uID;}
	BOOL OnLbuttonDwon(UINT nFlags, CPoint point);
	BOOL OnMouseMove(UINT nFlags, CPoint point);
	void SetSel(BOOL bSel){m_isSel = bSel;}
	BOOL IsSel(){return m_isSel;}
	UINT m_uGroupID;
private:
	CRect *m_pParent;
	UINT m_uID;
	UINT m_ImgIndex;
	UINT m_nWidth;
	BOOL m_isSel;
};

class CCatchToolBar :
	public CRect
{
public:
	CCatchToolBar(void);
	~CCatchToolBar(void);

	void Show(BOOL bShow);
	void SetSel(CToolItem *pSelTool);
private:
	friend class CCatchScreenDlg;

	void SetParent(CWnd *pWnd);
	void SetPos(POINT pt);
	void AddItem(CToolItem *pItem);
	void CalcItem();

	void Draw(HDC hdc);
	void DrawBk(HDC hdc);
	void ReDraw();
	BOOL OnLbuttonDwon(UINT nFlags, CPoint point);
	BOOL OnMouseMove(UINT nFlags, CPoint point);
	void SetClickfun(PITEMCLICK_FUN fun);
public:
	CWnd *m_pWnd;
	BOOL m_bShow;
	PITEMCLICK_FUN m_fclick;
	static HBITMAP m_hBkImg;
	static HBITMAP m_hHotBkImg;
	std::vector<CToolItem *> m_vItem;
};
