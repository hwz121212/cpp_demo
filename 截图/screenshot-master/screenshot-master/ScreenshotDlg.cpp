// CatchScreenDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ScreenshotDlg.h"
#include <math.h>
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CScreenshotDlg dialog

CScreenshotDlg::CScreenshotDlg(CWnd* pParent /*=NULL*/)
: CDialog(CScreenshotDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CScreenshotDlg)
	// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32

	//*******************************************************************************
	//��ʼ����Ƥ����,������resizeMiddle ����
	m_rectTracker.m_nStyle=BaseTracker::resizeMiddle|BaseTracker::solidLine;  
	m_rectTracker.m_rect.SetRect(-1,-2,-3,-4);
	//���þ�����ɫ
	m_rectTracker.SetRectColor(RGB(10,100,130));
	//���þ��ε���ʱ���
	m_rectTracker.SetResizeCursor(32645,32644,32642,32643,32646);

	m_hCursor=AfxGetApp()->LoadCursor(IDC_CURSOR1);  

	m_bDrawImage = FALSE;
	m_bDraw=FALSE;
	m_bFirstDraw=FALSE;
	m_bQuit=FALSE;
	m_nDrawType = 0;
	m_startPt=0;
	m_endPt = 0;
	m_startPt2 = 0;
	m_bFisrtDrawArrow = FALSE;
	m_bMenuVisiable = FALSE;

	for (int i = 0;i<5;i++)
	{
		m_bBtnState[i] = FALSE;
	}
	//memset(m_bBtnState,0,5);
	m_bMoveRect = TRUE;

	//��ȡ��Ļ�ֱ���
	m_xScreen = GetSystemMetrics(SM_CXSCREEN);
	m_yScreen = GetSystemMetrics(SM_CYSCREEN);

	//��ȡ��Ļ��λͼ��
	TCHAR *pszFind, szFilePath[MAX_PATH] = {0};
	GetModuleFileName(NULL, szFilePath, MAX_PATH);
	pszFind=wcsrchr(szFilePath,L'\\')+1;
	*pszFind=0;
	m_csCacheFile.Format(L"%s", szFilePath);
	if (m_csCacheFile.IsEmpty() )
	{
		m_csCacheFile = _T("C:\\");
	}
	TRACE(m_csCacheFile);
	m_csCacheFile += "cache.tmp";

	CRect rect(0, 0,m_xScreen,m_yScreen);
	m_hBitmap = CopyScreenToBitmap(&rect);

	//��ʼ��ˢ�´������� m_rgn
	m_rgn.CreateRectRgn(0,0,50,50);
	//*******************************************************************************

	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}
CScreenshotDlg::~CScreenshotDlg()
{
	POSITION pos = m_list.GetHeadPosition();
	for (int i = 0;i<m_list.GetCount();i++)
	{
		CShape *pShape = m_list.GetNext(pos);
		delete pShape;
	}
	delete m_pDlgBar;
	m_pDlgBar = NULL;
}

void CScreenshotDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);	
	//{{AFX_DATA_MAP(CScreenshotDlg)
	//}}AFX_DATA_MAP
	//DDX_Control(pDX, IDC_EDIT_INPUT, m_Edit);
}

BEGIN_MESSAGE_MAP(CScreenshotDlg, CDialog)
	//{{AFX_MSG_MAP(CScreenshotDlg)
	ON_COMMAND_RANGE(IDC_MENU_RECT,IDC_MENU_SHOWTOOLBAR,OnMenuCommand)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_ERASEBKGND()
	ON_WM_SETCURSOR()
	ON_WM_RBUTTONUP()
	ON_WM_CTLCOLOR()
	ON_MESSAGE (WM_GETCOMMAND,OnGetCommond)
	ON_MESSAGE (WM_UPDATE_TOOLBAR,OnUpdateToolBar)
	//}}AFX_MSG_MAP
	ON_WM_CONTEXTMENU()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CScreenshotDlg message handlers

BOOL CScreenshotDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	//**************************************************************************
	//�ѶԻ������ó�ȫ�����㴰��
	SetWindowPos(&wndTopMost,0,0,m_xScreen,m_yScreen,SWP_SHOWWINDOW);
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	//KillTimer(1001);
	m_rectTracker.SetHandler(this->GetSafeHwnd() );
	CreateDarkBitmapFile();

	m_Edit.Create(ES_MULTILINE | WS_CHILD | WS_VISIBLE | WS_TABSTOP|ES_WANTRETURN| ES_AUTOVSCROLL,
		CRect(10, 10, 100, 100), this, 1);

	VERIFY(m_Font.CreateFont(
		-12,                        // nHeight
		0,                         // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		FW_NORMAL,                 // nWeight
		FALSE,                     // bItalic
		FALSE,                     // bUnderline
		0,                         // cStrikeOut
		GB2312_CHARSET,              // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		DEFAULT_QUALITY,           // nQuality
		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		_T("����") ));                 // lpszFacename

	m_Edit.SetFont(&m_Font);
	m_Edit.Invalidate();
	m_Edit.ShowWindow(SW_HIDE);

	m_pDlgBar = new CDlgBar();
	m_pDlgBar->Create(IDD_DIALOG_BAR,this);

	hMainWnd = ::GetForegroundWindow();
	::ShowWindow(hMainWnd, SW_HIDE);

	this->SetActiveWindow();
	this->SetForegroundWindow();
	this->SetFocus();
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CScreenshotDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		//**************************************************************************
		CPaintDC dc(this);

		//������Ƥ�����
		if(m_bFirstDraw)
		{
			m_rectTracker.Draw(&dc);
		}
		if ( !m_bMoveRect) //��ͼ���򲻿��ƶ�ʱ��ˢ��
		{
			CPen pen(PS_SOLID,1,RGB(255,0,0) );
			CPen *pOldPen = dc.SelectObject(&pen);

			POSITION pos=m_list.GetHeadPosition(); 
			for (int i = 0;i<m_list.GetCount();i++)
			{
				CShape *pShape = m_list.GetNext(pos);
				pShape->Drawing(&dc);
			}

			dc.SelectObject(pOldPen);
			pen.DeleteObject();
		}

		//*************************************************************************
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CScreenshotDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CScreenshotDlg::OnOK() 
{
	// TODO: Add extra validation here
	::ShowWindow(hMainWnd, SW_SHOW);
	EndDialog(IDOK);
	//CDialog::OnOK();
}

void CScreenshotDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	//***************************************************************
	::ShowWindow(hMainWnd, SW_SHOW);
	EndDialog(IDCANCEL);
	if(m_bFirstDraw)
	{
		//ȡ���ѻ����α���
		m_bFirstDraw=FALSE;
		m_bDraw=FALSE;
		m_rectTracker.m_rect.SetRect(-1,-1,-1,-1);
		Invalidate();
	}
	else
	{
		CDialog::OnCancel();
	}
	//*******************************************************************
}

void CScreenshotDlg::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	//**************************************************************************************
	if(m_bDraw)
	{
		//��̬�������δ�С,��ˢ�»���
		m_endPt = point;
		m_rectTracker.m_rect.SetRect(m_startPt.x,m_startPt.y,point.x,point.y);
		Invalidate();
	}

	// ��ͼ��ɺ��ڽ�ͼ�����ͼ
	if (m_bDrawImage && m_nDrawType)
	{
		//��̬�������δ�С,��ˢ�»���
		point.x = point.x>=m_endPt.x?m_endPt.x:point.x;
		point.x = point.x<=m_startPt.x?m_startPt.x:point.x;
		point.y = point.y>=m_endPt.y?m_endPt.y:point.y;
		point.y = point.y<=m_startPt.y?m_startPt.y:point.y;
		CRect rcInvalidate(m_startPt2.x,m_startPt2.y,point.x,point.y);

		if (4 == m_nDrawType)
		{
			m_pointList.AddTail(point);
		}

		CClientDC dc(this);
		CPen pen(PS_SOLID,1,RGB(255,0,0) );
		CPen *pOldPen = dc.SelectObject(&pen);
		DrawImage(&dc,point);
		dc.SelectObject(pOldPen);
		m_prePt = point;

		pen.DeleteObject();
	}

	//*****************************************************************************************
	CDialog::OnMouseMove(nFlags, point);
}

void CScreenshotDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	//*****************************************************************************************
	//ATLTRACE("OnLButtonDown\n");
	int nHitTest;
	nHitTest=m_rectTracker.HitTest(point);

	//�жϻ���λ��
	if(nHitTest<0)
	{
		if(!m_bFirstDraw)
		{
			//��һ�λ�����
			m_startPt = point;
			m_endPt = point;
			m_bDraw=TRUE;
			m_bFirstDraw=TRUE;
			//���õ�����갴��ʱ��С�ľ��δ�С
			m_rectTracker.m_rect.SetRect(point.x, point.y, point.x+8, point.y+8);	

			Invalidate();
		}
	}
	else //û����
	{
		if (m_bMoveRect) //û��ѡ���ͼ����
		{
			Invalidate();

			if(m_bFirstDraw)
			{
				//������Сʱ,Track���Զ��������δ�С,��Щ�ڼ�,��Ϣ��CRectTracker�ڲ�����
				m_rectTracker.Track(this, point, TRUE);
				Invalidate();

			}
		}
		else if (PtInRect(&CRect(m_startPt,m_endPt),point) ) //ѡ�˻�ͼ�����ҵ��ڽ�ͼ������
		{
			if (4 == m_nDrawType)
			{
				m_pointList.AddTail(point);
			}
			if (5 == m_nDrawType  ) //д����
			{
				CRect rc;
				m_Edit.GetWindowRect(&rc);
				ScreenToClient(&rc);

				if (!m_Edit.IsWindowVisible()) //�༭�򲻿ɼ������show����
				{
					int nWith = rc.Width();
					int nheight = rc.Height();
					rc.left = point.x;
					rc.top = point.y;

					// ��֤EDIT��ȫ��ʾ�ڽ�ͼ������
					if (rc.left + 50 < m_endPt.x)
					{
						rc.right = rc.left + 50;
					}
					else
					{
						rc.right = m_endPt.x;
						rc.left = rc.right - 100;
					}
					if (rc.top + 16 < m_endPt.y)
					{
						rc.bottom = rc.top + 16;
					}
					else
					{
						rc.bottom = m_endPt.y;
						rc.top = rc.bottom - 16;
					}
					
					m_Edit.MoveWindow(&rc);
					m_Edit.SetMoveRect(CRect(m_startPt,m_endPt) );
					m_Edit.ShowWindow(SW_SHOW);
					m_Edit.SetFocus();
				}
				else  //�༭��ɼ���������أ�����EDIT�Ĵ�С�����ֱ��棬����������
				{
					m_Edit.ShowWindow(SW_HIDE);
					InvalidateRect(CRect(m_startPt,m_endPt) ); //���߿�ľ��α�EDIT��������Ҫˢ�²������

					CString str,strText;
					int nLineCount = m_Edit.GetLineCount();

					// ��ֹ�Զ�����(���ֶ���Enter)ˢ���ı�ʱ�������
					for (int i = 0;i < nLineCount;i++)
					{
						int len = m_Edit.LineLength(m_Edit.LineIndex(i) );
						m_Edit.GetLine(i, strText.GetBuffer(len), len);
						strText.ReleaseBuffer(len);
						str += strText + _T("\r\n");
					}

					CShape *pShape = new CEditText(rc,str);
					m_list.AddTail(pShape);
					m_Edit.SetWindowText(_T("") );
				}

			}
			m_startPt2 = point;
			m_prePt = point;

			if (m_nDrawType>0 && m_nDrawType<5)
			{
				m_bDrawImage = TRUE;
			}
			

		}

	}
	//****************************************************************************************
	CDialog::OnLButtonDown(nFlags, point);
}

