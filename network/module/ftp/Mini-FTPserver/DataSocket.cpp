// DataSocket.cpp : implementation file
//

#include "stdafx.h"
#include "FTPserver.h"
#include "DataSocket.h"
#include "ControlSocket.h"
#include "ConnectThread.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define PACKET_SIZE 4096

/////////////////////////////////////////////////////////////////////////////
// CDataSocket

CDataSocket::CDataSocket(CControlSocket *pSocket)
{
	m_pControlSocket = pSocket;
	m_strListing = "";
	m_File.m_hFile = NULL;
	m_bConnected = FALSE;

	m_nTotalBytesSend = 0;
	m_nTotalBytesTransfered = 0;
}

CDataSocket::~CDataSocket()
{
}


// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(CDataSocket, CAsyncSocket)
	//{{AFX_MSG_MAP(CDataSocket)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0

/////////////////////////////////////////////////////////////////////////////
// CDataSocket member functions

/********************************************************************/
/*																	*/
/* �������� : CDataSocket::SendListing								*/
/* ����     : ����Ŀ¼��Ϣ											*/
/*																	*/
/********************************************************************/
void CDataSocket::SendListing(LPCTSTR lpszListing)
{
	m_strListing = lpszListing;
	// Ҫ���͵ĳ���
	m_nTotalBytesSend = m_strListing.GetLength();
	m_nTotalBytesTransfered = 0;
	// ��ʼ����
	OnSend(0);
}

/********************************************************************/
/*																	*/
/* �������� : CDataSocket::SendFile									*/
/* ����     : �����ļ�												*/
/*																	*/
/********************************************************************/
void CDataSocket::SendFile(LPCTSTR lpszFilename)
{
	if (!PrepareSendFile(lpszFilename))
	{
		m_pControlSocket->m_nStatus = STATUS_IDLE;
		
		m_pControlSocket->SendResponse("426 Connection closed; transfer aborted.");

		AfxGetThread()->PostThreadMessage(WM_DESTROYDATACONNECTION, 0, 0);
		return;
	}
	
	// ��ʼ����
	OnSend(0);
}

/********************************************************************/
/*																	*/
/* �������� : CDataSocket::RetrieveFile								*/
/* ����     : �����ļ�												*/
/*																	*/
/********************************************************************/
void CDataSocket::RetrieveFile(LPCTSTR lpszFilename)
{
	if (!PrepareReceiveFile(lpszFilename))
	{
		m_pControlSocket->m_nStatus = STATUS_IDLE;
		
		m_pControlSocket->SendResponse("450 Can't access file.");
		
		AfxGetThread()->PostThreadMessage(WM_DESTROYDATACONNECTION, 0, 0);
		return;
	}
}

/********************************************************************/
/*																	*/
/* �������� : CDataSocket::PrepareReceiveFile						*/
/* ����     : ׼�������ļ�											*/
/*																	*/
/********************************************************************/
BOOL CDataSocket::PrepareReceiveFile(LPCTSTR lpszFilename)
{
	// �ļ��Ѿ�����ر���
	if (m_File.m_hFile != NULL)
	{
		m_File.Close();
	}
	
	// ��Ŀ���ļ�
	if (!m_File.Open(lpszFilename, CFile::modeWrite | CFile::modeCreate | CFile::modeNoTruncate | CFile::shareDenyWrite))
	{
		return FALSE;
	}
	m_nTotalBytesReceive = 0;
	m_nTotalBytesTransfered = 0;
	return TRUE;
}

/********************************************************************/
/*																	*/
/* �������� : CDataSocket::PrepareSendFile							*/
/* ����     : ׼�������ļ�											*/
/*																	*/
/********************************************************************/
BOOL CDataSocket::PrepareSendFile(LPCTSTR lpszFilename)
{
	// �ļ��Ѿ�����ر���
	if (m_File.m_hFile != NULL)
	{
		m_File.Close();
	}
	
	// ��Ŀ���ļ�
	if (!m_File.Open(lpszFilename, CFile::modeRead | CFile::shareDenyNone | CFile::typeBinary))
	{
		return FALSE;
	}
	m_nTotalBytesSend = m_File.GetLength();
	m_nTotalBytesTransfered = 0;
	return TRUE;
}

