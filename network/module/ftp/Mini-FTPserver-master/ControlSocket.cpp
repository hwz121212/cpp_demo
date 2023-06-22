// ControlSocket.cpp : implementation file
//

#include "stdafx.h"
#include "FTPserver.h"
#include "FTPserverDlg.h"
#include "ControlSocket.h"
#include "ConnectThread.h"
#include "DataSocket.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CControlSocket

CControlSocket::CControlSocket()
{
	m_nStatus = STATUS_LOGIN;
	m_pDataSocket = NULL;
	m_strRemoteHost = "";
	m_nRemotePort = -1;
	m_bPassiveMode = FALSE;
}

CControlSocket::~CControlSocket()
{
}


// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(CControlSocket, CSocket)
	//{{AFX_MSG_MAP(CControlSocket)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0

/////////////////////////////////////////////////////////////////////////////
// CControlSocket member functions

/********************************************************************/
/*																	*/
/* �������� : CControlSocket::SendResponse							*/
/* ����     : ������Ϣ���ͻ��� 										*/
/*																	*/
/********************************************************************/
BOOL CControlSocket::SendResponse(LPCTSTR pstrFormat, ...)
{
	CString str;
	
	// ��ʽ��������Ϣ
	va_list args;
	va_start(args, pstrFormat);
	str.FormatV(pstrFormat, args);
	
	// ������Ϣ
	int nBytes = CSocket::Send(str + "\r\n", str.GetLength()+2);
	// ����ʧ��
	if (nBytes == SOCKET_ERROR)
	{
		Close();	
		
		// ֪ͨ�̹߳ر�
		m_pThread->PostThreadMessage(WM_QUIT, 1, 0);
		
		return FALSE;
	}

	// ���»ʱ��
	((CConnectThread *)AfxGetThread())->UpdateTime();

	return TRUE;
}

/********************************************************************/
/*																	*/
/* �������� : CControlSocket::OnClose								*/
/* ����     : ���׽��ֱ��ر�ʱ�����߳�								*/
/*																	*/
/********************************************************************/
void CControlSocket::OnClose(int nErrorCode) 
{
	Close();
	// �����߳�
	m_pThread->PostThreadMessage(WM_QUIT, 0, 0);
	CSocket::OnClose(nErrorCode);
}

#define BUFFERSIZE 4096

/********************************************************************/
/*																	*/
/* �������� : CControlSocket::OnReceive								*/
/* ����     : �������ݽ��뻺����ʱ����								*/
/*																	*/
/********************************************************************/
void CControlSocket::OnReceive(int nErrorCode) 
{

	TCHAR buff[BUFFERSIZE+1];
	
	int nRead = Receive(buff, BUFFERSIZE);
	switch (nRead)
	{
	case 0:
		Close();
		break;
		
	case SOCKET_ERROR:
		if (GetLastError() != WSAEWOULDBLOCK) 
		{
			TCHAR szError[256];
			wsprintf(szError, "OnReceive error: %d", GetLastError());
			//AfxMessageBox (szError);
		}
		break;
		
	default:
		if (nRead != SOCKET_ERROR && nRead != 0)
		{		
			buff[nRead] = 0;
			m_RxBuffer += CString(buff);
			
			GetCommandLine();
		}
		break;
	}
	CSocket::OnReceive(nErrorCode);
}

/********************************************************************/
/*																	*/
/* �������� : CControlSocket::GetCommandLine						*/
/* ����     : �����������������Ϣ									*/
/*																	*/
/********************************************************************/
void CControlSocket::GetCommandLine()
{
	CString strTemp;
	int nIndex;
	
	while(!m_RxBuffer.IsEmpty())
	{
		nIndex = m_RxBuffer.Find("\r\n");
		if (nIndex != -1)
		{
			strTemp = m_RxBuffer.Left(nIndex);
			m_RxBuffer = m_RxBuffer.Mid(nIndex + 2);
			if (!strTemp.IsEmpty())
			{
				m_strCommands.AddTail(strTemp);
				// ��������
				ProcessCommand();
			}
		}
		else
		{
			break;
		}
	}
}

