// ntTelnetDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ntTelnet.h"
#include "ntTelnetDlg.h"

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
// CntTelnetDlg dialog

CntTelnetDlg::CntTelnetDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CntTelnetDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CntTelnetDlg)
	m_uPort = 25;
	m_strUrl = _T("smtp.sohu.com");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CntTelnetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CntTelnetDlg)
	DDX_Control(pDX, IDC_EDIT1, m_edit);
	DDX_Text(pDX, IDC_EDIT_PORT, m_uPort);
	DDX_Text(pDX, IDC_EDIT_URL, m_strUrl);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CntTelnetDlg, CDialog)
	//{{AFX_MSG_MAP(CntTelnetDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_CONNECTION, OnConnection)
	ON_BN_CLICKED(IDC_DIS_CONNECTION, OnDisConnection)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CntTelnetDlg message handlers

BOOL CntTelnetDlg::OnInitDialog()
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
	WSAStartup(MAKEWORD(2,2),&ws);	
	bConnection=FALSE;
	CButton*pButt=(CButton*)GetDlgItem(IDC_CONNECTION);
	pButt->EnableWindow(!bConnection);
	pButt=(CButton*)GetDlgItem(IDC_DIS_CONNECTION);
	pButt->EnableWindow(bConnection);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CntTelnetDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CntTelnetDlg::OnPaint() 
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
HCURSOR CntTelnetDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}


BOOL CntTelnetDlg::UrlToIp(char *szIp, char *szUrl)
{
	hostent *dnsInfo=NULL;
	struct in_addr  Addr ;
	int   ii = 0;
	//所需内存由Socket负责自动分配并释放
	
	if(dnsInfo==NULL)
	{
		dnsInfo = gethostbyname(szUrl);
		while(dnsInfo->h_addr_list[ii] != NULL)
		{			
			memcpy(&Addr, dnsInfo->h_addr_list[ii],sizeof(Addr));		
			strcpy(szIp, inet_ntoa(Addr));
			ii++;
		}	
	}
	
	return TRUE;

}

void CntTelnetDlg::OnConnection() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_edit.SetWindowText("");

	m_edit.SetReadOnly(FALSE);
	
	char ip[32]={0};
	UrlToIp(ip,(LPSTR)(LPCSTR)m_strUrl);
	
	sock = socket(AF_INET, SOCK_STREAM , IPPROTO_TCP);
	addr_in.sin_family = AF_INET;
	addr_in.sin_addr.S_un.S_addr =inet_addr(ip);
	addr_in.sin_port =htons(m_uPort);
	
	if(connect(sock, (struct sockaddr*)&addr_in,sizeof(addr_in))==0)
	{
		m_edit.StartCmdEditor(sock);
	}
	bConnection=TRUE;
	//接口
	CButton*pButt=(CButton*)GetDlgItem(IDC_CONNECTION);
	pButt->EnableWindow(!bConnection);
	pButt=(CButton*)GetDlgItem(IDC_DIS_CONNECTION);
	pButt->EnableWindow(bConnection);

	//更改窗口标题
	CString strText;
	strText.Format("telnet %s:%d",m_strUrl,m_uPort);
	SetWindowText(strText);
}

void CntTelnetDlg::OnDisConnection() 
{
	// TODO: Add your control notification handler code here
	shutdown(sock,SD_BOTH);
	closesocket(sock);
	sock=NULL;
	bConnection=FALSE;
	//接口
	CButton*pButt=(CButton*)GetDlgItem(IDC_CONNECTION);
	pButt->EnableWindow(!bConnection);
	pButt=(CButton*)GetDlgItem(IDC_DIS_CONNECTION);
	pButt->EnableWindow(bConnection);
}

void CntTelnetDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	WSACleanup();
}
