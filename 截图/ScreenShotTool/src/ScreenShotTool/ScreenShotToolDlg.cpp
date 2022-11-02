
// ScreenShotToolDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ScreenShotTool.h"
#include "ScreenShotToolDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#pragma comment(lib, "msimg32")     // 【AlphaBlend,透明化】

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
    CAboutDlg();

    // 对话框数据
    enum { IDD = IDD_ABOUTBOX };

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

    // 实现
protected:
    DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CScreenShotToolDlg 对话框


CScreenShotToolDlg::CScreenShotToolDlg(CWnd* pParent /*=NULL*/)
: CDialog(CScreenShotToolDlg::IDD, pParent)
{
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

    m_nDestWidth            = 0;
    m_nDestHeight            = 0;
    m_rectRectangleNum = 0;

    m_bIsDrawArea      = true;
    m_bIsDrawLine      = false;
    m_bIsDrawPen       = false;
    m_bIsDrawRectangle = false;
    m_bIsChooseRect    = false;
    m_bIsShowMagnify   = true;

    m_pGray            = NULL;
}

void CScreenShotToolDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CScreenShotToolDlg, CDialog)
    ON_WM_SYSCOMMAND()
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    //}}AFX_MSG_MAP
    ON_WM_LBUTTONDOWN()
    ON_WM_LBUTTONUP()
    ON_WM_RBUTTONDOWN()
    ON_WM_SETCURSOR()
    ON_WM_MBUTTONDBLCLK()
    ON_WM_MOUSEMOVE()
    ON_WM_LBUTTONDBLCLK()
    ON_BN_CLICKED(IDC_BTN_SAVE,      &CScreenShotToolDlg::OnBnClickedBtnSave)
    ON_COMMAND(IDI_ICON_LINE,        &CScreenShotToolDlg::OnBnClickedBtnLine)
    ON_COMMAND(IDI_ICON_RECT,        &CScreenShotToolDlg::OnBnClickedBtnRectangle)
    ON_COMMAND(IDI_ICON_PEN,         &CScreenShotToolDlg::OnBnClickedBtnPen)
    ON_COMMAND(IDI_ICON_CANCEL,      &CScreenShotToolDlg::OnBnClickedBtnCancel)
    ON_COMMAND(IDI_ICON_SAVE,        &CScreenShotToolDlg::OnBnClickedBtnSave)
    ON_COMMAND(IDI_ICON_EXIT,        &CScreenShotToolDlg::OnBnClickedBtnClose)
    ON_COMMAND(IDI_ICON_OK,          &CScreenShotToolDlg::OnBnClickedBtnOK)
    ON_NOTIFY_EX(TTN_NEEDTEXT, 0,  &CScreenShotToolDlg::OnToolBarTip)
    ON_WM_CLOSE()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


// CScreenShotToolDlg 消息处理程序

BOOL CScreenShotToolDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    // 将“关于...”菜单项添加到系统菜单中。

    // IDM_ABOUTBOX 必须在系统命令范围内。
    ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
    ASSERT(IDM_ABOUTBOX < 0xF000);

    CMenu* pSysMenu = GetSystemMenu(FALSE);
    if (pSysMenu != NULL)
    {
        BOOL bNameValid;
        CString strAboutMenu;
        bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
        ASSERT(bNameValid);
        if (!strAboutMenu.IsEmpty())
        {
            pSysMenu->AppendMenu(MF_SEPARATOR);
            pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
        }
    }

    // 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
    //  执行此操作
    SetIcon(m_hIcon, TRUE);			// 设置大图标
    SetIcon(m_hIcon, FALSE);		// 设置小图标

    // TODO: 在此添加额外的初始化代码

    ///////////////////////////////////////////////////////////////////////////////////

    InitParam();                    // 【初始化橡皮筋，子窗口等参数】
    InitScreenDC();                 // 【初始化截图】
    InitToolBar();                  // 【初始化工具条】
    InitGrayBitMap();               // 【灰色位图，刷背景用】
    InitMagnifyDlg();               // 【初始化放大镜对话框】

    //InitToolTip();                  // 【初始化提示框】
    //InitColorDlg();

    ShowWindow(SW_SHOWMAXIMIZED);   // 【窗体最大化显示】

    ////////////////////////////////////////////////////////////////////////////////////

    return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

//************************************
// Method:    初始化橡皮筋，快捷键，子窗口
// Author:    WangJie
// DateTime:  [7/15/2016]
//************************************
void CScreenShotToolDlg::InitParam()
{
    // 【遍历所有子窗口，保存起来】
    m_allWindowsRect.EnumAllWindows();

    // 【初始化截图橡皮筋style】
    m_rtAreaTracker.m_nStyle = CRectTracker::resizeOutside | CRectTracker::dottedLine;
    m_rtAreaTracker.m_rect.SetRectEmpty();

    // 【初始化当前矩形框橡皮筋】
    m_rtCurRectangleTracker.m_nStyle = CRectTracker::solidLine;
    m_rtCurRectangleTracker.m_rect.SetRectEmpty();

    // 【加载对应的加速键表】
    m_hAccel  =  ::LoadAccelerators(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_ACC_SAVE));
}

//************************************
// Method:    初始化屏幕DC,先在内存中作图，
//            然后用此函数将做好的图复制到前台，
//            同时禁止背景刷新，这样就消除了闪烁。
//            以上也就是双缓冲绘图的基本的思路
// Author:    WangJie
// DateTime:  [7/15/2016]
//************************************
void CScreenShotToolDlg::InitScreenDC()
{
    m_nDestWidth  = GetSystemMetrics(SM_CXSCREEN);  // 【计算的屏幕宽度】
    m_nDestHeight = GetSystemMetrics(SM_CYSCREEN);  // 【计算的屏幕高度】

    CWindowDC dc(GetDesktopWindow());               // 【以当前的桌面窗体创建一个DC】

    m_DeskTopDC.CreateCompatibleDC(&dc);                   // 【创建一个兼容桌面窗体的dc并关联bitmap】
    m_bmpDestTop.CreateCompatibleBitmap(&dc, m_nDestWidth, m_nDestHeight);
    m_DeskTopDC.SelectObject(m_bmpDestTop);         // 【将位图选到内存DC】

    // 【按原来背景填充客户区，不然会是黑色】
    m_DeskTopDC.BitBlt(0, 0, m_nDestWidth, m_nDestHeight, &dc, 0, 0, SRCCOPY);    // 【将窗口DC内容传到应用程序中的dc,支持图形块的复制，速度很快】
}

