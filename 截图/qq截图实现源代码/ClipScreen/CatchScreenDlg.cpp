// CatchScreenDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ClipScreen.h"
#include "CatchScreenDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define TOOLBAR_ANXIS 8
#define ID_SAVE 0x101
#define ID_FINISH 0x102
#define ID_TEXT 0x103
#define ID_RECT 0x104
/////////////////////////////////////////////////////////////////////////////
// CCatchScreenDlg dialog

HICON g_hUpIco = NULL;
HICON g_hDownIco = NULL;


void OnItemClick(CToolItem *pPtem,CWnd *pWnd)
{
	CCatchScreenDlg *pThis = (CCatchScreenDlg *)pWnd;
	if(pThis == NULL) return;
	if(pPtem == NULL) return;
	{
// 		if(pPtem->GetID() == ID_FINISH)
// 		{
// 			pThis->m_toolBar.Show(FALSE);
// 			pThis->RedrawWindow();
// 			pThis->CopyScreenToBitmap(pThis->m_rectTracker.m_rect,TRUE);
// 			pThis->OnOK();
// 		}
		pThis->m_toolBar.SetSel(pPtem);
		pThis->SetDrawWay(pPtem->GetID());
		if(pPtem->GetID() == ID_TOOL_SAVE)
		{
			pThis->m_toolBar.Show(FALSE);
			pThis->m_PaintWnd.SetSaveFlag();
			pThis->RedrawWindow();
			pThis->Save();
			pThis->OnOK();
		}
	}
}
static void SetNormalRect(CRect& rect, int left, int top, int width, int height)
{
	// set it
	rect.left = left;
	rect.top = top;
	rect.right = left + width;
	rect.bottom = top + height;
	
	// normalize it
	int nTemp;
	if (rect.left > rect.right)
	{
		nTemp = rect.left;
		rect.left = rect.right;
		rect.right = nTemp;
	}
	if (rect.top > rect.bottom)
	{
		nTemp = rect.top;
		rect.top = rect.bottom;
		rect.bottom = nTemp;
	}
}

CCatchScreenDlg::~CCatchScreenDlg()
{
	DestroyIcon(g_hUpIco);
	DestroyIcon(g_hDownIco);
	delete m_pBitmap;
	Gdiplus::GdiplusShutdown(m_gdiToken);
}
CCatchScreenDlg::CCatchScreenDlg(CWnd *pSnapshotWnd,CWnd* pParent /*=NULL*/)
	: CDialog(CCatchScreenDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCatchScreenDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	
//////////////////////////////////////////////////////////////////////////
	g_hUpIco = (HICON)::LoadImage(AfxGetInstanceHandle(),
		MAKEINTRESOURCE(IDI_ICON_UP),IMAGE_ICON,32,32,LR_VGACOLOR);
	g_hDownIco = (HICON)::LoadImage(AfxGetInstanceHandle(),
		MAKEINTRESOURCE(IDI_ICON_DOWN),IMAGE_ICON,32,32,LR_VGACOLOR);
	m_pSnapShotWnd = pSnapshotWnd;
	m_IsEnableMove = TRUE;
	m_pBitmap = NULL;
	m_bUseInitPos = TRUE;
	Gdiplus::GdiplusStartup(&m_gdiToken,&m_gdipStart,NULL);
	//获取屏幕分辩率
	if(m_pSnapShotWnd != NULL && m_pSnapShotWnd->GetSafeHwnd())
	{
		CRect rtWnd;
		m_pSnapShotWnd->GetWindowRect(&rtWnd);
		m_leftX = rtWnd.left;
		m_topY = rtWnd.top;
		m_xScreen = rtWnd.Width();
		m_yScreen = rtWnd.Height();
	}
	else
	{
		m_leftX = 0;
		m_topY = 0;
		m_xScreen = GetSystemMetrics(SM_CXSCREEN);
		m_yScreen = GetSystemMetrics(SM_CYSCREEN);
	}
	//截取屏幕到位图中
	CRect rect(m_leftX, m_topY,m_leftX + m_xScreen,m_topY + m_yScreen);
	//设置矩形调整时光标,默认的太小了,弄了个大点的
	m_rectTracker.SetResizeCursor(IDC_CURSOR6,IDC_CURSOR5,IDC_CURSOR2,IDC_CURSOR3,IDC_CURSOR4);
	m_pBitmap = new CBitmap;
	m_pBitmap->Attach(CopyScreenToBitmap(&rect));
    m_hCursor=AfxGetApp()->LoadCursor(IDC_CURSOR1);  //彩色光标
 	m_rectTracker.m_nStyle=CMyTracker::resizeMiddle|CMyTracker::solidLine;  
 	m_rectTracker.m_rect.SetRect(-1,-2,-3,-4);
 	//设置矩形颜色
 	m_rectTracker.SetRectColor(RGB(10,100,130));
	//设置ToolBar
	m_toolBar.SetParent(this);
	m_toolBar.SetClickfun(OnItemClick);
	int nSize = sizeof(g_ToolBar)/(sizeof(*g_ToolBar));
	
	for (int i = 0;i < nSize;++i)
	{
		m_toolBar.AddItem(new CToolItem(&m_toolBar,21,21,g_ToolBarID[i],g_ToolBar[i],g_ToolGroup[i],g_ToolGroupSel[i]));
	}
    m_startPt=CPoint(0,0);
	m_bDraw=FALSE;
	m_bFirstDraw=FALSE;
	m_bShowMsg=FALSE;
	m_nHitPos= -1;

}





void CCatchScreenDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCatchScreenDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCatchScreenDlg, CDialog)
	//{{AFX_MSG_MAP(CCatchScreenDlg)
	ON_WM_LBUTTONDOWN()
	ON_WM_ERASEBKGND()
	ON_WM_SETCURSOR()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_PAINT()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_RBUTTONDBLCLK()
	ON_WM_RBUTTONUP()
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCatchScreenDlg message handlers

void CCatchScreenDlg::OnOK() 
{
	// TODO: Add extra validation here
	CDialog::OnOK();
}


BOOL CCatchScreenDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
//**************************************************************************	
	//把对化框设置成全屏顶层窗口
	if(m_pSnapShotWnd)
		MoveWindow(m_leftX,m_topY,m_xScreen,m_yScreen);
	else
		SetWindowPos(&wndTopMost,m_leftX,m_topY,m_xScreen,m_yScreen,SWP_SHOWWINDOW);

	m_PaintWnd.SetParent(this);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

//重画窗口
void CCatchScreenDlg::PaintWindow()
{
	CRect rect;
    m_rectTracker.GetTrueRect(&rect);
	rect.NormalizeRect();
	InvalidateRect(&rect,TRUE);

}
//拷贝屏幕
HBITMAP CCatchScreenDlg::CopyScreenToBitmap(LPRECT lpRect,BOOL bSave)
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
	hScrDC = CreateDC("DISPLAY", NULL, NULL, NULL);
	
	//为屏幕设备描述表创建兼容的内存设备描述表
	hMemDC = CreateCompatibleDC(hScrDC);

	if(bSave && m_pSnapShotWnd != NULL)
	{
		ClientToScreen(lpRect);
	}
	// 获得选定区域坐标
	nX = lpRect->left;
	nY = lpRect->top;
	nX2 = lpRect->right;
	nY2 = lpRect->bottom;
	
	//确保选定区域是可见的
	if (nX < m_leftX)
		nX = m_leftX;
	if (nY < m_topY)
		nY = m_topY;
	if (nX2 > m_leftX + m_xScreen)
		nX2 = m_leftX + m_xScreen;
	if (nY2 > m_topY + m_yScreen)
		nY2 = m_topY + m_yScreen;
	nWidth = nX2 - nX;
	nHeight = nY2 - nY;
	
	if(nWidth <= 0 || nHeight <= 0 ) return NULL;
	// 创建一个与屏幕设备描述表兼容的位图
	hBitmap = CreateCompatibleBitmap
		(hScrDC, nWidth, nHeight);
	// 把新位图选到内存设备描述表中
	hOldBitmap = (HBITMAP)SelectObject(hMemDC, hBitmap);
	// 把屏幕设备描述表拷贝到内存设备描述表中
// 	if(bSave)
// 	{
// 		//创建军兼容DC,当bSave为中时把开始保存的全屏位图,按截取矩形大小保存
// 		if(m_pSnapShotWnd != 0)
// 		{
// 			POINT pt = {nX,nY};
// 			m_pSnapShotWnd->ClientToScreen(&pt);
// 			nX = pt.x;
// 			nY = pt.y;
// 		}

