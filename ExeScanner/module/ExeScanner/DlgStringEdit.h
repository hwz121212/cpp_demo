/******************************************************************************
*
* PROJECT:		ExeScanner
* FILE NAME:	DlgStringEdit.h
*
* DEVELOPER:	Vishalsinh Jhala
* DATE:			2-Apr-2005
*
* DESCRIPTION:	Declaration file for class DlgStringEdit.
*
* NOTES:		Allows user to modify string resource
*
******************************************************************************/
#pragma once


// CDlgStringEdit dialog

class CDlgStringEdit : public CDialog
{
	DECLARE_DYNAMIC(CDlgStringEdit)

public:
	CDlgStringEdit(CString str,int iOrgSize,CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgStringEdit();

// Dialog Data
	enum { IDD = IDD_DIALOG_MODIFY_STRING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnEnChangeEditStr();
	CString m_strString;
	int m_iTotChars;
	afx_msg void OnBnClickedOk2();
};
