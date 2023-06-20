/*
  This project was created and edited using the MFC SelfBanner AppWizard.

  Comment: 远程控制－远程Cmd客户端.

  Project: ntCmdCliennt.
  Author : dhyao
  Date   : 星期一, 二月 02, 2009
*/
/////////////////////////////////////////////////////////////////////
// CmdEditCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "ntTelnet.h"
#include "CmdEditCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCmdEditCtrl

//DWORD WINAPI ReadThread(LPVOID lpParam);

CCmdEditCtrl::CCmdEditCtrl()
{
	nCmdRecord = 0;
	nMaxCmdRecord = 100;
	history = NULL;
	slave_sock = INVALID_SOCKET;
}

CCmdEditCtrl::~CCmdEditCtrl()
{
	PRECORD_LIST temp;

	if (history != NULL)
	{
		while (history->flink != NULL)
		{
			history = history->flink;
		}

		while (history != NULL)
		{
			temp = history;
			history = history->blink;
			HeapFree(GetProcessHeap(), 0, temp);
		}

		history = NULL;
		nCmdRecord = 0;
	}
}


BEGIN_MESSAGE_MAP(CCmdEditCtrl, CEdit)
	//{{AFX_MSG_MAP(CCmdEditCtrl)
	ON_WM_CHAR()
	ON_WM_RBUTTONDOWN()
	ON_WM_KEYDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCmdEditCtrl message handlers


DWORD WINAPI ReadThread(LPVOID lpParam)
{
	CCmdEditCtrl *pCmdedit=(CCmdEditCtrl *)lpParam;
	char buffer[200];
	fd_set readfds;
	
	//更改监听套接字工作模式至非阻塞模式+select
	ULONG sockmode = 1;
	if (ioctlsocket(pCmdedit->slave_sock, FIONBIO, &sockmode) == SOCKET_ERROR)
	{
		printf("ioctlsocket() failed \n");
		return -1;
	}	

	while (TRUE)
	{
		FD_ZERO(&readfds);
		FD_SET(pCmdedit->slave_sock, &readfds);
		int ret=-1;
		if((ret = select(0, &readfds, NULL, NULL, NULL))== SOCKET_ERROR)
			break;

		if (FD_ISSET(pCmdedit->slave_sock, &readfds))
		{
			ret = recv(pCmdedit->slave_sock, buffer, 199, 0);
			if(ret==SOCKET_ERROR)
			{
				if (WSAGetLastError() != WSAEWOULDBLOCK)
				{
					TRACE("Receive failed with error\n");
					closesocket(pCmdedit->slave_sock);
					break;
				}
			}//http连接一次随之中断
			else if(ret==0)
			{
				closesocket(pCmdedit->slave_sock);
				break;
			}
			else if (ret>0)
			{
				buffer[ret] = '\0';
				pCmdedit->SetSel(pCmdedit->nCmdStartPos, pCmdedit->GetWindowTextLength());
				pCmdedit->ReplaceSel(buffer);
				pCmdedit->nCmdStartPos = pCmdedit->GetWindowTextLength();
			}
		}
	}

	return 0;
}

BOOL CCmdEditCtrl::StartCmdEditor(SOCKET sock)
{
	nCmdStartPos = 0;
	slave_sock = sock;
	LimitText(1000000);

	ReadWrite();
	return TRUE;
}



void CCmdEditCtrl::AddCmdRecord(char *cmd)
{
	PRECORD_LIST list, temp;
	int cmdlen;

	for (cmdlen = 0; cmd[cmdlen] && cmd[cmdlen] != 0x0a && cmd[cmdlen] != 0x0d; cmdlen++);

	if (history != NULL)
	{
		temp = history;

		while (temp->flink != NULL)				//指向链表头部
			temp = temp->flink;

		while (temp != NULL)
		{
			if (strncmp(temp->cmd, cmd, cmdlen) == 0 && temp->cmd[cmdlen] == '\0')	//查找重复记录
			{
				if (temp->flink != NULL)				//移除节点
				{
					history = temp->flink;
					temp->flink->blink = temp->blink;
				}

				if (temp->blink != NULL)
				{
					history = temp->blink;
					temp->blink->flink = temp->flink;
				}

				while (history->blink != NULL)
					history = history->blink;

				if (temp != history)
				{
					history->blink = temp;
					temp->flink = history;
					temp->blink = NULL;
					history = temp;
				}

				return;
			}

			temp = temp->blink;
		}
	}

	list = (PRECORD_LIST)HeapAlloc(GetProcessHeap(), 0, sizeof(RECORD_LIST) + cmdlen + 1);

	if (list != NULL)
	{
		strncpy(list->cmd, cmd, cmdlen);
		list->cmd[cmdlen] = '\0';

		if (nCmdRecord > nMaxCmdRecord)
		{
			temp = history;

			while (temp->flink != NULL)
				temp = temp->flink;

			temp->blink->flink = NULL;
			HeapFree(GetProcessHeap(), 0, temp);
			nCmdRecord--;
		}

		if (history == NULL)
		{
			list->flink = NULL;
			list->blink = NULL;
			history = list;
		}
		else
		{
			while (history->blink != NULL)
				history = history->blink;

			list->flink = history;
			list->blink = NULL;
			history->blink = list;
			history = list;
		}

		nCmdRecord++;
	}
}

char *CCmdEditCtrl::GetNextCmdRecord()
{
	if (history == NULL)
		return "";

	if (history->blink != NULL)
		history = history->blink;

	return history->cmd;
}

