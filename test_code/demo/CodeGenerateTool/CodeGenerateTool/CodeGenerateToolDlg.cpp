
// CodeGenerateToolDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "CodeGenerateTool.h"
#include "CodeGenerateToolDlg.h"
#include "afxdialogex.h"


#include "CreateDlg.h"
#include "InsertDlg.h"
#include "StructToCahce.h"
#include "Dlg_CodeToGood.h"
#include "Dlg_InterfaceToInvoke.h"
#include "Dlg_Select.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
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

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CCodeGenerateToolDlg 对话框



CCodeGenerateToolDlg::CCodeGenerateToolDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCodeGenerateToolDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCodeGenerateToolDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CCodeGenerateToolDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CCodeGenerateToolDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CCodeGenerateToolDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON_CREATE, &CCodeGenerateToolDlg::OnBnClickedButtonCreate)
	ON_BN_CLICKED(IDC_BUTTON_INSERT, &CCodeGenerateToolDlg::OnBnClickedButtonInsert)
	ON_BN_CLICKED(IDC_BUTTON_STRUCT_TO_CACHE, &CCodeGenerateToolDlg::OnBnClickedButtonStructToCache)
	ON_BN_CLICKED(IDC_BUTTON_CODE_TO_GOOD, &CCodeGenerateToolDlg::OnBnClickedButtonCodeToGood)
	ON_BN_CLICKED(IDC_BUTTON_INTERFACE_TO_INVOKE, &CCodeGenerateToolDlg::OnBnClickedButtonInterfaceToInvoke)
	ON_BN_CLICKED(IDC_BUTTON_SELECT, &CCodeGenerateToolDlg::OnBnClickedButtonSelect)
END_MESSAGE_MAP()


// CCodeGenerateToolDlg 消息处理程序

BOOL CCodeGenerateToolDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

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

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO:  在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CCodeGenerateToolDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CCodeGenerateToolDlg::OnPaint()
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
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CCodeGenerateToolDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CCodeGenerateToolDlg::OnBnClickedOk()
{
	// TODO:  在此添加控件通知处理程序代码
	CDialogEx::OnOK();
}


void CCodeGenerateToolDlg::OnBnClickedCancel()
{
	// TODO:  在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
}


void CCodeGenerateToolDlg::OnBnClickedButtonCreate()
{
	// TODO:  在此添加控件通知处理程序代码
	CCreateDlg dlg;
	dlg.DoModal();
	
}


void CCodeGenerateToolDlg::OnBnClickedButtonInsert()
{
	// TODO:  在此添加控件通知处理程序代码
	CInsertDlg dlg;
	dlg.DoModal();
}


void CCodeGenerateToolDlg::OnBnClickedButtonStructToCache()
{
	CStructToCahce dlg;
	dlg.DoModal();
}


void CCodeGenerateToolDlg::OnBnClickedButtonCodeToGood()
{
	// TODO:  在此添加控件通知处理程序代码
	CDlg_CodeToGood dlg;
	dlg.DoModal();
}


void CCodeGenerateToolDlg::OnBnClickedButtonInterfaceToInvoke()
{
	// TODO:  在此添加控件通知处理程序代码

	CDlg_InterfaceToInvoke dlg;
	dlg.DoModal();
}


void CCodeGenerateToolDlg::OnBnClickedButtonSelect()
{
	CDlg_Select dlg;
	dlg.DoModal();	
}
