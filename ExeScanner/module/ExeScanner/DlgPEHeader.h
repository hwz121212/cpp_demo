/******************************************************************************
*
* PROJECT:		ExeScanner
* FILE NAME:	DlgPEHeader.h
*
* DEVELOPER:	Vishalsinh Jhala
* DATE:			2-Apr-2005
*
* DESCRIPTION:	Declaration file for class CChildView.
*
* NOTES:		Displays replica of Dos and Win Header and allows user to
*				view their values.
*
******************************************************************************/
#pragma once


// CDlgPEHeader dialog

class CDlgPEHeader : public CDialog
{
	DECLARE_DYNAMIC(CDlgPEHeader)

public:
	CDlgPEHeader(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgPEHeader();

// Dialog Data
	enum { IDD = IDD_DIALOG_PEHEADER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnShowFieldDetails(UINT nID);
};
