// SystemTray.cpp : implementation file
//

#include "stdafx.h"
#include "FTPserver.h"
#include "SystemTray.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CSystemTray, CDialog)

/////////////////////////////////////////////////////////////////////////////
// CSystemTray dialog

CSystemTray::CSystemTray()
{
    ZeroMemory(&m_NotifyIconData, sizeof(m_NotifyIconData));
}

CSystemTray::~CSystemTray()
{
	// ɾ������
    m_NotifyIconData.uFlags = 0;
    Shell_NotifyIcon(NIM_DELETE, &m_NotifyIconData);
	
	// �ر����崰��
    DestroyWindow();
}


BEGIN_MESSAGE_MAP(CSystemTray, CDialog)
	//{{AFX_MSG_MAP(CSystemTray)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSystemTray message handlers

/********************************************************************/
/*																	*/
/* �������� : CSystemTray::Create									*/
/* ����     : ��������												*/
/*																	*/
/********************************************************************/
BOOL CSystemTray::Create(UINT uCallbackMessage, LPCTSTR lpszToolTip, HICON hIcon, UINT uID)
{
	// ����һ�����崰��
    if (!CWnd::CreateEx(0, AfxRegisterWndClass(0), "CSystemTray Dummy Window", WS_POPUP, 0,0,0,0, NULL, 0))
		return FALSE;
	
    // ��������״̬
    m_NotifyIconData.cbSize = sizeof(NOTIFYICONDATA);
    m_NotifyIconData.hWnd   = m_hWnd;
    m_NotifyIconData.uID    = uID;
    m_NotifyIconData.hIcon  = hIcon;
    m_NotifyIconData.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP;
    m_NotifyIconData.uCallbackMessage = uCallbackMessage;
    lstrcpy(m_NotifyIconData.szTip, lpszToolTip);
	
    // �������
    return Shell_NotifyIcon(NIM_ADD, &m_NotifyIconData);
}

/********************************************************************/
/*																	*/
/* �������� : CSystemTray::OnTrayNotification						*/
/* ����     : ���̶�������											*/
/*																	*/
/********************************************************************/
LRESULT CSystemTray::OnTrayNotification(WPARAM wParam, LPARAM lParam)
{
    // ����������Ϣֱ�ӷ���
    if (wParam != m_NotifyIconData.uID)
        return 0L;
	
    CWnd *pMainWnd = AfxGetMainWnd();
	
    switch(LOWORD(lParam))
	{
		// �Ҽ������������̲˵�
		case WM_RBUTTONUP:
			{    
				CMenu menu, *pSubMenu;
				if (!menu.LoadMenu(m_NotifyIconData.uID)) 
					return 0;
				
				pSubMenu = menu.GetSubMenu(0);
				
				if (pSubMenu == NULL)
					return 0;
				
				CPoint pos;
				GetCursorPos(&pos);
				
				// ��ʾ�����˵�
				pMainWnd->SetForegroundWindow();
				pSubMenu->TrackPopupMenu(0, pos.x, pos.y, pMainWnd, NULL);
				
				menu.DestroyMenu();
				break;
			} 
		// ���˫����ʾ������
		case WM_LBUTTONDBLCLK:
			{
				pMainWnd->ShowWindow(SW_SHOW);
				pMainWnd->SetForegroundWindow();
				break;
			}
		default:
			break;
	}
    return 1;
}

/********************************************************************/
/*																	*/
/* �������� : CSystemTray::WindowProc								*/
/* ����     : ������Ϣ��Ӧ											*/
/*																	*/
/********************************************************************/
LRESULT CSystemTray::WindowProc(UINT uMsg, WPARAM wParam, LPARAM lParam) 
{
    if (uMsg == m_NotifyIconData.uCallbackMessage)
	{
        return OnTrayNotification(wParam, lParam);
	}
	return CWnd::WindowProc(uMsg, wParam, lParam);
}
