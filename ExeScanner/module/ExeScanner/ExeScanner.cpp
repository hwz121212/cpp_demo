/******************************************************************************
*
* PROJECT:		ExeScanner
* FILE NAME:	ExeScanner.cpp
*
* DEVELOPER:	Vishalsinh Jhala
* DATE:			2-Apr-2005
*
* DESCRIPTION:	Defination file for class CExeScannerApp.
*
* NOTES:		Holds the main app object. Machine generated.
*
******************************************************************************/

#include "stdafx.h"
#include "ExeScanner.h"
#include "MainFrm.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CExeScannerApp

BEGIN_MESSAGE_MAP(CExeScannerApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
END_MESSAGE_MAP()


// CExeScannerApp construction

CExeScannerApp::CExeScannerApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CExeScannerApp object
#ifdef _DEBUG
CFile g_File;
CString g_strDbg;
#endif

CExeScannerApp theApp;

// CExeScannerApp initialization

BOOL CExeScannerApp::InitInstance()
{
	// InitCommonControls() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	InitCommonControls();

	CWinApp::InitInstance();

	#ifdef _DEBUG
		g_File.Open("$dbg$.$$$",CFile::modeCreate|CFile::modeWrite|CFile::typeBinary);
	#endif

	
	// Initialize OLE libraries
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}
	AfxEnableControlContainer();
	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));
	// To create the main window, this code creates a new frame window
	// object and then sets it as the application's main window object
	CMainFrame* pFrame = new CMainFrame;
	m_pMainWnd = pFrame;
	// create and load the frame with its resources
	pFrame->LoadFrame(IDR_MAINFRAME,
		WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, NULL,
		NULL);
	// The one and only window has been initialized, so show and update it
	pFrame->SetIcon(LoadIcon(MAKEINTRESOURCE(IDR_MAINFRAME)),TRUE);
	pFrame->ShowWindow(SW_SHOWMAXIMIZED);
	pFrame->UpdateWindow();
	// call DragAcceptFiles only if there's a suffix
	//  In an SDI app, this should occur after ProcessShellCommand
	return TRUE;
}


// CExeScannerApp message handlers



// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
	CDC m_dcPhoto;
	CBitmap m_bmpPhoto;
	CDC m_dcDisp;
	CBitmap m_bmpDisp;
	CDC m_dcMask;
	CBitmap m_bmpMask;

	int m_iScanPos,m_iStage;
	CPen m_penG1,m_penG2;
	CPen m_penR1,m_penR2,m_penWhite;
	CBrush m_brGreen, m_brRed;

	void Stage1();
	void Stage2();
	void Stage3();

public:
	CAboutDlg();
	CString m_strNotice;
// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual BOOL OnInitDialog();
protected:
	virtual void OnCancel();
	virtual void OnOK();
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	m_strNotice = "LICENSE AGREEMENT:\r\n"
		       "--------------------------\r\n"  
				"ExeScanner is free software; you can redistribute it and/or modify it provided that you agree to the following terms and conditions:\r\n"
				  "1. The software should always contain this license agreement\r\n"
				  "2. The source code should always contain the given copyright notice.\r\n"
				  "3. Use of software or source code for commercial purposes or for making profit requires permissions from Vishalsinh Jhala.\r\n"
				  "4. This software is not to be used in any way to illegally modify it or other softwares.\r\n\r\n"
                  "DISCLAIMER:\r\n"
				  "------------------\r\n"
				  "THE SOFTWARE IS PROVIDED \"AS-IS\" AND WITHOUT WARRANTY OF ANY KIND, EXPRESS, IMPLIED OR OTHERWISE, INCLUDING WITHOUT LIMITATION, ANY  WARRANTY OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.\r\n"  
				  "IN NO EVENT SHALL VISHALSINH JHALA BE LIABLE FOR ANY SPECIAL, INCIDENTAL, INDIRECT OR CONSEQUENTIAL DAMAGES OF ANY KIND, OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER OR NOT ADVISED OF THE POSSIBILITY OF DAMAGE, AND ON ANY THEORY OF LIABILITY, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.";

	m_dcPhoto.CreateCompatibleDC(CDC::FromHandle(::GetDC(NULL)));
	m_bmpPhoto.LoadBitmap(MAKEINTRESOURCE(IDB_BITMAP_VISHAL));
	m_dcPhoto.SelectObject(m_bmpPhoto);

	m_dcDisp.CreateCompatibleDC(CDC::FromHandle(::GetDC(NULL)));
	m_bmpDisp.CreateCompatibleBitmap(CDC::FromHandle(::GetDC(NULL)),123,175);
	m_dcDisp.SelectObject(m_bmpDisp);

	m_dcMask.CreateCompatibleDC(CDC::FromHandle(::GetDC(NULL)));
	m_bmpMask.CreateCompatibleBitmap(CDC::FromHandle(::GetDC(NULL)),123,175);
	m_dcMask.SelectObject(m_bmpMask);

	m_penG1.CreatePen(PS_SOLID,1,RGB(0,255,0));
	m_penG2.CreatePen(PS_SOLID,1,RGB(0,128,0));

	m_penR1.CreatePen(PS_SOLID,1,RGB(255,0,0));
	m_penR2.CreatePen(PS_SOLID,1,RGB(128,0,0));

	m_penWhite.CreatePen(PS_SOLID,1,RGB(255,255,255));

	m_brGreen.CreateSolidBrush(RGB(0,255,0));
	m_brRed.CreateSolidBrush(RGB(255,0,0));

	m_iScanPos = 0;
	m_iStage = 0;
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX,IDC_EDIT1,m_strNotice);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	ON_WM_TIMER()
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()

