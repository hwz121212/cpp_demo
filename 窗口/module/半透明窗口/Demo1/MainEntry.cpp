#include <Windows.h>
#include <tchar.h>
#include <assert.h>
#include "resource.h"
#include "GdiplusH.h"
//////////////////////////////////////////////////////////////////////////
void _ShadowWnd(HWND hWnd);

LRESULT CALLBACK _DefWndProc(HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam)
{	
	static const UINT IDC_BTNEXIT		= 1002;

	switch (nMsg)
	{
		case WM_NCCREATE:
		{
			_ShadowWnd(hWnd);
			break;
		}
		case WM_CREATE:
		{
			//--: Move Main wnd to center of screen
			RECT rtMainWnd;
			GetClientRect(hWnd, &rtMainWnd);
			UINT nSrnWidth		= GetSystemMetrics(SM_CXFULLSCREEN);
			UINT nSrnHeight		= GetSystemMetrics(SM_CYFULLSCREEN);
			UINT nWndWidth		= (rtMainWnd.right - rtMainWnd.left);
			UINT nWndHeight		= (rtMainWnd.bottom - rtMainWnd.top);
			UINT nLeft			= (nSrnWidth - nWndWidth) / 2;
			UINT nTop			= (nSrnHeight - nWndHeight) / 2;
			MoveWindow(hWnd, nLeft, nTop, nWndWidth, nWndHeight, TRUE);

			CreateWindow(_T("Button"), _T("Exit"), WS_VISIBLE|WS_CHILD, 150, 0, 100, 50, hWnd, (HMENU)IDC_BTNEXIT, NULL, NULL);
					
			break;
		}
		case WM_COMMAND:
		{
			switch (wParam)
			{
				case IDC_BTNEXIT:
				{
					DestroyWindow(hWnd);
					break;
				}
			}
			break;
		}
		case WM_NCHITTEST:
		{
			return HTCAPTION;
		}
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			break;
		}
	}

	return ::DefWindowProc(hWnd, nMsg, wParam, lParam);
}

//////////////////////////////////////////////////////////////////////////
bool RegisterWndClass(HINSTANCE hInstance, const TCHAR* szClsName)
{
	WNDCLASSEX wcs		= {0};
	wcs.cbSize			= sizeof(WNDCLASSEX);
	wcs.hbrBackground	= (HBRUSH)GetStockObject(COLOR_APPWORKSPACE);
	wcs.lpszClassName	= szClsName;
	wcs.lpfnWndProc		= _DefWndProc;
	wcs.hCursor			= LoadCursor(NULL, MAKEINTRESOURCE(IDC_ARROW));
	wcs.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPLICATION));
	wcs.style			= CS_VREDRAW|CS_HREDRAW;
	
	return (0 != RegisterClassEx(&wcs));
}

//////////////////////////////////////////////////////////////////////////
bool CreateMainWnd(HINSTANCE hInstance, const TCHAR* szClsName, const TCHAR* szWndCaption)
{
	if (NULL != szClsName && NULL != szWndCaption)
	{
		DWORD dwStyleEx = WS_EX_LEFT|WS_EX_LTRREADING|WS_EX_TOPMOST; 
		DWORD dwStyle   = WS_POPUP|WS_VISIBLE|WS_CLIPCHILDREN;
		HWND hWnd = CreateWindowEx(dwStyleEx, szClsName, szWndCaption, dwStyle, 0, 0, 556, 379, NULL, NULL, hInstance, NULL);
	
		if (NULL != hWnd && IsWindow(hWnd))
		{
			ShowWindow(hWnd, SW_SHOW);
			UpdateWindow(hWnd);
			return true;
		}
	}
	return false;
}

//////////////////////////////////////////////////////////////////////////
void RunMessageLoop()
{
	MSG Msg;
	while (true)
	{
		while (PeekMessage(&Msg, NULL, 0, 0, PM_REMOVE))
		{
			if (WM_QUIT == Msg.message)
			{
				return;
			}

			TranslateMessage(&Msg);
			DispatchMessage(&Msg);
		}
	}
}

//////////////////////////////////////////////////////////////////////////
BOOL WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE hPreInstance, LPTSTR szCmdLine, int nShowCmd)
{
	const TCHAR* szClsName		= _T("Win32App");
	const TCHAR* szWndCaption	= _T("Demo0047");

	if (RegisterWndClass(hInstance, szClsName))
	{
		if (CreateMainWnd(hInstance, szClsName, szWndCaption))
		{
			RunMessageLoop();
		}
	}

	return TRUE;
}



