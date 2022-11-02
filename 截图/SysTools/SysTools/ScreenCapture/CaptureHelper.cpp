#include "StdAfx.h"
#include "CaptureHelper.h"

// CImage support
#include <atlimage.h>			

// gdi plus support
#include <GdiPlus.h>			
#pragma comment(lib, "Gdiplus")

// IRichEditOle interface support
#include <Richole.h>
#pragma comment(lib,"riched20.lib")

// ITextServices interface support
#include <imm.h>
#include <textserv.h>
const IID IID_ITextServices = {0x8d33f740, 0xcf58, 0x11ce, {0xa8, 0x9d, 0x00, 0xaa, 0x00, 0x6c, 0xad, 0xc5}};

//////////////////////////////////////////////////////////////////////////
HBITMAP CopyScreenRectToBitmap(LPRECT lpRect)
{
	if (IsRectEmpty(lpRect)) return NULL;      

	HDC hScrDC,hMemDC;
	int nX,nY,nX2,nY2;
	int nWidth,nHeight;
	int xScrn,yScrn;

	HGDIOBJ hOldBitmap,hBitmap;
	hScrDC = CreateDC(_T("DISPLAY"), NULL, NULL, NULL);     
	hMemDC = CreateCompatibleDC(hScrDC);

	nX = lpRect->left;     
	nY = lpRect->top;     
	nX2 = lpRect->right;     
	nY2 = lpRect->bottom;      // get screen resolution      

	xScrn = GetDeviceCaps(hScrDC, HORZRES);
	yScrn = GetDeviceCaps(hScrDC, VERTRES);

	//make sure bitmap rectangle is visibleW
	if (nX < 0)
		nX = 0;     

	if (nY < 0)         
		nY = 0;     

	if (nX2 > xScrn)         
		nX2 = xScrn;     

	if (nY2 > yScrn)         
		nY2 = yScrn;      

	nWidth = nX2 - nX;     
	nHeight = nY2 - nY;

	//
	hBitmap = CreateCompatibleBitmap(hScrDC, nWidth, nHeight);      
	hOldBitmap = SelectObject (hMemDC, hBitmap);

	BitBlt(hMemDC, 0, 0, nWidth, nHeight, hScrDC, nX, nY, SRCCOPY);
	
	hBitmap = SelectObject(hMemDC, hOldBitmap);
	DeleteDC(hScrDC);     
	DeleteDC(hMemDC);      

	return (HBITMAP)hBitmap; 
}

HBITMAP CaptureDesktop()
{
	RECT rt; 
	::GetWindowRect(GetDesktopWindow(), &rt);
	return CopyScreenRectToBitmap(&rt);
}

//////////////////////////////////////////////////////////////////////////
void FillRect(HDC hDC, const RECT& rc, DWORD dwBrushColor)
{
	ASSERT(::GetObjectType(hDC)==OBJ_DC || ::GetObjectType(hDC)==OBJ_MEMDC);
	HBRUSH hBrush = ::CreateSolidBrush(RGB(GetBValue(dwBrushColor), GetGValue(dwBrushColor), GetRValue(dwBrushColor)));
	::FillRect(hDC, &rc, hBrush);
	::DeleteObject(hBrush);
}

void DrawRect(HDC hDC, const RECT& rc, int nSize, DWORD dwPenColor)
{
	ASSERT(::GetObjectType(hDC)==OBJ_DC || ::GetObjectType(hDC)==OBJ_MEMDC);
	HPEN hPen = ::CreatePen(PS_SOLID | PS_INSIDEFRAME, nSize, RGB(GetBValue(dwPenColor), GetGValue(dwPenColor), GetRValue(dwPenColor)));
	HPEN hOldPen = (HPEN)::SelectObject(hDC, hPen);
	::SelectObject(hDC, ::GetStockObject(HOLLOW_BRUSH));
	::Rectangle(hDC, rc.left, rc.top, rc.right, rc.bottom);
	::SelectObject(hDC, hOldPen);
	::DeleteObject(hPen);
}

