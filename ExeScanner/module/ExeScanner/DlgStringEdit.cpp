/******************************************************************************
*
* PROJECT:		ExeScanner
* FILE NAME:	DlgStringEdit.cpp
*
* DEVELOPER:	Vishalsinh Jhala
* DATE:			2-Apr-2005
*
* DESCRIPTION:	Defination file for class DlgStringEdit.
*
* NOTES:		Allows user to modify string resource
*
******************************************************************************/

#include "stdafx.h"
#include "ExeScanner.h"
#include "DlgStringEdit.h"


// CDlgStringEdit dialog

IMPLEMENT_DYNAMIC(CDlgStringEdit, CDialog)
CDlgStringEdit::CDlgStringEdit(CString str,int iOrgSize,CWnd* pParent /*=NULL*/)
	: CDialog(CDlgStringEdit::IDD, pParent)
{
	m_strString = str;
	m_iTotChars = iOrgSize;
}

CDlgStringEdit::~CDlgStringEdit()
{
}

void CDlgStringEdit::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX,IDC_EDIT_STR,m_strString);
	DDV_MaxChars(pDX,m_strString,m_iTotChars);
}


BEGIN_MESSAGE_MAP(CDlgStringEdit, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	ON_EN_CHANGE(IDC_EDIT_STR, OnEnChangeEditStr)
	ON_BN_CLICKED(IDOK2, OnBnClickedOk2)
END_MESSAGE_MAP()


// CDlgStringEdit message handlers

void CDlgStringEdit::OnBnClickedOk()
{
	UpdateData();
	OnOK();
}

void CDlgStringEdit::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	OnCancel();
}

void CDlgStringEdit::OnEnChangeEditStr()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}

void CDlgStringEdit::OnBnClickedOk2()
{
	UpdateData();
	if(m_strString.GetLength() >= m_iTotChars)
	{	
		AfxMessageBox("Delete a Character First");
		return;
	}

	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT_STR);
	CPoint pt = pEdit->GetCaretPos();
	int n = pEdit->CharFromPos(pt);

    int nCharIndex = LOWORD(n);
	m_strString.Insert(nCharIndex,'\n');
	UpdateData(FALSE);

}
