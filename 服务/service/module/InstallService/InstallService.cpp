// InstallService.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "InstallService.h"
#include <winsvc.h>


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// The one and only application object

CWinApp theApp;

using namespace std;


BOOL InstallService( CString strName,
					CString strDisplayName,
					CString strImagePath,
					CString strDesc,
					DWORD dwServiceType,
									DWORD dwStartType);


const CString strExeToBeInstalled = _T("winvnc");

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;

	// initialize MFC and print and error on failure
	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
	{
		// TODO: change error code to suit your needs
		cerr << _T("Fatal Error: MFC initialization failed") << endl;
		nRetCode = 1;
	}
	else
	{

		TCHAR szFilePath[MAX_PATH + 1]={0};
		GetModuleFileName(NULL, szFilePath, MAX_PATH);
		(_tcsrchr(szFilePath, _T('\\')))[1] = 0;
		CString strVncPath = szFilePath;
		strVncPath += strExeToBeInstalled + _T(".exe");
		strVncPath += _T(" -service");
		
		BOOL bInstalled = FALSE;
		
		CString strRegiserName = _T("winvnc");
		CString strDisplayName = strExeToBeInstalled;	
		
		CString strExePath = strVncPath;
		CString strServerDescription = _T("UltraVNC Desprition");
		DWORD dwServiceType = SERVICE_WIN32_OWN_PROCESS;
		DWORD dwStartType = SERVICE_DEMAND_START;
		
		bInstalled = InstallService( strRegiserName, strDisplayName, strExePath,
			strServerDescription, dwServiceType, dwStartType);
		
		CString strInfo;
		if (bInstalled)
		{			
			strInfo.Format(_T("Install %s service succeed!"), (LPCTSTR)strExeToBeInstalled);			
		}
		else
		{
			strInfo.Format(_T("Install %s service failed!"), (LPCTSTR)strExeToBeInstalled);			
		}
		
		cout<<(LPCTSTR)strInfo<<endl;

		Sleep(1500);
	
	}

	return nRetCode;
}



BOOL InstallService( CString strName,
									CString strDisplayName,
									CString strImagePath,
									CString strDesc,
									DWORD dwServiceType,
									DWORD dwStartType)
{
	SC_HANDLE hSCM = NULL;				// SCM DataBase 句柄
	SC_HANDLE hService = NULL;			// 操作服务的句柄
	BOOL bReturn = FALSE;			
	
	hSCM = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (hSCM != NULL)
	{
		hService = CreateService( 
			hSCM,						// SCM数据库(Database)句柄 
			strName,					// SCM Database中保存的服务名称(服务的数据库中注册的名字)
			strDisplayName,				// 用户界面或服务管理器重显示的服务名称
			SERVICE_ALL_ACCESS,			// 服务的访问权限 
			dwServiceType,				// 服务类型，默认提供 SERVICE_WIN32_OWN_PROCESS 运行在自己的进程下
			dwStartType,				// 启动类型，默认提供 SERVICE_AUTO_START在SCM启动后自动启动类型 
			SERVICE_ERROR_NORMAL,		// 服务错误的处理方式，提供SERVICE_ERROR_NORMAL 有错误出现，仅仅记录到日志中
			strImagePath,				// 可执行路径
			NULL,						// 组序 
			NULL,						// 标签值 
			NULL,						// 依赖服务 
			NULL,						// LocalSystem 帐户 
			NULL);						// 密码
		
		if (hService != NULL)
		{
			if (!strDesc.IsEmpty())		// 为服务添加服务描述， 改变服务的可选配置
			{
				SERVICE_DESCRIPTION sdBuf;
				sdBuf.lpDescription = (char *)(LPCTSTR)strDesc;
				ChangeServiceConfig2(hService, SERVICE_CONFIG_DESCRIPTION, &sdBuf);
			}
			
			bReturn = TRUE;
		}
	}
	
	if (hService != NULL)
	{
		CloseServiceHandle(hService);
		hService = NULL;
	}
	
	if (hSCM != NULL)
	{
		CloseServiceHandle(hSCM);
		hSCM = NULL;
	}
	
	return bReturn;
}


