// ntPop3Mail.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "stdafx.h"
#include <stdlib.h>
#include "winsock2.h"
#include "base64.h"
#include <sys/stat.h>

#include <time.h>
#include <io.h>

//winsock 2.2 library
#pragma comment(lib,"ws2_32.lib")


#define POP3_PORT 110 // Standard port for pop3 servers
#define RESPONSE_BUFFER_SIZE 1024
#define PARA_BUF 128
#define RESPONSE_NUM  8
#define _T(x)   x


enum eResponse
{
	CONNECTION = 0,
		IDENTIFICATION,
		AUTHENTIFICATION,
		STATUS,
		LIST,
		TOP,
		RETRIEVE,
		DELE,
		QUIT,
		// Include any others here
		LAST_RESPONSE	// Do not add entries past this one
};
TCHAR response_buf[ RESPONSE_BUFFER_SIZE ];
TCHAR sMsg[ RESPONSE_BUFFER_SIZE*20]={0};
TCHAR sMsgDecode[ RESPONSE_BUFFER_SIZE*20]={0};


static TCHAR* error_table[RESPONSE_NUM]=
{
		"Server didn't connect",
		"Bad user name",							// IDENTIFICATION
		"Invalid username/password combination",	// AUTHENTIFICATION
		"STATus couldn't be retrieved",				// STATUS
		"LIST failed"								// LIST
		"TOP"										// TOP
		"RETRieve failed",							// RETRIEVE
		"Could not DELEte message",					// DELE
		"Error while QUITting"						// QUIT
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

char m_sError[PARA_BUF];
SOCKET sconnection;
CONFIG_INFO ci;

BOOL GetResponse( UINT executed_action );
int GetNumEmail();
BOOL DeleteEmail( UINT nMsg );
BOOL GetEmail( UINT nMsg, char* msg);
BOOL GetList(); 
BOOL GetTop(UINT nMsg,UINT nLine=0);


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
	printf("Please enter pop3 Server Name:");
	gets_s(ci.m_szServerName);
//	sprintf(ci.m_szServerName,"pop3.126.com");
//	sprintf(ci.m_szServerName,"pop3.sohu.com");
//	sprintf(ci.m_szServerName,"pop3.sina.com.cn");
	//设置SOCKADDR_IN地址结构
	serAddr.sin_family = AF_INET;
	serAddr.sin_port = htons(POP3_PORT);
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
	if(!GetResponse(CONNECTION) )
	{
		closesocket(sconnection);
		return FALSE;
	}

//	sprintf(ci.m_szAccount,"USER ntsmtp\r\n");
	char szTem[PARA_BUF];
	printf("Enter Email Address: ");
	gets_s(szTem);
	sprintf(ci.m_szAccount,"USER %s\r\n",szTem);
	send(sconnection,ci.m_szAccount,strlen(ci.m_szAccount),0);

	if( !GetResponse( IDENTIFICATION ) )
	{
		closesocket(sconnection);
		return FALSE;
	}

//	sprintf(ci.m_szPassword,"PASS nt123456\r\n");
	printf("Enter Passwd: ");
	gets_s(szTem);
	sprintf(ci.m_szPassword,"PASS %s\r\n",szTem);
	send(sconnection,ci.m_szPassword,strlen(ci.m_szPassword),0);

	if( !GetResponse( AUTHENTIFICATION ) )
	{
		closesocket(sconnection);
		return FALSE;
	}

	int k=GetNumEmail();
	
	if(k>=1)
	{
		GetEmail(1,NULL);
//		GetList();
//		GetTop(1,5);
	}
	


	printf("Closing the connection.\n");
	
	closesocket(sconnection);
	WSACleanup();

	return 0;
}

