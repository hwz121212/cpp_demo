// FTPserverDlg.cpp : implementation file
//

#include "stdafx.h"
#include "FTPserver.h"
#include "FTPserverDlg.h"
#include "DlgQuit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFTPserverDlg dialog

/********************************************************************/
/*																	*/
/* �������� : CFTPserverDlg::CFTPserverDlg							*/
/* ����     : ���캯����ʼ��										*/
/*																	*/
/********************************************************************/
CFTPserverDlg::CFTPserverDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFTPserverDlg::IDD, pParent)
{
	// ��ȡ����IP
	GetLocalHostIP(m_strHostIP);

	CString strTempState;
	strTempState.Format("����IP:%s ��ǰ������:0",m_strHostIP);

	//{{AFX_DATA_INIT(CFTPserverDlg)
	m_bRunning = FALSE;
	m_strState = _T(strTempState);
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_nConnectionCount = 0;
	m_nTimeout = 5;

	ReadRegistry();
}

void CFTPserverDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFTPserverDlg)
	DDX_Text(pDX, IDC_EDIT_MAXUSER, m_nMaxUsers);
	DDV_MinMaxUInt(pDX, m_nMaxUsers, 0, 500);
	DDX_Text(pDX, IDC_EDIT_PORT, m_nPort);
	DDV_MinMaxUInt(pDX, m_nPort, 0, 65535);
	DDX_Text(pDX, IDC_EDIT_USER, m_strName);
	DDV_MaxChars(pDX, m_strName, 20);
	DDX_Text(pDX, IDC_EDIT_PASSWORD, m_strPassword);
	DDV_MaxChars(pDX, m_strPassword, 20);
	DDX_Text(pDX, IDC_EDIT_WELCOME, m_strWelcomeMessage);
	DDX_Text(pDX, IDC_EDIT_QUIT, m_strGoodbyeMessage);
	DDX_Text(pDX, IDC_EDIT_FOLDER, m_strHomeDirectory);
	DDX_Check(pDX, IDC_CHECK_DOWNLOAD, m_bAllowDownload);
	DDX_Check(pDX, IDC_CHECK_UPLOAD, m_bAllowUpload);
	DDX_Check(pDX, IDC_CHECK_RENAME, m_bAllowRename);
	DDX_Check(pDX, IDC_CHECK_DELETE, m_bAllowDelete);
	DDX_Check(pDX, IDC_CHECK_CREATEDIR, m_bAllowCreateDirectory);
	DDX_Text(pDX, IDC_STATE, m_strState);
	DDV_MaxChars(pDX, m_strState, 50);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CFTPserverDlg, CDialog)
	//{{AFX_MSG_MAP(CFTPserverDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_BROWSE, OnBtnBrowse)
	ON_BN_CLICKED(IDC_BTN_START, OnBtnStart)
	ON_BN_CLICKED(IDC_BTN_STOP, OnBtnStop)
	ON_BN_CLICKED(IDC_BTN_QUIT, OnBtnQuit)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_THREADSTART, OnThreadStart)
	ON_MESSAGE(WM_THREADCLOSE, OnThreadClose)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFTPserverDlg message handlers

