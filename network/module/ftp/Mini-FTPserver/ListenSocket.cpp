// ListenSocket.cpp : implementation file
//

#include "stdafx.h"
#include "FTPserver.h"
#include "FTPserverDlg.h"
#include "ListenSocket.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CListenSocket

CListenSocket::CListenSocket()
:m_pWndServer(NULL)
{
}

CListenSocket::~CListenSocket()
{
}


// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(CListenSocket, CAsyncSocket)
	//{{AFX_MSG_MAP(CListenSocket)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0

/////////////////////////////////////////////////////////////////////////////
// CListenSocket member functions

/********************************************************************/
/*																	*/
/* �������� : CListenSocket::OnAccept								*/
/* ����     : ���յ�������ʱ����									*/
/*																	*/
/********************************************************************/
void CListenSocket::OnAccept(int nErrorCode) 
{
	// ������ʱ��SOCKET
	CSocket tmpSocket;

	// ��������
	Accept(tmpSocket);

	// ʹ��SUSPENDEDģʽ�����µ��̣߳�����������ִ���߳�ǰ�����߳���Ϣ
	CConnectThread* pThread = (CConnectThread*)AfxBeginThread(RUNTIME_CLASS(CConnectThread), THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);
	if (!pThread)
	{
		tmpSocket.Close();
		TRACE("Could not create thread\n");
		return;
	}
	
	// ����߳�ָ�뵽�߳�����
	CFTPserverDlg *pWnd = (CFTPserverDlg *)m_pWndServer;
	pWnd->m_CriticalSection.Lock();
    pWnd->m_ThreadList.AddTail(pThread);

	pWnd->m_CriticalSection.Unlock();

	// ���ݴ���ָ��
	pThread->m_pWndServer = m_pWndServer;
	
	// ����SOCKET
	pThread->m_hSocket = tmpSocket.Detach();
	
	// ��������SUSPENDEDģʽ�µ��߳�
	pThread->ResumeThread();

	CAsyncSocket::OnAccept(nErrorCode);
}
