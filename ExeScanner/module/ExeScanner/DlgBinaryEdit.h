/******************************************************************************
*
* PROJECT:		ExeScanner
* FILE NAME:	DlgBinaryEdit.h
*
* DEVELOPER:	Vishalsinh Jhala
* DATE:			2-Apr-2005
*
* DESCRIPTION:	Declaration file for class CChildView.
*
* NOTES:		Wrapper for Dialog. Allows to edit specified value in a
*				binary resource.
*
******************************************************************************/
#pragma once


// CDlgBinaryEdit dialog

class CDlgBinaryEdit : public CDialog
{
	DECLARE_DYNAMIC(CDlgBinaryEdit)

public:
	CDlgBinaryEdit(CString str,CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgBinaryEdit();

// Dialog Data
	enum { IDD = IDD_DIALOG_BINARYEDIT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnEnChangeEditHex();
	afx_msg void OnBnClickedOk();
	CString m_strBin;
	BOOL IsValidHexChar(char ch);
};