void CScreenshotDlg::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	//****************************************************************************************
	if (!m_nDrawType && m_bDraw )  //��ͼ��굯��ʱ
	{
		CRect rc(m_startPt,point);
		rc.NormalizeRect();
		m_startPt.x = rc.left;
		m_startPt.y = rc.top;
		m_endPt.x = rc.right;
		m_endPt.y = rc.bottom;
		//m_endPt = point;
	}

	// ��ʾ����������
	if (m_bDraw )
	{
	   	CRect rcClient;
		GetClientRect(&rcClient);
		if (rcClient.bottom > m_endPt.y+24 + 8)
		{
			m_pDlgBar->MoveWindow(&CRect(m_endPt.x-(25*8+60),m_endPt.y + 8,m_endPt.x,m_endPt.y+24 + 8));
		}
		else
		{
			m_pDlgBar->MoveWindow(&CRect(m_endPt.x-(25*8+60),m_startPt.y - 8 -24,m_endPt.x,m_startPt.y- 8));
		}
		
		m_pDlgBar->ShowWindow(SW_SHOW);
	}

	// ������Ч��point �����ھ��ε���|�±�����Ϊ��㲻�ھ����ڣ�����-1
	point.x = point.x>=m_endPt.x?m_endPt.x -1:point.x;
	point.x = point.x<=m_startPt.x?m_startPt.x:point.x;
	point.y = point.y>=m_endPt.y?m_endPt.y -1:point.y;
	point.y = point.y<=m_startPt.y?m_startPt.y:point.y;

	// �ѻ�ͼԪ�����ӵ��б�
	if (m_nDrawType && PtInRect(&CRect(m_startPt,m_endPt),point) && m_bDrawImage)
	{

		CShape *pShape = NULL;
		switch (m_nDrawType)
		{
		case 1:
			pShape = new CRectangle(m_startPt2,point);
			m_list.AddTail(pShape);
			break;

		case 2:
			if (m_startPt2 != point)
			{
				pShape = new CLine(m_startPt2,point);
				m_list.AddTail(pShape);
			}
			break;

		case 3:
			pShape = new CEllipse(m_startPt2,point);
			m_list.AddTail(pShape);
			break;

		case 4:
			pShape = new CCurve(&m_pointList);
			m_list.AddTail(pShape);
			m_pointList.RemoveAll();
			break;
		}

		Invalidate();

	}

	m_bDraw=FALSE;
	m_bDrawImage = FALSE;
	
	m_bFisrtDrawArrow = FALSE;
	//****************************************************************************************
	CDialog::OnLButtonUp(nFlags, point);
}

void CScreenshotDlg::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	int nHitTest;
	nHitTest=m_rectTracker.HitTest(point);
	//������Ǿ����ڲ�˫��
	if(nHitTest==8)      
	{
		//������ͼλ��
		if(m_rectTracker.m_rect.top > m_rectTracker.m_rect.bottom)
		{
			LONG temp = m_rectTracker.m_rect.top;
			m_rectTracker.m_rect.top = m_rectTracker.m_rect.bottom;
			m_rectTracker.m_rect.bottom = temp;
		}
		if (m_rectTracker.m_rect.left > m_rectTracker.m_rect.right)
		{
			LONG temp = m_rectTracker.m_rect.left;
			m_rectTracker.m_rect.left = m_rectTracker.m_rect.right;
			m_rectTracker.m_rect.right = temp;
		}
		m_bFirstDraw = FALSE;
		Invalidate();
		UpdateWindow();
		HBITMAP hBitmap = CopyScreenToBitmap(m_rectTracker.m_rect);
		if (m_csSaveFileName.IsEmpty()) {
			SaveBitmapToClipboard(hBitmap);
		}
		else
		{
			SaveBitmapToFile(hBitmap, m_csSaveFileName);
		}
		//GetParent()->SendMessage(WM_BUTTON_CATCHSCREEN, 0, (LPARAM)&hBitmap);
		DeleteObject(hBitmap);
		::ShowWindow(hMainWnd, SW_SHOW);
		EndDialog(IDOK);
	}
	CDialog::OnLButtonDblClk(nFlags, point);
}

void CScreenshotDlg::OnRButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	//****************************************************************************************
	//if(m_bFirstDraw)
	//{
	//	//����Ѿ���ȡ���������ȡ����
	//	m_bFirstDraw=FALSE;
	//	//������δ�С
	//	m_rectTracker.m_rect.SetRect(-1,-1,-1,-1);
	//	Invalidate();
	//}
	//else
	//{
	//	//���
	//	if (OpenClipboard()) 
	//	{
	//		//��ռ�����
	//		EmptyClipboard();
	//		//�رռ�����
	//		CloseClipboard();
	//	}
	//	//�رճ���
	//	PostQuitMessage(0);
	//}
	//****************************************************************************************
	CDialog::OnRButtonUp(nFlags, point);
}

HBRUSH CScreenshotDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);


	if(pWnd->GetDlgCtrlID() == IDC_EDIT_INPUT)
	{
		pDC->SetTextColor(RGB(255,0,0));
		CFont *pFont = this->GetFont();
		LOGFONT lf;
		pFont->GetLogFont(&lf);
	}

	return hbr;
}

BOOL CScreenshotDlg::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	//�������������ȫ���Ի��򱳾�
	CRect rect;
	GetClientRect(&rect);

	CRect rc(m_startPt,m_endPt);
	rc.NormalizeRect();
	ClientToScreen(&rc);

	BITMAP bmp;
	//m_pBitmap->GetBitmap(&bmp);
	GetObject(m_hBitmap,sizeof(bmp),&bmp);

	CDC menDC;
	menDC.CreateCompatibleDC(pDC);
	
	HBITMAP hdarkmap = (HBITMAP)LoadImage(NULL, m_csCacheFile,IMAGE_BITMAP,0,0,LR_CREATEDIBSECTION|LR_LOADFROMFILE);
	CBitmap darkmap;
	darkmap.Attach(hdarkmap);
	CBitmap *pOldBitmap = menDC.SelectObject(&darkmap);

	CDC dc;
	dc.CreateCompatibleDC(pDC);
	CBitmap lightbmp;
	lightbmp.Attach(m_hBitmap);
	CBitmap *pOld = dc.SelectObject(&lightbmp);

	menDC.BitBlt(rc.left,rc.top,rc.Width(),rc.Height(),&dc,rc.left,rc.top,SRCCOPY);
	pDC->BitBlt(0,0,rect.Width(),rect.Height(),&menDC,0,0,SRCCOPY);

	menDC.SelectObject(pOldBitmap);
	dc.SelectObject(pOld);
	darkmap.Detach();
	lightbmp.Detach();
	DeleteObject(hdarkmap);
	DeleteDC(menDC.GetSafeHdc() );
	DeleteDC(dc.GetSafeHdc());
	return TRUE;
	//return CDialog::OnEraseBkgnd(pDC);
}