/********************************************************************/
/*																	*/
/* �������� : CControlSocket::ProcessCommand						*/
/* ����     : ���������м�����										*/
/*																	*/
/********************************************************************/
void CControlSocket::ProcessCommand()
{
	CString strCommand, strArguments;

	// ��ȡ�����
	CString strBuff = m_strCommands.RemoveHead();
	int nIndex = strBuff.Find(" ");
	if (nIndex == -1)
	{
		strCommand = strBuff;
	}
	else
	{
		strCommand = strBuff.Left(nIndex);
		strArguments = strBuff.Mid(nIndex+1);
	}
	strCommand.MakeUpper();

	// �����ж��������
	if (strCommand.Right(4) == "ABOR")
	{
		strCommand = "ABOR";
	}

	// ��½û�����˺ż�����
	if ((m_nStatus == STATUS_LOGIN) && (strCommand != "USER" && strCommand != "PASS"))
	{
		SendResponse("530 Please login with USER and PASS.");
		return;
	}
	
	// �����˺�
	if (strCommand == "USER")
	{
		m_nStatus = STATUS_LOGIN;
		m_strUserName = strArguments;

		CString strPeerAddress;
		UINT nPeerPort;
		GetPeerName(strPeerAddress, nPeerPort);

		SendResponse("331 Please specify the password.");
	}
	else
	// ��������
	if (strCommand == "PASS")
	{
		// �˺Ų���Ϊ��
		if (m_strUserName.IsEmpty())
		{
			SendResponse("503 Login with USER first.");
		}
		else
		{
			// ����û���
			if (!m_strUserName.CompareNoCase(pWnd->m_strName))
			{
				// �������
				if (!strArguments.Compare(pWnd->m_strPassword) || strArguments.IsEmpty() || !m_strUserName.CompareNoCase("anonymous"))
				{
					// ������Ŀ¼
					m_strCurrentDir = "/";

					// ���سɹ���½
					m_nStatus = STATUS_IDLE;
					SendResponse("230 Login successful.");
					return;
				}
			}
			SendResponse("530 Not logged in, user or password incorrect!");
		}
	}		
	else
	// �ر�����
	if ((strCommand == "QUIT") || (strCommand == "BYE"))
	{
		// ���عر�
		CConnectThread *pThread = (CConnectThread *)m_pThread;
		SendResponse("220 %s", ((CFTPserverDlg *)pThread->m_pWndServer)->GetGoodbyeMessage());

		Close();

		// �ر������߳�
		m_pThread->PostThreadMessage(WM_QUIT, 1, 0);
	}
	else
	// �ı䴫������
	if (strCommand == "TYPE")
	{
		// ���ش�������
		SendResponse("200 Type set to %s", strArguments);
	}
	else
	// ���ص�ǰĿ¼
	if (strCommand == "PWD")
	{
		SendResponse("257 \"%s\" is current directory.", m_strCurrentDir);
	}
	else
	// �����ϼ�Ŀ¼
	if (strCommand == "CDUP")
	{
		DoChangeDirectory("..");
	}
	else
	// �ı乤��Ŀ¼
	if (strCommand == "CWD")
	{
		DoChangeDirectory(strArguments);
	}
	else
	// PORTģʽ��ת��IP��2�ֽڵĶ˿ڵ�ַΪ��׼��ַ
	// (PORT a1,a2,a3,a4,p1,p2) -> a1.a2.a3.a4, p1*256+p2
	if (strCommand == "PORT")
	{
		CString strSub;
		int nCount=0;

		while (AfxExtractSubString(strSub, strArguments, nCount++, ','))
		{
			switch(nCount)
			{
				case 1:	// a1
					m_strRemoteHost = strSub;
					m_strRemoteHost += ".";
					break;
				case 2:	// a2
					m_strRemoteHost += strSub;
					m_strRemoteHost += ".";
					break;
				case 3:	// a3
					m_strRemoteHost += strSub;
					m_strRemoteHost += ".";
					break;
				case 4:	// a4
					m_strRemoteHost += strSub;
					break;
				case 5:	// p1
					m_nRemotePort = 256*atoi(strSub);
					break;
				case 6:	// p2
					m_nRemotePort += atoi(strSub);
					break;
			}
		}
		SendResponse("200 Port command successful.");
	}
	else
	// PASVģʽ
	if (strCommand == "PASV")
	{	
		// ɾ���Ѿ����ڵ���������
		DestroyDataConnection();

		// �����µ�����SOCKET
		m_pDataSocket = new CDataSocket(this);

		if (!m_pDataSocket->Create())
		{
			DestroyDataConnection();	
			SendResponse("421 Failed to create socket.");
			return;
		}
		// ��ʼ����
		m_pDataSocket->Listen();
		m_pDataSocket->AsyncSelect();
		
		CString strIPAddress, strTmp;
		UINT nPort;
		
		// ��ȡ����IP
		GetSockName(strIPAddress, nPort);
		// ��ȡ�����˿�
		m_pDataSocket->GetSockName(strTmp, nPort);
		// ��'.'����','
		strIPAddress.Replace(".", ",");
		// ����PASV������Ϣ
		SendResponse("227 Entering Passive Mode (%s,%d,%d).", strIPAddress, nPort/256, nPort%256);
		
		m_bPassiveMode = TRUE;
	} 
	else
	// �г���ǰĿ¼�ļ�
	if (strCommand == "LIST")
	{
		CString strListing;
		if (!GetDirectoryList(strArguments, strListing))
		{
			// something went wrong
			return;
		}

		SendResponse("150 Opening ASCII mode data connection for directory list."); 

		// ������������
		if (CreateDataConnection())
		{
			if (strListing.IsEmpty())
			{
				// �ر���������
				DestroyDataConnection();

				SendResponse("226 Transfer complete."); 
				m_nStatus = STATUS_IDLE;
				return;
			}
		}
		else
		{
			// �ر���������
			DestroyDataConnection();
			return;
		}

		m_nStatus = STATUS_LIST;
		
		// ����Ŀ¼��Ϣ
		m_pDataSocket->SendListing(strListing);
	} 
	else
	// �����ļ�
	if (strCommand == "RETR")
	{
		CString strResult;
		int nResult = CheckFileName(strArguments, FTP_DOWNLOAD, strResult);
		m_nStatus = STATUS_DOWNLOAD;
		switch(nResult)
		{
			case ERROR_ACCESS_DENIED:
				SendResponse("550 Permission denied.");
				break;
			case ERROR_FILE_NOT_FOUND:
				SendResponse("550 File not found.");
				break;
			default:
				SendResponse("150 Opening BINARY mode data connection for file transfer.");

				// �����µ��������������ļ�����
				if (!CreateDataConnection())
				{
					m_nStatus = STATUS_IDLE;
					// ����ر���������
					DestroyDataConnection();
				}
				else
				{
					//m_nStatus = STATUS_DOWNLOAD;
					// �����ļ�
					m_pDataSocket->SendFile(strResult);
				}
				break;
		}
	}
	else
	// �ϴ��ļ�
	if (strCommand == "STOR")
	{
		CString strResult;
		int nResult = CheckFileName(strArguments, FTP_UPLOAD, strResult);
		m_nStatus = STATUS_UPLOAD;
		switch(nResult)
		{
			case ERROR_ACCESS_DENIED:
				SendResponse("550 Permission denied.");
				break;
			case ERROR_FILE_NOT_FOUND:
				SendResponse("550 Filename invalid.");
				break;
			default:
				SendResponse("150 Opening BINARY mode data connection for file transfer.");

				// �����µ��������������ļ�����
				if (!CreateDataConnection())
				{
					m_nStatus = STATUS_IDLE;
					// ����ر���������
					DestroyDataConnection();
				}
				else
				{
					//m_nStatus = STATUS_UPLOAD;
					// �����ļ�
					m_pDataSocket->RetrieveFile(strResult);
				}
				break;
		}
	}
	else
	// �õ��ļ���С
	if (strCommand == "SIZE")
	{
		CString strResult;
		int nResult = CheckFileName(strArguments, FTP_DOWNLOAD, strResult);
		switch(nResult)
		{
			case ERROR_ACCESS_DENIED:
				SendResponse("550 Permission denied.");
				break;
			case ERROR_FILE_NOT_FOUND:
				SendResponse("550 File not found.");
				break;
			default:
			{
				CFileStatus status;
				CFile::GetStatus(strResult, status);
				SendResponse("213 %d", status.m_size);
				break;
			}
		}
	}
	else
	// ɾ���ļ�
	if (strCommand == "DELE")
	{
		CString strResult;
		int nResult = CheckFileName(strArguments, FTP_DELETE, strResult);
		switch(nResult)
		{
			case ERROR_ACCESS_DENIED:
				SendResponse("550 Permission denied.");
				break;
			case ERROR_FILE_NOT_FOUND:
				SendResponse("550 File not found.");
				break;
			default:
				CString strRelativePath;
				GetRelativePath(strResult, strRelativePath);

				// ɾ���ļ�
				if (!DeleteFile(strResult))
				{
					SendResponse("450 Internal error deleting the file: \"%s\".",  strRelativePath);
				}
				else
				{
					SendResponse("250 File \"%s\" was deleted successfully.", strRelativePath);
				}
				break;
		}
	}
	else
	// �ļ�����from
	if (strCommand == "RNFR")
	{
		int nResult = 0; 
		// ����ļ�
		nResult = CheckFileName(strArguments, FTP_RENAME, strReName);
		if (nResult != ERROR_SUCCESS)
		{
			// ���Ŀ¼
			nResult = CheckDirectory(strArguments,FTP_RENAME,strReName);
			if (nResult != ERROR_SUCCESS)
			{
				switch(nResult)
				{
				case ERROR_ACCESS_DENIED:
					SendResponse("550 Permission denied.");
					break;
				case ERROR_PATH_NOT_FOUND:
					SendResponse("550 Directory not found.");
					break;
				case ERROR_FILE_NOT_FOUND:
					SendResponse("550 File not found.");
					break;
				}
			}
			else
			{
				SendResponse("350 Directory exists, ready for destination name.");
			}	
		}
		else
		{
			SendResponse("350 File exists, ready for destination name.");
		}
	}
	else
	// �ļ�����to
	if (strCommand == "RNTO")
	{
		CString strResult;
		CheckFileName(strArguments, FTP_RENAME, strResult);
		// �ƶ��ļ�
		if (!MoveFile(strReName,strResult))
		{
			SendResponse("450 Internal error renamed the file.");
		}
		else
		{
			SendResponse("250 renamed successfully.");
		}
	}
	else
	// ɾ��Ŀ¼
	if (strCommand == "RMD")
	{		
		CString strResult;
		int nResult = CheckDirectory(strArguments, FTP_DELETE, strResult);
		switch(nResult)
		{
			case ERROR_ACCESS_DENIED:
				SendResponse("550 Permission denied.");
				break;
			case ERROR_PATH_NOT_FOUND:
				SendResponse("550 Directory not found.");
				break;
			default:
				// ɾ��Ŀ¼
				if (!RemoveDirectory(strResult))
				{
					if (GetLastError() == ERROR_DIR_NOT_EMPTY)
					{
						SendResponse("550 Directory not empty.");
					}
					else
					{
						SendResponse("450 Internal error deleting the directory.");
					}
				}
				else
				{
					SendResponse("250 Directory deleted successfully.");
				}
				break;
		}
	}
	else
	// ����Ŀ¼
	if (strCommand == "MKD")
	{		
		CString strResult;
		int nResult = CheckDirectory(strArguments, FTP_CREATE_DIR, strResult);
		switch(nResult)
		{
			case ERROR_SUCCESS:
				SendResponse("550 Directory already exists.");
				break;
			case ERROR_ACCESS_DENIED:
				SendResponse("550 Can't create directory. Permission denied.");
				break;
			default:
				// ����Ŀ¼
				if (!MakeSureDirectoryPathExists(strResult))
				{
					SendResponse("450 Internal error creating the directory.");
				}
				else
				{
					SendResponse("250 Directory created successfully.");
				}
				break;
		}
	}
	else
	// �ж���������
	if (strCommand == "ABOR")
	{		
		if (m_pDataSocket)
		{
			if (m_nStatus != STATUS_IDLE)
			{
				SendResponse("426 Data connection closed.");
			}
			// �ر������߳�
			m_pThread->PostThreadMessage(WM_DESTROYDATACONNECTION, 0, 0);
		}
		SendResponse("226 ABOR command successful.");
	} 
	else
	// ���ϵͳ��Ϣ
	if (strCommand == "SYST")
	{
		SendResponse("215 UNIX Slyar FTPserver");
	}
	else
	// �޶���
	if (strCommand == "NOOP")
	{
		SendResponse("200 OK");
	}
	else
	{
		SendResponse("502 Command not implemented.");
	}
}

