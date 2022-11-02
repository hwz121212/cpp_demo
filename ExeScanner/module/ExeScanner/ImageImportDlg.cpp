/******************************************************************************
*
* PROJECT:		ExeScanner
* FILE NAME:	ImageImportDlg.cpp
*
* DEVELOPER:	Vishalsinh Jhala
* DATE:			2-Apr-2005
*
* DESCRIPTION:	Defination file for class ImageImportDlg.
*
* NOTES:		Used for Importing Images like icons/bitmaps/cursors.
*				Helps user in comparing dimensions and preview the resources
*				before modifying them
*
******************************************************************************/

#include "stdafx.h"
#include "ExeScanner.h"
#include "ImageImportDlg.h"

// CImageImportDlg dialog

IMPLEMENT_DYNAMIC(CImageImportDlg, CDialog)
CImageImportDlg::CImageImportDlg(IMAGE_RESOURCE irSrc, IMAGE_RESOURCE irDest,CWnd* pParent /*=NULL*/)
	: CDialog(CImageImportDlg::IDD, pParent)
	, m_strSrcFile(_T(""))
	, m_strDestFile(_T(""))
	, m_iSrcWidth(0)
	, m_iDestWidth(0)
	, m_iSrcHeight(0)
	, m_iDestHeight(0)
	, m_iSrcBPP(0)
	, m_iDestBPP(0)
	, m_iSrcSize(0)
	, m_iDestSize(0)
{
	int iPos = irSrc.strFile.ReverseFind('\\');
	m_strSrcFile		= irSrc.strFile.Mid(iPos+1);
	m_strDestFile		= irDest.strFile;
	m_iSrcWidth			= irSrc.iWidth;
	m_iDestWidth		= irDest.iWidth;
	m_iSrcHeight		= irSrc.iHeight;
	m_iDestHeight		= irDest.iHeight;
	m_iSrcBPP			= irSrc.iBPP;
	m_iDestBPP			= irDest.iBPP;
	m_iSrcSize			= irSrc.iSize;
	m_iDestSize			= irDest.iSize;
	pDCSrc				= irSrc.pDC;
	pDCDest				= irDest.pDC;
}

CImageImportDlg::~CImageImportDlg()
{
}

void CImageImportDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_SRC_FILE, m_strSrcFile);
	DDX_Text(pDX, IDC_DEST_FILE, m_strDestFile);
	DDX_Text(pDX, IDC_SRC_WIDTH, m_iSrcWidth);
	DDX_Text(pDX, IDC_DEST_WIDTH, m_iDestWidth);
	DDX_Text(pDX, IDC_SRC_HEIGHT, m_iSrcHeight);
	DDX_Text(pDX, IDC_DEST_HEIGHT, m_iDestHeight);
	DDX_Text(pDX, IDC_SRC_BPP, m_iSrcBPP);
	DDX_Text(pDX, IDC_DEST_BPP, m_iDestBPP);
	DDX_Text(pDX, IDC_SRC_SIZE, m_iSrcSize);
	DDX_Text(pDX, IDC_DEST_SIZE, m_iDestSize);
}


BEGIN_MESSAGE_MAP(CImageImportDlg, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTON_PREVIEW_1, OnBnClickedPreview1)
	ON_BN_CLICKED(IDC_BUTTON_PREVIEW_2, OnBnClickedPreview2)
END_MESSAGE_MAP()


// CImageImportDlg message handlers

void CImageImportDlg::OnBnClickedOk()
{
	if(m_iSrcWidth == m_iDestWidth &&
		m_iSrcHeight == m_iDestHeight &&
		m_iSrcBPP == m_iDestBPP &&
		m_iSrcSize == m_iDestSize )
	{
		OnOK();
	}
	else
	{
		AfxMessageBox("Image Dimensions does NOT match(Width,height,bpp). Can NOT update");
		OnCancel();
	}
}

BOOL CImageImportDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	this->UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

HBRUSH CImageImportDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	/*if(pWnd->GetDlgCtrlID() == IDC_SRC_IMAGE)
	{
		pDC->StretchBlt(0,0,m_iSrcWidth,m_iSrcHeight,pDCSrc,0,0,m_iSrcWidth,m_iSrcHeight,SRCCOPY);
	}
	if(pWnd->GetDlgCtrlID() == IDC_DEST_IMAGE)
	{
		pDC->StretchBlt(0,0,m_iDestWidth,m_iDestHeight,pDCDest,0,0,m_iDestWidth,m_iDestHeight,SRCCOPY);
	}*/
	return hbr;
}
/******************************************************************************
* Function Name: OnBnClickedPreview1
*
* Note: Allows user to preview image in a new window
*
******************************************************************************/
void CImageImportDlg::OnBnClickedPreview1()
{
	if( !m_wndPreview.GetSafeHwnd())
		m_wndPreview.CreateEx(0,::AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW,(HCURSOR)0,(HBRUSH)::GetStockObject(WHITE_BRUSH),NULL),
			"Preview",WS_VISIBLE|WS_OVERLAPPEDWINDOW,0,0,500,500,this->GetSafeHwnd(),(HMENU)NULL);

	m_wndPreview.SetDlg(this,pDCSrc,m_iSrcWidth,m_iSrcHeight);
	m_wndPreview.Invalidate();
}
/******************************************************************************
* Function Name: OnBnClickedPreview2
*
* Note: Allows user to preview image in a new window
*
******************************************************************************/
void CImageImportDlg::OnBnClickedPreview2()
{
	if( !m_wndPreview.GetSafeHwnd())
		m_wndPreview.CreateEx(0,::AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW,(HCURSOR)0,(HBRUSH)::GetStockObject(WHITE_BRUSH),NULL),
			"Preview",WS_VISIBLE|WS_OVERLAPPEDWINDOW,0,0,500,500,this->GetSafeHwnd(),(HMENU)NULL);

	m_wndPreview.SetDlg(this,pDCDest,m_iDestWidth,m_iDestHeight);
	m_wndPreview.Invalidate();
}
