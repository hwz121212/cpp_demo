
#include "stdafx.h"
#include <psapi.h>
#pragma comment(lib, "Psapi.lib")
#include "ScreenCapture/CaptureWnd.h"


SYSTOOLS_API vector<wstring> OpenFileDlg(HWND hParentWnd, const char* lpszDefExt, const char* lpszFileName, const char* lpszFilter)
{
	vector<wstring> vRet;

	TCHAR lpDllName[MAX_PATH] = {0};
	HMODULE hModule = NULL;
	GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS,(LPCTSTR)&OpenFileDlg,&hModule);
	if (hModule != NULL)
	{
		DWORD dwRet = GetModuleBaseName(GetCurrentProcess(),hModule,lpDllName,sizeof(lpDllName));
		if (dwRet > 0)
		{
			AfxSetResourceHandle(GetModuleHandle(lpDllName));
			CFileDialog fd(TRUE, lpszDefExt, lpszFileName, OFN_OVERWRITEPROMPT|OFN_HIDEREADONLY|~OFN_NODEREFERENCELINKS, lpszFilter, hParentWnd != NULL ? CWnd::FromHandle(hParentWnd) : NULL);
			if(fd.DoModal() == IDOK) 
			{
				CString strPath;
				POSITION pos = fd.GetStartPosition();
				while(pos != NULL)
				{
					strPath = fd.GetNextPathName(pos);
					int len = MultiByteToWideChar(CP_ACP,0,strPath.GetBuffer(),strPath.GetLength(),NULL,0);
					wchar_t* m_wchar=new wchar_t[len+1];
					MultiByteToWideChar(CP_ACP,0,strPath.GetBuffer(),strPath.GetLength(),m_wchar,len);
					m_wchar[len]='\0';
					vRet.push_back(m_wchar);
					delete m_wchar;
				}
			}
		}
	}
	return vRet;
}

SYSTOOLS_API void ShowCaptureWnd()
{
	HMODULE hModule = ::GetModuleHandle(NULL);

	if (hModule != NULL)
	{
		// 初始化 MFC 并在失败时显示错误
		if (!AfxWinInit(hModule, NULL, ::GetCommandLine(), 0))
		{
			return 1;
		}
	}
	RECT rt;
	rt.left = 0;rt.right=0;rt.top=0;rt.bottom=0;
	CCaptureWnd *m_capWnd = new CCaptureWnd();
	CString csWndClass = AfxRegisterWndClass(CS_OWNDC, 0, 0, 0);
	m_capWnd->CreateEx(0, csWndClass, _T("Capture_Wnd"), WS_POPUP |WS_VISIBLE,0,20,0,20,NULL,NULL);
	m_capWnd->ShowWindow(SW_SHOW);
}
