// FontD.cpp : implementation file
//

#include "stdafx.h"
#include "WC.h"
#include "FontD.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFontD dialog


CFontD::CFontD(CWnd* pParent /*=NULL*/)
	: CDialog(CFontD::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFontD)
	m_string = _T("");
	//}}AFX_DATA_INIT
}


void CFontD::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFontD)
	DDX_Text(pDX, IDC_STRING, m_string);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFontD, CDialog)
	//{{AFX_MSG_MAP(CFontD)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFontD message handlers

void CFontD::OnOK() 
{
	// TODO: Add extra validation here
		UpdateData(true);
	CDialog::OnOK();
}
