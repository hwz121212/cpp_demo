
// CodeGenerateToolDlg.cpp : ʵ���ļ�
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


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
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

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CCodeGenerateToolDlg �Ի���



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


// CCodeGenerateToolDlg ��Ϣ�������

BOOL CCodeGenerateToolDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

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

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO:  �ڴ���Ӷ���ĳ�ʼ������

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CCodeGenerateToolDlg::OnPaint()
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
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CCodeGenerateToolDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CCodeGenerateToolDlg::OnBnClickedOk()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CDialogEx::OnOK();
}


void CCodeGenerateToolDlg::OnBnClickedCancel()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CDialogEx::OnCancel();
}


void CCodeGenerateToolDlg::OnBnClickedButtonCreate()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CCreateDlg dlg;
	dlg.DoModal();
	
}


void CCodeGenerateToolDlg::OnBnClickedButtonInsert()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
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
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CDlg_CodeToGood dlg;
	dlg.DoModal();
}


void CCodeGenerateToolDlg::OnBnClickedButtonInterfaceToInvoke()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������

	CDlg_InterfaceToInvoke dlg;
	dlg.DoModal();
}


void CCodeGenerateToolDlg::OnBnClickedButtonSelect()
{
	CDlg_Select dlg;
	dlg.DoModal();	
}