BOOL CScreenshotDlg::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	CPoint pt;
	GetCursorPos(&pt);
	ScreenToClient(&pt);


	if (pWnd == this &&m_rectTracker.SetCursor(this, nHitTest) && !m_bDraw &&m_bFirstDraw) 
	{
		CRect rc(m_startPt,m_endPt);
		if ( rc.PtInRect(pt) )
		{
			if (m_nDrawType)  //׼����ͼ��ʮ���ι��
			{
				SetCursor(::LoadCursor(AfxGetApp()->m_hInstance,MAKEINTRESOURCE(IDC_CURSOR7) ) );
			}
			else if (m_bMoveRect)  //û�л�ͼ�����϶���ͼ�������ι��
			{
				SetCursor(::LoadCursor(NULL,MAKEINTRESOURCE(IDC_SIZEALL) ) );
			}
			else              //ȡ����ͼ,�Ѿ������϶���ͼ������ͨ���
			{
				SetCursor(::LoadCursor(NULL,MAKEINTRESOURCE(IDC_ARROW) ) );
			}
		}

		return TRUE; 
	}

	//��������Ҽ��˵�������ָ�������ͷ״
	if (m_bMenuVisiable)
	{
		SetCursor(::LoadCursor(NULL,MAKEINTRESOURCE(IDC_ARROW) ) );
		return TRUE;
	}
	
	SetCursor(m_hCursor);
	
	
	return TRUE;

}


HBITMAP CScreenshotDlg::CopyScreenToBitmap(LPRECT lpRect)
//lpRect ����ѡ������
{
	HDC       hScrDC, hMemDC;      
	// ��Ļ���ڴ��豸������
	HBITMAP    hBitmap, hOldBitmap;   
	// λͼ���
	int       nX, nY, nX2, nY2;      
	// ѡ����������
	int       nWidth, nHeight;

	// ȷ��ѡ������Ϊ�վ���
	if (IsRectEmpty(lpRect))
		return NULL;
	//Ϊ��Ļ�����豸������
	hScrDC = CreateDC(L"DISPLAY", NULL, NULL, NULL);

	//Ϊ��Ļ�豸�����������ݵ��ڴ��豸������
	hMemDC = CreateCompatibleDC(hScrDC);
	// ���ѡ����������
	nX = lpRect->left;
	nY = lpRect->top;
	nX2 = lpRect->right;
	nY2 = lpRect->bottom;

	//ȷ��ѡ�������ǿɼ���
	if (nX < 0)
		nX = 0;
	if (nY < 0)
		nY = 0;
	if (nX2 > m_xScreen)
		nX2 = m_xScreen;
	if (nY2 > m_yScreen)
		nY2 = m_yScreen;
	nWidth = nX2 - nX;
	nHeight = nY2 - nY;
	// ����һ������Ļ�豸��������ݵ�λͼ
	hBitmap = CreateCompatibleBitmap
		(hScrDC, nWidth, nHeight);
	// ����λͼѡ���ڴ��豸��������
	hOldBitmap = (HBITMAP)SelectObject(hMemDC, hBitmap);
	// ����Ļ�豸�����������ڴ��豸��������
	//if(bSave)
	//{
	//	//����������DC,��bSaveΪ��ʱ�ѿ�ʼ�����ȫ��λͼ,����ȡ���δ�С����
	//	CDC dcCompatible;
	//	dcCompatible.CreateCompatibleDC(CDC::FromHandle(hMemDC));
	//	dcCompatible.SelectObject(m_pBitmap);
	//       
	//	BitBlt(hMemDC, 0, 0, nWidth, nHeight,
	//		dcCompatible, nX, nY, SRCCOPY);

	//}
	//else
	{
		BitBlt(hMemDC, 0, 0, nWidth, nHeight,
			hScrDC, nX, nY, SRCCOPY);
	}

	hBitmap = (HBITMAP)SelectObject(hMemDC, hOldBitmap);
	//�õ���Ļλͼ�ľ��
	//��� 
	DeleteDC(hScrDC);
	DeleteDC(hMemDC);

	SaveBitmapToFile(hBitmap, m_csCacheFile);
	// ����λͼ���
	return hBitmap;
}

//����λͼ�����а�
void CScreenshotDlg::SaveBitmapToClipboard(HBITMAP hBmp)
{
	if (OpenClipboard())
	{
		//��ռ�����
		EmptyClipboard();
		//����Ļ����ճ������������,
		//hBitmap Ϊ�ղŵ���Ļλͼ���
		SetClipboardData(CF_BITMAP, hBmp);
		//�رռ�����
		CloseClipboard();
	}
}