void DrawRoundRect(HDC hDC, const RECT& rc, int nSize, int width, int height, DWORD dwPenColor)
{
	ASSERT(::GetObjectType(hDC)==OBJ_DC || ::GetObjectType(hDC)==OBJ_MEMDC);
	HPEN hPen = ::CreatePen(PS_SOLID | PS_INSIDEFRAME, nSize, RGB(GetBValue(dwPenColor), GetGValue(dwPenColor), GetRValue(dwPenColor)));
	HPEN hOldPen = (HPEN)::SelectObject(hDC, hPen);
	::SelectObject(hDC, ::GetStockObject(HOLLOW_BRUSH));
	::RoundRect(hDC, rc.left, rc.top, rc.right, rc.bottom, width, height);
	::SelectObject(hDC, hOldPen);
	::DeleteObject(hPen);
}

void DrawLine( HDC hDC, const CPoint& begin, const CPoint& end, int nSize, DWORD dwPenColor,int nStyle /*= PS_SOLID*/ )
{
	ASSERT(::GetObjectType(hDC)==OBJ_DC || ::GetObjectType(hDC)==OBJ_MEMDC);

	LOGPEN lg;
	lg.lopnColor = RGB(GetBValue(dwPenColor), GetGValue(dwPenColor), GetRValue(dwPenColor));
	lg.lopnStyle = nStyle;
	lg.lopnWidth.x = nSize;
	HPEN hPen = CreatePenIndirect(&lg);
	HPEN hOldPen = (HPEN)::SelectObject(hDC, hPen);
	POINT ptTemp = { 0 };
	::MoveToEx(hDC, begin.x, begin.y, &ptTemp);
	::LineTo(hDC, end.x, end.y);
	::SelectObject(hDC, hOldPen);
	::DeleteObject(hPen);
}

void DrawText(HDC hDC, RECT& rc, LPCTSTR pstrText, DWORD dwTextColor, UINT uStyle)
{
	ASSERT(::GetObjectType(hDC)==OBJ_DC || ::GetObjectType(hDC)==OBJ_MEMDC);
	if( pstrText == NULL ) return;
	::SetBkMode(hDC, TRANSPARENT);
	::SetTextColor(hDC, RGB(GetBValue(dwTextColor), GetGValue(dwTextColor), GetRValue(dwTextColor)));
	//
	LOGFONT lf = { 0 };
	::GetObject(::GetStockObject(DEFAULT_GUI_FONT), sizeof(LOGFONT), &lf);
	lf.lfCharSet = DEFAULT_CHARSET;
	_tcscpy_s(lf.lfFaceName, LF_FACESIZE, _T("Arial"));
	HFONT hDefaultFont = ::CreateFontIndirect(&lf);
	HFONT hOldFont = (HFONT)::SelectObject(hDC, hDefaultFont);
	::DrawText(hDC, pstrText, -1, &rc, uStyle | DT_NOPREFIX);
	::SelectObject(hDC, hOldFont);
	::DeleteObject(hDefaultFont);
}

HBITMAP ChopBitmap(HBITMAP hSrcBitmap, int cx, int cy, int width, int height)
{
	HDC hSourceDC = ::CreateCompatibleDC(NULL);
	HDC hDestDC = ::CreateCompatibleDC(NULL);

	BITMAP bm = {0};
	::GetObject(hSrcBitmap, sizeof(bm), &bm);
	HBITMAP hbmResult = ::CreateCompatibleBitmap(CClientDC(NULL), width, height);

	HBITMAP hbmOldSource = (HBITMAP)::SelectObject(hSourceDC, hSrcBitmap);
	HBITMAP hbmOldDest = (HBITMAP)::SelectObject(hDestDC, hbmResult);

	::BitBlt(hDestDC, 0, 0, width, height, hSourceDC, cx, cy, SRCCOPY );

	::SelectObject(hSourceDC, hbmOldSource);
	::SelectObject(hDestDC, hbmOldDest);

	::DeleteDC(hSourceDC);
	::DeleteDC(hDestDC);

	return hbmResult;
}