/********************************************************************/
/*																	*/
/* �������� : CDataSocket::OnAccept									*/
/* ����     : ����������Ӧ											*/
/*																	*/
/********************************************************************/
void CDataSocket::OnAccept(int nErrorCode) 
{
	// ������ʱ�����������
	CAsyncSocket tmpSocket;
	Accept(tmpSocket);
	
	// ����SOCKET
	SOCKET socket = tmpSocket.Detach();
	Close();
	
	// ����SOCKET����ǰ�������Ӷ���
	Attach(socket);
	
	// �Ѿ�����
	m_bConnected = TRUE;
	
	CAsyncSocket::OnAccept(nErrorCode);
}

/********************************************************************/
/*																	*/
/* �������� : CDataSocket::OnClose									*/
/* ����     : �ر�������Ӧ											*/
/*																	*/
/********************************************************************/
void CDataSocket::OnClose(int nErrorCode) 
{
	if (m_pControlSocket)
	{
		// ֹͣ����
		ShutDown(1);
		
		if (m_pControlSocket->m_nStatus == STATUS_UPLOAD)
		{
			while(Receive() != 0)
			{
				// ���ջ������е�����				
			}
		}
		else
		{
			m_pControlSocket->SendResponse("426 Connection closed; transfer aborted.");

			AfxGetThread()->PostThreadMessage(WM_DESTROYDATACONNECTION, 0, 0);
		}
	}
	m_pControlSocket->m_nStatus = STATUS_IDLE;	
	m_bConnected = FALSE;
	CAsyncSocket::OnClose(nErrorCode);
}

/********************************************************************/
/*																	*/
/* �������� : CDataSocket::OnConnect								*/
/* ����     : ����������Ӧ											*/
/*																	*/
/********************************************************************/
void CDataSocket::OnConnect(int nErrorCode) 
{
	m_bConnected = TRUE;
	if (nErrorCode)
	{
		m_pControlSocket->m_nStatus = STATUS_IDLE;
		
		m_pControlSocket->SendResponse("425 Can't open data connection.");

		AfxGetThread()->PostThreadMessage(WM_DESTROYDATACONNECTION, 0, 0);
	}
	CAsyncSocket::OnConnect(nErrorCode);
}

/********************************************************************/
/*																	*/
/* �������� : CDataSocket::OnReceive								*/
/* ����     : ����������Ӧ											*/
/*																	*/
/********************************************************************/
void CDataSocket::OnReceive(int nErrorCode) 
{
	CAsyncSocket::OnReceive(nErrorCode);
	Receive();
}

