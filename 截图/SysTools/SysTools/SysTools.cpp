// SysTools.cpp : ���� DLL Ӧ�ó���ĵ���������
//

#include "stdafx.h"
#include "SysTools.h"
#include <psapi.h>
#pragma comment(lib, "Psapi.lib")
#include "ScreenCapture/CaptureWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Ψһ��Ӧ�ó������

CWinApp theApp;

using namespace std;

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;

	HMODULE hModule = ::GetModuleHandle(NULL);

	if (hModule != NULL)
	{
		// ��ʼ�� MFC ����ʧ��ʱ��ʾ����
		if (!AfxWinInit(hModule, NULL, ::GetCommandLine(), 0))
		{
			// TODO: ���Ĵ�������Է���������Ҫ
			_tprintf(_T("����: MFC ��ʼ��ʧ��\n"));
			nRetCode = 1;
		}
		else
		{
			// TODO: �ڴ˴�ΪӦ�ó������Ϊ��д���롣
		}
	}
	else
	{
		// TODO: ���Ĵ�������Է���������Ҫ
		_tprintf(_T("����: GetModuleHandle ʧ��\n"));
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
		
			/*������ʱ������,�洢ͼƬ·��*/
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
		// ��ʼ�� MFC ����ʧ��ʱ��ʾ����
		if (!AfxWinInit(hModule, NULL, ::GetCommandLine(), 0))
		{
			return;
		}
	}

	CCaptureWnd::GetInstance()->EventRegister(hWnd);
	CCaptureWnd::GetInstance()->ShowWindow(SW_SHOW);
	SetForegroundWindow(CCaptureWnd::GetInstance()->GetSafeHwnd());
}
