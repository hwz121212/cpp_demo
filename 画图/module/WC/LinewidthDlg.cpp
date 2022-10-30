// LinewidthDlg.cpp : implementation file
//

#include "stdafx.h"
#include "WC.h"
#include "LinewidthDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLinewidthDlg dialog


CLinewidthDlg::CLinewidthDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLinewidthDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLinewidthDlg)
	m_radio1=0;
	m_radio2=1;
	m_radio3=1;
	m_radio4=1;
	m_radio5=1;
	//}}AFX_DATA_INIT
}


void CLinewidthDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLinewidthDlg)
	DDX_Radio(pDX, IDC_RADIO1, m_radio1);
	DDX_Radio(pDX, IDC_RADIO2, m_radio2);
	DDX_Radio(pDX, IDC_RADIO3, m_radio3);
	DDX_Radio(pDX, IDC_RADIO4, m_radio4);
	DDX_Radio(pDX, IDC_RADIO5, m_radio5);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLinewidthDlg, CDialog)
	//{{AFX_MSG_MAP(CLinewidthDlg)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_RADIO1, OnRadio1)
	ON_BN_CLICKED(IDC_RADIO2, OnRadio2)
	ON_BN_CLICKED(IDC_RADIO3, OnRadio3)
	ON_BN_CLICKED(IDC_RADIO4, OnRadio4)
	ON_BN_CLICKED(IDC_RADIO5, OnRadio5)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLinewidthDlg message handlers

HBRUSH CLinewidthDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	//HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	
	// TODO: Return a different brush if the default is not desired
	//return hbr;
		return m_brush;
}

BOOL CLinewidthDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	UpdateData(false);
	m_brush.CreateSolidBrush(RGB(255,255,0)); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CLinewidthDlg::OnRadio1() 
{
	// TODO: Add your control notification handler code here
	m_radio1=0;
	m_radio2=1;
	m_radio3=1;
	m_radio4=1;
	m_radio5=1;
	UpdateData(false);
}

void CLinewidthDlg::OnRadio2() 
{
	// TODO: Add your control notification handler code here
	m_radio1=1;
	m_radio2=0;
	m_radio3=1;
	m_radio4=1;
	m_radio5=1;
	UpdateData(false);
}

void CLinewidthDlg::OnRadio3() 
{
	// TODO: Add your control notification handler code here
	m_radio1=1;
	m_radio2=1;
	m_radio3=0;
	m_radio4=1;
	m_radio5=1;	
	UpdateData(false);
}

void CLinewidthDlg::OnRadio4() 
{
	// TODO: Add your control notification handler code here
	m_radio1=1;
	m_radio2=1;
	m_radio3=1;
	m_radio4=0;
	m_radio5=1;	
	UpdateData(false);
}

void CLinewidthDlg::OnRadio5() 
{
	// TODO: Add your control notification handler code here
	m_radio1=1;
	m_radio2=1;
	m_radio3=1;
	m_radio4=1;
	m_radio5=0;	
	UpdateData(false);
}
