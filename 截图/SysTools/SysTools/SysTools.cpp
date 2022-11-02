// SysTools.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "SysTools.h"
#include <psapi.h>
#pragma comment(lib, "Psapi.lib")
#include "ScreenCapture/CaptureWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 唯一的应用程序对象

CWinApp theApp;

using namespace std;

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;

	HMODULE hModule = ::GetModuleHandle(NULL);

	if (hModule != NULL)
	{
		// 初始化 MFC 并在失败时显示错误
		if (!AfxWinInit(hModule, NULL, ::GetCommandLine(), 0))
		{
			// TODO: 更改错误代码以符合您的需要
			_tprintf(_T("错误: MFC 初始化失败\n"));
			nRetCode = 1;
		}
		else
		{
			// TODO: 在此处为应用程序的行为编写代码。
		}
	}
	else
	{
		// TODO: 更改错误代码以符合您的需要
		_tprintf(_T("错误: GetModuleHandle 失败\n"));
		nRetCode = 1;
	}

	return nRetCode;
}

SYSTOOLS_API vector<wstring> OpenFileDlg(HWND hParentWnd, const char* lpszDefExt, const char* lpszFileName, const char* lpszFilter,bool bAllowMultiSelect)
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
			DWORD dwFlags = OFN_OVERWRITEPROMPT | OFN_HIDEREADONLY | OFN_ALLOWMULTISELECT;
			if (!bAllowMultiSelect)
			{
				dwFlags &= ~OFN_ALLOWMULTISELECT; 
			}
			CFileDialog fd(TRUE, lpszDefExt, lpszFileName, dwFlags, lpszFilter, hParentWnd != NULL ? CWnd::FromHandle(hParentWnd) : NULL);
		
			/*定义临时缓冲区,存储图片路径*/
			TCHAR        szLargeBuf[4096];
			memset(szLargeBuf,'\0',4096);
			fd.m_ofn.lpstrFile = szLargeBuf;
#ifdef	UNICODE
			fd.m_ofn.nMaxFile = 4096;
#else
			fd.m_ofn.nMaxFile=sizeof(szLargeBuf);
#endif

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

SYSTOOLS_API vector<wstring> SaveFileDlg(HWND hParentWnd, const char* lpszDefExt, const char* lpszFileName, const char* lpszFilter)
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
			DWORD dwFlags = OFN_OVERWRITEPROMPT | OFN_HIDEREADONLY;
			CFileDialog fd(FALSE, lpszDefExt, lpszFileName, dwFlags, lpszFilter, hParentWnd != NULL ? CWnd::FromHandle(hParentWnd) : NULL);
			if(fd.DoModal() == IDOK) 
			{
				CString strPath = fd.GetPathName();
				int len = MultiByteToWideChar(CP_ACP,0,strPath.GetBuffer(),strPath.GetLength(),NULL,0);
				wchar_t* m_wchar=new wchar_t[len+1];
				MultiByteToWideChar(CP_ACP,0,strPath.GetBuffer(),strPath.GetLength(),m_wchar,len);
				m_wchar[len]='\0';
				vRet.push_back(m_wchar);
				delete m_wchar;
			}
		}
	}
	return vRet;
}

SYSTOOLS_API void ShowCaptureWnd(HWND hWnd)
{
	HMODULE hModule = ::GetModuleHandle(NULL);

	if (hModule != NULL)
	{
		// 初始化 MFC 并在失败时显示错误
		if (!AfxWinInit(hModule, NULL, ::GetCommandLine(), 0))
		{
			return;
		}
	}

	CCaptureWnd::GetInstance()->EventRegister(hWnd);
	CCaptureWnd::GetInstance()->ShowWindow(SW_SHOW);
	SetForegroundWindow(CCaptureWnd::GetInstance()->GetSafeHwnd());
}