/********************************************************************/
/*																	*/
/* �������� : CControlSocket::DoChangeDirectory						*/
/* ����     : �л���ָ��Ŀ¼										*/
/*																	*/
/********************************************************************/
void CControlSocket::DoChangeDirectory(LPCTSTR lpszDirectory)
{
	CString strLocalPath;
	int nResult = CheckDirectory(lpszDirectory, FTP_LIST, strLocalPath);
	switch(nResult)
	{
	case ERROR_ACCESS_DENIED:
		// ��Ȩ�޷���
		SendResponse("550 \"%s\": Permission denied.", lpszDirectory);
		break;
	case ERROR_PATH_NOT_FOUND:
		// Ŀ¼������
		SendResponse("550 \"%s\": Directory not found.", lpszDirectory);
		break;
	default:
		// �ɹ�
		CString strRelativePath;
		if (GetRelativePath(strLocalPath, strRelativePath))
		{
			m_strCurrentDir = strRelativePath;
		}
		SendResponse("250 \"%s\" is current directory.", m_strCurrentDir);
		break;
	}
}

/********************************************************************/
/*																	*/
/* �������� : CControlSocket::CheckDirectory						*/
/* ����     : ���Ŀ¼�Ƿ�������û��Ƿ���Ȩ����					*/
/*																	*/
/********************************************************************/
int CControlSocket::CheckDirectory(CString strDirectory, int nOption, CString &strResult)
{
	// ����UNIX��ʽ
	strDirectory.Replace("\\","/");
	while(strDirectory.Replace("//","/"));
	strDirectory.TrimRight("/");
	
	if (strDirectory.IsEmpty())
	{
		if (nOption == FTP_LIST)
		{
			strDirectory = "/";
		}
		else
		{
			// Ŀ¼������
			return ERROR_PATH_NOT_FOUND;
		}
	}
	else
	{
		// ���ǵ�ǰĿ¼
		if (strDirectory.Left(1) != "/")
		{ 
			// ·����Ϊ��ǰ·�������·��
			if (m_strCurrentDir.Right(1) != '/')
				strDirectory = m_strCurrentDir + "/" + strDirectory;
			else
				strDirectory = m_strCurrentDir + strDirectory;
		}
	}
	
	// �ж�Ŀ¼�Ƿ����
	BOOL bPathExists = GetLocalPath(strDirectory, strResult);
	
	// ·���������Ҳ������Ǵ����ļ���
	if (!bPathExists && (nOption != FTP_CREATE_DIR))
	{ 
		return ERROR_PATH_NOT_FOUND;
	}
	
	// �ж��û��Ƿ���Ȩ�޷���
	if (!CheckAccessRights(strResult, nOption))
	{
		// ��Ȩ��
		return ERROR_ACCESS_DENIED;
	}
	
	// ·��������
	if (!bPathExists)
	{
		return ERROR_PATH_NOT_FOUND;
	}
	
	// �ɹ�
	return ERROR_SUCCESS;
}

