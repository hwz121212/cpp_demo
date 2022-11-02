/******************************************************************************
*
* PROJECT:		ExeScanner
* FILE NAME:	DlgBinaryEdit.cpp
*
* DEVELOPER:	Vishalsinh Jhala
* DATE:			2-Apr-2005
*
* DESCRIPTION:	Defination file for class CDlgBinaryEdit.
*
* NOTES:		Wrapper for Dialog. Allows to edit specified value in a
*				binary resource.
*
******************************************************************************/

#include "stdafx.h"
#include "ExeScanner.h"
#include "DlgBinaryEdit.h"


// CDlgBinaryEdit dialog

IMPLEMENT_DYNAMIC(CDlgBinaryEdit, CDialog)
CDlgBinaryEdit::CDlgBinaryEdit(CString str,CWnd* pParent /*=NULL*/)
	: CDialog(CDlgBinaryEdit::IDD, pParent)
	, m_strBin(str)
{

}

CDlgBinaryEdit::~CDlgBinaryEdit()
{
}

void CDlgBinaryEdit::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_HEX, m_strBin);
	DDV_MaxChars(pDX,m_strBin,2);
}


BEGIN_MESSAGE_MAP(CDlgBinaryEdit, CDialog)
	ON_EN_CHANGE(IDC_EDIT_HEX, OnEnChangeEditHex)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()


/******************************************************************************
* Function Name: IsValidHexChar
*
* Note: Returns true if input character is 0-9 and a-f or A-F.
*
******************************************************************************/
BOOL CDlgBinaryEdit::IsValidHexChar(char ch)
{
	if( ch < 48 || (ch > 57 && ch < 65) || (ch > 70 && ch < 97) || ch > 102 )
		return FALSE;
	else
		return TRUE;
}
void CDlgBinaryEdit::OnEnChangeEditHex()
{
	UpdateData();
	CString m_strValidBin("");	
	for(int i = 0; i<m_strBin.GetLength();i++)
	{
		if( IsValidHexChar(m_strBin.GetAt(i)) )
		{
			m_strValidBin.AppendChar(m_strBin.GetAt(i));
		}
	}
	m_strBin.Format("%s",m_strValidBin);

	UpdateData(FALSE);
}

void CDlgBinaryEdit::OnBnClickedOk()
{
	if(AfxMessageBox("Do you wish to save the Changes?",MB_YESNO) == IDNO )
		return;

	UpdateData();
	
	CString strTemp(m_strBin);
	m_strBin.Format("%02s",strTemp);

	UpdateData(FALSE);

	OnOK();
}