// 		CDC dcCompatible;
// 		dcCompatible.CreateCompatibleDC(CDC::FromHandle(hMemDC));
// 		dcCompatible.SelectObject(m_pBitmap);
//         
// 		BitBlt(hMemDC, 0, 0, nWidth, nHeight,
// 			hScrDC, nX, nY, SRCCOPY);
// 		
// 	}
// 	else
	{
		BitBlt(hMemDC, 0, 0, nWidth, nHeight,
			hScrDC, nX, nY, SRCCOPY);
	}
	
	hBitmap = (HBITMAP)SelectObject(hMemDC, hOldBitmap);
	//得到屏幕位图的句柄
	//清除 
	DeleteDC(hScrDC);
	DeleteDC(hMemDC);
	
	if(bSave)
	{
		
		if (OpenClipboard()) 
		{
			//清空剪贴板
			EmptyClipboard();
			//把屏幕内容粘贴到剪贴板上,
			//hBitmap 为刚才的屏幕位图句柄
			SetClipboardData(CF_BITMAP, hBitmap);
			//关闭剪贴板
			CloseClipboard();
		}
	}
	// 返回位图句柄
	
	return hBitmap;
}

BOOL CCatchScreenDlg::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	//用整个桌面填充全屏对话框背景   
	CRect rc = 	m_rectTracker.m_rect;
	rc.NormalizeRect();
    COLORREF brush(RGB(190,253,159));
	COLORREF brush1(RGB(255,255,255));
	CRect rect;
	GetClientRect(&rect);
//////////////////////////////////////////////////////////////////////////
 	CDC dcMem; 
 	dcMem.CreateCompatibleDC(pDC); 
 	CBitmap bitMap;
	rc.NormalizeRect();
 	bitMap.CreateCompatibleBitmap(pDC,rect.Width(),rect.Height());
 	dcMem.SelectObject(&bitMap); 
	dcMem.FillSolidRect(&rect,brush1);	
 //////////////////////////////////////////////////////////////////////////  	
	BITMAP bmp;
	m_pBitmap->GetBitmap(&bmp);	
	CDC dcCompatible;
	dcCompatible.CreateCompatibleDC(pDC);	
	dcCompatible.SelectObject(m_pBitmap);
	if(m_bFirstDraw)
	{	
		if (m_nHitPos == 8 || m_nHitPos == -1)
		{
			//绘画矩形框框的边框
			m_rectTracker.Draw(&dcMem);
		}
		else
		{
			//填充选中矩形的颜色值，目前取消
			//dcMem.FillSolidRect(&rc,brush);
			//绘画矩形框框的边框
			m_rectTracker.Draw(&dcMem);
		}
	}

	
//////////////////////////////////////////////////////////////////////////
    dcMem.BitBlt(0,0,rect.Width(),rect.Height(),&dcCompatible,0,0,SRCAND);
	//绘画整体阴影
	Gdiplus::Graphics g(dcMem);
	Gdiplus::SolidBrush gpbrush(Gdiplus::Color(200,172,172,172));
	Gdiplus::RectF rtFclient(0,0,rect.Width(),rect.Height());
	Gdiplus::RectF rtFDrawClient(m_rectTracker.m_rect.left,m_rectTracker.m_rect.top,m_rectTracker.m_rect.Width(),m_rectTracker.m_rect.Height());
	Gdiplus::Region regionClient(rtFclient);
	regionClient.Exclude(rtFDrawClient);
	g.FillRegion(&gpbrush,&regionClient);

	//在矩形框具体的绘画
	DrawInfo(&dcMem,rc);
	//绘画toolbar
	m_toolBar.Draw(dcMem.GetSafeHdc());

	pDC->BitBlt(0,0,rect.Width(),rect.Height(),&dcMem,0,0,SRCCOPY);	

	return TRUE;
}

BOOL CCatchScreenDlg::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	// TODO: Add your message handler code here and/or call default
	//设置改变截取矩形大小时光标
	if (pWnd == this && m_IsEnableMove && m_rectTracker.SetCursor(this, nHitTest)
		&&!m_bDraw &&m_bFirstDraw) //此处判断保截取时当标始中为彩色光标
    {
		return TRUE; 
	}
	
	//设置彩色光标
	if(m_IsEnableMove)
		SetCursor(m_hCursor);
	
	return TRUE;
//	return CDialog::OnSetCursor(pWnd, nHitTest, message);
}

