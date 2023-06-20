// ntSendMail.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdlib.h>
#include "winsock2.h"
#include "base64.h"
#include <sys/stat.h>

#include <time.h>
#include <io.h>

//winsock 2.2 library
#pragma comment(lib,"ws2_32.lib")


#define SMTP_PORT 25 // Standard port for SMTP servers
#define RESPONSE_BUFFER_SIZE 1024
#define PARA_BUF 128
#define RESPONSE_NUM  6
#define _T(x)   x

// Helper Code
struct response_code
{
	UINT nResponse;		// Response we're looking for
	TCHAR* sMessage;	// Error message if we don't get it
};

enum eResponse
	{
		GENERIC_SUCCESS = 0,
		AUTHLOGIN_SUCCESS,
		AUTH_SUCCESS,
		CONNECT_SUCCESS,
		DATA_SUCCESS,
		QUIT_SUCCESS,
		LAST_RESPONSE
	};

TCHAR *response_buf;
	static response_code response_table[RESPONSE_NUM]=
	{
		//GENERIC_SUCCESS
		{250, _T("SMTP 服务器错误")},
		//AUTHLOGIN_SUCCESS
		{334, _T("SMTP 验证连接错误")},
		//AUTH_SUCCESS
		{235, _T("SMTP 用户名/密码验证错误")},
		//CONNECT_SUCCESS
		{220, _T("SMTP 服务器不可用")},
		//DATA_SUCCESS
		{354, _T("SMTP 服务器不能接收数据")},
		//QUIT_SUCCESS
		{221, _T("SMTP 没有中止会话")}
		
	};

typedef struct _tagCONFIG_INFO {
		char m_szServerName[PARA_BUF];
		char m_nPort[1];
		char m_nNeedAuth[1];
		char m_szAccount[PARA_BUF];
		char m_szPassword[PARA_BUF];
		char m_szFrom[PARA_BUF];
		char m_szTo[PARA_BUF];
		char m_szUsername[PARA_BUF];
		char m_szFilename[PARA_BUF*2];
		char m_szSubject[PARA_BUF];
		char m_szBody[1024];
}CONFIG_INFO, * LPCONFIG_INFO;

SOCKET sconnection;
CONFIG_INFO ci;


BOOL AuthorizationLogin();
BOOL GetResponse(UINT response_expected);
BOOL SendMail();


int main(int argc, char* argv[])
{
	WSADATA	wsock;
	SOCKADDR_IN          serAddr;
	int                  nRet=0;
	//初始化Winsock 2.2
	
	printf("\nInitialising Winsock...\n");
	if (WSAStartup(MAKEWORD(2,2),&wsock) != 0)
	{
		fprintf(stderr,"WSAStartup() failed %d\n, WSAGetLastError()");
        exit(0); 
	} 
	printf("Initialised successfully.\n");
	
	//创建监听socket
	printf("\nCreating TCP Socket...\n");
	if ((sconnection = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) 
		== INVALID_SOCKET)
	{
		printf("Creation of socket failed %d\n", WSAGetLastError());
		WSACleanup();
		return 0;
	}
	printf("TCP Socket Created successfully.\n");
	
	ZeroMemory(&ci,sizeof(CONFIG_INFO));
	//邮件服务器
//	printf("Enter mail server name: ");
//	sprintf(ci.m_szServerName,"smtp.126.com");
//	sprintf(ci.m_szServerName,"smtp.sohu.com");
//	sprintf(ci.m_szServerName,"smtp.sina.com.cn");
	printf("Please enter smtp Server Name:");
	gets(ci.m_szServerName);
	//设置SOCKADDR_IN地址结构
	serAddr.sin_family = AF_INET;
	serAddr.sin_port = htons(SMTP_PORT);
	//From CAsyncSocket::Connect
	serAddr.sin_addr.s_addr = inet_addr(ci.m_szServerName);	
	if (serAddr.sin_addr.s_addr == INADDR_NONE)
	{
		LPHOSTENT lphost;
		lphost = gethostbyname(ci.m_szServerName);
		if (lphost != NULL)
			serAddr.sin_addr.s_addr = ((LPIN_ADDR)lphost->h_addr)->s_addr;
		else
		{
			WSASetLastError(WSAEINVAL);
			return 0;
		}
	}
	//连接服务端
	printf("Connecting to %s:%d...\n",
		inet_ntoa(serAddr.sin_addr), htons(serAddr.sin_port));
	if (connect(sconnection, (SOCKADDR *) &serAddr, sizeof(serAddr)) 
		== SOCKET_ERROR)
	{
		printf("connection failed with error %d\n", WSAGetLastError());
		closesocket(sconnection);
		WSACleanup();
		return 0;
	} 
	printf("connection successfully.\n");

	response_buf = new TCHAR[RESPONSE_BUFFER_SIZE];


	if(!GetResponse(CONNECT_SUCCESS))
	{
		closesocket(sconnection);
		delete response_buf;
		response_buf = NULL;
		return FALSE;
	}
	
	//发送“EHLO %SMTP-Server Name%”：
	char szHello[PARA_BUF];
	sprintf(szHello,_T("EHLO %s\r\n"),ci.m_szServerName);
	send(sconnection,szHello,strlen(szHello),0);
	if(!GetResponse(GENERIC_SUCCESS))
	{
		printf("Cann't connect the server!\n");
		closesocket(sconnection);
		delete response_buf;
		response_buf = NULL;
		return FALSE;
	}
	
	//用户名、密码
	printf("Enter Account name: ");
	gets(ci.m_szAccount);

	printf("Enter passwd: ");
	gets(ci.m_szPassword);

	printf("Enter Sender Email: ");
	gets(ci.m_szFrom);

	printf("Enter Subject: ");
	gets(ci.m_szSubject);

	printf("Enter Receiver Email: ");
	gets(ci.m_szTo);

	printf("Enter User name: ");
	gets(ci.m_szUsername);

	printf("Enter Email Content: ");
	gets(ci.m_szBody);

	printf("Enter Attachment name: ");
	gets(ci.m_szFilename);

/*	sprintf(ci.m_szAccount,"ntsmtp");
	sprintf(ci.m_szPassword,"nt123456");
	sprintf(ci.m_szFrom,"ntsmtp@sohu.com");
	sprintf(ci.m_szSubject,"hello,good!");
	sprintf(ci.m_szTo,"ntsmtp@sina.com.cn");
	sprintf(ci.m_szUsername,"BaBy");
	sprintf(ci.m_szBody,"I love you!");
	sprintf(ci.m_szFilename,"D:\\pratice\\ntSendMail\\StdAfx.h");
*/	
	if(AuthorizationLogin())
	{
		if(SendMail())
			printf("Send ok!\n");
	}


	printf("Closing the connection.\n");
	
	closesocket(sconnection);
	WSACleanup();
	return 0;
}