// App command to run the dialog
void CExeScannerApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}


// CExeScannerApp message handlers


void CAboutDlg::OnTimer(UINT_PTR nIDEvent)
{
	CWnd* pWnd = GetDlgItem(IDC_STATIC_PHOTO);
	
	m_iScanPos++;
	if( m_iScanPos>175 )
	{
		m_iScanPos = 1;
		m_iStage++;

		if(m_iStage >2)
			m_iStage = 0;
	}
	if(m_iStage == 0)
	{
		GetDlgItem(IDC_STATIC_SCANSTATUS)->SetWindowText("Laser Scan...");
		Stage1();
	}
	if(m_iStage == 1)
	{
		GetDlgItem(IDC_STATIC_SCANSTATUS)->SetWindowText("Infrared Scan...");
		Stage2();
	}
	if(m_iStage == 2)
	{
		GetDlgItem(IDC_STATIC_SCANSTATUS)->SetWindowText("Scan Complete.");
		Stage3();
	}

	pWnd->Invalidate(FALSE);

	CDialog::OnTimer(nIDEvent);
}
void CAboutDlg::Stage1()
{
	CRect rect(0,0,123,m_iScanPos);
	m_dcMask.FillRect(rect,&m_brGreen);

	m_dcDisp.BitBlt(0,0,123,m_iScanPos,&m_dcPhoto,0,0,NOTSRCCOPY);
	m_dcDisp.BitBlt(0,0,123,m_iScanPos,&m_dcMask,0,0,SRCAND);
	m_dcDisp.BitBlt(0,m_iScanPos,123,175-m_iScanPos,&m_dcPhoto,0,0,BLACKNESS);

	m_dcDisp.SelectObject(m_penG1);

	m_dcDisp.MoveTo(0,m_iScanPos);
	m_dcDisp.LineTo(123,m_iScanPos);
	m_dcDisp.MoveTo(0,m_iScanPos+2);
	m_dcDisp.LineTo(123,m_iScanPos+2);

	m_dcDisp.SelectObject(m_penG2);
	m_dcDisp.MoveTo(0,m_iScanPos+1);
	m_dcDisp.LineTo(123,m_iScanPos+1);

}
void CAboutDlg::Stage2()
{
	CRect rect(0,0,123,m_iScanPos);
	m_dcMask.FillRect(rect,&m_brRed);

	m_dcDisp.BitBlt(0,0,123,m_iScanPos,&m_dcPhoto,0,0,NOTSRCCOPY);
	m_dcDisp.BitBlt(0,0,123,m_iScanPos,&m_dcMask,0,0,SRCAND);
	m_dcDisp.BitBlt(0,m_iScanPos,123,175-m_iScanPos,&m_dcPhoto,0,0,BLACKNESS);

	m_dcDisp.SelectObject(m_penR1);

	m_dcDisp.MoveTo(0,m_iScanPos);
	m_dcDisp.LineTo(123,m_iScanPos);
	m_dcDisp.MoveTo(0,m_iScanPos+2);
	m_dcDisp.LineTo(123,m_iScanPos+2);

	m_dcDisp.SelectObject(m_penR2);
	m_dcDisp.MoveTo(0,m_iScanPos+1);
	m_dcDisp.LineTo(123,m_iScanPos+1);
}
void CAboutDlg::Stage3()
{
	m_dcDisp.BitBlt(0,0,123,175,&m_dcPhoto,0,0,SRCCOPY);
	if(m_iScanPos)
	{
		m_dcDisp.SelectObject(m_penWhite);
		m_dcDisp.MoveTo(0,(175-m_iScanPos/2));
		m_dcDisp.LineTo(123,(175-m_iScanPos/2));

		m_dcDisp.MoveTo((0+m_iScanPos/2),0);
		m_dcDisp.LineTo((0+m_iScanPos/2),175);
	}

}
BOOL CAboutDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetTimer(1357,15,NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CAboutDlg::OnCancel()
{
	KillTimer(1357);

	CDialog::OnCancel();
}

void CAboutDlg::OnOK()
{
	KillTimer(1357);

	CDialog::OnOK();
}

HBRUSH CAboutDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	if(pWnd->GetDlgCtrlID() == IDC_STATIC_PHOTO)
	{
		pDC->BitBlt(0,0,123,175,&m_dcDisp,0,0,SRCCOPY);
	}
	return hbr;
}