//************************************
// Method:    初始化工具条
// Author:    WangJie
// DateTime:  [7/15/2016]
//************************************
void CScreenShotToolDlg::InitToolBar()
{
    // 【创建 ToolBar 和加载资源】
    m_ilImageList.Create(TOOLBAR_SIZE,TOOLBAR_SIZE, ILC_COLOR24|ILC_MASK, 1,1);
    m_ilImageList.Add(AfxGetApp()->LoadIcon(IDI_ICON_LINE)); 
    m_ilImageList.Add(AfxGetApp()->LoadIcon(IDI_ICON_RECT)); 
    m_ilImageList.Add(AfxGetApp()->LoadIcon(IDI_ICON_PEN)); 
    m_ilImageList.Add(AfxGetApp()->LoadIcon(IDI_ICON_CANCEL)); 
    m_ilImageList.Add(AfxGetApp()->LoadIcon(IDI_ICON_SAVE)); 
    m_ilImageList.Add(AfxGetApp()->LoadIcon(IDI_ICON_EXIT)); 
    m_ilImageList.Add(AfxGetApp()->LoadIcon(IDI_ICON_OK)); 

    UINT btnIDs[7];
    btnIDs[0]=IDI_ICON_LINE;
    btnIDs[1]=IDI_ICON_RECT;
    btnIDs[2]=IDI_ICON_PEN;
    btnIDs[3]=IDI_ICON_CANCEL;
    btnIDs[4]=IDI_ICON_SAVE;
    btnIDs[5]=IDI_ICON_EXIT;
    btnIDs[6]=IDI_ICON_OK; 

    m_tbToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_ALIGN_BOTTOM | CBRS_TOOLTIPS | CBRS_SIZE_DYNAMIC ,CRect(TOOLBAR_WIDTH, 0, 0, 0));
    m_tbToolBar.SetButtons(btnIDs,7);
    m_tbToolBar.GetToolBarCtrl().SetImageList(&m_ilImageList);
    m_tbToolBar.SetSizes(CSize(TOOLBAR_SIZE,TOOLBAR_SIZE),CSize(TOOLBAR_SIZE,TOOLBAR_SIZE));

    RepositionBars(AFX_IDW_CONTROLBAR_FIRST,AFX_IDW_CONTROLBAR_LAST,0); // 显示工具栏、状态栏

    m_tbToolBar.EnableToolTips(TRUE);
    m_tbToolBar.ShowWindow(SW_HIDE);
}

//************************************
// Method:    初始化放大镜对话会
// Author:    WangJie
// DateTime:  [7/20/2016]
//************************************
void CScreenShotToolDlg::InitMagnifyDlg()
{
    m_dlgMagnify = new CCMagnifyDlg();
    m_dlgMagnify->Create(IDD_DLG_MAGNIFY,NULL);
    m_dlgMagnify->ShowWindow(SW_HIDE);
}

//************************************
// Method:    工具栏提示
// Author:    WangJie
// DateTime:  [7/15/2016]
//************************************
BOOL CScreenShotToolDlg::OnToolBarTip(UINT id, NMHDR * pNMHDR, LRESULT * pResult)
{
    TOOLTIPTEXT *pTTT = (TOOLTIPTEXT*)pNMHDR;
    UINT_PTR uID = pNMHDR->idFrom;

    if ( pTTT->uFlags && TTF_IDISHWND)
    {
        switch ( uID )
        {
        case IDI_ICON_LINE:
            pTTT->lpszText = _T("画线");
            break;
        case IDI_ICON_RECT:
            pTTT->lpszText = _T("矩形");
            break;
        case IDI_ICON_PEN:
            pTTT->lpszText = _T("画笔");
            break;
        case IDI_ICON_CANCEL:
            pTTT->lpszText = _T("撤销");
            break;
        case IDI_ICON_SAVE:
            pTTT->lpszText = _T("保存");
            break;
        case IDI_ICON_EXIT:
            pTTT->lpszText = _T("关闭");
            break;
        case IDI_ICON_OK:
            pTTT->lpszText = _T("结束");
            break;
        default:
            break;
        }

        return  TRUE;
    }
    return FALSE;
}

//************************************
// Method:    创建颜色画框
// Author:    WangJie
// DateTime:  [7/20/2016]
//************************************
void CScreenShotToolDlg::InitColorToolBar()
{

}

//************************************
// Method:    初始化提示框
// Author:    WangJie
// DateTime:  [7/15/2016]
//************************************
void CScreenShotToolDlg::InitToolTip()
{
    m_tToolTip.Create(this);
    m_tToolTip.AddTool(this,_T("1、区域内右键保存截图，左键双击保存截图至内存\n2、区域外左键重绘，右键退出"));
    m_tToolTip.SetMaxTipWidth(200);
    m_tToolTip.SetDelayTime(TTDT_INITIAL, 10);  // 【鼠标指向多久后显示提示，毫秒】
    m_tToolTip.SetDelayTime(TTDT_AUTOPOP,6000); // 【鼠标保持指向，提示显示多久，毫秒】
    m_tToolTip.Activate(TRUE);
}

void CScreenShotToolDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
    if ((nID & 0xFFF0) == IDM_ABOUTBOX)
    {
        CAboutDlg dlgAbout;
        dlgAbout.DoModal();
    }
    else
    {
        CDialog::OnSysCommand(nID, lParam);
    }
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CScreenShotToolDlg::OnPaint()
{
    CPaintDC dc(this); // 用于绘制的设备上下文

    if (IsIconic())
    {
        CPaintDC dc(this); // 用于绘制的设备上下文
        SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

        // 使图标在工作区矩形中居中
        int cxIcon = GetSystemMetrics(SM_CXICON);
        int cyIcon = GetSystemMetrics(SM_CYICON);
        CRect rect;
        GetClientRect(&rect);
        int x = (rect.Width() - cxIcon + 1) / 2;
        int y = (rect.Height() - cyIcon + 1) / 2;

        // 绘制图标
        dc.DrawIcon(x, y, m_hIcon);
    }
    else
    {
        CDialog::OnPaint();
    }

    // 【每次重绘都显示应用程序窗体之前截取到的窗体】
    dc.BitBlt(0,0,m_nDestWidth,m_nDestHeight,&m_DeskTopDC,0,0,SRCCOPY);

    // 【画截图区域,蓝色点状边】
    DrawScreen(dc);

    // 【改变截图区域的背景色为高亮】
    ChangeBackGround(dc.m_hDC, m_rtAreaTracker.m_rect);
    //DrawGrayScreen(dc.m_hDC);

    // 【绘制截图上所有绘图元素】
    DrawAllGraphParam(dc);

}