//����λͼ���ļ���
void CScreenshotDlg::SaveBitmapToFile(HBITMAP hBmp, LPCWSTR lpfilename)
{
	HDC hdc;   //�豸������   
	int ibits;     
	WORD wbitcount;     //��ǰ��ʾ�ֱ�����ÿ��������ռ�ֽ���   

	//λͼ��ÿ��������ռ�ֽ����������ɫ���С��λͼ�������ֽڴ�С��λͼ�ļ���С   ��д���ļ��ֽ���   
	DWORD dwpalettesize=0, dwbmbitssize, dwdibsize, dwwritten;   

	BITMAP bitmap;   //λͼ���Խṹ   
	BITMAPFILEHEADER bmfhdr;   //λͼ�ļ�ͷ�ṹ   
	BITMAPINFOHEADER bi;   //λͼ��Ϣͷ�ṹ   
	LPBITMAPINFOHEADER lpbi;   //ָ��λͼ��Ϣͷ�ṹ   

	//�����ļ��������ڴ�������ɫ����   
	HANDLE fh, hdib, hpal, holdpal=NULL;   

	//����λͼ�ļ�ÿ��������ռ�ֽ���   
	hdc = CreateDC(L"DISPLAY", NULL, NULL, NULL);   
	ibits = GetDeviceCaps(hdc, BITSPIXEL) * GetDeviceCaps(hdc, PLANES);   
	DeleteDC(hdc);   

	if (ibits <=  1)   
		wbitcount =  1;   
	else if (ibits <= 4)   
		wbitcount = 4;   
	else if (ibits <= 8)   
		wbitcount = 8;   
	else if (ibits <= 16)   
		wbitcount = 16;   
	else if (ibits <= 24)   
		wbitcount = 24;   
	else     
		wbitcount  =  32;   

	//�����ɫ���С   
	if (wbitcount <= 8)   
		dwpalettesize = (1 << wbitcount) * sizeof(RGBQUAD);   

	//����λͼ��Ϣͷ�ṹ   
	GetObject(hBmp, sizeof(BITMAP), (LPSTR)&bitmap);   
	bi.biSize   =   sizeof(BITMAPINFOHEADER);   
	bi.biWidth   =   bitmap.bmWidth;   
	bi.biHeight   =   bitmap.bmHeight;   
	bi.biPlanes   =   1;   
	bi.biBitCount   =   wbitcount;   
	bi.biCompression   =   BI_RGB;   
	bi.biSizeImage   =   0;   
	bi.biXPelsPerMeter   =   0;   
	bi.biYPelsPerMeter   =   0;   
	bi.biClrUsed   =   0;   
	bi.biClrImportant   =   0;   

	dwbmbitssize   =   ((bitmap.bmWidth   *   wbitcount+31)/32)*   4   *   bitmap.bmHeight   ;   
	//Ϊλͼ���ݷ����ڴ�   
	hdib   =   GlobalAlloc(GHND,dwbmbitssize   +   dwpalettesize   +     sizeof(BITMAPINFOHEADER));   
	lpbi   =   (LPBITMAPINFOHEADER)GlobalLock(hdib);   
	*lpbi   =   bi;   

	//   �����ɫ��     
	hpal   =   GetStockObject(DEFAULT_PALETTE);   
	if   (hpal)   
	{   
		hdc   =   ::GetDC(NULL);   
		holdpal   =   SelectPalette(hdc,   (HPALETTE)hpal,   false);   
		RealizePalette(hdc);   
	}   

	//   ��ȡ�õ�ɫ�����µ�����ֵ   
	GetDIBits(hdc,   hBmp,   0,   (UINT)   bitmap.bmHeight,(LPSTR)lpbi   +     
		sizeof(BITMAPINFOHEADER)+dwpalettesize,(BITMAPINFO*)lpbi,   DIB_RGB_COLORS);   

	//�ָ���ɫ��     
	if   (holdpal)   
	{   
		SelectPalette(hdc,   (HPALETTE)holdpal,   true);   
		RealizePalette(hdc);   
		::ReleaseDC(NULL,   hdc);   
	}   

	//����λͼ�ļ�     
	fh   =   CreateFile(lpfilename,   GENERIC_WRITE,   0,   NULL,   
		CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL|   
		FILE_FLAG_SEQUENTIAL_SCAN,   NULL);   
	if   (fh   ==   INVALID_HANDLE_VALUE)   
		return;   

	//   ����λͼ�ļ�ͷ   
	bmfhdr.bfType   =   0x4d42;   //   "bm"   
	dwdibsize   =   sizeof(BITMAPFILEHEADER)   +   sizeof(BITMAPINFOHEADER)+     
		dwpalettesize   +   dwbmbitssize;     
	bmfhdr.bfSize   =   dwdibsize;   
	bmfhdr.bfReserved1   =   0;   
	bmfhdr.bfReserved2   =   0;   
	bmfhdr.bfOffBits   =   (DWORD)sizeof(BITMAPFILEHEADER)   +     
		(DWORD)sizeof(BITMAPINFOHEADER)+   dwpalettesize;   

	//   д��λͼ�ļ�ͷ   
	WriteFile(fh,   (LPSTR)&bmfhdr,   sizeof(BITMAPFILEHEADER),   &dwwritten,   NULL);   

	//   д��λͼ�ļ���������   
	WriteFile(fh,   (LPSTR)lpbi,   dwdibsize,   &dwwritten,   NULL);   
	//���     
	GlobalUnlock(hdib);   
	GlobalFree(hdib);   
	CloseHandle(fh);   

}   


//*******************************************************************************************
LRESULT CScreenshotDlg::OnUpdateToolBar(WPARAM wParam, LPARAM lParam)
{
	CPoint *ptEnd = (CPoint*)lParam;
	CPoint *ptStart = (CPoint*)wParam;

	ASSERT(ptEnd && ptStart);

	CRect rc(*ptStart,*ptEnd);
	rc.NormalizeRect();
	// ���϶���������ʱ��Ҫ����������ʾ�����2������(m_startPt��m_endPt)
	m_startPt.x = rc.left;
	m_startPt.y = rc.top;
	m_endPt.x =rc.right;
	m_endPt.y = rc.bottom;

	CRect rcClient;
	GetClientRect(&rcClient);
	if (rcClient.bottom > m_endPt.y + 24 + 8)
	{
		m_pDlgBar->MoveWindow(&CRect(m_endPt.x-(25*8+60),m_endPt.y + 8,m_endPt.x,m_endPt.y+24 + 8) );
	}
	else
	{
		m_pDlgBar->MoveWindow(&CRect(m_endPt.x-(25*8+60),m_startPt.y -8 -24,m_endPt.x,m_startPt.y -8) );
	}

	m_pDlgBar->ShowWindow(SW_SHOW);

	return 0;
}