//****************************************
// 函数功能：服务器验证
//****************************************
BOOL AuthorizationLogin()
{
	char* szAccount;
	char* szPassword;
	printf("Authing......\n ");
	//发送“AUTH LOGIN”：
	send(sconnection,_T("AUTH LOGIN\r\n"), strlen(_T("AUTH LOGIN\r\n")),0);
	
	if(!GetResponse(AUTHLOGIN_SUCCESS))
	{
		closesocket(sconnection);
		delete response_buf;
		response_buf = NULL;
		return FALSE;
	}

	//发送经过Base64编码的用户帐号：
	base64_encode(ci.m_szAccount,strlen(ci.m_szAccount),&szAccount);
//	char buf[200];
//	base64_decode(szAccount,buf);
	//注意添加回车换行
	strcat(szAccount,"\r\n");
	send(sconnection,szAccount,strlen(szAccount),0);

	if(!GetResponse(AUTHLOGIN_SUCCESS))
	{
		closesocket(sconnection);
		delete response_buf;
		response_buf = NULL;
		return FALSE;
	}	
	//发送经过Base64编码的用户密码
	base64_encode(ci.m_szPassword,strlen(ci.m_szPassword),&szPassword);
	strcat(szPassword,"\r\n");
	send(sconnection,szPassword,strlen(szPassword),0);
	if(!GetResponse(AUTH_SUCCESS))
	{
		closesocket(sconnection);
		delete response_buf;
		response_buf = NULL;
		return FALSE;
	}
	
	return TRUE;
}

//****************************************
// 函数功能：获得服务器返回信息
//****************************************
BOOL GetResponse(UINT response_expected)
{	
	char szResponse[RESPONSE_BUFFER_SIZE];
	UINT response;
	response_code *pResp;
	
	if(recv(sconnection,response_buf, RESPONSE_BUFFER_SIZE,0) == SOCKET_ERROR)
	{
		return FALSE;
	}

	strncpy(szResponse,response_buf,3);
	sscanf(szResponse, _T("%d"), &response);

	pResp = &response_table[response_expected];
	if(response != pResp->nResponse)
	{
		printf(_T("%d:%s"), response, (LPCTSTR)pResp->sMessage);
		return FALSE;
	}
	
	return TRUE;
}