/********************************************************************/
/*																	*/
/* �������� : CDataSocket::OnSend									*/
/* ����     : ����������Ӧ											*/
/*																	*/
/********************************************************************/
void CDataSocket::OnSend(int nErrorCode) 
{
	CAsyncSocket::OnSend(nErrorCode);

	switch(m_pControlSocket->m_nStatus)
	{
		case STATUS_LIST:
		{
			while (m_nTotalBytesTransfered < m_nTotalBytesSend)
			{
				DWORD dwRead;
				int dwBytes;

				CString strDataBlock;
				
				dwRead = m_strListing.GetLength();
				
				if (dwRead <= PACKET_SIZE)
				{
					strDataBlock = m_strListing;
				}
				else
				{
					strDataBlock = m_strListing.Left(PACKET_SIZE);
					dwRead = strDataBlock.GetLength();
				}
				
				if ((dwBytes = Send(strDataBlock, dwRead)) == SOCKET_ERROR)
				{
					if (GetLastError() == WSAEWOULDBLOCK) 
					{
						Sleep(0);
						return;
					}
					else
					{
						TCHAR szError[256];
						wsprintf(szError, "Server Socket failed to send: %d", GetLastError());

						Close();

						m_nTotalBytesSend = 0;
						m_nTotalBytesTransfered = 0;

						m_pControlSocket->m_nStatus = STATUS_IDLE;

						m_pControlSocket->SendResponse("426 Connection closed; transfer aborted.");

						AfxGetThread()->PostThreadMessage(WM_DESTROYDATACONNECTION, 0, 0);
					}
				}
				else
				{
					m_nTotalBytesTransfered += dwBytes;
					m_strListing = m_strListing.Mid(dwBytes);
				}
			}
			if (m_nTotalBytesTransfered == m_nTotalBytesSend)
			{
				Close();

				m_nTotalBytesSend = 0;
				m_nTotalBytesTransfered = 0;

				m_pControlSocket->m_nStatus = STATUS_IDLE;

				m_pControlSocket->SendResponse("226 Transfer complete");

				AfxGetThread()->PostThreadMessage(WM_DESTROYDATACONNECTION, 0, 0);
			}
			break;
		}

		case STATUS_DOWNLOAD:
		{
			while (m_nTotalBytesTransfered < m_nTotalBytesSend)
			{
				// �����ļ�
				byte data[PACKET_SIZE];
				
				m_File.Seek(m_nTotalBytesTransfered, CFile::begin);

				DWORD dwRead = m_File.Read(data, PACKET_SIZE);
    
				int dwBytes;

				if ((dwBytes = Send(data, dwRead)) == SOCKET_ERROR)
				{
					if (GetLastError() == WSAEWOULDBLOCK) 
					{
						Sleep(0);
						break;
					}
					else
					{
						TCHAR szError[256];
						wsprintf(szError, "Server Socket failed to send: %d", GetLastError());

						m_File.Close();
						m_File.m_hFile = NULL;

						Close();

						m_nTotalBytesSend = 0;
						m_nTotalBytesTransfered = 0;

						m_pControlSocket->m_nStatus = STATUS_IDLE;

						m_pControlSocket->SendResponse("426 Connection closed; transfer aborted.");

						AfxGetThread()->PostThreadMessage(WM_DESTROYDATACONNECTION, 0, 0);
					}
				}
				else
				{
					m_nTotalBytesTransfered += dwBytes;

					// ���»ʱ��
					((CConnectThread *)AfxGetThread())->UpdateTime();
				}
			}
			if (m_nTotalBytesTransfered == m_nTotalBytesSend)
			{
                m_File.Close();
				m_File.m_hFile = NULL;

                Close();

                m_nTotalBytesSend = 0;
                m_nTotalBytesTransfered = 0;


				m_pControlSocket->m_nStatus = STATUS_IDLE;

				m_pControlSocket->SendResponse("226 Transfer complete");

				AfxGetThread()->PostThreadMessage(WM_DESTROYDATACONNECTION, 0, 0);
			}
			break;
		}
	}
}

/********************************************************************/
/*																	*/
/* �������� : CDataSocket::Receive									*/
/* ����     : ���ǽ������ݺ���										*/
/*																	*/
/********************************************************************/
int CDataSocket::Receive()
{
	int nRead = 0;
	
	// �ϴ�״̬
	if (m_pControlSocket->m_nStatus == STATUS_UPLOAD)
	{
		if (m_File.m_hFile == NULL)
			return 0;
		
		byte data[PACKET_SIZE];
		nRead = CAsyncSocket::Receive(data, PACKET_SIZE);
		
		switch(nRead)
		{
		case 0:
			{
				m_File.Close();
				m_File.m_hFile = NULL;
				Close();

				m_pControlSocket->SendResponse("226 Transfer complete");

				AfxGetThread()->PostThreadMessage(WM_DESTROYDATACONNECTION, 0, 0);
				break;
			}
		case SOCKET_ERROR:
			{
				if (GetLastError() != WSAEWOULDBLOCK)
				{
					m_File.Close();
					m_File.m_hFile = NULL;
					Close();
					m_pControlSocket->SendResponse("426 Connection closed; transfer aborted.");

					AfxGetThread()->PostThreadMessage(WM_DESTROYDATACONNECTION, 0, 0);
				}
				break;
			}
		default:
			{
				// ���»ʱ��
				((CConnectThread *)AfxGetThread())->UpdateTime();
				TRY
				{
					m_File.Write(data, nRead);
				}
				CATCH_ALL(e)
				{
					m_File.Close();
					m_File.m_hFile = NULL;
					Close();
					m_pControlSocket->SendResponse("450 Can't access file.");

					AfxGetThread()->PostThreadMessage(WM_DESTROYDATACONNECTION, 0, 0);
					return 0;
				}
				END_CATCH_ALL;
				break;
			}
		}
	}
	return nRead;
}
