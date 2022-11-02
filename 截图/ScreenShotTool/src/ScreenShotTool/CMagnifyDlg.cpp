// CMAgnifyDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ScreenShotTool.h"
#include "CMAgnifyDlg.h"


// CCMAgnifyDlg 对话框

IMPLEMENT_DYNAMIC(CCMagnifyDlg, CDialog)

CCMagnifyDlg::CCMagnifyDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCMagnifyDlg::IDD, pParent)
{
    m_nDestWidth  = 0; 
    m_nDestHeight = 0;
}

CCMagnifyDlg::~CCMagnifyDlg()
{
}

void CCMagnifyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CCMagnifyDlg, CDialog)
    ON_WM_CLOSE()
    ON_WM_ERASEBKGND()
    ON_WM_PAINT()
    ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()


// CCMAgnifyDlg 消息处理程序

BOOL CCMagnifyDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    // TODO:  在此添加额外的初始化
    InitScreen();

    return TRUE;  // return TRUE unless you set the focus to a control
    // 异常: OCX 属性页应返回 FALSE
}

void CCMagnifyDlg::OnClose()
{
    // TODO: 在此添加消息处理程序代码和/或调用默认值
    PostQuitMessage(0);

    CDialog::OnClose();
}

BOOL CCMagnifyDlg::OnEraseBkgnd(CDC* pDC)
{
    // TODO: 在此添加消息处理程序代码和/或调用默认值
    return CDialog::OnEraseBkgnd(pDC);
}

BOOL CCMagnifyDlg::PreTranslateMessage(MSG* pMsg)
{
    // 【这里截获esc消息，退出】
    if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE)
    {
        PostQuitMessage(0);
    }

    return CDialog::PreTranslateMessage(pMsg);
}

void CCMagnifyDlg::OnPaint()
{
    CPaintDC dc(this);
    DrawMagnify(dc);
}

//************************************
// Method:    初始化屏幕
// Author:    WangJie
// DateTime:  [7/20/2016]
//************************************
void CCMagnifyDlg::InitScreen()
{
    m_nDestWidth  = GetSystemMetrics(SM_CXSCREEN);  // 【计算的屏幕宽度】
    m_nDestHeight = GetSystemMetrics(SM_CYSCREEN);  // 【计算的屏幕高度】

    CWindowDC dc(GetDesktopWindow());               // 【以当前的桌面窗体创建一个DC】

    m_DeskTopDC.CreateCompatibleDC(&dc);                   // 【创建一个兼容桌面窗体的dc并关联bitmap】
    m_bmpDestTop.CreateCompatibleBitmap(&dc, m_nDestWidth, m_nDestHeight);
    m_DeskTopDC.SelectObject(m_bmpDestTop);         // 【将位图选到内存DC】

    // 【按原来背景填充客户区，不然会是黑色】
    m_DeskTopDC.BitBlt(0, 0, m_nDestWidth, m_nDestHeight, &dc, 0, 0, SRCCOPY);    // 【将窗口DC内容传到应用程序中的dc,支持图形块的复制，速度很快】

    SetWindowLong(GetSafeHwnd(),GWL_EXSTYLE,GetWindowLong(GetSafeHwnd(),GWL_EXSTYLE)|WS_EX_LAYERED);
    SetLayeredWindowAttributes(0,255,LWA_ALPHA);
}

//************************************
// Method:    画放大镜
// Author:    WangJie
// DateTime:  [7/20/2016]
//************************************
void CCMagnifyDlg::DrawMagnify(CDC& dc)
{
    dc.BitBlt(0,0,m_nDestWidth,m_nDestHeight,&m_DeskTopDC,0,0,SRCCOPY);

    CPoint point;

    GetCursorPos(&point);

    this->GetWindowRect(m_rect);

    ClientToScreen(&m_rect);

    //放大绘制
    dc.StretchBlt(  0, 0,                // 【目标区的起始坐标】
                    m_rect.Width(),      // 【目标区的长宽】
                    m_rect.Height(),     // 【目标区的长宽】
                    &m_DeskTopDC,        // 【桌面源dc】
                    point.x, point.y,    // 【源y坐标】
                    m_rect.Width()/MAGNIFY_MULTIPLE,   // 【源矩形的宽度】
                    m_rect.Height()/MAGNIFY_MULTIPLE,  // 【源矩形的高度】
                    SRCCOPY);
}

//************************************
// Method:    画放大镜对话框边框
// Author:    WangJie
// DateTime:  [7/20/2016]
//************************************
void CCMagnifyDlg::DrawBorder(CDC *pDC)
{
    if (m_hWnd )
    {
        CBrush Brush(RGB(255, 0, 0));
        CBrush* pOldBrush = pDC->SelectObject(&Brush);

        CRect rtWnd;
        GetWindowRect(&rtWnd); 

        CPoint point;

        //【填充顶部框架】
        point.x = rtWnd.Width();
        point.y = GetSystemMetrics(SM_CYFRAME)+1;
        pDC->PatBlt(0, 0, point.x, point.y, PATCOPY);

        //【填充左侧框架】
        point.x = GetSystemMetrics(SM_CXFRAME);
        point.y = rtWnd.Height();
        pDC->PatBlt(0, 0, point.x, point.y, PATCOPY);

        //【填充底部框架】
        point.x = rtWnd.Width(); 
        point.y = GetSystemMetrics(SM_CYFRAME) + 1;
        pDC->PatBlt(0, rtWnd.Height()-point.y, point.x, point.y, PATCOPY);

        //【填充右侧框架】
        point.x = GetSystemMetrics(SM_CXFRAME);
        point.y = rtWnd.Height();
        pDC->PatBlt(rtWnd.Width()-point.x, 0, point.x, point.y, PATCOPY);
    }
}