void CScreenshotDlg::DrawImage(CDC *pDC,CPoint point)
{
	ASSERT(pDC);

	int nType = 0; 
	CBrush *pOldBrush = NULL;

	switch (m_nDrawType)
	{
	case 1:   //������
		pOldBrush = (CBrush*)pDC->SelectStockObject(NULL_BRUSH);
		nType = pDC->SetROP2(R2_NOTXORPEN);
		pDC->Rectangle(&CRect(m_startPt2.x,m_startPt2.y,m_prePt.x,m_prePt.y) );
		pDC->Rectangle(&CRect(m_startPt2.x,m_startPt2.y,point.x,point.y) );

		pDC->SelectObject(pOldBrush);
		pDC->SetROP2(nType);
		break;

	case 2:  //����
		nType = pDC->SetROP2(R2_NOTXORPEN);

		if (m_startPt2 != point)
		{
			if (m_bFisrtDrawArrow)
			{
				DrawArrow(pDC,m_startPt2,m_prePt);
			}	
			m_bFisrtDrawArrow =TRUE;
			DrawArrow(pDC,m_startPt2,point);
		}
		pDC->SetROP2(nType);
		break;

	case 3:  //����Բ
		pOldBrush = (CBrush*)pDC->SelectStockObject(NULL_BRUSH);
		nType = pDC->SetROP2(R2_NOTXORPEN);
		pDC->Ellipse(&CRect(m_startPt2.x,m_startPt2.y,m_prePt.x,m_prePt.y) );
		pDC->Ellipse(&CRect(m_startPt2.x,m_startPt2.y,point.x,point.y) );

		pDC->SelectObject(pOldBrush);
		pDC->SetROP2(nType);
		break;

	case 4: //������
		pDC->MoveTo(m_prePt);
		pDC->LineTo(point);
		break;

		// default:
	}
}
LRESULT CScreenshotDlg::OnGetCommond(WPARAM wParam, LPARAM lParam)
{
	m_bMoveRect = FALSE; //�������ƶ���ͼ������

	int nType =  *(int*)lParam;

	// �ָ�ԭ״
	if (nType > 0 && nType < 6)
	{
		m_pDlgBar->m_btnRect.SetImage(IDB_BITMAP_RECT,25,24);
		m_pDlgBar->m_arrow.SetImage(IDB_BITMAP_ARROW,25,24);
		m_pDlgBar->m_btnEllipse.SetImage(IDB_BITMAP_ELLIPSE,25,24);
		m_pDlgBar->m_btnLine.SetImage(IDB_BITMAP_BRUSH,25,24);
		m_pDlgBar->m_btnStr.SetImage(IDB_BITMAP_FONT,25,24);
	}

	switch (nType)
	{
	case 1:			//����
		if (1 != m_nDrawType || 0 == m_bBtnState[0])
		{
			m_pDlgBar->m_btnRect.SetImage(IDB_BITMAP_RECT2,25,24);
			m_nDrawType = 1;
			m_bBtnState[0] = TRUE;
		}
		else
		{
			m_pDlgBar->m_btnRect.SetImage(IDB_BITMAP_RECT,25,24);
			m_nDrawType = 0;
			m_bBtnState[0] = FALSE;
		}    
		break;

	case 2:
		if (2 != m_nDrawType || 0 == m_bBtnState[1])
		{
			m_pDlgBar->m_arrow.SetImage(IDB_BITMAP_ARROW2,25,24);
			m_nDrawType = 2;
			m_bBtnState[1] = TRUE;
		}
		else
		{
			m_pDlgBar->m_arrow.SetImage(IDB_BITMAP_ARROW,25,24);
			m_nDrawType = 0;
			m_bBtnState[1] = FALSE;

		}    
		break;

	case 3:
		if (3 != m_nDrawType || 0 == m_bBtnState[2])
		{
			m_pDlgBar->m_btnEllipse.SetImage(IDB_BITMAP_ECLLIPSE2,25,24);
			m_nDrawType = 3;
			m_bBtnState[2] = TRUE;
		}
		else
		{
			m_pDlgBar->m_btnEllipse.SetImage(IDB_BITMAP_ELLIPSE,25,24);
			m_nDrawType = 0;
			m_bBtnState[2] = FALSE;

		}    

		break;

	case 4:              //����
		if (4 != m_nDrawType || 0 == m_bBtnState[3])
		{
			m_pDlgBar->m_btnLine.SetImage(IDB_BITMAP_BRUSH2,25,24);
			m_nDrawType = 4;
			m_bBtnState[3] = TRUE;
		}
		else
		{
			m_pDlgBar->m_btnLine.SetImage(IDB_BITMAP_BRUSH,25,24);
			m_nDrawType = 0;
			m_bBtnState[3] = FALSE;
		}    
		break;

	case 5:				 //����		
		//EDIT�ɼ�����ʱ�������壬���EDIT���أ������ı����ݸ��µ�����
		if (m_Edit.IsWindowVisible() )
		{
			m_Edit.ShowWindow(SW_HIDE);
			InvalidateRect(CRect(m_startPt,m_endPt) ); //���߿�ľ��α�EDIT��������Ҫˢ�²������

			CRect rc;
			m_Edit.GetWindowRect(&rc);
			ScreenToClient(&rc);

			CString str,strText;
			int nLineCount = m_Edit.GetLineCount();

			// ��ֹ�Զ�����(���ֶ���Enter)ˢ���ı�ʱ�������
			for (int i = 0;i < nLineCount;i++)
			{
				int len = m_Edit.LineLength(m_Edit.LineIndex(i) );
				m_Edit.GetLine(i, strText.GetBuffer(len), len);
				strText.ReleaseBuffer(len);
				str += strText + _T("\r\n");
			}

			CString strTitle;
			m_Edit.GetWindowText(strTitle);
			if (!strTitle.IsEmpty() )
			{
				CShape *pShape = new CEditText(rc,str);
				m_list.AddTail(pShape);
				m_Edit.SetWindowText(_T("") );
			}

		}

		// ���ư�ť״̬
		if (5 != m_nDrawType || 0 == m_bBtnState[4])
		{
			m_pDlgBar->m_btnStr.SetImage(IDB_BITMAP_FONT2,25,24);
			m_nDrawType = 5;
			m_bBtnState[4] = TRUE;
		}
		else
		{
			m_pDlgBar->m_btnStr.SetImage(IDB_BITMAP_FONT,25,24);
			m_nDrawType = 0;
			m_bBtnState[4] = FALSE;
		}    
		break;

	case 6:              //������һ������
		if (m_list.GetCount() )
		{
			CShape *pShape = m_list.GetTail();
			delete pShape;
			pShape = NULL;
			m_list.RemoveTail();
			Invalidate();
		}
		else
		{
			Reset();
		}
		break;

	case 7:              //�����ͼ
		{
			//����Ĭ���ļ���
			CTime   time   =   CTime::GetCurrentTime(); 
			CString  str = _T("��ͼ_")+time.Format("%Y%m%d%H%M%S")+_T(".bmp");

			CFileDialog   dlg(   FALSE,NULL,str,
				OFN_HIDEREADONLY   |   OFN_OVERWRITEPROMPT,   
				_T("BMP(*.bmp) || "),     this  );

			HBITMAP hBitmap = CopyScreenToBitmap(&CRect(m_startPt,m_endPt));
			if (IDOK == dlg.DoModal())
			{
				// ������ͼ�е�8����
				m_bFirstDraw = FALSE;
				Invalidate();
				UpdateWindow();

				CString csFileName = dlg.GetPathName();
				SaveBitmapToFile(hBitmap,csFileName);
				m_csSaveFileName = csFileName;
				::ShowWindow(hMainWnd, SW_SHOW);
				EndDialog(IDOK);
			}
			break;

		}

	case 8:             //ȡ����ͼ�˳�
		::ShowWindow(hMainWnd, SW_SHOW);
		EndDialog(IDCANCEL);
		break;

	case 9:            //��ɽ�ͼ�˳�
		if (m_rectTracker.m_rect.top > m_rectTracker.m_rect.bottom)
		{
			LONG temp = m_rectTracker.m_rect.top;
			m_rectTracker.m_rect.top = m_rectTracker.m_rect.bottom;
			m_rectTracker.m_rect.bottom = temp;
		}
		if (m_rectTracker.m_rect.left > m_rectTracker.m_rect.right)
		{
			LONG temp = m_rectTracker.m_rect.left;
			m_rectTracker.m_rect.left = m_rectTracker.m_rect.right;
			m_rectTracker.m_rect.right = temp;
		} 
		//����λͼ��ճ����,bSave ΪTRUE,
		m_bFirstDraw = FALSE;
		Invalidate();
		UpdateWindow();
		HBITMAP hBitmap = CopyScreenToBitmap(m_rectTracker.m_rect);
		if (m_csSaveFileName.IsEmpty())
		{
			SaveBitmapToClipboard(hBitmap);
		}
		else
		{
			SaveBitmapToFile(hBitmap, m_csSaveFileName);
		}
//		GetParent()->SendMessage(WM_BUTTON_CATCHSCREEN, 0, (LPARAM)&hBitmap);
		DeleteObject(hBitmap);
		::ShowWindow(hMainWnd, SW_SHOW);
		EndDialog(IDOK);
		break;

	}

	if (nType > 0 && nType < 5 && m_Edit.IsWindowVisible() )
	{
		CRect rc;
		m_Edit.GetWindowRect(&rc);
		ScreenToClient(&rc);

		m_Edit.ShowWindow(SW_HIDE);
		InvalidateRect(CRect(m_startPt,m_endPt) ); //���߿�ľ��α�EDIT��������Ҫˢ�²������

		CString str,strText;
		int nLineCount = m_Edit.GetLineCount();

		// ��ֹ�Զ�����(���ֶ���Enter)ˢ���ı�ʱ�������
		for (int i = 0;i < nLineCount;i++)
		{
			int len = m_Edit.LineLength(m_Edit.LineIndex(i) );
			m_Edit.GetLine(i, strText.GetBuffer(len), len);
			strText.ReleaseBuffer(len);
			str += strText + _T("\r\n");
		}

		CShape *pShape = new CEditText(rc,str);
		m_list.AddTail(pShape);
		m_Edit.SetWindowText(_T("") );
		InvalidateRect(CRect(m_startPt,m_endPt) );
	}

	return 0;
}



