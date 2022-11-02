#ifndef __CAPTUREHELPER_H__
#define __CAPTUREHELPER_H__

// capture
HBITMAP CaptureDesktop();

// render
void FillRect(HDC hDC, const RECT& rc, DWORD dwBrushColor);
void DrawRect(HDC hDC, const RECT& rc, int nSize, DWORD dwPenColor);
void DrawRoundRect(HDC hDC, const RECT& rc, int nSize, int width, int height, DWORD dwPenColor);
void DrawLine( HDC hDC, const CPoint& begin, const CPoint& end, int nSize, DWORD dwPenColor,int nStyle /*= PS_SOLID*/ );
void DrawText(HDC hDC, RECT& rc, LPCTSTR pstrText, DWORD dwTextColor, UINT uStyle);

HBITMAP ChopBitmap(HBITMAP hSrcBitmap, int cx, int cy, int width, int height);
HBITMAP CopyBitmap(HBITMAP hSrcBitmap);
void CreateMaskBitmap(HBITMAP &hBitmap, int width, int height, DWORD argb);
DWORD GetRGB(HBITMAP hBitmap, int x, int y);
void SaveFile(CString image, HBITMAP hBitmap);
HBITMAP LoadFile(CString image);

// gdi plus
void InitGDIPlus();
void DeInitGDIPlus();

// richedit
SIZE EstimateTextSize(CRichEditCtrl *pRE, CSize szAvailable);

#endif