//****************************************
// 函数功能：生成并传输邮件内容
//****************************************
BOOL SendMail()
{
	char szFrom[PARA_BUF]={0},szTo[PARA_BUF]={0},szSubject[PARA_BUF]={0},szBodyHead[PARA_BUF*10]={0};
	char szBody[PARA_BUF*20]={0},szContent[PARA_BUF*80]={0},szAttachment[PARA_BUF*10]={0},szTemp[PARA_BUF*50]={0};
	char szDateTime[PARA_BUF]={0},szFileBuf[1024*5]={0};

	printf("Sending Mail......\n ");
	//发送Header中的“MAIL FROM”
	sprintf(szFrom,_T("MAIL FROM:<%s>\r\n"),ci.m_szFrom);
	
	send(sconnection,szFrom,strlen(szFrom),0);
	if(!GetResponse(GENERIC_SUCCESS))
	{
		closesocket(sconnection);
		delete response_buf;
		response_buf = NULL;
		return FALSE;
	}

	//发送Header中的“RCPT TO”	
	sprintf(szTo,_T("RCPT TO:<%s>\r\n"),ci.m_szTo);
	send(sconnection,szTo,strlen(szTo),0);
	
	
	if(!GetResponse(GENERIC_SUCCESS))
	{
		closesocket(sconnection);
		delete response_buf;
		response_buf = NULL;
		return FALSE;
	}

	//发送“DATA\r\n”
	send(sconnection,"DATA\r\n",strlen("DATA\r\n"),0);
	if(!GetResponse(DATA_SUCCESS))
	{
		closesocket(sconnection);
		delete response_buf;
		response_buf = NULL;
		return FALSE;
	}	
	//以下进行 Body 的处理
	sprintf(szFrom,_T("FROM:%s<%s>\r\n"),ci.m_szUsername,ci.m_szFrom);
	sprintf(szTo,_T("TO:<%s>\r\n"),ci.m_szTo);
	time_t ltime=time(NULL);
	_tzset();
	sprintf(szDateTime,"Date:%s",ctime(&ltime));

	sprintf(szSubject,_T("Subject:%s\r\n"),ci.m_szSubject);
	
	sprintf(szBodyHead,_T("X-Mailer: ntSmtp [ch]\r\n"));
	strcat(szBodyHead,"MIME_Version:1.0\r\n");
	strcat(szBodyHead,"Content-type:multipart/mixed;Boundary=ntSmtp\r\n\r\n");
	strcat(szBodyHead,"--ntSmtp\r\n");
	strcat(szBodyHead,"Content-type:text/plain;Charset=gb2312\r\n");
	strcat(szBodyHead,"Content-Transfer-Encoding:8bit\r\n\r\n");
	
	sprintf(szBody,_T("%s\r\n\r\n"),ci.m_szBody);

	//以下进行附件的处理
	if(!_access(ci.m_szFilename,0))
	{
		sprintf(szTemp,"--ntSmtp\r\n");
		strcat(szTemp,"Content-Type:application/octet-stream;Name=%s\r\n");
		strcat(szTemp,"Content-Disposition:attachment;FileName=%s\r\n");
		strcat(szTemp,"Content-Transfer-Encoding:Base64\r\n\r\n");
		strcat(szTemp,"%s\r\n\r\n");
	
		sprintf(szContent,szFrom);
		strcat(szContent,szTo);
		strcat(szContent,szDateTime);
		strcat(szContent,szSubject);
		strcat(szContent,szBodyHead);
		strcat(szContent,szBody);
		HANDLE hFile;
		hFile=CreateFile(ci.m_szFilename,GENERIC_READ,FILE_SHARE_READ, NULL
			,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
		if(hFile!=INVALID_HANDLE_VALUE)
		{
			struct _stat filestat;
			_stat(ci.m_szFilename,&filestat);
			
			DWORD dwNum;
			ReadFile(hFile,szFileBuf,filestat.st_size,&dwNum,NULL);
			char* file;
			base64_encode(szFileBuf,strlen(szFileBuf),&file);
			char fname[100]={0};
			char ext[3]={0};
			_splitpath(ci.m_szFilename,NULL,NULL,fname,ext);
			strcat(fname,ext);
			sprintf(szAttachment,szTemp,fname,fname,file);

			CloseHandle(hFile);
			strcat(szContent,szAttachment);
		}
		//连接成Content 
		strcat(szContent,"--ntSmtp\r\n.\r\n");
	}
	else
	{
		//连接成 Content
		strcat(szContent,szFrom);
		strcat(szContent,szTo);
		strcat(szContent,szDateTime);
		strcat(szContent,szSubject);
		strcat(szContent,szBodyHead);
		strcat(szContent,szBody);
		strcat(szContent,"--ntSmtp\r\n.\r\n");	
	}

	//发送 Content 
	send(sconnection,szContent,strlen(szContent),0);
	if(!GetResponse(GENERIC_SUCCESS))
	{
		closesocket(sconnection);
		delete response_buf;
		response_buf = NULL;
		return FALSE;
	}

	//发送Quit
	send(sconnection,_T("QUIT\r\n"),strlen(_T("QUIT\r\n")),0);
	if(!GetResponse(QUIT_SUCCESS))
	{
		closesocket(sconnection);
		delete response_buf;
		response_buf = NULL;
		return FALSE;
	}

	return TRUE;
}