HBITMAP CopyBitmap(HBITMAP hSrcBitmap)
{
	HDC hSrcDC = ::CreateCompatibleDC(NULL);
	HDC hDstDC = ::CreateCompatibleDC(NULL);

	BITMAP   bm = {0};
	::GetObject(hSrcBitmap, sizeof(bm), &bm);
	HBITMAP hbmresult = ::CreateCompatibleBitmap(CClientDC(NULL), bm.bmWidth, bm.bmHeight);
	HBITMAP hbmoldsource = (HBITMAP)::SelectObject(hSrcDC, hSrcBitmap);
	HBITMAP hbmolddest = (HBITMAP)::SelectObject(hDstDC, hbmresult);

	::BitBlt(hDstDC, 0, 0, bm.bmWidth, bm.bmHeight, hSrcDC, 0, 0, SRCCOPY);

	::SelectObject(hSrcDC, hbmoldsource);
	::SelectObject(hDstDC, hbmolddest);
	::DeleteObject(hSrcDC);
	::DeleteObject(hDstDC);

	return   hbmresult;
}

void CreateMaskBitmap(HBITMAP &hBitmap, int width, int height, DWORD argb)
{
	Gdiplus::Bitmap bmp(width, height, PixelFormat32bppARGB);
	Gdiplus::Graphics g(&bmp);
	Gdiplus::Color color(argb);
	g.Clear(color);
	::DeleteObject(hBitmap);
	bmp.GetHBITMAP(color, &hBitmap);
}

DWORD GetRGB(HBITMAP hBitmap, int x, int y)
{
	CImage img;
	img.Attach(hBitmap);
	DWORD color = img.GetPixel(x, y);
	img.Detach();

	return color;
}

void SaveFile(CString image, HBITMAP hBitmap)
{
	CImage img;
	img.Attach(hBitmap);
	img.Save(image);
}

HBITMAP LoadFile(CString image)
{
	CImage img;
	img.Load(image);
	return img.Detach();
}

//////////////////////////////////////////////////////////////////////////
ULONG_PTR	gdiplusToken;
void InitGDIPlus()
{
	Gdiplus::GdiplusStartupInput	gdiplusStartupInput;
	::CoInitialize(NULL);
	::OleInitialize(NULL);
	Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
}

void DeInitGDIPlus()
{
	Gdiplus::GdiplusShutdown(gdiplusToken);
}

//////////////////////////////////////////////////////////////////////////
SIZE EstimateTextSize(CRichEditCtrl *pRE, CSize szAvailable)
{
	IRichEditOle* pREOle = NULL;
	pRE->SendMessage(EM_GETOLEINTERFACE, 0, (LPARAM)&pREOle);

	ITextServices *pTextService = NULL;   
	if (S_OK != pREOle->QueryInterface(IID_ITextServices, (void **)&pTextService))
	{
		return CSize(0,0);
	}

	RECT rt = {0};
	rt.right = szAvailable.cx;
	rt.bottom = szAvailable.cy;
	HDC hPaintDC = ::CreateCompatibleDC(NULL);
	ASSERT(hPaintDC);

	pTextService->TxDraw(
		DVASPECT_CONTENT,  		// Draw Aspect
		/*-1*/0,					// Lindex
		NULL,					// Info for drawing optimazation
		NULL,					// target device information
		hPaintDC,			    // Draw device HDC
		NULL, 				   	// Target device HDC
		(RECTL*)&rt,			// Bounding client rectangle
		NULL, 		            // Clipping rectangle for metafiles
		(RECT*)&rt,				// Update rectangle
		NULL, 	   				// Call back function
		NULL,					// Call back parameter
		0);				        // What view of the object

	LONG lWidth = rt.right - rt.left;
	LONG lHeight = 0;
	SIZEL szExtent = { -1, -1 };
	pTextService->TxGetNaturalSize(
		DVASPECT_CONTENT, 
		hPaintDC,
		NULL,
		NULL,
		TXTNS_FITTOCONTENT,
		&szExtent,
		&lWidth,
		&lHeight);

	return CSize(lWidth, lHeight);
}
