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
	SC_HANDLE hSCM = NULL;				// SCM DataBase ���
	SC_HANDLE hService = NULL;			// ��������ľ��
	BOOL bReturn = FALSE;			
	
	hSCM = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (hSCM != NULL)
	{
		hService = CreateService( 
			hSCM,						// SCM���ݿ�(Database)��� 
			strName,					// SCM Database�б���ķ�������(��������ݿ���ע�������)
			strDisplayName,				// �û������������������ʾ�ķ�������
			SERVICE_ALL_ACCESS,			// ����ķ���Ȩ�� 
			dwServiceType,				// �������ͣ�Ĭ���ṩ SERVICE_WIN32_OWN_PROCESS �������Լ��Ľ�����
			dwStartType,				// �������ͣ�Ĭ���ṩ SERVICE_AUTO_START��SCM�������Զ��������� 
			SERVICE_ERROR_NORMAL,		// �������Ĵ���ʽ���ṩSERVICE_ERROR_NORMAL �д�����֣�������¼����־��
			strImagePath,				// ��ִ��·��
			NULL,						// ���� 
			NULL,						// ��ǩֵ 
			NULL,						// �������� 
			NULL,						// LocalSystem �ʻ� 
			NULL);						// ����
		
		if (hService != NULL)
		{
			if (!strDesc.IsEmpty())		// Ϊ������ӷ��������� �ı����Ŀ�ѡ����
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