//************************************
// Method:    初始化灰色画刷
// Author:    WangJie
// DateTime:  [7/20/2016]
//************************************
void CScreenShotToolDlg::InitGrayBitMap()
{
    if (NULL == m_pGray)
    {
        //灰色位图
        HDC    memdc = CreateCompatibleDC(m_DeskTopDC);
        m_pGray = CreateCompatibleBitmap(m_DeskTopDC, Gray_Width, Gray_Height);
        SelectObject(memdc, m_pGray);
        RECT rect;
        rect.left = 0;
        rect.right = Gray_Width;
        rect.top = 0;
        rect.bottom = Gray_Height;
        HBRUSH brush = CreateSolidBrush(BACKGROUND_COLOR);
        int res = FillRect(memdc, &rect, brush);

        DeleteObject(brush);
        DeleteDC(memdc);
    }
}

//************************************
// Method:    改变背景色
// Author:    WangJie
// DateTime:  [7/15/2016]
//************************************
void CScreenShotToolDlg::ChangeBackGround(HDC& hdc, CRect rect)
{
    HDC    memdc = CreateCompatibleDC(hdc);
    SelectObject(memdc, m_pGray);

    BLENDFUNCTION blend;
    memset( &blend, 0, sizeof( blend) );
    blend.BlendOp= AC_SRC_OVER;
    blend.SourceConstantAlpha= ALPHA_GRAY; // 透明度 最大255

    if (rect.left == rect.right || rect.top == rect.bottom)
    {
        //【没选择矩形,全屏灰化】 
        AlphaBlend(hdc,0,0,m_nDestWidth,m_nDestHeight,memdc,0,0,Gray_Width,Gray_Height, blend);
    }
    else
    {
        //【除去矩形后，alpha混合其余四个区域都灰化】
        int x1 = rect.left < rect.right  ? rect.left   : rect.right;
        int y1 = rect.top  < rect.bottom ? rect.top    : rect.bottom;
        int x2 = rect.left < rect.right  ? rect.right  : rect.left;
        int y2 = rect.top  < rect.bottom ? rect.bottom : rect.top;

        if (y1 > 0)
        {
            AlphaBlend(hdc,0,0,m_nDestWidth, y1,memdc,0,0,Gray_Width,Gray_Height, blend);
        }

        if (y2 < m_nDestHeight)
        {
            AlphaBlend(hdc,0,y2,m_nDestWidth, m_nDestHeight-y2,memdc,0,0,Gray_Width,Gray_Height, blend);
        }

        if (x1 > 0)
        {
            AlphaBlend(hdc,0,y1,x1, y2 - y1,memdc,0,0,Gray_Width,Gray_Height, blend);
        }

        if (x2 < m_nDestWidth)
        {
            AlphaBlend(hdc,x2,y1,m_nDestWidth - x2, y2 - y1,memdc,0,0,Gray_Width,Gray_Height, blend);
        }
    }

    DeleteDC(memdc);
}

//************************************
// Method:    绘制截图
// Author:    WangJie
// DateTime:  [7/15/2016]
//************************************
void CScreenShotToolDlg::DrawScreen(CDC& dc)
{
    CPen pen(PS_DOT,1,RGB(255,0,0));
    if (!m_rtAreaTracker.m_rect.IsRectNull())
    {
        m_rtAreaTracker.Draw(&dc, &pen);
    }
}

//************************************
// Method:    绘制截图上所有绘图元素
// Author:    WangJie
// DateTime:  [7/15/2016]
//************************************
void CScreenShotToolDlg::DrawAllGraphParam(CDC& dc)
{
	if (m_rtAreaTracker.m_rect.IsRectEmpty())
	{
		return ;
	}
    vector<GraphParam*>::iterator iters = m_vecGraphParam.begin();
    for (; iters != m_vecGraphParam.end(); ++iters)
    {
        // 【此处画矩形】
        if ((*iters)->m_nType == GRAPH_RECTANGLE)
        {
            CPen pen(PS_SOLID ,2, RECTANGLE_COLOR);

            // 【使用画笔画矩形】
            CPen* pOldPen     = dc.SelectObject(&pen);
            CBrush* pbrush    = CBrush::FromHandle((HBRUSH)GetStockObject(NULL_BRUSH));
            CBrush* pOldBrush = dc.SelectObject(pbrush);

            CRect rectCur;
            (*iters)->m_rt.GetTrueRect(&rectCur);        //【得到矩形区域的大小】
            CRect rectArea;
            m_rtAreaTracker.GetTrueRect(&rectArea);      //【得到截图区域的大小】
            CRect rectNew;
            rectNew.IntersectRect(rectCur, rectArea);    //【画刷创建好，这里得判断区域是否越界，越界则要以边框为界】

            dc.Rectangle(&rectNew);                      //【画出矩形】

            dc.SelectObject(pOldBrush);
            dc.SelectObject(pOldPen);

            //(*iters)->m_rt.Draw(&dc, &pen);            //【使用橡皮筋画矩形】
        }
        else if ((*iters)->m_nType == GRAPH_LINE)        //【此处画线】
        {
            CPen pen(PS_SOLID, 2, LINE_COLOR);           //【画一条红的】

            dc.SetROP2(R2_COPYPEN);
            CPen* pOldPen = dc.SelectObject(&pen);

            dc.MoveTo((*iters)->m_pPosX);
            dc.LineTo((*iters)->m_pPosY);
            
            dc.SelectObject(pOldPen);
        }
        else if ((*iters)->m_nType == GRAPH_PEN)
        {
            CPen pen(PS_SOLID, 2, PEN_COLOR);           //【画一条红的】
            dc.SetROP2(R2_COPYPEN);
            CPen* pOldPen = dc.SelectObject(&pen);

            vector<CPoint>::iterator iter = (*iters)->m_vecPen.begin() + 1;
            for (; iter != (*iters)->m_vecPen.end(); ++iter)
            {
                dc.MoveTo(*(iter - 1));
                dc.LineTo(*iter);
            }
            dc.SelectObject(pOldPen);
        }
    }
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CScreenShotToolDlg::OnQueryDragIcon()
{
    return static_cast<HCURSOR>(m_hIcon);
}


void CScreenShotToolDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
    // TODO: 在此添加消息处理程序代码和/或调用默认值
    if (!m_bIsChooseRect)
    {
        m_dlgMagnify->ShowWindow(SW_HIDE);
    }

    DrawAreaStart(point);        // 【画截图】
    DrawLineStart(point);        // 【画线】
    DrawPenStart(point);         // 【画笔】
    DrawRectangleStart(point);   // 【画矩形】

    CDialog::OnLButtonDown(nFlags, point);
}

