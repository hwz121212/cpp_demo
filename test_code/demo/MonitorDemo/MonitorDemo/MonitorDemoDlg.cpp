
// MonitorDemoDlg.cpp : 实现文件
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

#define STRING_CPU_PERCENT		_T("CPU 使用率")
#define STRING_MEMORY_PERCENT	_T("内存占用率")
#define STRING_NET_IO			_T("网络IO速度 正式版支持该功能")

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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


// CMonitorDemoDlg 对话框




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


// CMonitorDemoDlg 消息处理程序

BOOL CMonitorDemoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	//================================================
	// 初始化pic控件
	InitOScopeCtrl();
	InitUserTreeControl();
	InsertPcTitleToUserTreeControl();
 
	//================================================
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMonitorDemoDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMonitorDemoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CMonitorDemoDlg::OnBnClickedTimerSwitch()
{
	// TODO: 在此添加控件通知处理程序代码


}

void CMonitorDemoDlg::OnBnClickedExit()
{
	// TODO: 在此添加控件通知处理程序代码
	m_OScopeCtrl.InvalidateCtrl();
	KillTimer(1);
	OnCancel();
}

void CMonitorDemoDlg::OnTvnSelchangedUserTree(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
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

	// 根据树形控件选择不同 更改曲线图坐标文字
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
	// TODO: 在此添加消息处理程序代码和/或调用默认值
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
