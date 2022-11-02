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
	//初始化像皮筋类,新增的resizeMiddle 类型
	m_rectTracker.m_nStyle=BaseTracker::resizeMiddle|BaseTracker::solidLine;  
	m_rectTracker.m_rect.SetRect(-1,-2,-3,-4);
	//设置矩形颜色
	m_rectTracker.SetRectColor(RGB(10,100,130));
	//设置矩形调整时光标
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

	//获取屏幕分辩率
	m_xScreen = GetSystemMetrics(SM_CXSCREEN);
	m_yScreen = GetSystemMetrics(SM_CYSCREEN);

	//截取屏幕到位图中
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

	//初始化刷新窗口区域 m_rgn
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
	//把对化框设置成全屏顶层窗口
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
		_T("宋体") ));                 // lpszFacename

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

		//画出像皮筋矩形
		if(m_bFirstDraw)
		{
			m_rectTracker.Draw(&dc);
		}
		if ( !m_bMoveRect) //截图区域不可移动时才刷新
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
		//取消已画矩形变量
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
		//动态调整矩形大小,并刷新画出
		m_endPt = point;
		m_rectTracker.m_rect.SetRect(m_startPt.x,m_startPt.y,point.x,point.y);
		Invalidate();
	}

	// 截图完成后在截图区域绘图
	if (m_bDrawImage && m_nDrawType)
	{
		//动态调整矩形大小,并刷新画出
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

	//判断击中位置
	if(nHitTest<0)
	{
		if(!m_bFirstDraw)
		{
			//第一次画矩形
			m_startPt = point;
			m_endPt = point;
			m_bDraw=TRUE;
			m_bFirstDraw=TRUE;
			//设置当当鼠标按下时最小的矩形大小
			m_rectTracker.m_rect.SetRect(point.x, point.y, point.x+8, point.y+8);	

			Invalidate();
		}
	}
	else //没击中
	{
		if (m_bMoveRect) //没有选择绘图类型
		{
			Invalidate();

			if(m_bFirstDraw)
			{
				//调束大小时,Track会自动调整矩形大小,在些期间,消息归CRectTracker内部处理
				m_rectTracker.Track(this, point, TRUE);
				Invalidate();

			}
		}
		else if (PtInRect(&CRect(m_startPt,m_endPt),point) ) //选了绘图类型且点在截图区域内
		{
			if (4 == m_nDrawType)
			{
				m_pointList.AddTail(point);
			}
			if (5 == m_nDrawType  ) //写文字
			{
				CRect rc;
				m_Edit.GetWindowRect(&rc);
				ScreenToClient(&rc);

				if (!m_Edit.IsWindowVisible()) //编辑框不可见，点击show出来
				{
					int nWith = rc.Width();
					int nheight = rc.Height();
					rc.left = point.x;
					rc.top = point.y;

					// 保证EDIT完全显示在截图区域内
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
				else  //编辑框可见，点击隐藏，并将EDIT的大小，文字保存，最后清除内容
				{
					m_Edit.ShowWindow(SW_HIDE);
					InvalidateRect(CRect(m_startPt,m_endPt) ); //虚线框的矩形比EDIT大，所以需要刷新搽除虚线

					CString str,strText;
					int nLineCount = m_Edit.GetLineCount();

					// 防止自动换行(非手动按Enter)刷新文本时单行输出
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
	if (!m_nDrawType && m_bDraw )  //截图鼠标弹起时
	{
		CRect rc(m_startPt,point);
		rc.NormalizeRect();
		m_startPt.x = rc.left;
		m_startPt.y = rc.top;
		m_endPt.x = rc.right;
		m_endPt.y = rc.bottom;
		//m_endPt = point;
	}

	// 显示浮动工具条
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

	// 计算有效的point 点落在矩形的右|下边则认为这点不在矩形内，所以-1
	point.x = point.x>=m_endPt.x?m_endPt.x -1:point.x;
	point.x = point.x<=m_startPt.x?m_startPt.x:point.x;
	point.y = point.y>=m_endPt.y?m_endPt.y -1:point.y;
	point.y = point.y<=m_startPt.y?m_startPt.y:point.y;

	// 把绘图元素增加到列表
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
	//如果在是矩形内部双击
	if(nHitTest==8)      
	{
		//调整截图位置
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
	//	//如果已经截取矩则清除截取矩形
	//	m_bFirstDraw=FALSE;
	//	//清除矩形大小
	//	m_rectTracker.m_rect.SetRect(-1,-1,-1,-1);
	//	Invalidate();
	//}
	//else
	//{
	//	//清空
	//	if (OpenClipboard()) 
	//	{
	//		//清空剪贴板
	//		EmptyClipboard();
	//		//关闭剪贴板
	//		CloseClipboard();
	//	}
	//	//关闭程序
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
	//用整个桌面填充全屏对话框背景
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
			if (m_nDrawType)  //准备绘图，十字形光标
			{
				SetCursor(::LoadCursor(AfxGetApp()->m_hInstance,MAKEINTRESOURCE(IDC_CURSOR7) ) );
			}
			else if (m_bMoveRect)  //没有绘图，可拖动截图区域，菱形光标
			{
				SetCursor(::LoadCursor(NULL,MAKEINTRESOURCE(IDC_SIZEALL) ) );
			}
			else              //取消绘图,已经不可拖动截图区域，普通光标
			{
				SetCursor(::LoadCursor(NULL,MAKEINTRESOURCE(IDC_ARROW) ) );
			}
		}

		return TRUE; 
	}

	//如果弹出右键菜单，则光标恢复正常箭头状
	if (m_bMenuVisiable)
	{
		SetCursor(::LoadCursor(NULL,MAKEINTRESOURCE(IDC_ARROW) ) );
		return TRUE;
	}
	
	SetCursor(m_hCursor);
	
	
	return TRUE;

}


HBITMAP CScreenshotDlg::CopyScreenToBitmap(LPRECT lpRect)
//lpRect 代表选定区域
{
	HDC       hScrDC, hMemDC;      
	// 屏幕和内存设备描述表
	HBITMAP    hBitmap, hOldBitmap;   
	// 位图句柄
	int       nX, nY, nX2, nY2;      
	// 选定区域坐标
	int       nWidth, nHeight;

	// 确保选定区域不为空矩形
	if (IsRectEmpty(lpRect))
		return NULL;
	//为屏幕创建设备描述表
	hScrDC = CreateDC(L"DISPLAY", NULL, NULL, NULL);

	//为屏幕设备描述表创建兼容的内存设备描述表
	hMemDC = CreateCompatibleDC(hScrDC);
	// 获得选定区域坐标
	nX = lpRect->left;
	nY = lpRect->top;
	nX2 = lpRect->right;
	nY2 = lpRect->bottom;

	//确保选定区域是可见的
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
	// 创建一个与屏幕设备描述表兼容的位图
	hBitmap = CreateCompatibleBitmap
		(hScrDC, nWidth, nHeight);
	// 把新位图选到内存设备描述表中
	hOldBitmap = (HBITMAP)SelectObject(hMemDC, hBitmap);
	// 把屏幕设备描述表拷贝到内存设备描述表中
	//if(bSave)
	//{
	//	//创建军兼容DC,当bSave为中时把开始保存的全屏位图,按截取矩形大小保存
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
	//得到屏幕位图的句柄
	//清除 
	DeleteDC(hScrDC);
	DeleteDC(hMemDC);

	SaveBitmapToFile(hBitmap, m_csCacheFile);
	// 返回位图句柄
	return hBitmap;
}

//保存位图到剪切板
void CScreenshotDlg::SaveBitmapToClipboard(HBITMAP hBmp)
{
	if (OpenClipboard())
	{
		//清空剪贴板
		EmptyClipboard();
		//把屏幕内容粘贴到剪贴板上,
		//hBitmap 为刚才的屏幕位图句柄
		SetClipboardData(CF_BITMAP, hBmp);
		//关闭剪贴板
		CloseClipboard();
	}
}

//保存位图到文件中
void CScreenshotDlg::SaveBitmapToFile(HBITMAP hBmp, LPCWSTR lpfilename)
{
	HDC hdc;   //设备描述表   
	int ibits;     
	WORD wbitcount;     //当前显示分辨率下每个像素所占字节数   

	//位图中每个像素所占字节数，定义调色板大小，位图中像素字节大小，位图文件大小   ，写入文件字节数   
	DWORD dwpalettesize=0, dwbmbitssize, dwdibsize, dwwritten;   

	BITMAP bitmap;   //位图属性结构   
	BITMAPFILEHEADER bmfhdr;   //位图文件头结构   
	BITMAPINFOHEADER bi;   //位图信息头结构   
	LPBITMAPINFOHEADER lpbi;   //指向位图信息头结构   

	//定义文件，分配内存句柄，调色板句柄   
	HANDLE fh, hdib, hpal, holdpal=NULL;   

	//计算位图文件每个像素所占字节数   
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

	//计算调色板大小   
	if (wbitcount <= 8)   
		dwpalettesize = (1 << wbitcount) * sizeof(RGBQUAD);   

	//设置位图信息头结构   
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
	//为位图内容分配内存   
	hdib   =   GlobalAlloc(GHND,dwbmbitssize   +   dwpalettesize   +     sizeof(BITMAPINFOHEADER));   
	lpbi   =   (LPBITMAPINFOHEADER)GlobalLock(hdib);   
	*lpbi   =   bi;   

	//   处理调色板     
	hpal   =   GetStockObject(DEFAULT_PALETTE);   
	if   (hpal)   
	{   
		hdc   =   ::GetDC(NULL);   
		holdpal   =   SelectPalette(hdc,   (HPALETTE)hpal,   false);   
		RealizePalette(hdc);   
	}   

	//   获取该调色板下新的像素值   
	GetDIBits(hdc,   hBmp,   0,   (UINT)   bitmap.bmHeight,(LPSTR)lpbi   +     
		sizeof(BITMAPINFOHEADER)+dwpalettesize,(BITMAPINFO*)lpbi,   DIB_RGB_COLORS);   

	//恢复调色板     
	if   (holdpal)   
	{   
		SelectPalette(hdc,   (HPALETTE)holdpal,   true);   
		RealizePalette(hdc);   
		::ReleaseDC(NULL,   hdc);   
	}   

	//创建位图文件     
	fh   =   CreateFile(lpfilename,   GENERIC_WRITE,   0,   NULL,   
		CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL|   
		FILE_FLAG_SEQUENTIAL_SCAN,   NULL);   
	if   (fh   ==   INVALID_HANDLE_VALUE)   
		return;   

	//   设置位图文件头   
	bmfhdr.bfType   =   0x4d42;   //   "bm"   
	dwdibsize   =   sizeof(BITMAPFILEHEADER)   +   sizeof(BITMAPINFOHEADER)+     
		dwpalettesize   +   dwbmbitssize;     
	bmfhdr.bfSize   =   dwdibsize;   
	bmfhdr.bfReserved1   =   0;   
	bmfhdr.bfReserved2   =   0;   
	bmfhdr.bfOffBits   =   (DWORD)sizeof(BITMAPFILEHEADER)   +     
		(DWORD)sizeof(BITMAPINFOHEADER)+   dwpalettesize;   

	//   写入位图文件头   
	WriteFile(fh,   (LPSTR)&bmfhdr,   sizeof(BITMAPFILEHEADER),   &dwwritten,   NULL);   

	//   写入位图文件其余内容   
	WriteFile(fh,   (LPSTR)lpbi,   dwdibsize,   &dwwritten,   NULL);   
	//清除     
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
	// 当拖动矩形区域时需要更新用来表示区域的2个变量(m_startPt和m_endPt)
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
	case 1:   //画矩形
		pOldBrush = (CBrush*)pDC->SelectStockObject(NULL_BRUSH);
		nType = pDC->SetROP2(R2_NOTXORPEN);
		pDC->Rectangle(&CRect(m_startPt2.x,m_startPt2.y,m_prePt.x,m_prePt.y) );
		pDC->Rectangle(&CRect(m_startPt2.x,m_startPt2.y,point.x,point.y) );

		pDC->SelectObject(pOldBrush);
		pDC->SetROP2(nType);
		break;

	case 2:  //画线
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

	case 3:  //画椭圆
		pOldBrush = (CBrush*)pDC->SelectStockObject(NULL_BRUSH);
		nType = pDC->SetROP2(R2_NOTXORPEN);
		pDC->Ellipse(&CRect(m_startPt2.x,m_startPt2.y,m_prePt.x,m_prePt.y) );
		pDC->Ellipse(&CRect(m_startPt2.x,m_startPt2.y,point.x,point.y) );

		pDC->SelectObject(pOldBrush);
		pDC->SetROP2(nType);
		break;

	case 4: //画曲线
		pDC->MoveTo(m_prePt);
		pDC->LineTo(point);
		break;

		// default:
	}
}
LRESULT CScreenshotDlg::OnGetCommond(WPARAM wParam, LPARAM lParam)
{
	m_bMoveRect = FALSE; //不能再移动截图区域了

	int nType =  *(int*)lParam;

	// 恢复原状
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
	case 1:			//矩形
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

	case 4:              //曲线
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

	case 5:				 //文字		
		//EDIT可见，此时点了字体，则把EDIT隐藏，并把文本内容更新到区域
		if (m_Edit.IsWindowVisible() )
		{
			m_Edit.ShowWindow(SW_HIDE);
			InvalidateRect(CRect(m_startPt,m_endPt) ); //虚线框的矩形比EDIT大，所以需要刷新搽除虚线

			CRect rc;
			m_Edit.GetWindowRect(&rc);
			ScreenToClient(&rc);

			CString str,strText;
			int nLineCount = m_Edit.GetLineCount();

			// 防止自动换行(非手动按Enter)刷新文本时单行输出
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

		// 控制按钮状态
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

	case 6:              //撤销上一步操作
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

	case 7:              //保存截图
		{
			//设置默认文件名
			CTime   time   =   CTime::GetCurrentTime(); 
			CString  str = _T("截图_")+time.Format("%Y%m%d%H%M%S")+_T(".bmp");

			CFileDialog   dlg(   FALSE,NULL,str,
				OFN_HIDEREADONLY   |   OFN_OVERWRITEPROMPT,   
				_T("BMP(*.bmp) || "),     this  );

			HBITMAP hBitmap = CopyScreenToBitmap(&CRect(m_startPt,m_endPt));
			if (IDOK == dlg.DoModal())
			{
				// 消除截图中的8个点
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

	case 8:             //取消截图退出
		::ShowWindow(hMainWnd, SW_SHOW);
		EndDialog(IDCANCEL);
		break;

	case 9:            //完成截图退出
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
		//保存位图到粘贴板,bSave 为TRUE,
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
		InvalidateRect(CRect(m_startPt,m_endPt) ); //虚线框的矩形比EDIT大，所以需要刷新搽除虚线

		CString str,strText;
		int nLineCount = m_Edit.GetLineCount();

		// 防止自动换行(非手动按Enter)刷新文本时单行输出
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
	// TODO: 在此处添加消息处理程序代码
	POINT pt;
	GetCursorPos(&pt);

	//点在截图矩形范围外则退出截图
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
	muEC.AppendMenu(MF_STRING, IDC_MENU_RECT, _T("矩形工具"));
	muEC.SetMenuItemBitmaps(IDC_MENU_RECT, MF_BYCOMMAND, &bmp, &bmp);
	bmp.Detach();
	
	bmp.LoadBitmap(IDB_BMP_ARROW);
	muEC.AppendMenu(MF_STRING, IDC_MENU_ARROW, _T("箭头工具"));
	muEC.SetMenuItemBitmaps(IDC_MENU_ARROW, MF_BYCOMMAND, &bmp, &bmp);
	bmp.Detach();

	bmp.LoadBitmap(IDB_BMP_ECLLIPSE);
	muEC.AppendMenu(MF_STRING, IDC_MENU_ECLLIPSE, _T("椭圆工具"));
	muEC.SetMenuItemBitmaps(IDC_MENU_ECLLIPSE, MF_BYCOMMAND, &bmp, &bmp);
	bmp.Detach();

	bmp.LoadBitmap(IDB_BMP_BRUSH);
	muEC.AppendMenu(MF_STRING, IDC_MENU_CURVE, _T("曲线工具"));
	muEC.SetMenuItemBitmaps(IDC_MENU_CURVE, MF_BYCOMMAND, &bmp, &bmp);
	bmp.Detach();

	bmp.LoadBitmap(IDB_BMP_FONT);
	muEC.AppendMenu(MF_STRING, IDC_MENU_TEXT, _T("文字工具"));
	muEC.SetMenuItemBitmaps(IDC_MENU_TEXT, MF_BYCOMMAND, &bmp, &bmp);
	bmp.Detach();

	bmp.LoadBitmap(IDB_BMP_BACK);
	muEC.AppendMenu(MF_STRING, IDC_MENU_BACK, _T("撤销"));
	muEC.SetMenuItemBitmaps(IDC_MENU_BACK, MF_BYCOMMAND, &bmp, &bmp);
	bmp.Detach();

	muEC.AppendMenu(MF_SEPARATOR);

	muEC.AppendMenu(MF_STRING, IDC_MENU_SELETE, _T("重新选择"));
	muEC.SetMenuItemBitmaps(IDC_MENU_SELETE, MF_BYCOMMAND, &bmp, &bmp);

	bmp.LoadBitmap(IDB_BMP_OK);
	muEC.AppendMenu(MF_STRING, IDC_MENU_OK, _T("完成截图"));
	muEC.SetMenuItemBitmaps(IDC_MENU_OK, MF_BYCOMMAND, &bmp, &bmp);
	bmp.Detach();

	bmp.LoadBitmap(IDB_BMP_SAVE);
	muEC.AppendMenu(MF_STRING, IDC_MENU_SAVE, _T("保存"));
	muEC.SetMenuItemBitmaps(IDC_MENU_SAVE, MF_BYCOMMAND, &bmp, &bmp);
	bmp.Detach();

	muEC.AppendMenu(MF_SEPARATOR);
	CString str;
	if (m_pDlgBar->IsWindowVisible() )
	{
		str = _T("隐藏工具条");
	}
	else
	{
		str = _T("显示工具条");
	}
	muEC.AppendMenu(MF_STRING, IDC_MENU_SHOWTOOLBAR, str);
	muEC.SetMenuItemBitmaps(IDC_MENU_SHOWTOOLBAR, MF_BYCOMMAND, &bmp, &bmp);

	bmp.LoadBitmap(IDB_BMP_CANCEL);
	muEC.AppendMenu(MF_STRING, IDC_MENU_CANCEL, _T("退出截图"));
	muEC.SetMenuItemBitmaps(IDC_MENU_CANCEL, MF_BYCOMMAND, &bmp, &bmp);
	bmp.Detach();

	m_bMenuVisiable = TRUE; //菜单可见时，鼠标移上去正常显示(不显示彩色光标)

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

	// 恢复原状
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

	//三角形箭头的另外2个点	
	CPoint pt1( ptEnd.x + int( Par * cosy - ( Par / 2.0 * siny ) ),ptEnd.y + int( Par * siny + ( Par / 2.0 * cosy ) ));
	CPoint pt2(ptEnd.x + int( Par * cosy + Par / 2.0 * siny ),ptEnd.y - int( Par / 2.0 * cosy - Par * siny ));

	//在起点和三角形底部终点画条线
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
