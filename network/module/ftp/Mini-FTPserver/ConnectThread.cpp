// ConnectThread.cpp : implementation file
//

#include "stdafx.h"
#include "FTPserver.h"
#include "FTPserverDlg.h"
#include "ConnectThread.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CConnectThread

IMPLEMENT_DYNCREATE(CConnectThread, CWinThread)

CConnectThread::CConnectThread()
{
	m_nTimerID = 0;
	m_LastDataTransferTime = CTime::GetCurrentTime();
}

CConnectThread::~CConnectThread()
{
}

/********************************************************************/
/*																	*/
/* �������� : CConnectThread::InitInstance							*/
/* ����     : �߳�����ʱִ�еĲ���									*/
/*																	*/
/********************************************************************/
BOOL CConnectThread::InitInstance()
{
	try
	{
		// ����SOCKET
		m_ControlSocket.Attach(m_hSocket);
		m_ControlSocket.m_pThread = this;
		
		m_ControlSocket.pThread = (CConnectThread*)this;
		m_ControlSocket.pWnd = (CFTPserverDlg *)(this->m_pWndServer);

		// ֪ͨ����������һ�����߳�
		m_pWndServer->SendMessage(WM_THREADSTART, (WPARAM)this, 0);
		
		// �����������
		if (((CFTPserverDlg *)m_pWndServer)->CheckMaxUsers())
		{
			m_ControlSocket.SendResponse("421 Too many users are connected, please try again later.");
			PostThreadMessage(WM_QUIT,0,0);
		}
		else
		{
			// ���ͻ�ӭ��Ϣ
			m_ControlSocket.SendResponse("220 %s", ((CFTPserverDlg *)m_pWndServer)->GetWelcomeMessage());
			m_nTimerID = ::SetTimer(NULL, 0, 1000, TimerProc);
		}
	}
	catch (CException* e)
	{
		e->Delete();
	}
	return TRUE;
}

/********************************************************************/
/*																	*/
/* �������� : CConnectThread::ExitInstance							*/
/* ����     : �߳��˳�ʱִ�еĲ���									*/
/*																	*/
/********************************************************************/
int CConnectThread::ExitInstance()
{
	CFTPserverDlg *pWnd = (CFTPserverDlg *)m_pWndServer;
	
	try
	{
		pWnd->m_CriticalSection.Lock();
		
		// ���߳�������ɾ���߳�
		POSITION pos = pWnd->m_ThreadList.Find(this);
		if(pos != NULL)
		{
			pWnd->m_ThreadList.RemoveAt(pos);
		}

		pWnd->m_CriticalSection.Unlock();

		// �ر����ӵ��׽���
		m_ControlSocket.Close();

		// ֪ͨ���ڹر���һ���߳�
		pWnd->SendMessage(WM_THREADCLOSE, (WPARAM)this, 0);

	}
	catch(CException *e)
	{
		pWnd->m_CriticalSection.Unlock();
		e->Delete();
	}
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CConnectThread, CWinThread)
	//{{AFX_MSG_MAP(CConnectThread)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
	ON_THREAD_MESSAGE(WM_DESTROYDATACONNECTION, OnDestroyDataConnection)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CConnectThread message handlers

/********************************************************************/
/*																	*/
/* �������� : CConnectThread::OnDestroyDataConnection				*/
/* ����     : �ر�����������Ϣ��Ӧ									*/
/*																	*/
/********************************************************************/
void CConnectThread::OnDestroyDataConnection(WPARAM wParam, LPARAM lParam)
{
	// �ر���������
	m_ControlSocket.DestroyDataConnection();
}

/********************************************************************/
/*																	*/
/* �������� : TimerProc												*/	
/* ����     : ��ʱ����Ӧ����										*/
/*																	*/
/********************************************************************/
VOID CALLBACK CConnectThread::TimerProc(HWND hwnd, UINT uMsg, UINT_PTR uIDEvent, DWORD dwTime)
{
	CConnectThread *pThread = (CConnectThread *)AfxGetThread();
	
	if (uIDEvent == pThread->m_nTimerID)
	{
		int nConnectionTimeout = ((CFTPserverDlg *)pThread->m_pWndServer)->GetTimeout();
		// ��鳬ʱ
		CTime time = pThread->m_LastDataTransferTime;
		time += CTimeSpan(0, 0, nConnectionTimeout, 0);
		if (time < CTime::GetCurrentTime())
		{
			pThread->m_ControlSocket.SendResponse("426 Connection timed out, aborting transfer");
			pThread->PostThreadMessage(WM_QUIT,0,0);
		}
	}
} 

/********************************************************************/
/*																	*/
/* �������� : UpdateTime											*/	
/* ����     : ���»ʱ��											*/
/*																	*/
/********************************************************************/
void CConnectThread::UpdateTime()
{
	m_LastDataTransferTime = CTime::GetCurrentTime();
}