/********************************************************************/
/*																	*/
/* �������� : CControlSocket::CheckFileName							*/
/* ����     : ����ļ��Ƿ�������û��Ƿ���Ȩ����					*/
/*																	*/
/********************************************************************/
int CControlSocket::CheckFileName(CString strFilename, int nOption, CString &strResult)
{
	// ����UNIX��ʽ
	strFilename.Replace("\\", "/");
	while(strFilename.Replace("//", "/"));
	strFilename.TrimRight("/");
	
	if (strFilename == "")
	{
		// �ļ���Ϊ��
		return ERROR_FILE_NOT_FOUND;
	}
	
	// ���浱ǰĿ¼
	CString strDirectory = m_strCurrentDir;
	
	// �ͻ��˸�������������·������ȡ�ļ���
	int nPos = strFilename.ReverseFind('/');
	if (nPos != -1)
	{
		strDirectory = strFilename.Left(nPos);
		if (strDirectory == "")
		{
			strDirectory = "/";
		}
		strFilename = strFilename.Mid(nPos+1);
	}
	
	// �õ�����·��
	CString strLocalPath;
	if (!GetLocalPath(strDirectory, strLocalPath))
	{
		// directory does not exist
		return ERROR_FILE_NOT_FOUND;
	}
	
	// ��������·��
	strResult = strLocalPath + "\\" + strFilename;
	
	BOOL bFileExist;
	bFileExist = FileExists(strResult, FALSE);

	// ����ļ��Ƿ����
	if ((nOption != FTP_UPLOAD) && !bFileExist)
	{
		// �ļ�������
		return ERROR_FILE_NOT_FOUND;
	}

	// �����ļ������ϴ�
	if ((nOption == FTP_UPLOAD) && bFileExist && (!pWnd->m_bAllowDelete))
	{
		// �û���Ȩ�޸���
		return ERROR_ACCESS_DENIED;
	}
	
	// ����ļ�����Ȩ��
	if (!CheckAccessRights(strLocalPath, nOption))
	{
		// �û���Ȩ�޷���
		return ERROR_ACCESS_DENIED;
	}
	
	// �ɹ�
	return ERROR_SUCCESS;
}