void CScreenshotDlg::OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/)
{
	// TODO: �ڴ˴������Ϣ����������
	POINT pt;
	GetCursorPos(&pt);

	//���ڽ�ͼ���η�Χ�����˳���ͼ
	CPoint pt1 = pt;
	ScreenToClient(&pt1);

	CRect rc(m_startPt,m_endPt);
	if (rc.IsRectEmpty()  )
	{
		::ShowWindow(hMainWnd, SW_SHOW);
		EndDialog(IDCANCEL);
		return;
	}

	if ( !rc.PtInRect(pt1) )
	{
		Reset();
		return;
	}
	
	CMenu muEC;
	muEC.CreatePopupMenu();
	CBitmap bmp;
	bmp.LoadBitmap(IDB_BMP_RECT);
	muEC.AppendMenu(MF_STRING, IDC_MENU_RECT, _T("���ι���"));
	muEC.SetMenuItemBitmaps(IDC_MENU_RECT, MF_BYCOMMAND, &bmp, &bmp);
	bmp.Detach();
	
	bmp.LoadBitmap(IDB_BMP_ARROW);
	muEC.AppendMenu(MF_STRING, IDC_MENU_ARROW, _T("��ͷ����"));
	muEC.SetMenuItemBitmaps(IDC_MENU_ARROW, MF_BYCOMMAND, &bmp, &bmp);
	bmp.Detach();

	bmp.LoadBitmap(IDB_BMP_ECLLIPSE);
	muEC.AppendMenu(MF_STRING, IDC_MENU_ECLLIPSE, _T("��Բ����"));
	muEC.SetMenuItemBitmaps(IDC_MENU_ECLLIPSE, MF_BYCOMMAND, &bmp, &bmp);
	bmp.Detach();

	bmp.LoadBitmap(IDB_BMP_BRUSH);
	muEC.AppendMenu(MF_STRING, IDC_MENU_CURVE, _T("���߹���"));
	muEC.SetMenuItemBitmaps(IDC_MENU_CURVE, MF_BYCOMMAND, &bmp, &bmp);
	bmp.Detach();

	bmp.LoadBitmap(IDB_BMP_FONT);
	muEC.AppendMenu(MF_STRING, IDC_MENU_TEXT, _T("���ֹ���"));
	muEC.SetMenuItemBitmaps(IDC_MENU_TEXT, MF_BYCOMMAND, &bmp, &bmp);
	bmp.Detach();

	bmp.LoadBitmap(IDB_BMP_BACK);
	muEC.AppendMenu(MF_STRING, IDC_MENU_BACK, _T("����"));
	muEC.SetMenuItemBitmaps(IDC_MENU_BACK, MF_BYCOMMAND, &bmp, &bmp);
	bmp.Detach();

	muEC.AppendMenu(MF_SEPARATOR);

	muEC.AppendMenu(MF_STRING, IDC_MENU_SELETE, _T("����ѡ��"));
	muEC.SetMenuItemBitmaps(IDC_MENU_SELETE, MF_BYCOMMAND, &bmp, &bmp);

	bmp.LoadBitmap(IDB_BMP_OK);
	muEC.AppendMenu(MF_STRING, IDC_MENU_OK, _T("��ɽ�ͼ"));
	muEC.SetMenuItemBitmaps(IDC_MENU_OK, MF_BYCOMMAND, &bmp, &bmp);
	bmp.Detach();

	bmp.LoadBitmap(IDB_BMP_SAVE);
	muEC.AppendMenu(MF_STRING, IDC_MENU_SAVE, _T("����"));
	muEC.SetMenuItemBitmaps(IDC_MENU_SAVE, MF_BYCOMMAND, &bmp, &bmp);
	bmp.Detach();

	muEC.AppendMenu(MF_SEPARATOR);
	CString str;
	if (m_pDlgBar->IsWindowVisible() )
	{
		str = _T("���ع�����");
	}
	else
	{
		str = _T("��ʾ������");
	}
	muEC.AppendMenu(MF_STRING, IDC_MENU_SHOWTOOLBAR, str);
	muEC.SetMenuItemBitmaps(IDC_MENU_SHOWTOOLBAR, MF_BYCOMMAND, &bmp, &bmp);

	bmp.LoadBitmap(IDB_BMP_CANCEL);
	muEC.AppendMenu(MF_STRING, IDC_MENU_CANCEL, _T("�˳���ͼ"));
	muEC.SetMenuItemBitmaps(IDC_MENU_CANCEL, MF_BYCOMMAND, &bmp, &bmp);
	bmp.Detach();

	m_bMenuVisiable = TRUE; //�˵��ɼ�ʱ���������ȥ������ʾ(����ʾ��ɫ���)

	muEC.TrackPopupMenu(TPM_RIGHTBUTTON,pt.x,pt.y,this);
	muEC.DestroyMenu();

	m_bMenuVisiable = FALSE;

}