char *CCmdEditCtrl::GetPrevCmdRecord()
{
	if (history == NULL)
		return "";

	if (history->flink != NULL)
		history = history->flink;

	return history->cmd;
}

void CCmdEditCtrl::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	if (slave_sock == INVALID_SOCKET)
	{
		MessageBox("连接不可用", "", MB_ICONERROR);
		SetReadOnly();
		return;
	}

	int ret, nStart, nEnd;
	char *pch;
	HLOCAL hText;
	GetSel(nStart, nEnd);

	switch ((TCHAR)nChar)
	{
	case VK_RETURN:
		
		AppendText("\x0d\x0a");
		hText = GetHandle();
		pch = (char *)LocalLock(hText) + nCmdStartPos;
		AddCmdRecord(pch);

		if (stricmp(pch, "cls\x0d\x0a") == 0)
		{
			nCmdStartPos = 0;//先不发送
			ret = send(slave_sock, "\x0d\x0a", strlen("\x0d\x0a"), 0);
		}
		else
		{
			ret = send(slave_sock, pch, strlen(pch), 0);
		}

		LocalUnlock(hText);
//		ReadWrite();

		return;

	case VK_BACK:
		if (nStart <= nCmdStartPos)
			return;
		
		break;

	default:
		
		if (nStart <= nCmdStartPos)
		{
			int nTextEnd = GetWindowTextLength();
			SetSel(nTextEnd, nTextEnd);
		}
		
		break;
	}


	CEdit::OnChar(nChar, nRepCnt, nFlags);
}

void CCmdEditCtrl::OnRButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	int nStart, nEnd;

	GetSel(nStart, nEnd);

	if (nStart != nEnd)
	{
		Copy();
		SetSel(GetWindowTextLength(), GetWindowTextLength());
	}
	else
	{
		if (slave_sock == INVALID_SOCKET)
		{
			MessageBox("连接不可用", "", MB_ICONERROR);
			SetReadOnly();
			return ;
		}

		SetSel(GetWindowTextLength(), GetWindowTextLength());
		Paste();
	}

	CEdit::OnRButtonDown(nFlags, point);
}

void CCmdEditCtrl::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	int nStart, nEnd;
	char *p;

	GetSel(nStart, nEnd);

	switch ((TCHAR)nChar)
	{
	case VK_UP:
		p = GetPrevCmdRecord();
		SetSel(nCmdStartPos, GetWindowTextLength());
		ReplaceSel(p);
		
		return;

	case VK_DOWN:
		p = GetNextCmdRecord();
		SetSel(nCmdStartPos, GetWindowTextLength());
		ReplaceSel(p);
		
		return;

	case VK_LEFT:
	case VK_DELETE:
		if (nStart <= nCmdStartPos)
			return;
		break;
	
	default:
		break;
	}

	CEdit::OnKeyDown(nChar, nRepCnt, nFlags);
}


void CCmdEditCtrl::OnSocketRead()
{
	char buffer[200];
	struct timeval tv;
	fd_set readfds;
	
	tv.tv_sec = 0; 
	tv.tv_usec = 100000;
	while (1)
	{
		FD_ZERO(&readfds);
		FD_SET(slave_sock, &readfds);
		int ret = select(0, &readfds, NULL, NULL, &tv);
		if (ret == SOCKET_ERROR)
			break;
		if (!FD_ISSET(slave_sock, &readfds))
			continue;
		ret = recv(slave_sock, buffer, 199, 0);
		if (ret != SOCKET_ERROR)
		{
			buffer[ret] = '\0';
			SetSel(nCmdStartPos, GetWindowTextLength());
			ReplaceSel(buffer);
			nCmdStartPos = GetWindowTextLength();
		}

	}
}

void CCmdEditCtrl::InsertText(int nInsertAfterChar, LPCTSTR lpstrText, BOOL bNoScroll, BOOL bCanUndo)
{
	SetSel(nInsertAfterChar, nInsertAfterChar, bNoScroll);
	ReplaceSel(lpstrText, bCanUndo);
}

void CCmdEditCtrl::AppendText(LPCTSTR lpstrText, BOOL bNoScroll, BOOL bCanUndo)
{
	InsertText(GetWindowTextLength(), lpstrText, bNoScroll, bCanUndo);
}

LRESULT CCmdEditCtrl::OnPaste(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
{
	if (slave_sock == INVALID_SOCKET)
	{
		MessageBox("连接不可用", "", MB_ICONERROR);
		SetReadOnly();
		return 0;
	}

	if (OpenClipboard())
	{
		HANDLE hMem = GetClipboardData(CF_TEXT);

		if (hMem == NULL)
			return -1;

		char *p = (char *)GlobalLock(hMem);

		if (p == NULL)
			return -1;

		send(slave_sock, p, strlen(p), 0);

		GlobalUnlock(hMem);
		CloseClipboard();
		return 0;
	}

	return -1;
}



void CCmdEditCtrl::ReadWrite()
{
	DWORD dwThreadId;
	hThread = CreateThread(NULL, 0, ReadThread, this, 0, &dwThreadId);
	if (hThread == NULL)
	{
		closesocket(slave_sock);
	}
}

BOOL CCmdEditCtrl::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return FALSE;
}


BOOL CCmdEditCtrl::CloseCmd()
{
	
	return TerminateThread(hThread,0);
//	WaitForSingleObject(hThread,INFINITE);
}
