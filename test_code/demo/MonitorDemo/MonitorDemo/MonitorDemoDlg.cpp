
// MonitorDemoDlg.cpp : ʵ���ļ�
//
/*******************************************************************************
*  @file       
*  @author      def< qq group: 324164944 >
*  @blog        http://www.cnblogs.com/itdef/
*  @brief    
/*******************************************************************************/
#include "stdafx.h"
#include "MonitorDemo.h"
#include "MonitorDemoDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define STRING_CPU_PERCENT		_T("CPU ʹ����")
#define STRING_MEMORY_PERCENT	_T("�ڴ�ռ����")
#define STRING_NET_IO			_T("����IO�ٶ� ��ʽ��֧�ָù���")

// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CMonitorDemoDlg �Ի���




CMonitorDemoDlg::CMonitorDemoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMonitorDemoDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_bStartTimer = FALSE ;
	m_timer_Event_id = NONE_EVENT;
}

void CMonitorDemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_USER_TREE, m_tree);
}

BEGIN_MESSAGE_MAP(CMonitorDemoDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(ID_TIMER_SWITCH, &CMonitorDemoDlg::OnBnClickedTimerSwitch)
	ON_BN_CLICKED(IDEXIT, &CMonitorDemoDlg::OnBnClickedExit)
	ON_NOTIFY(TVN_SELCHANGED, IDC_USER_TREE, &CMonitorDemoDlg::OnTvnSelchangedUserTree)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CMonitorDemoDlg ��Ϣ�������

BOOL CMonitorDemoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	//================================================
	// ��ʼ��pic�ؼ�
	InitOScopeCtrl();
	InitUserTreeControl();
	InsertPcTitleToUserTreeControl();
 
	//================================================
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CMonitorDemoDlg::InitUserTreeControl()
{
	DWORD dwStyle = GetWindowLong(m_tree.m_hWnd,GWL_STYLE);
	dwStyle |= TVS_HASBUTTONS | TVS_HASLINES | TVS_LINESATROOT;
	SetWindowLong(m_tree.m_hWnd,GWL_STYLE,dwStyle);
	m_hRoot = m_tree.InsertItem(_T("USER_LIST"));
}

void CMonitorDemoDlg::InsertPcTitleToUserTreeControl()
{
	HTREEITEM hItem_user = m_tree.InsertItem(_T("admin:MyComputer"),0,1,m_hRoot,TVI_FIRST);
	m_tree.InsertItem(STRING_CPU_PERCENT,0,1,hItem_user);	
	m_tree.InsertItem(STRING_MEMORY_PERCENT,0,1,hItem_user);	
	m_tree.InsertItem(STRING_NET_IO,0,1,hItem_user);

	m_tree.Expand(m_hRoot,TVE_EXPAND);
	m_tree.Expand(hItem_user,TVE_EXPAND);
}

void CMonitorDemoDlg::InitOScopeCtrl()
{
	CRect rect;
	GetDlgItem(IDC_PLOT)->GetWindowRect(rect) ;
	ScreenToClient(rect) ;
	m_OScopeCtrl.Create(WS_VISIBLE | WS_CHILD, rect, this) ; 

	// customize the control
	m_OScopeCtrl.SetRange(0.0, 100.0, 1) ;
	m_OScopeCtrl.SetYUnits(_T(" ")) ;
	m_OScopeCtrl.SetXUnits(_T(" ")) ;
	m_OScopeCtrl.SetBackgroundColor(RGB(0, 64, 0)) ;
	m_OScopeCtrl.SetGridColor(RGB(192, 255, 192)) ;
	m_OScopeCtrl.SetPlotColor(RGB(255, 255, 255)) ;
}

void CMonitorDemoDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CMonitorDemoDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CMonitorDemoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CMonitorDemoDlg::OnBnClickedTimerSwitch()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������


}

void CMonitorDemoDlg::OnBnClickedExit()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_OScopeCtrl.InvalidateCtrl();
	KillTimer(1);
	OnCancel();
}

void CMonitorDemoDlg::OnTvnSelchangedUserTree(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//=======================================
	*pResult = 0;
	TVITEM item = pNMTreeView->itemNew;
	if(item.hItem == m_hRoot)
		return;

	CString strItem;
	strItem = m_tree.GetItemText(item.hItem);

	if(!m_bStartTimer)
	{
		m_bStartTimer = TRUE;
		SetTimer(1,1000,NULL);
	}

	// �������οؼ�ѡ��ͬ ��������ͼ��������
	if(strItem.Compare(STRING_CPU_PERCENT) == 0)
	{
		CString strItem = m_tree.GetItemText(item.hItem);
		m_timer_Event_id =  CPU_EVENT;
		m_OScopeCtrl.InvalidateCtrl();
		m_OScopeCtrl.SetYUnits(strItem);
		m_OScopeCtrl.SetXUnits(_T("(Windows Timer: 1000 msec)")) ;
	}else if(strItem.Compare(STRING_MEMORY_PERCENT) == 0)
	{
		CString strItem = m_tree.GetItemText(item.hItem);
		m_timer_Event_id =  MEMORY_EVENT;
		m_OScopeCtrl.InvalidateCtrl();
		m_OScopeCtrl.SetYUnits(strItem) ;
		m_OScopeCtrl.SetXUnits(_T("(Windows Timer: 1000 msec)")) ;
	}else if(strItem.Compare(STRING_NET_IO) == 0)
	{
		CString strItem = m_tree.GetItemText(item.hItem);
		m_timer_Event_id =  NET_IO_EVENT;
		m_OScopeCtrl.InvalidateCtrl();
		m_OScopeCtrl.SetYUnits(strItem) ;
		m_OScopeCtrl.SetXUnits(_T("(Windows Timer: 1000 msec)")) ;
	}

	
}

void CMonitorDemoDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if( CPU_EVENT == m_timer_Event_id)
	{

	}else if( MEMORY_EVENT == m_timer_Event_id)
	{
		MEMORYSTATUSEX statex;

		statex.dwLength = sizeof (statex);

		GlobalMemoryStatusEx (&statex);

		m_OScopeCtrl.AppendPoint(statex.dwMemoryLoad);
	}
	

	CDialog::OnTimer(nIDEvent);
}