void CCatchScreenDlg::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	BOOL bHandle = FALSE;
	m_PaintWnd.OnMouseMove(nFlags,point,bHandle);
	if(bHandle) return CDialog::OnMouseMove(nFlags, point);
   	if (m_bShowMsg)
    {
		m_toolBar.Show(FALSE);
		PaintWindow();
		
    }
	if(m_bDraw)
	{
		//动态调整矩形大小,并刷新画出
		if(m_bUseInitPos && point != m_startPt)
		{
			m_rectTracker.m_rect.SetRect(0,0,0,0);
			m_bUseInitPos = FALSE;
		}
		m_rectTracker.m_rect.SetRect(m_startPt.x+4,m_startPt.y+4,point.x,point.y);
	}

	m_toolBar.OnMouseMove(nFlags,point);
	if(!m_IsEnableMove && !m_toolBar.PtInRect(point))
	{
		if(m_rectTracker.m_rect.PtInRect(point))
			SetCursor(LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_CURSOR7)));
		else
			SetCursor(m_hCursor);
	}

	CDialog::OnMouseMove(nFlags, point);
}

void CCatchScreenDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	int nHitTest;
	nHitTest=m_rectTracker.HitTest(point);
	BOOL bHandle = m_toolBar.OnLbuttonDwon(nFlags,point);
	if(bHandle || !m_IsEnableMove)
	{
		m_IsEnableMove = FALSE;
		if(!bHandle)
			m_PaintWnd.OnLButtonDown(nFlags,point);
		return CDialog::OnLButtonDown(nFlags, point);
	}
	//判断击中位置
	if(nHitTest<0 || !m_bFirstDraw)
	{
		if(!m_bFirstDraw)
		{
			m_nHitPos = CRectTracker::hitTopLeft;
			//第一次画矩形
			m_startPt=point;
			m_bDraw=TRUE;
			m_bFirstDraw=TRUE;
			m_bUseInitPos = TRUE;
			//设置当当鼠标按下时最小的矩形大小
			//m_rectTracker.m_rect.SetRect(point.x,point.y,point.x+4,point.y+4);
			//设置默认为整个屏幕
			m_rectTracker.m_rect.SetRect(0,0,m_xScreen,m_yScreen);
			//保证当鼠标当下时立刻显示信息
			if(m_bFirstDraw)
				m_bShowMsg=TRUE;		
			//	DrawTip();
		}
	}
	else
	{
		//保证当鼠标当下时立刻显示信息
		m_bShowMsg=TRUE;
		m_bUseInitPos = FALSE;
		m_nHitPos = nHitTest;
		if(m_bFirstDraw)
		{	//调束大小时,Track会自动调整矩形大小,在些期间,消息归CRectTracker内部处理
			m_rectTracker.Track(this,point,TRUE);
			PaintWindow();

		}
	}
	CDialog::OnLButtonDown(nFlags, point);
}

void CCatchScreenDlg::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	m_startPt = CPoint(0,0);
	m_bShowMsg=FALSE;
	m_bDraw=FALSE;
	m_nHitPos = -1;
	m_PaintWnd.OnLButtonUp(nFlags,point);

	CRect rtCleint;
	GetClientRect(&rtCleint);
	CRect rtRgn;
	rtRgn.IntersectRect(&m_rectTracker.m_rect,&rtCleint);

	POINT ptToolPos = {0,0};
	if(rtRgn.Width() >= m_toolBar.Width())
		ptToolPos.x = rtRgn.right - m_toolBar.Width();
	else
		ptToolPos.x = rtRgn.left;

	if (rtRgn.bottom + m_toolBar.Width() + TOOLBAR_ANXIS > rtCleint.bottom)
	{
		if(rtRgn.top - m_toolBar.Height() - TOOLBAR_ANXIS > 0)
			ptToolPos.y =rtRgn.top - m_toolBar.Height() - TOOLBAR_ANXIS;
		else 
			ptToolPos.y =rtRgn.top + TOOLBAR_ANXIS;
	}
	else
		ptToolPos.y =rtRgn.bottom + TOOLBAR_ANXIS;


	m_toolBar.Show(TRUE);
	m_toolBar.SetPos(ptToolPos);
	PaintWindow();
	InvalidateRect(NULL,TRUE);

	CDialog::OnLButtonUp(nFlags, point);
}

void CCatchScreenDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	m_IsEnableMove = TRUE;
	if(m_bFirstDraw)
	{
		//取消已画矩形变量
		m_bFirstDraw=FALSE;
		m_bUseInitPos = FALSE;
		m_bDraw=FALSE;
		PaintWindow();
		m_rectTracker.m_rect.SetRect(-1,-1,-1,-1);
	}
	else
	{
		CDialog::OnCancel();
	}
}

void CCatchScreenDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
}