//************************************
// Method:    开始画截图区域
// Author:    WangJie
// DateTime:  [7/15/2016]
//************************************
void CScreenShotToolDlg::DrawAreaStart(CPoint point)
{
    if (m_bIsDrawArea)
    {
        m_ptAreaStartPoint = m_ptAreaEndPoint = point;

        if (m_rtAreaTracker.HitTest(point) < 0)     //【如果未击中矩形选择框,重新画选择框】
        {
            m_rtAreaTracker.TrackRubberBand(this,m_ptAreaStartPoint,TRUE);
            m_rtAreaTracker.m_rect.NormalizeRect();
        }
        else           //【如果击中矩形选择框】
        {
            // 【Track这个函数用来支持人们用鼠标改变矩形框大小 或 拖拽矩形框时显示矩形框动作】
            m_rtAreaTracker.Track(this,m_ptAreaStartPoint,TRUE);
            m_rtAreaTracker.m_rect.NormalizeRect();
        }

        m_bIsChooseRect = true;
        m_bIsShowMagnify = false;

        Invalidate(FALSE);   // 【刷新窗口区域，使得CrectTracker对象重绘到窗口上】
        UpdateWindow();
    }
}

//************************************
// Method:    开始画线
// Author:    WangJie
// DateTime:  [7/15/2016]
//************************************
void CScreenShotToolDlg::DrawLineStart(CPoint point)
{
    if (m_bIsDrawLine)
    {
        m_bIsDrawArea   = false;
        m_bIsDrawRectangle = false;

        if (m_rtAreaTracker.m_rect.PtInRect(point))     // 【在里面则画线，不在就不画】
        {
            SetCursor(LoadCursor(NULL,IDC_PEN));
            m_ptLineStartPoint = m_ptLineEndPoint = point;
        }
    }
}

void CScreenShotToolDlg::DrawPenStart(CPoint point)
{
    m_vecPenPoint.clear();
    if (m_bIsDrawPen)
    {
        m_bIsDrawArea   = false;
        m_bIsDrawLine   = false;
        m_bIsDrawRectangle = false;

        if (m_rtAreaTracker.m_rect.PtInRect(point))     // 【在里面则画线，不在就不画】
        {
            SetCursor(LoadCursor(NULL,IDC_PEN));
            m_ptPenStartPoint = m_ptPenEndPoint = point;
        }
        m_vecPenPoint.push_back(point);
    }
}

//************************************
// Method:    开始画矩形
// Author:    WangJie
// DateTime:  [7/15/2016]
//************************************
void CScreenShotToolDlg::DrawRectangleStart(CPoint point)
{
    if (m_bIsDrawRectangle)
    {
        m_bIsDrawArea = false;
        m_bIsDrawLine = false;

        if (m_rtAreaTracker.m_rect.PtInRect(point))     // 【在里面则画线，不在就不画】
        {
            SetCursor(LoadCursor(NULL,IDC_PEN));
            m_ptRectangleStartPoint = m_ptRectangleEndPoint = point;

            m_rtCurRectangleTracker.TrackRubberBand(this,m_ptRectangleStartPoint,TRUE);
            m_rtCurRectangleTracker.m_rect.NormalizeRect();   //【使CRect符合规范，使其高度和宽度均为正值】

            // 【这里将绘制的东西保存到vector中，撤销时使用】
            GraphParam *gp   = new GraphParam();
            gp->m_nType      = GRAPH_RECTANGLE;
            gp->m_rt.m_rect  = m_rtCurRectangleTracker.m_rect;

            m_vecGraphParam.push_back(gp);

            Invalidate(FALSE);
            UpdateWindow();
        }
    }
}

void CScreenShotToolDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
    // TODO: 在此添加消息处理程序代码和/或调用默认值
    DrawAreaEnd(point);

    DrawLineEnd(point);

    DrawPenEnd(point);

    DrawRectangleEnd(point);

    Invalidate(FALSE);
    UpdateWindow();

    CDialog::OnLButtonUp(nFlags, point);
}

//************************************
// Method:    结束画截图区域
// Author:    WangJie
// DateTime:  [7/15/2016]
//************************************
void CScreenShotToolDlg::DrawAreaEnd(CPoint point)
{
    if (!m_bIsChooseRect)
    {
        if (!m_allWindowsRect.GetRect(point, m_rect))
        {
            memset((void*)&m_rect, 0, sizeof(m_rect) );
        }

        if (m_rect.bottom != 0 && m_rect.left != 0 && m_rect.right != 0 && m_rect.top !=0 )
        {
            m_bIsChooseRect = true;
            m_bIsDrawArea = true;
            m_bIsShowMagnify = false;
        }
    }
}

//************************************
// Method:    结束画线
// Author:    WangJie
// DateTime:  [7/15/2016]
//************************************
void CScreenShotToolDlg::DrawLineEnd(CPoint point)
{
    if (m_bIsDrawLine)
    {
        if (m_rtAreaTracker.m_rect.PtInRect(point))     // 在内部右键保存截图
        {
            m_ptLineEndPoint = point;
        }
        else
            m_ptLineEndPoint = m_ptLastInPoint;

        // 【这里将绘制的东西保存到vector中，撤销时使用】
        GraphParam *gp   = new GraphParam();
        gp->m_nType      = GRAPH_LINE;
        gp->m_pPosX      = m_ptLineStartPoint;
        gp->m_pPosY      = m_ptLineEndPoint;

        m_vecGraphParam.push_back(gp);
    }
}

