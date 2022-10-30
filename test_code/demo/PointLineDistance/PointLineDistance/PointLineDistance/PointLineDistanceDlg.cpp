
// PointLineDistanceDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "PointLineDistance.h"
#include "PointLineDistanceDlg.h"
#include "afxdialogex.h"

//#include <stdio.h>
//#include <stdlib.h> 
#include <math.h>


#include <string>
#include <map>
#include <vector>
using namespace std;

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


// CPointLineDistanceDlg �Ի���



CPointLineDistanceDlg::CPointLineDistanceDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CPointLineDistanceDlg::IDD, pParent)
	, m_x1(0)
	, m_x2(0)
	, m_x3(0)
	, m_y1(0)
	, m_y2(0)
	, m_y3(0)
	, m_strLine(_T(""))
	, m_strA(_T(""))
	, m_strB(_T(""))
	, m_strC(_T(""))
	, m_lA(0)
	, m_lB(0)
	, m_lC(0)
	, m_strDistance(_T(""))
	, m_dDistance(0)
	, m_dRes(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CPointLineDistanceDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1_X, m_x1);
	DDX_Text(pDX, IDC_EDIT2_X, m_x2);
	DDX_Text(pDX, IDC_EDIT3_X, m_x3);
	DDX_Text(pDX, IDC_EDIT1_Y, m_y1);
	DDX_Text(pDX, IDC_EDIT2_Y, m_y2);
	DDX_Text(pDX, IDC_EDIT3_Y, m_y3);
	DDX_Text(pDX, IDC_EDIT_LINE_STRING, m_strLine);
	DDX_Text(pDX, IDC_EDIT5, m_strA);
	DDX_Text(pDX, IDC_EDIT6, m_strB);
	DDX_Text(pDX, IDC_EDIT7, m_strC);
	DDX_Text(pDX, IDC_EDIT1, m_lA);
	DDX_Text(pDX, IDC_EDIT2, m_lB);
	DDX_Text(pDX, IDC_EDIT3, m_lC);
	DDX_Text(pDX, IDC_EDIT8, m_strDistance);
	DDX_Text(pDX, IDC_EDIT4, m_dDistance);
	DDX_Text(pDX, IDC_EDIT9, m_dRes);
}

BEGIN_MESSAGE_MAP(CPointLineDistanceDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CPointLineDistanceDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CPointLineDistanceDlg::OnBnClickedButton2)
END_MESSAGE_MAP()


// CPointLineDistanceDlg ��Ϣ�������

BOOL CPointLineDistanceDlg::OnInitDialog()
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

void CPointLineDistanceDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CPointLineDistanceDlg::OnPaint()
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
HCURSOR CPointLineDistanceDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


//��õڶ����㵽��������һ����͵��������ֱ�ߣ��Ĵ�ֱ����
double GetDistance(const pair<__int64, int>& p1, const pair<__int64, int>& p2, const pair<__int64, int>& p3)
{
	__int64 a = p3.second - p1.second;
	__int64 b = p1.first - p3.first;
	__int64 c = (p3.first * p1.second) - (p1.first * p3.second);
	return (abs((a * p2.first) + (b * p2.second) + c)) / sqrt(double(pow(a, 2) + pow(b, 2)));
}


void CPointLineDistanceDlg::OnBnClickedButton1()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);

	m_strLine = _T("(y3-y1)*x + (x1-x3)*y + x3*y1 - x1*y3 = 0");
	m_strA = _T("y3-y1");
	m_strB = _T("x1-x3");
	m_strC = _T("x3*y1 - x1*y3");

	m_lA = m_y3 - m_y1;
	m_lB = m_x1 - m_x3;
	m_lC = m_x3*m_y1 - m_x1*m_y3;


	m_strDistance = _T("(|A*x2 + B*y2 + C|)/sqrt(A*A + B*B)");
	
	m_dDistance = (abs(m_lA*m_x2 + m_lB*m_y2 + m_lC)) / sqrt(double(pow((double)m_lA, 2)) + pow((double)m_lB, 2));
	
	pair<__int64, int> p1(m_x1, m_y1);
	pair<__int64, int> p2(m_x2, m_y2);
	pair<__int64, int> p3(m_x3, m_y3);

	m_dRes = GetDistance(p1, p2, p3);
	UpdateData(FALSE);

}



void CPointLineDistanceDlg::OnBnClickedButton2()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������

	
}
