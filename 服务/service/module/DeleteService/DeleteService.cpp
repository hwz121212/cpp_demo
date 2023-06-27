// DeleteService.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "DeleteService.h"
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

const CString strExeToBeUnload = _T("winvnc");

BOOL DeleteService(CString strService,BOOL bStop);
DWORD StopServiceDepends(SC_HANDLE hSCM, SC_HANDLE hService, BOOL fStopDependencies, DWORD dwTimeout);

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
		
		BOOL bUnload = FALSE;
		
		bUnload = DeleteService(strExeToBeUnload, TRUE);
	
		
		CString strInfo = "";
		if (bUnload)
		{			
			strInfo.Format(_T("Unload %s service succeed!"), (LPCTSTR)strExeToBeUnload);			
		}
		else
		{
			strInfo.Format(_T("Unload %s service failed!"), (LPCTSTR)strExeToBeUnload);			
		}
		
		cout<<(LPCTSTR)strInfo<<endl;

	}

	Sleep(1500);

	return nRetCode;
}



BOOL DeleteService(CString strService,BOOL bStop)
{
	SC_HANDLE hSCM = NULL;				
	SC_HANDLE hService = NULL;	
	BOOL bReturn = FALSE;
	DWORD dwError = 0;
	
	hSCM = ::OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (hSCM != NULL) 
	{
		hService = ::OpenService(hSCM, strService, SERVICE_ALL_ACCESS);
		if (hService != NULL)
		{
			if(bStop)
			{
				dwError = StopServiceDepends(hSCM, hService, FALSE, INFINITE);
				if (dwError == ERROR_SUCCESS)
				{
					if (::DeleteService(hService))
					{
						bReturn = TRUE;
					}
				}
			}else{
				if (::DeleteService(hService))
				{
					bReturn = TRUE;
				}
			}
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
		hService = NULL;
	}
	
	return bReturn;
}

DWORD StopServiceDepends(SC_HANDLE hSCM, SC_HANDLE hService, BOOL fStopDependencies, DWORD dwTimeout)
{
	SERVICE_STATUS_PROCESS ss;
	DWORD dwStartTime = GetTickCount();
	DWORD dwBytesNeeded = 0;

	// 保证该服务没有被停止
	if ( !QueryServiceStatusEx( hService, SC_STATUS_PROCESS_INFO,
							(LPBYTE)&ss, sizeof(SERVICE_STATUS_PROCESS), &dwBytesNeeded ) )
		return GetLastError();

	if (ss.dwCurrentState == SERVICE_STOPPED) 
		return ERROR_SUCCESS;

	// 如果正在停止，则等待停止成功
	while (ss.dwCurrentState == SERVICE_STOP_PENDING) 
	{
		Sleep(2000);
		if (!QueryServiceStatusEx( hService, SC_STATUS_PROCESS_INFO, (LPBYTE)&ss, sizeof(SERVICE_STATUS_PROCESS), &dwBytesNeeded))
			return GetLastError();

		if (ss.dwCurrentState == SERVICE_STOPPED)
			return ERROR_SUCCESS;

		if (GetTickCount() - dwStartTime > dwTimeout)
			return ERROR_TIMEOUT;
	}

	// 如果服务正在运行, 依赖它的服务必须首先停止
	if (fStopDependencies) 
	{
		DWORD i;
		DWORD dwBytesNeeded;
		DWORD dwCount;
		LPENUM_SERVICE_STATUS lpDependencies = NULL;
		ENUM_SERVICE_STATUS ess;
		SC_HANDLE hDepService;

		// 获取需要分配内存的大小
		if (EnumDependentServices(hService, SERVICE_ACTIVE, lpDependencies, 0, &dwBytesNeeded, &dwCount)) 
		{
			// 如果调用成功，则没有依赖的服务
		} 
		else 
		{
			if (GetLastError() != ERROR_MORE_DATA)
				return GetLastError(); 

			lpDependencies = (LPENUM_SERVICE_STATUS)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, dwBytesNeeded);
			if (!lpDependencies)
				return GetLastError();

			__try 
			{
				// 枚举依赖的服务
				if (!EnumDependentServices( hService, 
										    SERVICE_ACTIVE, 
										    lpDependencies, 
										    dwBytesNeeded, 
										    &dwBytesNeeded,
										    &dwCount) )
					return GetLastError();
 
				for (i = 0; i < dwCount; i++) 
				{
					ess = *(lpDependencies + i);
					hDepService = OpenService( hSCM,
											   ess.lpServiceName, 
											   SERVICE_STOP | SERVICE_QUERY_STATUS );
					if (!hDepService)
						return GetLastError();

					__try 
					{
						// 发送停止码
						if (!ControlService( hDepService,SERVICE_CONTROL_STOP,(LPSERVICE_STATUS)&ss))
							return GetLastError();
 
						// 等待服务停止
						while (ss.dwCurrentState != SERVICE_STOPPED) 
						{
							Sleep(2000);
							if (!QueryServiceStatusEx( hDepService, 
													   SC_STATUS_PROCESS_INFO,
													   (LPBYTE)&ss, 
													   sizeof(SERVICE_STATUS_PROCESS),
													   &dwBytesNeeded))
								return GetLastError();

							if (ss.dwCurrentState == SERVICE_STOPPED)
								break;

							if (GetTickCount() - dwStartTime > dwTimeout)
								return ERROR_TIMEOUT;
						}
					} 
					__finally 
					{
						 // 关闭打开的服务句柄
						 CloseServiceHandle(hDepService);
					}
				}
			} 
			__finally 
			{
				//释放掉分配的内存
				HeapFree(GetProcessHeap(), 0, lpDependencies);
			}
		} 
	}
   
	// 发送停止命令到服务控制区
	if (!ControlService( hService, SERVICE_CONTROL_STOP, (LPSERVICE_STATUS)&ss))
		return GetLastError();

	// 等待服务停止
	while (ss.dwCurrentState != SERVICE_STOPPED) 
	{
		Sleep(2000);
		if (!QueryServiceStatusEx( hService, 
								   SC_STATUS_PROCESS_INFO,
								   (LPBYTE)&ss, 
								   sizeof(SERVICE_STATUS_PROCESS),
								   &dwBytesNeeded ))
			return GetLastError();

		if (ss.dwCurrentState == SERVICE_STOPPED)
			break;

		if (GetTickCount() - dwStartTime > dwTimeout)
			return ERROR_TIMEOUT;
	}

	// 返回成功标志
	return ERROR_SUCCESS;
}