//************************************
// Method:    画笔结束
// Author:    WangJie
// DateTime:  [7/20/2016]
//************************************
void CScreenShotToolDlg::DrawPenEnd(CPoint point)
{
    if (m_bIsDrawPen)
    {
        m_vecPenPoint.push_back(point);

        GraphParam *gp   = new GraphParam();
        gp->m_nType      = GRAPH_PEN;
        gp->m_vecPen     = m_vecPenPoint;

        m_vecGraphParam.push_back(gp);
        m_vecPenPoint.clear();
    }
}

//************************************
// Method:    结束画矩形
// Author:    WangJie
// DateTime:  [7/15/2016]
//************************************
void CScreenShotToolDlg::DrawRectangleEnd(CPoint point)
{
    if (m_bIsDrawRectangle)
    {
        if (!m_rtAreaTracker.m_rect.PtInRect(point))     // 在内部右键保存截图
            return;
    }
}

void CScreenShotToolDlg::OnMouseMove(UINT nFlags, CPoint point)
{
    CWindowDC dc(this);

    //ShowAreaCursor(nFlags, point);    // 【鼠标进入区域后，显示鼠标类型】

    ShowAutoArea(dc, point);            // 【鼠标移动时，显示自动识别的区域】

    ShowLineGo(dc, nFlags, point);      // 【这里实现鼠标画线时显示线的轨迹】

    ShowPenGo(dc, nFlags, point);       // 【显示画笔轨迹】

    ShowMagnifyDlg(dc);                 // 【显示放大镜对话框】

    ShowToolBar();                      // 【工具条跟随鼠标显示】

    CDialog::OnMouseMove(nFlags, point);
}

//************************************
// Method:    手动显示鼠标在截图区域的形状
//            此处使用橡皮筋的SetCursor,故此接口不用
// Author:    WangJie
// DateTime:  [7/15/2016]
//************************************
void CScreenShotToolDlg::ShowAreaCursor(int nFlags, CPoint point)
{
    int nHitTest = 0;
    nHitTest=m_rtAreaTracker.HitTest(point);

    switch (nHitTest)
    {
    case AREA_OUTSIDE:
        SetCursor(LoadCursor(NULL,IDC_ARROW));
        break;
    case AREA_LEFTUP:
    case AREA_RIGHTDOWN:
        SetCursor(LoadCursor(NULL,IDC_SIZENWSE));
        break;
    case AREA_RIGHTUP:
    case AREA_LETDOWN:
        SetCursor(LoadCursor(NULL,IDC_SIZENESW));
        break;
    case AREA_UP:
    case AREA_DOWN:
        SetCursor(LoadCursor(NULL,IDC_SIZENS));
        break;
    case AREA_RIGHT:
    case AREA_LEFT:
        SetCursor(LoadCursor(NULL,IDC_SIZEWE));
        break;
    case AREA_INSIDE:
        SetCursor(LoadCursor(NULL,IDC_SIZE));
        break;
    default:
        break;
    }
}

//************************************
// Method:    这里显示鼠标移动到的子窗口边框
// Author:    WangJie
// DateTime:  [7/15/2016]
//************************************
void CScreenShotToolDlg::ShowAutoArea(CDC& dc, CPoint point)
{
    if (!m_bIsChooseRect)
    {
        CRect rect;
        if (!m_allWindowsRect.GetRect(point, m_rect))   // 【这里判断在没在开始获取到的子窗口中】
        {
            memset((void*)&m_rect, 0, sizeof(m_rect) );
        }

        Invalidate(FALSE);
        UpdateWindow();

        if (m_rect.bottom != 0 && m_rect.left != 0 && m_rect.right != 0 && m_rect.top !=0 )
        {
            CWindowDC dc(this);
            ChangeBackGround(dc.m_hDC, m_rect);         // 【这里绘制背景灰化，使选中区域亮化】

            CPen pen(PS_SOLID,3,RGB(255,0,0));          // 【这里绘制成红色】

            CPen *pOldPen = dc.SelectObject(&pen);      // 【使用画笔画矩形】

            CBrush *pbrush = CBrush::FromHandle((HBRUSH)GetStockObject(NULL_BRUSH));

            CBrush *pOldBrush = dc.SelectObject(pbrush);

            dc.Rectangle(&m_rect);//画出矩形

            if(NULL != pOldPen && NULL != pOldBrush)
            {
                dc.SelectObject(pOldBrush);
                dc.SelectObject(pOldPen);
            }

            DeleteObject(pen);
            DeleteObject(pbrush);
        }
    }
}

//************************************
// Method:    这里实现鼠标画线时显示线的轨迹
// Author:    WangJie
// DateTime:  [7/15/2016]
//************************************
void CScreenShotToolDlg::ShowLineGo(CDC& dc, int nFlags, CPoint point)
{
    int nHitTest = 0;
    nHitTest=m_rtAreaTracker.HitTest(point);

    switch (nHitTest)
    {
    case AREA_OUTSIDE:
        break;
    case AREA_INSIDE:
        {
            m_bIsShowMagnify = false;

            m_ptLastInPoint  = point;

            if ((nFlags == MK_LBUTTON) && m_bIsDrawLine)  
            {
                SetCursor(LoadCursor(NULL,IDC_PEN));

                CPen pen(PS_DOT, 1,RGB(0,255,255));     //【异或模式画红色轨迹】
                dc.SetROP2(R2_XORPEN);
                CPen* pOldPen = dc.SelectObject(&pen);

                dc.MoveTo(m_ptLineStartPoint);
                dc.LineTo(m_ptLineEndPoint);

                dc.MoveTo(m_ptLineStartPoint);
                dc.LineTo(point);

                dc.SelectObject(pOldPen);

                m_ptLineEndPoint = point;
            }
        }
        break;
    default:
        break;
    }
}