void CScreenshotDlg::OnMenuCommand(UINT nID)
{
	int nType = 0;

	if (nID >= IDC_MENU_RECT && nID <=IDC_MENU_OK)
	{
		nType = nID - IDC_MENU_RECT + 1;
		SendMessage(WM_GETCOMMAND,0,(LPARAM)&nType);
	}

	if (IDC_MENU_SHOWTOOLBAR == nID)
	{
		if (m_pDlgBar->IsWindowVisible() )
		{
			m_pDlgBar->ShowWindow(SW_HIDE);
		}
		else
		{
			m_pDlgBar->ShowWindow(SW_SHOW);
		}
	}

	if (IDC_MENU_SELETE == nID)
	{
		Reset();
	}
	
	
}

void CScreenshotDlg::Reset()
{
	m_bDrawImage = FALSE;
	m_bDraw=FALSE;
	m_bFirstDraw=FALSE;
	m_bQuit=FALSE;
	m_nDrawType = 0;
	m_startPt=0;
	m_endPt = 0;
	m_startPt2 = 0;

	for (int i = 0;i<5;i++)
	{
		m_bBtnState[i] = FALSE;
	}
	m_bMoveRect = TRUE;

	POSITION pos = m_list.GetHeadPosition();
	for (int i = 0;i<m_list.GetCount();i++)
	{
		CShape *pShape = m_list.GetNext(pos);
		delete pShape;
	}
	m_list.RemoveAll();

	// �ָ�ԭ״
	m_pDlgBar->ShowWindow(SW_HIDE);
	m_pDlgBar->m_btnRect.SetImage(IDB_BITMAP_RECT,25,24);
	m_pDlgBar->m_arrow.SetImage(IDB_BITMAP_ARROW,25,24);
	m_pDlgBar->m_btnEllipse.SetImage(IDB_BITMAP_ELLIPSE,25,24);
	m_pDlgBar->m_btnLine.SetImage(IDB_BITMAP_BRUSH,25,24);
	m_pDlgBar->m_btnStr.SetImage(IDB_BITMAP_FONT,25,24);

	m_rectTracker.m_rect.SetRect(-1,-2,-3,-4);

	if (m_Edit.IsWindowVisible() )
	{
		m_Edit.SetWindowText(_T("") );
		m_Edit.ShowWindow(SW_HIDE);
	}
	Invalidate();
}

void CScreenshotDlg::DrawArrow(CDC *pDC,CPoint ptStart,CPoint ptEnd)
{
	ASSERT(pDC);


	double slopy , cosy , siny;
	double Par = 10.0;  //length of Arrow (>)
	slopy = atan2( double( ptStart.y - ptEnd.y ),double( ptStart.x - ptEnd.x ) );
	cosy = cos( slopy );
	siny = sin( slopy ); //need math.h for these functions

	//�����μ�ͷ������2����	
	CPoint pt1( ptEnd.x + int( Par * cosy - ( Par / 2.0 * siny ) ),ptEnd.y + int( Par * siny + ( Par / 2.0 * cosy ) ));
	CPoint pt2(ptEnd.x + int( Par * cosy + Par / 2.0 * siny ),ptEnd.y - int( Par / 2.0 * cosy - Par * siny ));

	//�����������εײ��յ㻭����
	pDC->MoveTo( ptStart );
	pDC->LineTo( CPoint((pt1.x + pt2.x)/2,(pt1.y+pt2.y)/2) );

	CPoint ptArray[3] = {ptEnd,pt1,pt2};
	//pDC->Polygon(ptArray,3);
	HRGN hRgn = CreatePolygonRgn(ptArray,3,ALTERNATE);
	CBrush brush(RGB(255,0,0));//
	CBrush *pBrush = pDC->SelectObject(&brush);
	pDC->FillRgn(CRgn::FromHandle(hRgn),&brush);
	pDC->SelectObject(pBrush);
}

void CScreenshotDlg::CreateDarkBitmapFile()
{
	CDC *pDC = GetDC();
	HBITMAP   hbmp=(HBITMAP)LoadImage(NULL, m_csCacheFile,IMAGE_BITMAP,0,0,LR_CREATEDIBSECTION|LR_LOADFROMFILE); 
	CBitmap   cbmp; 
	cbmp.Attach(hbmp); 
	BITMAP   bmp; 
	cbmp.GetBitmap(&bmp); 
	cbmp.Detach(); 
	UINT   *   pData   =   new   UINT[bmp.bmWidth   *   bmp.bmHeight]; 
	BITMAPINFO   bmpInfo; 
	bmpInfo.bmiHeader.biSize   =   sizeof(BITMAPINFOHEADER); 
	bmpInfo.bmiHeader.biWidth   =   bmp.bmWidth; 
	bmpInfo.bmiHeader.biHeight   =   -bmp.bmHeight; 
	bmpInfo.bmiHeader.biPlanes   =   1; 
	bmpInfo.bmiHeader.biCompression   =   BI_RGB; 
	bmpInfo.bmiHeader.biBitCount   =   32; 

	GetDIBits(pDC-> m_hDC,hbmp,0,bmp.bmHeight,pData,&bmpInfo,DIB_RGB_COLORS); 
	UINT   color,   r,   g,   b; 
	for(int i = 0;i < bmp.bmWidth*bmp.bmHeight;i++) 
	{ 
		color   =   pData[i]; 
		b   =   (color   <<   8   >>   24)*0.7; 
		g   =   (color   <<   16   >>   24)*0.7; 
		r   =   (color   <<   24   >>   24)*0.7; 
		//note   that   infact,   the   r   is   Blue,   b   =   Red, 
		//r   =   0;//mask   the   blue   bits 
		pData[i]   =   RGB(r,   g,   b); 
	} 
	SetDIBits(pDC-> m_hDC,   hbmp,0,   bmp.bmHeight,   pData,&bmpInfo,   DIB_RGB_COLORS); 
	SaveBitmapToFile(hbmp, m_csCacheFile);
	/*CDC   dcmem; 
	dcmem.CreateCompatibleDC(pDC); 
	HGDIOBJ   hold=::SelectObject(dcmem.m_hDC,hbmp); 
	pDC-> BitBlt(0,0,bmp.bmWidth,bmp.bmHeight,&dcmem,0,0,SRCCOPY); 
	::SelectObject(dcmem.m_hDC,hold); */
	delete   []pData;
	DeleteObject(hbmp);
}
