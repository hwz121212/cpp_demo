// set.cpp : implementation file
//

#include "stdafx.h"
#include "homework11.h"
#include "set.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// set dialog


set::set(CWnd* pParent /*=NULL*/)
	: CDialog(set::IDD, pParent)
{    width=0;
	//{{AFX_DATA_INIT(set)
	m_1 = 0.0;
	//}}AFX_DATA_INIT
}


void set::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(set)
	DDX_Text(pDX, IDC_EDIT1, m_1);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(set, CDialog)
	//{{AFX_MSG_MAP(set)
	ON_EN_CHANGE(IDC_EDIT1, OnChangeEdit1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// set message handlers

void set::OnChangeEdit1() 
{
UpdateData();
width=m_1;
UpdateData(false);
}