//************************************
// Method:    显示画笔轨迹
// Author:    WangJie
// DateTime:  [7/20/2016]
//************************************
void CScreenShotToolDlg::ShowPenGo(CDC& dc, int nFlags, CPoint point)
{
    int nHitTest = 0;
    nHitTest=m_rtAreaTracker.HitTest(point);

    switch (nHitTest)
    {
    case AREA_OUTSIDE:
        break;
    case AREA_INSIDE:
        {
            m_bIsShowMagnify = false;

            m_ptLastInPoint  = point;

            if ((nFlags == MK_LBUTTON) && m_bIsDrawPen)  
            {
                SetCursor(LoadCursor(NULL,IDC_PEN));

                CPen pen(PS_DOT, 1,RGB(0,255,255));     //画一条红的
                dc.SetROP2(R2_XORPEN);
                CPen* pOldPen = dc.SelectObject(&pen);

                vector<CPoint>::iterator iter = m_vecPenPoint.begin() + 1;

                for (; iter != m_vecPenPoint.end(); ++iter)
                {
                    dc.MoveTo(*(iter-1));
                    dc.LineTo(*iter);

                    m_ptPenStartPoint = point;
                }

                dc.SelectObject(pOldPen);
                m_vecPenPoint.push_back(point);
            }
        }
        break;
    default:
        break;
    }
}

//************************************
// Method:    显示工具栏坐标
// Author:    WangJie
// DateTime:  [7/13/2016]
//************************************
void CScreenShotToolDlg::ShowToolBar()
{
    if (m_bIsChooseRect )
    {
        CRect wrect;
        m_tbToolBar.GetWindowRect(&wrect);

        const int nBorder = 5;              // 【工具栏与截图区域间隔】

        int nWidth   = wrect.Width();
        int nHeight  = wrect.Height();

        int nWidEx   = nWidth - TOOLBAR_WIDTH;     // 【工具栏真实大小】

        if (m_rtAreaTracker.m_rect.right < nWidEx)
        {
            wrect.left   = m_rtAreaTracker.m_rect.left - TOOLBAR_WIDTH;         // 【工具栏一直靠左】
            wrect.right  = wrect.left + nWidth;                                 // 【区域右边即工具栏右边】
        }
        else
        {
            wrect.left   = m_rtAreaTracker.m_rect.right - nWidth;               // 【工具栏一直靠右】
            wrect.right  = m_rtAreaTracker.m_rect.right;                        // 【区域右边即工具栏右边】
        }


        int nBottomLine = m_rtAreaTracker.m_rect.bottom + nHeight + nBorder;

        if (nBottomLine >= m_nDestHeight)
        {
            wrect.top = m_rtAreaTracker.m_rect.top - nBorder - nHeight;
            wrect.bottom = m_rtAreaTracker.m_rect.top - nBorder;
        }
        else
        {
            wrect.top    = m_rtAreaTracker.m_rect.bottom + nBorder;             // 【使工具栏放在区域下方，中间相隔5个像素点】
            wrect.bottom = m_rtAreaTracker.m_rect.bottom + nHeight + nBorder;   // 【工具栏底部就是截图底部+ 5个像素点 +工具栏高度】
        }

        m_tbToolBar.MoveWindow(&wrect);
        m_tbToolBar.ShowWindow(SW_SHOW);
    }
    else
        m_tbToolBar.ShowWindow(SW_HIDE);

    UpdateWindow();
}

//************************************
// Method:    显示放大镜对话框，并随鼠标移动
// Author:    WangJie
// DateTime:  [7/20/2016]
//************************************
void CScreenShotToolDlg::ShowMagnifyDlg(CDC& dc)
{
    CreateMagnify(dc);                    // 【放大镜前用透明画布刷屏，放大镜显示在此】

    if (m_bIsShowMagnify && !m_bIsChooseRect)
    {
        CPoint point;
        GetCursorPos(&point);

        int nLeft   = 0;
        int nTop    = 0;
        int nRight  = 0;
        int nBottom = 0;

        if (point.x + MAGNIFY_SIZE > m_nDestWidth)       // 【超出右边界】
        {
            nLeft  = point.x - MAGNIFY_BORDER - MAGNIFY_SIZE;
            nRight = point.x - MAGNIFY_BORDER;
        }
        else
        {
            nLeft  = point.x + MAGNIFY_BORDER;
            nRight = point.x + MAGNIFY_BORDER + MAGNIFY_SIZE;
        }

        if (point.y  + MAGNIFY_SIZE > m_nDestHeight)      // 【超出下边界】
        {
            nTop    = point.y - MAGNIFY_BORDER - MAGNIFY_SIZE;
            nBottom = point.y - MAGNIFY_BORDER;
        }
        else
        {
            nTop    = point.y + MAGNIFY_BORDER;
            nBottom = point.y + MAGNIFY_BORDER + MAGNIFY_SIZE;
        }

        CRect rc(nLeft,nTop,nRight, nBottom);
        ClientToScreen(&rc);


        m_dlgMagnify->MoveWindow(&rc);
        m_dlgMagnify->ShowWindow(SW_SHOW);
        m_dlgMagnify->Invalidate(FALSE);
        m_dlgMagnify->UpdateWindow();
    }
    else
    {
        m_dlgMagnify->ShowWindow(SW_HIDE);
    }
}

//************************************
// Method:    创建放大镜
// Author:    WangJie
// DateTime:  [7/15/2016]
//************************************
void CScreenShotToolDlg::CreateMagnify(CDC& dc)
{
    if (m_bIsShowMagnify && !m_bIsChooseRect)
    {
        CWindowDC dc(GetDesktopWindow());

        CRect rc;
        GetWindowRect(&rc);
        ClientToScreen(&rc);

        // 【使用画笔画矩形】
        CPen pen(PS_SOLID,1,RGB(0, 0, 0));    // 【这里绘制成红色】
        dc.SetROP2(R2_XORPEN); 
        CPen * pOldPen = dc.SelectObject(&pen);
        CBrush *pbrush = CBrush::FromHandle((HBRUSH)GetStockObject(NULL_BRUSH));
        CBrush *pOldBrush = dc.SelectObject(pbrush);

        dc.Rectangle(&rc);          //透明画刷刷屏
        dc.SelectObject(pOldBrush);
        dc.SelectObject(pOldPen);
    }
}