//////////////////////////////////////////////////////////////////////////
HWND _CreateShadowWnd(HWND hParent)
{
	WNDCLASSEX ws	= {0};
	ws.cbSize		= sizeof(WNDCLASSEX);
	ws.style		= CS_HREDRAW | CS_VREDRAW;
	ws.hInstance	= GetModuleHandle(NULL);
	ws.lpfnWndProc	= DefWindowProc;
	ws.hbrBackground= (HBRUSH)(COLOR_WINDOW + 1);
	ws.hCursor		= (HCURSOR)LoadCursor(NULL, MAKEINTRESOURCE(IDC_ARROW));
	ws.lpszClassName = _T("ShadowWnd");
	RegisterClassEx(&ws);

	RECT rtWnd;
	GetWindowRect(hParent, &rtWnd);
	int nWndWidth	= rtWnd.right - rtWnd.left;
	int nWndHeight	= rtWnd.bottom - rtWnd.top;
	HWND hShadowWnd = CreateWindowEx(WS_EX_LAYERED|WS_EX_NOACTIVATE|WS_EX_LEFT|WS_EX_TRANSPARENT, 
									 _T("ShadowWnd"), 
									 NULL, 
									 GetWindowLongPtr(hParent, GWL_STYLE)|WS_VISIBLE, 
									 rtWnd.left, 
									 rtWnd.top, 
									 nWndWidth,
									 nWndHeight, 
									 hParent, 
									 NULL, 
								 	 GetModuleHandle(NULL), 
									 NULL);

	return hShadowWnd;
}

Image* _LoadImage( UINT nID, LPCTSTR lpszType, HINSTANCE hInstance /*=NULL*/)
{
	Image * pImage = NULL;

	if( lpszType == RT_BITMAP )
	{
		HBITMAP hBitmap = ::LoadBitmap( hInstance, MAKEINTRESOURCE(nID) );
		pImage = (Image*) Gdiplus::Bitmap::FromHBITMAP(hBitmap, 0);
		::DeleteObject(hBitmap);
		return pImage;
	}		

	HRSRC hRsrc = ::FindResource ( hInstance, MAKEINTRESOURCE(nID), lpszType); 

	DWORD dwSize = ::SizeofResource( hInstance, hRsrc);
	LPBYTE lpRsrc = (LPBYTE)::LoadResource( hInstance, hRsrc);

	HGLOBAL hMem = ::GlobalAlloc(GMEM_FIXED, dwSize);
	LPBYTE pMem = (LPBYTE)::GlobalLock(hMem);
	memcpy( pMem, lpRsrc, dwSize);
	IStream * pStream = NULL;
	::CreateStreamOnHGlobal( hMem, FALSE, &pStream);

	pImage = Image::FromStream(pStream);

	::GlobalUnlock(hMem);
	pStream->Release();
	::FreeResource(lpRsrc);

	return pImage;
}

void _DrawCtrl(HWND hCtrl, Graphics* pGraphics)
{
	RECT rtClient;
	GetClientRect(hCtrl, &rtClient);
	MapWindowPoints(hCtrl, GetParent(hCtrl), (POINT*)&rtClient, 2);
	
	HDC hDC			= GetDC(hCtrl);
	HDC hMemDC		= CreateCompatibleDC(hDC);
	HBITMAP hBitmap	= CreateCompatibleBitmap(hDC, rtClient.right - rtClient.left, rtClient.bottom - rtClient.top);
	HGDIOBJ hOldBmp = SelectObject(hMemDC, hBitmap);
	
	//BitBlt(hMemDC, 0, 0, rtClient.right - rtClient.left, rtClient.bottom - rtClient.top, hDC, 0, 0, SRCCOPY);
	SendMessage(hCtrl, WM_PRINT, (WPARAM)hMemDC, PRF_CHILDREN|PRF_CLIENT|PRF_ERASEBKGND|PRF_NONCLIENT|PRF_OWNED);

	Bitmap Bitmap(hBitmap, NULL);
	pGraphics->DrawImage(&Bitmap, rtClient.left, rtClient.top);
	
	SelectObject(hMemDC, hOldBmp);
	DeleteObject(hOldBmp);
	DeleteObject(hMemDC);			
	ReleaseDC(hCtrl, hDC);
}