BOOL GetResponse( UINT executed_action )
{
	char szResponse[RESPONSE_BUFFER_SIZE]={0};
	ZeroMemory(response_buf,sizeof(response_buf));
	if(recv(sconnection,response_buf, RESPONSE_BUFFER_SIZE,0)== SOCKET_ERROR )
	{
		printf("Socket Error\n");
		return FALSE;
	}
	
	strncpy(szResponse,response_buf,4);
	if(strncmp(szResponse,"-ERR",4)==0)
	{
		strcpy(m_sError,error_table[executed_action]);
		return FALSE; 
	}

	return TRUE;
}

int GetNumEmail()
{
	printf("Getting Number of Email......\n");
	char sStat[10]={ _T( "STAT\r\n" )};
	char *result;

	send(sconnection,sStat,strlen(sStat),0);
	
	if( !GetResponse( STATUS ))
		return -1;
	
	result=strpbrk(response_buf,"0123456789");

	return atoi(result);
	
}

BOOL DeleteEmail(UINT nMsg)
{
	char sDele[10] = {0};
	sprintf(sDele,"DELE %d\r\n",nMsg);

	//发送dele标记删除
	send(sconnection,sDele,strlen(sDele),0);
	if( !GetResponse(DELE))
		return FALSE;

	//必须发送“Quit”以实际删除
	send(sconnection,_T("QUIT\r\n"),strlen(_T("QUIT\r\n")),0);
	if(!GetResponse(QUIT))
	{
		closesocket(sconnection);
		return FALSE;
	}
	
	return TRUE;
}

BOOL GetEmail( UINT nMsg, char* msg)
{
	char sRetr[10] = {0};
	char szTem[RESPONSE_BUFFER_SIZE*20]={0};
	
	printf("Getting Email......\n");
	sprintf(sRetr,"RETR %d\r\n",nMsg);
	
	//发送RETR
	send(sconnection,sRetr,strlen(sRetr),0);
	//	
	if(!GetResponse(RETRIEVE))
		return FALSE;
	strcpy(sMsg,response_buf);
	while (strstr(sMsg,"\r\n.\r\n")==NULL)
	{
		// nChars = number of bytes read
		int nChars =recv(sconnection,response_buf, RESPONSE_BUFFER_SIZE,0);
		if(nChars== SOCKET_ERROR)return FALSE;
		//only the first nChars bytes of response_buf are valid !
		ZeroMemory(szTem,sizeof(szTem));
		strncpy(szTem,response_buf,nChars);
		strcat(sMsg,szTem);
	}

	//For test
	int k=strlen(sMsg);
	HFILE f=_lcreat("nttest.txt",0);
	_lwrite(f,sMsg,k);
	//
	ZeroMemory(szTem,sizeof(szTem));
	strcpy(szTem,sMsg);
	strcpy(szTem,strstr(sMsg,"\r\n")+2);//first line of output is +OK
	ZeroMemory(sMsg,sizeof(sMsg));
	strncpy(sMsg,szTem,strlen(szTem)-3);//last line is always .\r\n
	char* pmsg=strstr(sMsg,"\r\n\r\n");//breakpoint between header and body
	printf(pmsg+4);
	//base64_decode(pmsg+4,sMsgDecode);
	//printf(sMsgDecode);

	return TRUE;
}

BOOL GetList()
{ 
	char szList[10]; 	
	printf("Getting Email List......\n");
	sprintf (szList, "LIST\r\n");
	//发送list
	send(sconnection,szList,strlen(szList),0);
	if(!GetResponse(LIST))
		return FALSE;

	printf(response_buf);
	
	return TRUE; 
} 

BOOL GetTop(UINT nMsg,UINT nLine)
{
	char szTop[10]; 
	
	printf("Getting Top of %d line of Email %d......\n",nLine,nMsg);
	
	sprintf (szTop, "TOP %d %d\r\n",nMsg,nLine);
	//发送top
	send(sconnection,szTop,strlen(szTop),0);
	if(!GetResponse(LIST))
		return FALSE;
	
	printf(response_buf);
}