void CScreenShotToolDlg::OnRButtonDown(UINT nFlags, CPoint point)
{
    // TODO: 在此添加消息处理程序代码和/或调用默认值

    if (m_rtAreaTracker.m_rect.PtInRect(point))     // 【在内部右键保存截图】
    {
        SaveRectToMemDC();          // 【保存橡皮筋圈住的区域到内存dc】
        SaveToPicture();            // 【保存内存DC中数据到文件】
    }
    else
    {
        if (!m_bIsChooseRect)
        {
            m_bIsChooseRect   = true;
            m_bIsDrawArea     = false;
            m_rtAreaTracker.m_rect = m_rect;
        }
        else
        {
            ClearGraphParam();
        }
        Invalidate(FALSE);
        UpdateWindow();
    }

    CDialog::OnRButtonDown(nFlags, point);
}

BOOL CScreenShotToolDlg::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
    // TODO: 在此添加消息处理程序代码和/或调用默认值    
    if (m_bIsDrawArea)
    {
        if (m_rtAreaTracker.SetCursor(this,nHitTest) )
        {
            return FALSE;
        }
        else
            SetCursor(LoadCursor(NULL,IDC_ARROW));
    }

    return CDialog::OnSetCursor(pWnd, nHitTest, message);
}

//************************************
// Method:    中间键按下，获取橡皮筋圈住的区域，并传输到窗体左上角，实现预览
// Author:    WangJie
// DateTime:  [7/13/2016]
//************************************
void CScreenShotToolDlg::OnMButtonDblClk(UINT nFlags, CPoint point)
{
    CClientDC dc(this);
    CRect re(m_rtAreaTracker.m_rect);
    dc.BitBlt(0,0,re.Width(),re.Height(),&dc,re.left,re.top,SRCCOPY);

    CDialog::OnMButtonDblClk(nFlags, point);
}

//************************************
// Method:    在内部右键保存截图
// Author:    WangJie
// DateTime:  [7/13/2016]
//************************************
void CScreenShotToolDlg::OnLButtonDblClk(UINT nFlags, CPoint point)
{
    if (m_rtAreaTracker.m_rect.PtInRect(point))
    {
        SaveRectToMemDC();          // 【保存橡皮筋圈住的区域到内存dc】
        SaveToClipboard();
    }

    CDialog::OnLButtonDblClk(nFlags, point);
}

BOOL CScreenShotToolDlg::PreTranslateMessage(MSG* pMsg)
{
    ASSERT(pMsg != NULL);

    switch (pMsg->message)
    {
    //case WM_MOUSEMOVE:
        // 【提示框】
        //m_tToolTip.RelayEvent(pMsg);
    	//break;
    case WM_LBUTTONDOWN:
        break;
    case WM_LBUTTONUP:
        break;
    default:
        break;
    }

    // 【快捷键】
    if (m_hAccel)
    { 
        if (::TranslateAccelerator(m_hWnd,   m_hAccel,   pMsg))
            return(TRUE); 
    }

    return CDialog::PreTranslateMessage(pMsg);
}

//************************************
// Method:    将截图保存到内存DC中
// Author:    WangJie
// DateTime:  [7/13/2016]
//************************************
void CScreenShotToolDlg::SaveRectToMemDC()
{
    // 【保存橡皮筋圈住的区域到内存dc】
    CClientDC dc(this);
    CRect re(m_rtAreaTracker.m_rect);
    m_bmpSaveMap.CreateCompatibleBitmap(&dc,re.Width(),re.Height());

    CDC tempDc;
    tempDc.CreateCompatibleDC(&dc);
    tempDc.SelectObject(&m_bmpSaveMap);
    tempDc.BitBlt(0,0,re.Width(),re.Height(),&dc,re.left,re.top,SRCCOPY);
}

//************************************
// Method:    保存截图区域到文件
// Author:    WangJie
// DateTime:  [7/13/2016]
//************************************
void CScreenShotToolDlg::SaveToPicture()
{
    CFileDialog fDlg(FALSE,_T("png"), _T("未命名") ,4|2, _T("PNG 图片格式 (*.png)|*.png|BMP 图片格式 (*.bmp)|*.bmp|JPG 图片格式 (*.jpg)|*.jpg||"));
    if (fDlg.DoModal()==IDOK)
    {
        CString strExt = _T(".png");
        strExt = fDlg.GetFileExt();
        strExt.Trim();
        strExt.MakeLower();

        CString bmpfile = fDlg.GetPathName();

        //【用CImage保存图片】
        ATL::CImage image;
        image.Attach(m_bmpSaveMap);

        if(strExt == "bmp")
        {
            image.Save(bmpfile, Gdiplus::ImageFormatBMP);
        }
        else if(strExt == "jpg")
        {
            image.Save(bmpfile, Gdiplus::ImageFormatJPEG);
        }
        else
        {
            image.Save(bmpfile, Gdiplus::ImageFormatPNG);
        }

        image.Detach();

        OnClose();      // 【截图后退出程序】
    }
}

//************************************
// Method:    将截图保存到剪切板中，可以粘贴到聊天框画图中
// Author:    WangJie
// DateTime:  [7/13/2016]
//************************************
void CScreenShotToolDlg::SaveToClipboard()
{
    if (OpenClipboard())                            //【打开剪切板】
    {
        EmptyClipboard();                           //【清空剪贴板】
        SetClipboardData(CF_BITMAP, m_bmpSaveMap);  //【把屏幕内容粘贴到剪贴板上】
        CloseClipboard();                           //【关闭剪贴板】

        OnClose();
    }
}

//************************************
// Method:    点击工具栏画线按钮
// Author:    WangJie
// DateTime:  [7/13/2016]
//************************************
void CScreenShotToolDlg::OnBnClickedBtnLine()
{
    m_bIsDrawPen       = false;
    m_bIsDrawArea      = false;
    m_bIsDrawLine      = true;
    m_bIsDrawRectangle = false;
}

//************************************
// Method:    点击工具栏画矩形按钮
// Author:    WangJie
// DateTime:  [7/13/2016]
//************************************
void CScreenShotToolDlg::OnBnClickedBtnRectangle()
{
    m_bIsDrawPen       = false;
    m_bIsDrawArea      = false;
    m_bIsDrawLine      = false;
    m_bIsDrawRectangle = true;
}

//************************************
// Method:    画不规则图形
// Author:    WangJie
// DateTime:  [7/20/2016]
//************************************
void CScreenShotToolDlg::OnBnClickedBtnPen()
{
    m_bIsDrawPen       = true;
    m_bIsDrawArea      = false;
    m_bIsDrawLine      = false;
    m_bIsDrawRectangle = false;
}

