/******************************************************************************
*
* PROJECT:		ExeScanner
* FILE NAME:	ImageImportDlg.h
*
* DEVELOPER:	Vishalsinh Jhala
* DATE:			2-Apr-2005
*
* DESCRIPTION:	Declaration file for class ImageImportDlg.
*
* NOTES:		Used for Importing Images like icons/bitmaps/cursors.
*				Helps user in comparing dimensions and preview the resources
*				before modifying them
*
******************************************************************************/
#pragma once

#include "PEBase.h"
#include "resource.h"
#include "afxwin.h"
#include "previewwnd.h"

// CImageImportDlg dialog

class CImageImportDlg : public CDialog
{
	DECLARE_DYNAMIC(CImageImportDlg)

public:
	CImageImportDlg(IMAGE_RESOURCE irSrc, IMAGE_RESOURCE irDest,CWnd* pParent = NULL);   // standard constructor
	virtual ~CImageImportDlg();

// Dialog Data
	enum { IDD = IDD_IMAGE_IMPORT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString m_strSrcFile;
	CString m_strDestFile;
	int m_iSrcWidth;
	int m_iDestWidth;
	int m_iSrcHeight;
	int m_iDestHeight;
	int m_iSrcBPP;
	int m_iDestBPP;
	int m_iSrcSize;
	int m_iDestSize;
	CDC *pDCSrc,*pDCDest;

	CPreviewWnd m_wndPreview;

	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
public:
	afx_msg void OnBnClickedPreview1();
	afx_msg void OnBnClickedPreview2();
};
