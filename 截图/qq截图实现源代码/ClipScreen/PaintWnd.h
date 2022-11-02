#pragma once
#include "CatchToolBar.h"
#include <vector>
#include "TransEdit.h"
typedef struct DRAWITEM
{
	UINT nWay;
	UINT nWidth;
	COLORREF clr;
	POINT pt[12];
	CString strtext;
	DRAWITEM()
	{
		nWay = 0;
		nWidth = 0;
		clr = RGB(255,0,0);
		memset(pt,-1,sizeof(pt));
		strtext = "";
	}
}DRAWITEM;

typedef struct SelectItem
{
	UINT index;
	int nCenter;

	SelectItem()
	{
		index = -1;
		nCenter = 0;
	}
}SelectItem;
class CPngFromResource
{
public:
	CPngFromResource(UINT uIdRes)
	{
		m_Istream = NULL;
		PngFromResource(uIdRes);
	}
	~CPngFromResource()
	{
		if(m_Istream)
			m_Istream->Release();
	}

	operator IStream *()
	{
		return m_Istream;
	}
private:
	IStream * PngFromResource(UINT uIdRes)
	{
		HRSRC hPngSrc = FindResource(AfxGetInstanceHandle(),MAKEINTRESOURCE(uIdRes),_T("PNG"));
		if(hPngSrc)
		{
			HGLOBAL hPngGlobal = LoadResource(AfxGetInstanceHandle(),hPngSrc);
			if(hPngGlobal)
			{
				LPVOID pSrc = LockResource(hPngGlobal);
				if(pSrc)
				{
					ULARGE_INTEGER srcSize;  
					srcSize.QuadPart = SizeofResource(AfxGetApp()->m_hInstance, hPngSrc);  

					m_Istream = NULL;  
					CreateStreamOnHGlobal(NULL, TRUE, &m_Istream);   
					if(m_Istream)  
					{  
						m_Istream->SetSize(srcSize);  
						DWORD cbWritten = 0;  
						m_Istream->Write(pSrc, srcSize.LowPart, &cbWritten);

						//VERIFY( m_Imagebg.Load(pStream) == S_OK );  

						return m_Istream;
					}  
				}
			}
		}
		return NULL;
	}


	IStream *m_Istream;
};
/*
	IsSelect 是否选中
	MoveLine 进行移动
	DrawItem 进行绘画
	IsResetSelefLine 用于控制绘画点的个数
*/
class CPaintWnd
{
public:
	CPaintWnd(void);
	~CPaintWnd(void);
	void SetParent(CWnd *pParentWnd);
	void OnMouseMove(UINT nFlags, CPoint point,BOOL &bHandle);
	void MoveLine(CPoint ptoffset);
	void OnLButtonUp(UINT nFlags, CPoint point);
	void OnLButtonDown(UINT nFlags, CPoint point);
	void OnDraw(CDC *dc,const CRect &rtDraw);
	void SetDrawWay(UINT nWay);
	BOOL IsResetSelefLine();
	void Reset();
	int IsSelect(POINT pt);
	void SetSaveFlag(){Reset();m_nCurSelect = -1;m_nSelect.index = -1;}
private:
	void OnDel();
	void DrawItem(CDC *dc,const CRect &rtDraw,const DRAWITEM &item,BOOL bSelect);
	void DrawZX(CDC *dc,const CRect &rtDraw,POINT pt1,POINT pt2);
	void DrawArrow(CDC *dc,COLORREF clrtemp,UINT nWidth,POINT pt1,POINT pt2);
	void DrawEllipse(CDC *dc,POINT pt1,POINT pt2);
	void DrawPXX(CDC *dc,const CRect &rtDraw,POINT pt1,POINT pt2,POINT pt3);
	void DrawPng(CDC *dc,POINT pt1,UINT uIdRes);
	void DrawFillPoint(CDC *dc,POINT pt,int nOffset = 5);
	BOOL PtInText(CPoint point,CPoint pointTextS,CPoint pointTextE);
	BOOL ptInLine(CPoint point, CPoint lineStartPoint, CPoint lineEndPoint, double fTolerance);
	BOOL ptInLine(CPoint point, const CRect &rtDraw,CPoint lineStartPoint, CPoint lineEndPoint, double fTolerance);
	BOOL PtInPXX(CPoint point, const CRect &rtDraw,CPoint lineStartPoint, CPoint lineEndPoint,CPoint lineTwo, double fTolerance);
	BOOL PtInEllipse(CPoint point,CPoint lineStartPoint, CPoint lineEndPoint, double fTolerance);
	POINT GetTextEndPoint(POINT ptStart,const CString &str,const HFONT &font);
private:
	HFONT m_hFont;
	BOOL m_isBegPaint;
	SelectItem m_nSelect;//标准实时选中项(WM_MOUSEMOVE)
	BOOL m_IsEnableMoveLine;//是否允许选中项移动
	int m_nCurSelect;//当前选中项，可以选择修改属性
	CPoint m_PtStartMove;
	CWnd *m_pParentWnd;
	CRect m_rtDraw;
	UINT m_nWidth;
	UINT m_nPointNum;
	COLORREF m_clr;
	UINT m_nDrawWay;
	DRAWITEM m_item;
	CTransEdit *m_pedit;
	std::vector<DRAWITEM> m_vDrawItem;
};