BOOL CFTPserverDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	GetDlgItem(IDC_BTN_STOP)->EnableWindow(FALSE);
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CFTPserverDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else if ((nID & 0xFFF0) == SC_MINIMIZE)
	{
		ShowWindow(SW_HIDE);
	}
	else if ((nID & 0xFFF0) == SC_CLOSE)
	{
		CDlgQuit dlg;
		dlg.m_pWndServer = this;
		dlg.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CFTPserverDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CFTPserverDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

/********************************************************************/
/*																	*/
/* �������� : CFTPserverDlg::SetPort								*/
/* ����     : �������Ӷ˿�											*/
/*																	*/
/********************************************************************/
void CFTPserverDlg::SetPort(int nValue)
{
	m_nPort = nValue;
}

/********************************************************************/
/*																	*/
/* �������� : CFTPserverDlg::SetMaxUsers							*/
/* ����     : �������������										*/
/*																	*/
/********************************************************************/
void CFTPserverDlg::SetMaxUsers(int nValue)
{
	m_nMaxUsers = nValue;
}

/********************************************************************/
/*																	*/
/* �������� : CFTPserverDlg::SetWelcomeMessage						*/
/* ����     : ���û�ӭ��Ϣ											*/
/*																	*/
/********************************************************************/
void CFTPserverDlg::SetWelcomeMessage(LPCTSTR lpszText)
{
	m_strWelcomeMessage = lpszText;
}

/********************************************************************/
/*																	*/
/* �������� : CFTPserverDlg::SetGoodbyeMessage						*/
/* ����     : �����˳���Ϣ											*/
/*																	*/
/********************************************************************/
void CFTPserverDlg::SetGoodbyeMessage(LPCTSTR lpszText)
{
	m_strGoodbyeMessage = lpszText;
}

/********************************************************************/
/*																	*/
/* �������� : CFTPserverDlg::SetTimeout								*/
/* ����     : ���ó�ʱ												*/
/*																	*/
/********************************************************************/
void CFTPserverDlg::SetTimeout(int nValue)
{
	m_nTimeout = nValue;
}

/********************************************************************/
/*																	*/
/* �������� : CFTPserverDlg::IsActive								*/
/* ����     : �������Ƿ���������									*/
/*																	*/
/********************************************************************/
BOOL CFTPserverDlg::IsActive()
{
	return m_bRunning;
}

/********************************************************************/
/*																	*/
/* �������� : CFTPserverDlg::Stop									*/
/* ����     : ֹͣ����												*/
/*																	*/
/********************************************************************/
void CFTPserverDlg::Stop()
{
	if (!m_bRunning)
	{
		return;
	}
	
	m_bRunning = FALSE;	
	m_ListenSocket.Close();
	
	CConnectThread* pThread = NULL;
	
	// �ر��������е��߳�
	do
	{
		m_CriticalSection.Lock();
		
		POSITION pos = m_ThreadList.GetHeadPosition();
		if (pos != NULL)
		{
			pThread = (CConnectThread *)m_ThreadList.GetAt(pos);
			
			m_CriticalSection.Unlock();
			
			// �����߳̾��
			HANDLE hThread = pThread->m_hThread;
			
			// ֪ͨ�߳��˳�
			pThread->SetThreadPriority(THREAD_PRIORITY_HIGHEST);
			pThread->PostThreadMessage(WM_QUIT,0,0);
			
			// �ȴ��˳������5��
			if (WaitWithMessageLoop(hThread, 5000) == FALSE)
			{
				// ǿ���˳�
				m_CriticalSection.Lock();
				POSITION rmPos = m_ThreadList.Find(pThread);
				if (rmPos != NULL)
					m_ThreadList.RemoveAt(rmPos);
				m_CriticalSection.Unlock();
			}
		}
		else
		{
			m_CriticalSection.Unlock();	
			pThread = NULL;
		}
	}
	while (pThread != NULL);
}

/********************************************************************/
/*																	*/
/* �������� : CFTPserverDlg::Start									*/
/* ����     : ��������												*/
/*																	*/
/********************************************************************/
BOOL CFTPserverDlg::Start()
{
	if (m_bRunning)
	{
		return FALSE;
	}

	// ���������׽���
	if (m_ListenSocket.Create(m_nPort))
	{
		// ��ʼ����
		if (m_ListenSocket.Listen())
		{
			m_ListenSocket.m_pWndServer = this;
			m_bRunning = TRUE;
			return TRUE;
		}
	}
	return FALSE;
}

/********************************************************************/
/*																	*/
/* �������� : CFTPserverDlg::OnBtnBrowse							*/
/* ����     : Ŀ¼�����ť��Ӧ										*/
/*																	*/
/********************************************************************/
void CFTPserverDlg::OnBtnBrowse() 
{
	UpdateData(TRUE);
	CString strDir = BrowseForFolder(m_hWnd, "ѡ�����Ŀ¼:", BIF_RETURNONLYFSDIRS);
	if (!strDir.IsEmpty())
	{
		m_strHomeDirectory = strDir;
		UpdateData(FALSE);
	}	
}

/********************************************************************/
/*																	*/
/* �������� : CFTPserverDlg::CheckMaxUsers							*/
/* ����     : �����������											*/
/*																	*/
/********************************************************************/
BOOL CFTPserverDlg::CheckMaxUsers()
{
	if (m_nConnectionCount > m_nMaxUsers)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

/********************************************************************/
/*																	*/
/* �������� : CFTPserverDlg::GetPort								*/
/* ����     : ���ض˿�												*/
/*																	*/
/********************************************************************/
int CFTPserverDlg::GetPort()
{
	return m_nPort;
}

/********************************************************************/
/*																	*/
/* �������� : CFTPserverDlg::GetMaxUsers							*/
/* ����     : ������������										*/
/*																	*/
/********************************************************************/
int CFTPserverDlg::GetMaxUsers()
{
	return m_nMaxUsers;
}

/********************************************************************/
/*																	*/
/* �������� : CFTPserverDlg::GetConnectionCount						*/
/* ����     : ���ص�ǰ������										*/
/*																	*/
/********************************************************************/
int CFTPserverDlg::GetConnectionCount()
{
	return m_nConnectionCount;
}

/********************************************************************/
/*																	*/
/* �������� : CFTPserverDlg::GetWelcomeMessage						*/
/* ����     : ���ػ�ӭ��Ϣ											*/
/*																	*/
/********************************************************************/
CString CFTPserverDlg::GetWelcomeMessage()
{
	return m_strWelcomeMessage;
}

/********************************************************************/
/*																	*/
/* �������� : CFTPserverDlg::GetGoodbyeMessage						*/
/* ����     : �����˳���Ϣ											*/
/*																	*/
/********************************************************************/
CString CFTPserverDlg::GetGoodbyeMessage()
{
	return m_strGoodbyeMessage;
}

// ���س�ʱ
/********************************************************************/
/*																	*/
/* �������� : CFTPserverDlg::CFTPserverDlg							*/
/* ����     : ���캯����ʼ��										*/
/*																	*/
/********************************************************************/
int CFTPserverDlg::GetTimeout()
{
	return m_nTimeout;
}

/********************************************************************/
/*																	*/
/* �������� : CFTPserverDlg::ItemSwitch								*/
/* ����     : �ؼ�����												*/
/*																	*/
/********************************************************************/
void CFTPserverDlg::ItemSwitch(BOOL x)
{
	GetDlgItem(IDC_EDIT_PORT)->EnableWindow(x);
	GetDlgItem(IDC_EDIT_MAXUSER)->EnableWindow(x);
	GetDlgItem(IDC_EDIT_USER)->EnableWindow(x);
	GetDlgItem(IDC_EDIT_PASSWORD)->EnableWindow(x);
	GetDlgItem(IDC_EDIT_WELCOME)->EnableWindow(x);
	GetDlgItem(IDC_EDIT_QUIT)->EnableWindow(x);
	GetDlgItem(IDC_EDIT_USER)->EnableWindow(x);
	GetDlgItem(IDC_BTN_BROWSE)->EnableWindow(x);
	GetDlgItem(IDC_EDIT_FOLDER)->EnableWindow(x);
	GetDlgItem(IDC_CHECK_DOWNLOAD)->EnableWindow(x);
	GetDlgItem(IDC_CHECK_UPLOAD)->EnableWindow(x);
	GetDlgItem(IDC_CHECK_RENAME)->EnableWindow(x);
	GetDlgItem(IDC_CHECK_DELETE)->EnableWindow(x);
	GetDlgItem(IDC_CHECK_CREATEDIR)->EnableWindow(x);

	GetDlgItem(IDC_BTN_START)->EnableWindow(x);
	GetDlgItem(IDC_BTN_STOP)->EnableWindow(!x);
}

/********************************************************************/
/*																	*/
/* �������� : CFTPserverDlg::OnBtnStart								*/
/* ����     : ������ť��Ӧ											*/
/*																	*/
/********************************************************************/
void CFTPserverDlg::OnBtnStart() 
{
	ItemSwitch(FALSE);
	UpdateData(TRUE);
	if (m_strName == "")
	{
		m_strName = "anonymous";
		UpdateData(FALSE);
	}
	WriteRegistry();
	Start();
}

/********************************************************************/
/*																	*/
/* �������� : CFTPserverDlg::OnBtnStop								*/
/* ����     : ֹͣ��ť��Ӧ											*/
/*																	*/
/********************************************************************/
void CFTPserverDlg::OnBtnStop() 
{
	ItemSwitch(TRUE);
	Stop();
}

/********************************************************************/
/*																	*/
/* �������� : CFTPserverDlg::OnBtnQuit								*/
/* ����     : �˳���ť��Ӧ											*/
/*																	*/
/********************************************************************/
void CFTPserverDlg::OnBtnQuit() 
{
	if (IsWindow(m_hWnd))
	{
		DestroyWindow();
	}
	m_hWnd = NULL;
}

/********************************************************************/
/*																	*/
/* �������� : CFTPserverDlg::OnThreadStart							*/
/* ����     : �߳�������Ӧ											*/
/*																	*/
/********************************************************************/
LRESULT CFTPserverDlg::OnThreadStart(WPARAM wParam, LPARAM)
{
	m_nConnectionCount++;
	m_strState.Format("����IP:%s ��ǰ������:%d",m_strHostIP,m_nConnectionCount);
	UpdateData(FALSE);
	return TRUE;
}

/********************************************************************/
/*																	*/
/* �������� : CFTPserverDlg::OnThreadClose							*/
/* ����     : �̹߳ر���Ӧ											*/
/*																	*/
/********************************************************************/
LRESULT CFTPserverDlg::OnThreadClose(WPARAM wParam, LPARAM lParam)
{
	m_nConnectionCount--;
	m_strState.Format("����IP:%s ��ǰ������:%d",m_strHostIP,m_nConnectionCount);
	UpdateData(FALSE);
	return TRUE;
}


/********************************************************************/
/*																	*/
/* �������� : CFTPserverDlg::WriteRegistry							*/
/* ����     : ��������Ϣд��ע���									*/
/*																	*/
/********************************************************************/
void CFTPserverDlg::WriteRegistry()
{
	AfxGetApp()->WriteProfileString("����","�˻�",m_strName);
	AfxGetApp()->WriteProfileString("����","����",m_strPassword);
	AfxGetApp()->WriteProfileString("����","��ӭ��Ϣ",m_strWelcomeMessage);
	AfxGetApp()->WriteProfileString("����","�뿪��Ϣ",m_strGoodbyeMessage);
	AfxGetApp()->WriteProfileString("����","����Ŀ¼",m_strHomeDirectory);

	AfxGetApp()->WriteProfileInt("����","�˿�",m_nPort);
	AfxGetApp()->WriteProfileInt("����","����߳�",m_nMaxUsers);
	AfxGetApp()->WriteProfileInt("����","�����ļ�",m_bAllowDownload);
	AfxGetApp()->WriteProfileInt("����","�ϴ��ļ�",m_bAllowUpload);
	AfxGetApp()->WriteProfileInt("����","ɾ���ļ�",m_bAllowDelete);
	AfxGetApp()->WriteProfileInt("����","�ļ�������",m_bAllowRename);
	AfxGetApp()->WriteProfileInt("����","����Ŀ¼",m_bAllowCreateDirectory);
}


/********************************************************************/
/*																	*/
/* �������� : CFTPserverDlg::ReadRegistry							*/
/* ����     : ��ע����ж�ȡ������Ϣ								*/
/*																	*/
/********************************************************************/
void CFTPserverDlg::ReadRegistry()
{
	m_strName = AfxGetApp()->GetProfileString("����","�˻�",_T("anonymous"));
	m_strPassword = AfxGetApp()->GetProfileString("����","����",_T(""));
	m_strWelcomeMessage = AfxGetApp()->GetProfileString("����","��ӭ��Ϣ",_T("��ӭ���� Slyar FTPserver!"));
	m_strGoodbyeMessage = AfxGetApp()->GetProfileString("����","�뿪��Ϣ",_T("�ټ�!"));
	m_strHomeDirectory = AfxGetApp()->GetProfileString("����","����Ŀ¼",_T("C:\\"));

	m_nPort = AfxGetApp()->GetProfileInt("����","�˿�",21);
	m_nMaxUsers = AfxGetApp()->GetProfileInt("����","����߳�",100);
	m_bAllowDownload = AfxGetApp()->GetProfileInt("����","�����ļ�",1);
	m_bAllowUpload = AfxGetApp()->GetProfileInt("����","�ϴ��ļ�",0);
	m_bAllowDelete = AfxGetApp()->GetProfileInt("����","ɾ���ļ�",0);
	m_bAllowRename = AfxGetApp()->GetProfileInt("����","�ļ�������",0);
	m_bAllowCreateDirectory = AfxGetApp()->GetProfileInt("����","����Ŀ¼",0);
}