/********************************************************************/
/*																	*/
/* �������� : CControlSocket::CheckAccessRights						*/
/* ����     : ������Ȩ��											*/
/*																	*/
/********************************************************************/
BOOL CControlSocket::CheckAccessRights(LPCTSTR lpszDirectory, int nOption)
{
	// ��������·��
	CString strCheckDir = lpszDirectory;
	
	// һ��һ������Ƿ��ϼ�Ŀ¼��ͬ
	while(!strCheckDir.IsEmpty())
	{
		CString strPath1 = strCheckDir;
		strPath1.TrimRight("\\");
		CString strPath2 = pWnd->m_strHomeDirectory;
		strPath2.TrimRight("\\");
		
		// �ҵ�һ��ƥ�䣬ĳ���ϼ�Ŀ¼��ͬ
		if (strPath1.CompareNoCase(strPath2) == 0)
		{
			// ������Ȩ��
			if (((!pWnd->m_bAllowDownload) && (nOption == FTP_DOWNLOAD)) ||
				((!pWnd->m_bAllowUpload) && (nOption == FTP_UPLOAD)) ||
				((!pWnd->m_bAllowRename) && (nOption == FTP_RENAME)) ||
				((!pWnd->m_bAllowDelete) && (nOption == FTP_DELETE)) ||
				((!pWnd->m_bAllowCreateDirectory) && (nOption == FTP_CREATE_DIR)) ||
				((!pWnd->m_bAllowDownload && !pWnd->m_bAllowUpload) && (nOption == FTP_LIST)))
			{
				return FALSE;
			}
			return TRUE;
		}
		// �ض���һ��"\\"�������
		int nPos = strCheckDir.ReverseFind('\\');
		if (nPos != -1)
		{
			strCheckDir = strCheckDir.Left(nPos);
		}
		else
		{
			// ������ƥ�䣬���Ŀ¼���˳�ѭ��
			strCheckDir.Empty();
		}
	} 
	// �û���Ȩ�޷���
	return FALSE;
}