//************************************
// Method:    点击工具栏撤销按钮
// Author:    WangJie
// DateTime:  [7/13/2016]
//************************************
void CScreenShotToolDlg::OnBnClickedBtnCancel()
{
     // 【这里代表截图上画的东西都撤销完了，只剩截图了，这是重置所有标记，就可以重新截图】
    if (m_vecGraphParam.size() <= 0)
    {
        ResetFlag();
        return;
    }

    m_bIsDrawArea      = false;
    m_bIsDrawPen       = false;
    m_bIsDrawLine      = false;
    m_bIsDrawRectangle = false;

    vector<GraphParam*>::iterator iter = m_vecGraphParam.end() - 1;

    switch ((*iter)->m_nType)
    {
    case GRAPH_LINE:
        {
            (*iter)->m_pPosX = 0;
            (*iter)->m_pPosY = 0;
        }
        break;
    case GRAPH_RECTANGLE:
        {
            (*iter)->m_rt.m_rect.SetRectEmpty();
        }
        break;
    case GRAPH_PEN:
        {
            (*iter)->m_vecPen.clear();
        }
        break;
    default:
        break;
    }

    if ((*iter))
    {
        delete (*iter);
        (*iter) = NULL;
        m_vecGraphParam.erase(iter);
    }

    // 【这里判断大小，如果等于0了，则需要将截图标记激活，可以拖动】
    if (m_vecGraphParam.size() <= 0)
    {
        ResetFlag();
    }

    Invalidate(FALSE);
    UpdateWindow();
}

//************************************
// Method:    点击工具栏保存按钮
// Author:    WangJie
// DateTime:  [7/13/2016]
//************************************
void CScreenShotToolDlg::OnBnClickedBtnSave()
{
    SaveRectToMemDC();          // 【保存橡皮筋圈住的区域到内存dc】
    SaveToPicture();            // 【保存内存DC中数据到文件】
}

//************************************
// Method:    点击工具栏关闭按钮
// Author:    WangJie
// DateTime:  [7/13/2016]
//************************************
void CScreenShotToolDlg::OnBnClickedBtnClose()
{
    PostQuitMessage(0);
}

//************************************
// Method:    点击工具栏OK按钮，保存截图到剪切板
// Author:    WangJie
// DateTime:  [7/13/2016]
//************************************
void CScreenShotToolDlg::OnBnClickedBtnOK()
{
    SaveRectToMemDC();          // 【保存橡皮筋圈住的区域到内存dc】
    SaveToClipboard();
}

//************************************
// Method:    获取线线交点，暂时没用到
// Author:    WangJie
// DateTime:  [7/13/2016]
//************************************
bool CScreenShotToolDlg::GetLineToLineNode(LineParam para1,LineParam para2, CPoint &pPoint)
{
    if(para1.a == para2.a && para1.b == para2.b)
    {
        return false;
    }

    if(para1.b==0&&para2.a==0)
    {
        pPoint.x = (LONG)-para1.c;
        pPoint.y = (LONG)-para2.c;
    }
    else if(para1.a==0&&para2.b==0)
    {
        pPoint.x = (LONG)-para2.c;
        pPoint.y = (LONG)-para1.c;
    }
    else
    {
        pPoint.x = (LONG)(para2.b*para1.c-para1.b*para2.c)/(LONG)(para2.a*para1.b-para1.a*para2.b);
        pPoint.y = (LONG)(para1.a*para2.c-para2.a*para1.c)/(LONG)(para2.a*para1.b-para1.a*para2.b);
    }

    return true;
}

//************************************
// Method:    获取线线交点方程式的参数，暂时没用到
// Author:    WangJie
// DateTime:  [7/13/2016]
//************************************
void CScreenShotToolDlg::GetLineParam(long x1,long y1,long x2,long y2,LineParam& para)
{
    if (x1 == x2)
    {
        para.a = 1;
        para.b = 0;
        para.c = -x1;
    }
    else
    {
        para.b = 1;
        para.a = -(double)(y1-y2)/(double)(x1-x2);
        para.c = -(y1+para.a*x1);
    }
}

//************************************
// Method:    退出时清理new的资源，以免内存泄漏
// Author:    WangJie
// DateTime:  [7/11/2016]
//************************************
void CScreenShotToolDlg::OnClose()
{
    if (m_dlgMagnify)
    {
        delete m_dlgMagnify;
        m_dlgMagnify = NULL;
    }
    DeleteDC(m_DeskTopDC);
    DeleteObject(m_bmpDestTop);

    ClearGraphParam();
    PostQuitMessage(0);

    CDialog::OnClose();
}

//************************************
// Method:    清理截图中的绘图
// Author:    WangJie
// DateTime:  [7/13/2016]
//************************************
void CScreenShotToolDlg::ClearGraphParam()
{
    if (m_vecGraphParam.size() > 0)
    {
        vector<GraphParam*>::iterator iter = m_vecGraphParam.begin();

        for (; iter != m_vecGraphParam.end(); )
        {
            switch ((*iter)->m_nType)
            {
            case GRAPH_LINE:
                {
                    (*iter)->m_pPosX = 0;
                    (*iter)->m_pPosY = 0;
                }
                break;
            case GRAPH_RECTANGLE:
                {
                    (*iter)->m_rt.m_rect.SetRectEmpty();
                }
                break;
            default:
                break;
            }

            if ((*iter))
            {
                delete (*iter);
                (*iter) = NULL;
                iter = m_vecGraphParam.erase(iter);
            }
        }
    }
    
    ResetFlag();
}

//************************************
// Method:    重置标记
// Author:    WangJie
// DateTime:  [7/13/2016]
//************************************
void CScreenShotToolDlg::ResetFlag()
{
    m_bIsDrawArea       = true;
    m_bIsDrawPen        = false;
    m_bIsDrawLine       = false;
    m_bIsDrawRectangle  = false;
    m_bIsChooseRect     = false;
    m_bIsShowMagnify    = true;

    m_rtAreaTracker.m_rect.SetRectEmpty();

    m_tbToolBar.ShowWindow(SW_HIDE);

    Invalidate(FALSE);
    UpdateWindow();
}

BOOL CScreenShotToolDlg::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
	//return CDialog::OnEraseBkgnd(pDC);
}