void CCatchScreenDlg::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	int nHitTest;
	nHitTest=m_rectTracker.HitTest(point);
    //如果在是矩形内部双击
	if(nHitTest==8)      
	{
        //保存位图到粘贴板中,bSave 为TRUE,
		CopyScreenToBitmap(m_rectTracker.m_rect,TRUE);
		OnOK();
	}
	CDialog::OnLButtonDblClk(nFlags, point);
}

void CCatchScreenDlg::OnRButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default

    OnCancel();
	CDialog::OnRButtonDblClk(nFlags, point);
}

void CCatchScreenDlg::OnRButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	OnCancel();
	CDialog::OnRButtonUp(nFlags, point);
}

void CCatchScreenDlg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	m_pBitmap->Detach();
	delete m_pBitmap;
	m_pBitmap = NULL;
	CDialog::OnClose();
}

BOOL CCatchScreenDlg::Save()
{
	HBITMAP hBitmap = CopyScreenToBitmap(m_rectTracker.m_rect,TRUE);
	static char* s_ext[] = {"png", "bmp", "jpeg"};
	TCHAR BASED_CODE szFilter[] = _T("PNG Files (*.png)|*.png|BMP Files (*.bmp)|*.bmp|JPEG Files (.jpg)|*.jpeg;*.jpg||");
	CString strname;
	strname.Format(_T(" 截图测试%d"),GetTickCount());
	CFileDialog savefiledlg(FALSE,"png",strname,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,szFilter);
	if(savefiledlg.DoModal() != IDOK) return FALSE;
	CString sfileName = savefiledlg.GetPathName();
	OPENFILENAME& ofn = savefiledlg.GetOFN();

	if(!SaveimageToFile(hBitmap,sfileName,s_ext[ofn.nFilterIndex - 1]))
	{
		MessageBox("保存图片失败!!",_T("提示"),MB_OK);
		return FALSE;
	}
	return TRUE;
}
BOOL CCatchScreenDlg::SaveimageToFile(HBITMAP hBitmap,const TCHAR *szFile,const TCHAR* imgtype)
{
	if(hBitmap == NULL || szFile == NULL) return FALSE;
	C22 tszfile(szFile);
	CLSID pngClsid;
	CStringW fmt(L"image/");
	fmt += imgtype;
	int nRet = GetEncoderClsid(fmt, &pngClsid);
	if(nRet == -1) return FALSE;

	Gdiplus::Bitmap bitmap(hBitmap,NULL);
	if (bitmap.GetLastStatus() != Gdiplus::Ok) return FALSE;
	return bitmap.Save(tszfile,&pngClsid) == Gdiplus::Ok;
}

int CCatchScreenDlg::GetEncoderClsid(const WCHAR* format, CLSID* pClsid)
{
	UINT  num = 0;          // number of image encoders
	UINT  size = 0;         // size of the image encoder array in bytes

	Gdiplus::ImageCodecInfo* pImageCodecInfo = NULL;

	Gdiplus::GetImageEncodersSize(&num, &size);
	if(size == 0)
		return -1;  // Failure

	pImageCodecInfo = (Gdiplus::ImageCodecInfo*)(malloc(size));
	if(pImageCodecInfo == NULL)
		return -1;  // Failure

	GetImageEncoders(num, size, pImageCodecInfo);

	for(UINT j = 0; j < num; ++j)
	{
		if( wcscmp(pImageCodecInfo[j].MimeType, format) == 0 )
		{
			*pClsid = pImageCodecInfo[j].Clsid;
			free(pImageCodecInfo);
			return j;  // Success
		}    
	}

	free(pImageCodecInfo);
	return -1;  // Failure
}

void CCatchScreenDlg::DrawInfo(CDC *dc,const CRect &rtDraw)
{
	m_PaintWnd.OnDraw(dc,rtDraw);
}

void CCatchScreenDlg::SetDrawWay( UINT nWay )
{
	m_PaintWnd.SetDrawWay(nWay);
}
BOOL CCatchScreenDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	
	if(pMsg->message == WM_KEYDOWN)
	{
		if(pMsg->wParam == VK_DELETE)
		 SetDrawWay(ID_TOOL_DEL);
		else if(pMsg->wParam  == VK_RETURN)
		{
			OnOK();
			return TRUE;
		}
		else if(pMsg->wParam == 0x53)
		{
			if (GetAsyncKeyState(VK_CONTROL)&0x80000000)
			{
				m_toolBar.Show(FALSE);
				m_PaintWnd.SetSaveFlag();
				RedrawWindow();
				Save();
				OnOK();
			}
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}