void _BlendRefresh(HWND hShadow, HWND hWnd, Image* pImage, UINT nAlpha)
{
	RECT rtClient;
	GetClientRect(hShadow, &rtClient);
	UINT nWidth		= rtClient.right - rtClient.left;
	UINT nHeight	= rtClient.bottom - rtClient.top;

	HDC hDC = ::GetDC(hShadow);
	HDC hdcMemory = ::CreateCompatibleDC(hDC);

	BITMAPINFOHEADER stBmpInfoHeader = { 0 };   
	int nBytesPerLine			= ((nWidth * 32 + 31) & (~31)) >> 3;
	stBmpInfoHeader.biSize		= sizeof(BITMAPINFOHEADER);   
	stBmpInfoHeader.biWidth		= nWidth;   
	stBmpInfoHeader.biHeight	= nHeight;   
	stBmpInfoHeader.biPlanes	= 1;   
	stBmpInfoHeader.biBitCount	= 32;   
	stBmpInfoHeader.biCompression = BI_RGB;   
	stBmpInfoHeader.biClrUsed	= 0;   
	stBmpInfoHeader.biSizeImage = nBytesPerLine * nHeight;   

	PVOID pvBits = NULL;   
	HBITMAP hbmpMem = ::CreateDIBSection(NULL, (PBITMAPINFO)&stBmpInfoHeader, DIB_RGB_COLORS, &pvBits, NULL, 0);
	memset( pvBits, 0, nWidth * 4 * nHeight);

	if(hbmpMem)   
	{   
		HGDIOBJ hbmpOld = ::SelectObject(hdcMemory, hbmpMem); 
		Graphics graph(hdcMemory);

		graph.SetSmoothingMode(SmoothingModeNone);

		// Draw the backgroundb
		graph.DrawImage(pImage, 0, 0, nWidth, nHeight);

		// Draw controls
		HWND hCtrl = GetWindow(hWnd, GW_CHILD);
		do {
			if (NULL != hCtrl)
			{
				_DrawCtrl(hCtrl, &graph);
			}
		} while (hCtrl = GetWindow(hCtrl, GW_HWNDNEXT));

		BITMAP bmp;
		GetObject(hbmpMem, sizeof(bmp), &bmp);
		RECT rc;
		::GetWindowRect(hWnd, &rc);
		POINT ptSrc				= { 0, 0 };
		POINT ptWinPos			= { rc.left, rc.top };
		SIZE  szWin				= { bmp.bmWidth, bmp.bmHeight };
		BLENDFUNCTION stBlend	= { AC_SRC_OVER, 0, nAlpha, AC_SRC_ALPHA };

		::UpdateLayeredWindow(hShadow,
							  hDC,
							  &ptWinPos,
							  &szWin,
							  hdcMemory,
							  &ptSrc,
							  0,
							  &stBlend,
							  ULW_ALPHA);

		graph.ReleaseHDC(hdcMemory);
		::SelectObject( hdcMemory, hbmpOld);   
		::DeleteObject(hbmpMem); 
	}

	::DeleteDC(hdcMemory);
	::DeleteDC(hDC);
}

LRESULT CALLBACK _WndProc(HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam)
{
	HRESULT hResult = _DefWndProc(hWnd, nMsg, wParam, lParam);

	static bool bUpdate = false;
	static  Image* pImage;

	static HWND hShadowWnd = NULL;
	switch (nMsg)
	{
		case WM_CREATE:
		{
			pImage		= _LoadImage(IDB_PNG1, _T("PNG"), GetModuleHandle(NULL));
			hShadowWnd	= _CreateShadowWnd(hWnd);
			break;
		}
		case WM_MOVE:
		{
			RECT rtWnd;
			GetWindowRect(hWnd, &rtWnd);
			if (NULL != hShadowWnd && IsWindow(hShadowWnd))
			{
				SetWindowPos(hShadowWnd, NULL, rtWnd.left, rtWnd.top, 0, 0, SWP_NOZORDER|SWP_NOSIZE);
			}
			break;
		}
		case WM_SIZE:
		{
			RECT rtWnd;
			GetWindowRect(hWnd, &rtWnd);
			if (NULL != hShadowWnd && IsWindow(hShadowWnd))
			{
				SetWindowPos(hShadowWnd, NULL, 0, 0, rtWnd.right - rtWnd.left, rtWnd.bottom - rtWnd.top, SWP_NOZORDER|SWP_NOMOVE);
			}
			break;
		}
		case WM_PAINT:
		case WM_CAPTURECHANGED:
		case WM_CTLCOLOREDIT:
		case WM_CTLCOLORBTN:
		case WM_CTLCOLORSTATIC:
		case WM_CTLCOLORMSGBOX:
		case WM_CTLCOLORDLG:
		case WM_CTLCOLORLISTBOX:
		case WM_CTLCOLORSCROLLBAR:
		{
			if (NULL != pImage)
			{
				_BlendRefresh(hShadowWnd, hWnd, pImage, 200);
			}
			break;
		}
		case WM_DESTROY: 
		{
			if (NULL != hShadowWnd && IsWindow(hShadowWnd))
			{
				DestroyWindow(hShadowWnd);
			}
			break;
		}
	}
	return hResult;
}


void _ShadowWnd(HWND hWnd)
{
	LONG _OldWndProc = GetWindowLongPtr(hWnd, GWLP_WNDPROC);
	if (NULL != _OldWndProc)
	{
		SetWindowLongPtr(hWnd, GWLP_WNDPROC, (LONG)_WndProc);
		SetWindowLongPtr(hWnd, GWL_EXSTYLE, GetWindowLongPtr(hWnd, GWL_EXSTYLE)|WS_EX_LAYERED);
		SetLayeredWindowAttributes(hWnd, 0, 3, LWA_ALPHA);
	}

	return ;
}