/********************************************************************/
/*																	*/
/* �������� : CControlSocket::DestroyDataConnection					*/
/* ����     : �ر���������											*/
/*																	*/
/********************************************************************/
void CControlSocket::DestroyDataConnection()
{
	if (!m_pDataSocket)
	{
		return;
	}
	
	delete m_pDataSocket;
	
	// ���ô�����Ϣ
	m_pDataSocket = NULL;
	m_strRemoteHost = "";
	m_nRemotePort = -1;
	m_bPassiveMode = FALSE;
}

/********************************************************************/
/*																	*/
/* �������� : CControlSocket::GetDirectoryList						*/
/* ����     : ��ȡָ��Ŀ¼���ļ��б�								*/
/*																	*/
/********************************************************************/
BOOL CControlSocket::GetDirectoryList(LPCTSTR lpszDirectory, CString &strResult)
{
	CString strDirectory = lpszDirectory;
	
	// ��ս��
	strResult = "";
	
	// ȥ��ͨ�������˼����
	strDirectory.TrimRight('*');
	
	// �û�û��ָ��·����ʹ�õ�ǰ·��
	if (strDirectory.IsEmpty())
	{
		strDirectory = m_strCurrentDir;
	}
	
	CString strLocalPath;
	int nResult = CheckDirectory(strDirectory, FTP_LIST, strLocalPath);
	switch(nResult)
	{
	case ERROR_ACCESS_DENIED:
		// �û���Ȩ�޷���
		SendResponse("550 \"%s\": Permission denied.", lpszDirectory);
		return FALSE;
		
	case ERROR_PATH_NOT_FOUND:
		// ·��������
		SendResponse("550 \"%s\": Directory not found.", lpszDirectory);
		return FALSE;
		
	default:
		// �ɹ�
		break;
	}
	
	CFileFind find;
	BOOL bFound = FALSE;
	
	// �ж����ļ������ļ���
	if ((GetFileAttributes(strLocalPath) & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY)
	{
		CString strPath = strLocalPath;
		if (strPath.Right(1)==_T("\\"))
			bFound = find.FindFile(strLocalPath + "*.*");
		else
			bFound = find.FindFile(strLocalPath + "\\*.*");
	}
	else
	{
		// �ļ�
		bFound = find.FindFile(strLocalPath);
	}
	
	while (bFound)
	{
		bFound = find.FindNextFile();
		
		// ���� "." �� ".." 
		if (find.IsDots())
			continue;
		
		// Ȩ����Ϣ
		if (find.IsDirectory())
			strResult += "drwx------";
		else
			strResult += "-rwx------";
		
		// ����Ϣ
		strResult += " 1 user group ";

		// �ļ���С
		CString strLength;
		strLength.Format("%d", find.GetLength());
		CString strFiller = "              ";
		strResult += strFiller.Left(strFiller.GetLength() - strLength.GetLength());
		strResult += strLength;

		// �ļ�ʱ��
		strResult += GetFileDate(find);

		// �ļ���
		strResult += find.GetFileName();

		// ���н���
		strResult += "\r\n";
	}
	return TRUE;
}

/********************************************************************/
/*																	*/
/* �������� : CControlSocket::CreateDataConnection					*/
/* ����     : ������������											*/
/*																	*/
/********************************************************************/
BOOL CControlSocket::CreateDataConnection()
{
	// �ж���PASV����PORTģʽ
	if (!m_bPassiveMode)
	{
		// ��Ҫʹ��PORT��������ģʽ
		if (m_strRemoteHost == "" || m_nRemotePort == -1)
		{
			SendResponse("425 Can't open data connection.");
			return FALSE;
		}
		
		m_pDataSocket = new CDataSocket(this);
		if (m_pDataSocket->Create())
		{
			m_pDataSocket->AsyncSelect();
			
			// ���ӶԶ�����
			if (m_pDataSocket->Connect(m_strRemoteHost, m_nRemotePort) == 0)
			{
				if (GetLastError() != WSAEWOULDBLOCK)
				{
					SendResponse("425 Can't open data connection.");
					return FALSE;
				}
			}
		}
		else
		{
			SendResponse("421 Failed to create data connection socket.");
			return FALSE;
		}
	}
	
	// �ȴ�����(���10s)
	DWORD dwTickCount = GetTickCount() + 10000;
	while (!m_pDataSocket->m_bConnected)
	{
		DoEvents();
		if (dwTickCount < GetTickCount())
		{
			SendResponse("421 Failed to create data connection socket.");
			return FALSE;
		}
	}
	return TRUE;
}

/********************************************************************/
/*																	*/
/* �������� : CControlSocket::GetRelativePath						*/
/* ����     : ת������·��Ϊ���·��								*/
/*																	*/
/********************************************************************/
BOOL CControlSocket::GetRelativePath(LPCTSTR lpszLocalPath, CString &strRelativePath)
{
	CString strOffset = pWnd->m_strHomeDirectory;
	CString strLocalPath = lpszLocalPath;
	
	strOffset.MakeLower();
	strLocalPath.MakeLower();
	
	if (strOffset.Right(1) != '\\')
		strOffset += "\\";
	
	if (strLocalPath.Right(1) != '\\')
		strLocalPath += "\\";
	
	if (strOffset == strLocalPath)
	{
		strRelativePath = "/";
	}
	else
	{
		strRelativePath = strLocalPath;
		strRelativePath.Replace(strOffset, "/");
		strRelativePath.Replace("\\", "/");
		strRelativePath.TrimRight('/');
	}
	return TRUE;
}

/********************************************************************/
/*																	*/
/* �������� : CControlSocket::GetLocalPath							*/
/* ����     : ��ȡ����·��											*/
/*																	*/
/********************************************************************/
BOOL CControlSocket::GetLocalPath(LPCTSTR lpszRelativePath, CString &strLocalPath)
{
	CString strRelativePath = lpszRelativePath;
	
	// ����UNIX��ʽ
	strRelativePath.Replace("\\","/");
	while(strRelativePath.Replace("//","/"));
	
	CStringList partList;
	CString strSub;
	int nCount=0;
	BOOL bPathExists = TRUE;
	
	// �ֽ�·��
	while(AfxExtractSubString(strSub, strRelativePath, nCount++, '/'))
	{
		// ȥ���������'.'
		if (strSub != "..")
		{
			strSub.TrimLeft('.');
			strSub.TrimRight('.');
		}
		
		if (!strSub.IsEmpty())
			partList.AddTail(strSub);
	}
	
	// ���Ҹ�Ŀ¼
	CString strHomeDir;
	if (lpszRelativePath[0] == '/')
	{
		// ����·��
		strHomeDir = pWnd->m_strHomeDirectory;
	}
	else
	{
		// ���·���������ݹ����
		GetLocalPath(m_strCurrentDir, strHomeDir);
	}
	
	while(!partList.IsEmpty())
	{
		CString strPart = partList.GetHead();
		partList.RemoveHead();
		
		CString strCheckDir;
		
		if (strPart == "..")
		{
			// �ص���һ��
			int nPos = strHomeDir.ReverseFind('\\');
			if (nPos != -1)
			{
				strCheckDir = strHomeDir.Left(nPos);
			}
		}
		else
		{
			strCheckDir = strHomeDir + "\\" + strPart;
		}
		
		// Ŀ¼�Ƿ����
		if (FileExists(strCheckDir, TRUE))
		{
			strHomeDir = strCheckDir;
		}
		else
		{
			// �ļ��Ƿ����
			if (FileExists(strCheckDir, FALSE))
			{
				strHomeDir = strCheckDir;
			}
			else
			{
				strHomeDir = strCheckDir;
				
				// Ŀ¼������
				bPathExists = FALSE;
			}
		}
	}
	
	// �ɹ�
	strLocalPath = strHomeDir;
	
	// ���������Ż�ԭ':\\'
	if (strLocalPath.Right(1) == ':')
		strLocalPath += "\\";
	
	return bPathExists;
}
