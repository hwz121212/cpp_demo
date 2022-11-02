
// ScreenShotToolDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ScreenShotTool.h"
#include "ScreenShotToolDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#pragma comment(lib, "msimg32")     // ��AlphaBlend,͸������

// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
    CAboutDlg();

    // �Ի�������
    enum { IDD = IDD_ABOUTBOX };

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

    // ʵ��
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


// CScreenShotToolDlg �Ի���


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


// CScreenShotToolDlg ��Ϣ�������

BOOL CScreenShotToolDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    // ��������...���˵�����ӵ�ϵͳ�˵��С�

    // IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

    // ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
    //  ִ�д˲���
    SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
    SetIcon(m_hIcon, FALSE);		// ����Сͼ��

    // TODO: �ڴ���Ӷ���ĳ�ʼ������

    ///////////////////////////////////////////////////////////////////////////////////

    InitParam();                    // ����ʼ����Ƥ��Ӵ��ڵȲ�����
    InitScreenDC();                 // ����ʼ����ͼ��
    InitToolBar();                  // ����ʼ����������
    InitGrayBitMap();               // ����ɫλͼ��ˢ�����á�
    InitMagnifyDlg();               // ����ʼ���Ŵ󾵶Ի���

    //InitToolTip();                  // ����ʼ����ʾ��
    //InitColorDlg();

    ShowWindow(SW_SHOWMAXIMIZED);   // �����������ʾ��

    ////////////////////////////////////////////////////////////////////////////////////

    return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

//************************************
// Method:    ��ʼ����Ƥ���ݼ����Ӵ���
// Author:    WangJie
// DateTime:  [7/15/2016]
//************************************
void CScreenShotToolDlg::InitParam()
{
    // �����������Ӵ��ڣ�����������
    m_allWindowsRect.EnumAllWindows();

    // ����ʼ����ͼ��Ƥ��style��
    m_rtAreaTracker.m_nStyle = CRectTracker::resizeOutside | CRectTracker::dottedLine;
    m_rtAreaTracker.m_rect.SetRectEmpty();

    // ����ʼ����ǰ���ο���Ƥ�
    m_rtCurRectangleTracker.m_nStyle = CRectTracker::solidLine;
    m_rtCurRectangleTracker.m_rect.SetRectEmpty();

    // �����ض�Ӧ�ļ��ټ���
    m_hAccel  =  ::LoadAccelerators(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_ACC_SAVE));
}

//************************************
// Method:    ��ʼ����ĻDC,�����ڴ�����ͼ��
//            Ȼ���ô˺��������õ�ͼ���Ƶ�ǰ̨��
//            ͬʱ��ֹ����ˢ�£���������������˸��
//            ����Ҳ����˫�����ͼ�Ļ�����˼·
// Author:    WangJie
// DateTime:  [7/15/2016]
//************************************
void CScreenShotToolDlg::InitScreenDC()
{
    m_nDestWidth  = GetSystemMetrics(SM_CXSCREEN);  // ���������Ļ��ȡ�
    m_nDestHeight = GetSystemMetrics(SM_CYSCREEN);  // ���������Ļ�߶ȡ�

    CWindowDC dc(GetDesktopWindow());               // ���Ե�ǰ�����洰�崴��һ��DC��

    m_DeskTopDC.CreateCompatibleDC(&dc);                   // ������һ���������洰���dc������bitmap��
    m_bmpDestTop.CreateCompatibleBitmap(&dc, m_nDestWidth, m_nDestHeight);
    m_DeskTopDC.SelectObject(m_bmpDestTop);         // ����λͼѡ���ڴ�DC��

    // ����ԭ���������ͻ�������Ȼ���Ǻ�ɫ��
    m_DeskTopDC.BitBlt(0, 0, m_nDestWidth, m_nDestHeight, &dc, 0, 0, SRCCOPY);    // ��������DC���ݴ���Ӧ�ó����е�dc,֧��ͼ�ο�ĸ��ƣ��ٶȺܿ졿
}

//************************************
// Method:    ��ʼ��������
// Author:    WangJie
// DateTime:  [7/15/2016]
//************************************
void CScreenShotToolDlg::InitToolBar()
{
    // ������ ToolBar �ͼ�����Դ��
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

    RepositionBars(AFX_IDW_CONTROLBAR_FIRST,AFX_IDW_CONTROLBAR_LAST,0); // ��ʾ��������״̬��

    m_tbToolBar.EnableToolTips(TRUE);
    m_tbToolBar.ShowWindow(SW_HIDE);
}

//************************************
// Method:    ��ʼ���Ŵ󾵶Ի���
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
// Method:    ��������ʾ
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
            pTTT->lpszText = _T("����");
            break;
        case IDI_ICON_RECT:
            pTTT->lpszText = _T("����");
            break;
        case IDI_ICON_PEN:
            pTTT->lpszText = _T("����");
            break;
        case IDI_ICON_CANCEL:
            pTTT->lpszText = _T("����");
            break;
        case IDI_ICON_SAVE:
            pTTT->lpszText = _T("����");
            break;
        case IDI_ICON_EXIT:
            pTTT->lpszText = _T("�ر�");
            break;
        case IDI_ICON_OK:
            pTTT->lpszText = _T("����");
            break;
        default:
            break;
        }

        return  TRUE;
    }
    return FALSE;
}

//************************************
// Method:    ������ɫ����
// Author:    WangJie
// DateTime:  [7/20/2016]
//************************************
void CScreenShotToolDlg::InitColorToolBar()
{

}

//************************************
// Method:    ��ʼ����ʾ��
// Author:    WangJie
// DateTime:  [7/15/2016]
//************************************
void CScreenShotToolDlg::InitToolTip()
{
    m_tToolTip.Create(this);
    m_tToolTip.AddTool(this,_T("1���������Ҽ������ͼ�����˫�������ͼ���ڴ�\n2������������ػ棬�Ҽ��˳�"));
    m_tToolTip.SetMaxTipWidth(200);
    m_tToolTip.SetDelayTime(TTDT_INITIAL, 10);  // �����ָ���ú���ʾ��ʾ�����롿
    m_tToolTip.SetDelayTime(TTDT_AUTOPOP,6000); // ����걣��ָ����ʾ��ʾ��ã����롿
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CScreenShotToolDlg::OnPaint()
{
    CPaintDC dc(this); // ���ڻ��Ƶ��豸������

    if (IsIconic())
    {
        CPaintDC dc(this); // ���ڻ��Ƶ��豸������
        SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

        // ʹͼ���ڹ����������о���
        int cxIcon = GetSystemMetrics(SM_CXICON);
        int cyIcon = GetSystemMetrics(SM_CYICON);
        CRect rect;
        GetClientRect(&rect);
        int x = (rect.Width() - cxIcon + 1) / 2;
        int y = (rect.Height() - cyIcon + 1) / 2;

        // ����ͼ��
        dc.DrawIcon(x, y, m_hIcon);
    }
    else
    {
        CDialog::OnPaint();
    }

    // ��ÿ���ػ涼��ʾӦ�ó�����֮ǰ��ȡ���Ĵ��塿
    dc.BitBlt(0,0,m_nDestWidth,m_nDestHeight,&m_DeskTopDC,0,0,SRCCOPY);

    // ������ͼ����,��ɫ��״�ߡ�
    DrawScreen(dc);

    // ���ı��ͼ����ı���ɫΪ������
    ChangeBackGround(dc.m_hDC, m_rtAreaTracker.m_rect);
    //DrawGrayScreen(dc.m_hDC);

    // �����ƽ�ͼ�����л�ͼԪ�ء�
    DrawAllGraphParam(dc);

}

//************************************
// Method:    ��ʼ����ɫ��ˢ
// Author:    WangJie
// DateTime:  [7/20/2016]
//************************************
void CScreenShotToolDlg::InitGrayBitMap()
{
    if (NULL == m_pGray)
    {
        //��ɫλͼ
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
// Method:    �ı䱳��ɫ
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
    blend.SourceConstantAlpha= ALPHA_GRAY; // ͸���� ���255

    if (rect.left == rect.right || rect.top == rect.bottom)
    {
        //��ûѡ�����,ȫ���һ��� 
        AlphaBlend(hdc,0,0,m_nDestWidth,m_nDestHeight,memdc,0,0,Gray_Width,Gray_Height, blend);
    }
    else
    {
        //����ȥ���κ�alpha��������ĸ����򶼻һ���
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
// Method:    ���ƽ�ͼ
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
// Method:    ���ƽ�ͼ�����л�ͼԪ��
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
        // ���˴������Ρ�
        if ((*iters)->m_nType == GRAPH_RECTANGLE)
        {
            CPen pen(PS_SOLID ,2, RECTANGLE_COLOR);

            // ��ʹ�û��ʻ����Ρ�
            CPen* pOldPen     = dc.SelectObject(&pen);
            CBrush* pbrush    = CBrush::FromHandle((HBRUSH)GetStockObject(NULL_BRUSH));
            CBrush* pOldBrush = dc.SelectObject(pbrush);

            CRect rectCur;
            (*iters)->m_rt.GetTrueRect(&rectCur);        //���õ���������Ĵ�С��
            CRect rectArea;
            m_rtAreaTracker.GetTrueRect(&rectArea);      //���õ���ͼ����Ĵ�С��
            CRect rectNew;
            rectNew.IntersectRect(rectCur, rectArea);    //����ˢ�����ã�������ж������Ƿ�Խ�磬Խ����Ҫ�Ա߿�Ϊ�硿

            dc.Rectangle(&rectNew);                      //���������Ρ�

            dc.SelectObject(pOldBrush);
            dc.SelectObject(pOldPen);

            //(*iters)->m_rt.Draw(&dc, &pen);            //��ʹ����Ƥ����Ρ�
        }
        else if ((*iters)->m_nType == GRAPH_LINE)        //���˴����ߡ�
        {
            CPen pen(PS_SOLID, 2, LINE_COLOR);           //����һ����ġ�

            dc.SetROP2(R2_COPYPEN);
            CPen* pOldPen = dc.SelectObject(&pen);

            dc.MoveTo((*iters)->m_pPosX);
            dc.LineTo((*iters)->m_pPosY);
            
            dc.SelectObject(pOldPen);
        }
        else if ((*iters)->m_nType == GRAPH_PEN)
        {
            CPen pen(PS_SOLID, 2, PEN_COLOR);           //����һ����ġ�
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

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CScreenShotToolDlg::OnQueryDragIcon()
{
    return static_cast<HCURSOR>(m_hIcon);
}


void CScreenShotToolDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
    // TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
    if (!m_bIsChooseRect)
    {
        m_dlgMagnify->ShowWindow(SW_HIDE);
    }

    DrawAreaStart(point);        // ������ͼ��
    DrawLineStart(point);        // �����ߡ�
    DrawPenStart(point);         // �����ʡ�
    DrawRectangleStart(point);   // �������Ρ�

    CDialog::OnLButtonDown(nFlags, point);
}

//************************************
// Method:    ��ʼ����ͼ����
// Author:    WangJie
// DateTime:  [7/15/2016]
//************************************
void CScreenShotToolDlg::DrawAreaStart(CPoint point)
{
    if (m_bIsDrawArea)
    {
        m_ptAreaStartPoint = m_ptAreaEndPoint = point;

        if (m_rtAreaTracker.HitTest(point) < 0)     //�����δ���о���ѡ���,���»�ѡ���
        {
            m_rtAreaTracker.TrackRubberBand(this,m_ptAreaStartPoint,TRUE);
            m_rtAreaTracker.m_rect.NormalizeRect();
        }
        else           //��������о���ѡ���
        {
            // ��Track�����������֧�����������ı���ο��С �� ��ק���ο�ʱ��ʾ���ο�����
            m_rtAreaTracker.Track(this,m_ptAreaStartPoint,TRUE);
            m_rtAreaTracker.m_rect.NormalizeRect();
        }

        m_bIsChooseRect = true;
        m_bIsShowMagnify = false;

        Invalidate(FALSE);   // ��ˢ�´�������ʹ��CrectTracker�����ػ浽�����ϡ�
        UpdateWindow();
    }
}

//************************************
// Method:    ��ʼ����
// Author:    WangJie
// DateTime:  [7/15/2016]
//************************************
void CScreenShotToolDlg::DrawLineStart(CPoint point)
{
    if (m_bIsDrawLine)
    {
        m_bIsDrawArea   = false;
        m_bIsDrawRectangle = false;

        if (m_rtAreaTracker.m_rect.PtInRect(point))     // �����������ߣ����ھͲ�����
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

        if (m_rtAreaTracker.m_rect.PtInRect(point))     // �����������ߣ����ھͲ�����
        {
            SetCursor(LoadCursor(NULL,IDC_PEN));
            m_ptPenStartPoint = m_ptPenEndPoint = point;
        }
        m_vecPenPoint.push_back(point);
    }
}

//************************************
// Method:    ��ʼ������
// Author:    WangJie
// DateTime:  [7/15/2016]
//************************************
void CScreenShotToolDlg::DrawRectangleStart(CPoint point)
{
    if (m_bIsDrawRectangle)
    {
        m_bIsDrawArea = false;
        m_bIsDrawLine = false;

        if (m_rtAreaTracker.m_rect.PtInRect(point))     // �����������ߣ����ھͲ�����
        {
            SetCursor(LoadCursor(NULL,IDC_PEN));
            m_ptRectangleStartPoint = m_ptRectangleEndPoint = point;

            m_rtCurRectangleTracker.TrackRubberBand(this,m_ptRectangleStartPoint,TRUE);
            m_rtCurRectangleTracker.m_rect.NormalizeRect();   //��ʹCRect���Ϲ淶��ʹ��߶ȺͿ�Ⱦ�Ϊ��ֵ��

            // �����ｫ���ƵĶ������浽vector�У�����ʱʹ�á�
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
    // TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
    DrawAreaEnd(point);

    DrawLineEnd(point);

    DrawPenEnd(point);

    DrawRectangleEnd(point);

    Invalidate(FALSE);
    UpdateWindow();

    CDialog::OnLButtonUp(nFlags, point);
}

//************************************
// Method:    ��������ͼ����
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
// Method:    ��������
// Author:    WangJie
// DateTime:  [7/15/2016]
//************************************
void CScreenShotToolDlg::DrawLineEnd(CPoint point)
{
    if (m_bIsDrawLine)
    {
        if (m_rtAreaTracker.m_rect.PtInRect(point))     // ���ڲ��Ҽ������ͼ
        {
            m_ptLineEndPoint = point;
        }
        else
            m_ptLineEndPoint = m_ptLastInPoint;

        // �����ｫ���ƵĶ������浽vector�У�����ʱʹ�á�
        GraphParam *gp   = new GraphParam();
        gp->m_nType      = GRAPH_LINE;
        gp->m_pPosX      = m_ptLineStartPoint;
        gp->m_pPosY      = m_ptLineEndPoint;

        m_vecGraphParam.push_back(gp);
    }
}

//************************************
// Method:    ���ʽ���
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
// Method:    ����������
// Author:    WangJie
// DateTime:  [7/15/2016]
//************************************
void CScreenShotToolDlg::DrawRectangleEnd(CPoint point)
{
    if (m_bIsDrawRectangle)
    {
        if (!m_rtAreaTracker.m_rect.PtInRect(point))     // ���ڲ��Ҽ������ͼ
            return;
    }
}

void CScreenShotToolDlg::OnMouseMove(UINT nFlags, CPoint point)
{
    CWindowDC dc(this);

    //ShowAreaCursor(nFlags, point);    // ���������������ʾ������͡�

    ShowAutoArea(dc, point);            // ������ƶ�ʱ����ʾ�Զ�ʶ�������

    ShowLineGo(dc, nFlags, point);      // ������ʵ����껭��ʱ��ʾ�ߵĹ켣��

    ShowPenGo(dc, nFlags, point);       // ����ʾ���ʹ켣��

    ShowMagnifyDlg(dc);                 // ����ʾ�Ŵ󾵶Ի���

    ShowToolBar();                      // �����������������ʾ��

    CDialog::OnMouseMove(nFlags, point);
}

//************************************
// Method:    �ֶ���ʾ����ڽ�ͼ�������״
//            �˴�ʹ����Ƥ���SetCursor,�ʴ˽ӿڲ���
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
// Method:    ������ʾ����ƶ������Ӵ��ڱ߿�
// Author:    WangJie
// DateTime:  [7/15/2016]
//************************************
void CScreenShotToolDlg::ShowAutoArea(CDC& dc, CPoint point)
{
    if (!m_bIsChooseRect)
    {
        CRect rect;
        if (!m_allWindowsRect.GetRect(point, m_rect))   // �������ж���û�ڿ�ʼ��ȡ�����Ӵ����С�
        {
            memset((void*)&m_rect, 0, sizeof(m_rect) );
        }

        Invalidate(FALSE);
        UpdateWindow();

        if (m_rect.bottom != 0 && m_rect.left != 0 && m_rect.right != 0 && m_rect.top !=0 )
        {
            CWindowDC dc(this);
            ChangeBackGround(dc.m_hDC, m_rect);         // ��������Ʊ����һ���ʹѡ������������

            CPen pen(PS_SOLID,3,RGB(255,0,0));          // ��������Ƴɺ�ɫ��

            CPen *pOldPen = dc.SelectObject(&pen);      // ��ʹ�û��ʻ����Ρ�

            CBrush *pbrush = CBrush::FromHandle((HBRUSH)GetStockObject(NULL_BRUSH));

            CBrush *pOldBrush = dc.SelectObject(pbrush);

            dc.Rectangle(&m_rect);//��������

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
// Method:    ����ʵ����껭��ʱ��ʾ�ߵĹ켣
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

                CPen pen(PS_DOT, 1,RGB(0,255,255));     //�����ģʽ����ɫ�켣��
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
// Method:    ��ʾ���ʹ켣
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

                CPen pen(PS_DOT, 1,RGB(0,255,255));     //��һ�����
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
// Method:    ��ʾ����������
// Author:    WangJie
// DateTime:  [7/13/2016]
//************************************
void CScreenShotToolDlg::ShowToolBar()
{
    if (m_bIsChooseRect )
    {
        CRect wrect;
        m_tbToolBar.GetWindowRect(&wrect);

        const int nBorder = 5;              // �����������ͼ��������

        int nWidth   = wrect.Width();
        int nHeight  = wrect.Height();

        int nWidEx   = nWidth - TOOLBAR_WIDTH;     // ����������ʵ��С��

        if (m_rtAreaTracker.m_rect.right < nWidEx)
        {
            wrect.left   = m_rtAreaTracker.m_rect.left - TOOLBAR_WIDTH;         // ��������һֱ����
            wrect.right  = wrect.left + nWidth;                                 // �������ұ߼��������ұߡ�
        }
        else
        {
            wrect.left   = m_rtAreaTracker.m_rect.right - nWidth;               // ��������һֱ���ҡ�
            wrect.right  = m_rtAreaTracker.m_rect.right;                        // �������ұ߼��������ұߡ�
        }


        int nBottomLine = m_rtAreaTracker.m_rect.bottom + nHeight + nBorder;

        if (nBottomLine >= m_nDestHeight)
        {
            wrect.top = m_rtAreaTracker.m_rect.top - nBorder - nHeight;
            wrect.bottom = m_rtAreaTracker.m_rect.top - nBorder;
        }
        else
        {
            wrect.top    = m_rtAreaTracker.m_rect.bottom + nBorder;             // ��ʹ���������������·����м����5�����ص㡿
            wrect.bottom = m_rtAreaTracker.m_rect.bottom + nHeight + nBorder;   // ���������ײ����ǽ�ͼ�ײ�+ 5�����ص� +�������߶ȡ�
        }

        m_tbToolBar.MoveWindow(&wrect);
        m_tbToolBar.ShowWindow(SW_SHOW);
    }
    else
        m_tbToolBar.ShowWindow(SW_HIDE);

    UpdateWindow();
}

//************************************
// Method:    ��ʾ�Ŵ󾵶Ի��򣬲�������ƶ�
// Author:    WangJie
// DateTime:  [7/20/2016]
//************************************
void CScreenShotToolDlg::ShowMagnifyDlg(CDC& dc)
{
    CreateMagnify(dc);                    // ���Ŵ�ǰ��͸������ˢ�����Ŵ���ʾ�ڴˡ�

    if (m_bIsShowMagnify && !m_bIsChooseRect)
    {
        CPoint point;
        GetCursorPos(&point);

        int nLeft   = 0;
        int nTop    = 0;
        int nRight  = 0;
        int nBottom = 0;

        if (point.x + MAGNIFY_SIZE > m_nDestWidth)       // �������ұ߽硿
        {
            nLeft  = point.x - MAGNIFY_BORDER - MAGNIFY_SIZE;
            nRight = point.x - MAGNIFY_BORDER;
        }
        else
        {
            nLeft  = point.x + MAGNIFY_BORDER;
            nRight = point.x + MAGNIFY_BORDER + MAGNIFY_SIZE;
        }

        if (point.y  + MAGNIFY_SIZE > m_nDestHeight)      // �������±߽硿
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
// Method:    �����Ŵ�
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

        // ��ʹ�û��ʻ����Ρ�
        CPen pen(PS_SOLID,1,RGB(0, 0, 0));    // ��������Ƴɺ�ɫ��
        dc.SetROP2(R2_XORPEN); 
        CPen * pOldPen = dc.SelectObject(&pen);
        CBrush *pbrush = CBrush::FromHandle((HBRUSH)GetStockObject(NULL_BRUSH));
        CBrush *pOldBrush = dc.SelectObject(pbrush);

        dc.Rectangle(&rc);          //͸����ˢˢ��
        dc.SelectObject(pOldBrush);
        dc.SelectObject(pOldPen);
    }
}

void CScreenShotToolDlg::OnRButtonDown(UINT nFlags, CPoint point)
{
    // TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

    if (m_rtAreaTracker.m_rect.PtInRect(point))     // �����ڲ��Ҽ������ͼ��
    {
        SaveRectToMemDC();          // ��������Ƥ��Ȧס�������ڴ�dc��
        SaveToPicture();            // �������ڴ�DC�����ݵ��ļ���
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
    // TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ    
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
// Method:    �м�����£���ȡ��Ƥ��Ȧס�����򣬲����䵽�������Ͻǣ�ʵ��Ԥ��
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
// Method:    ���ڲ��Ҽ������ͼ
// Author:    WangJie
// DateTime:  [7/13/2016]
//************************************
void CScreenShotToolDlg::OnLButtonDblClk(UINT nFlags, CPoint point)
{
    if (m_rtAreaTracker.m_rect.PtInRect(point))
    {
        SaveRectToMemDC();          // ��������Ƥ��Ȧס�������ڴ�dc��
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
        // ����ʾ��
        //m_tToolTip.RelayEvent(pMsg);
    	//break;
    case WM_LBUTTONDOWN:
        break;
    case WM_LBUTTONUP:
        break;
    default:
        break;
    }

    // ����ݼ���
    if (m_hAccel)
    { 
        if (::TranslateAccelerator(m_hWnd,   m_hAccel,   pMsg))
            return(TRUE); 
    }

    return CDialog::PreTranslateMessage(pMsg);
}

//************************************
// Method:    ����ͼ���浽�ڴ�DC��
// Author:    WangJie
// DateTime:  [7/13/2016]
//************************************
void CScreenShotToolDlg::SaveRectToMemDC()
{
    // ��������Ƥ��Ȧס�������ڴ�dc��
    CClientDC dc(this);
    CRect re(m_rtAreaTracker.m_rect);
    m_bmpSaveMap.CreateCompatibleBitmap(&dc,re.Width(),re.Height());

    CDC tempDc;
    tempDc.CreateCompatibleDC(&dc);
    tempDc.SelectObject(&m_bmpSaveMap);
    tempDc.BitBlt(0,0,re.Width(),re.Height(),&dc,re.left,re.top,SRCCOPY);
}

//************************************
// Method:    �����ͼ�����ļ�
// Author:    WangJie
// DateTime:  [7/13/2016]
//************************************
void CScreenShotToolDlg::SaveToPicture()
{
    CFileDialog fDlg(FALSE,_T("png"), _T("δ����") ,4|2, _T("PNG ͼƬ��ʽ (*.png)|*.png|BMP ͼƬ��ʽ (*.bmp)|*.bmp|JPG ͼƬ��ʽ (*.jpg)|*.jpg||"));
    if (fDlg.DoModal()==IDOK)
    {
        CString strExt = _T(".png");
        strExt = fDlg.GetFileExt();
        strExt.Trim();
        strExt.MakeLower();

        CString bmpfile = fDlg.GetPathName();

        //����CImage����ͼƬ��
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

        OnClose();      // ����ͼ���˳�����
    }
}

//************************************
// Method:    ����ͼ���浽���а��У�����ճ���������ͼ��
// Author:    WangJie
// DateTime:  [7/13/2016]
//************************************
void CScreenShotToolDlg::SaveToClipboard()
{
    if (OpenClipboard())                            //���򿪼��а塿
    {
        EmptyClipboard();                           //����ռ����塿
        SetClipboardData(CF_BITMAP, m_bmpSaveMap);  //������Ļ����ճ�����������ϡ�
        CloseClipboard();                           //���رռ����塿

        OnClose();
    }
}

//************************************
// Method:    ������������߰�ť
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
// Method:    ��������������ΰ�ť
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
// Method:    ��������ͼ��
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
// Method:    ���������������ť
// Author:    WangJie
// DateTime:  [7/13/2016]
//************************************
void CScreenShotToolDlg::OnBnClickedBtnCancel()
{
     // ����������ͼ�ϻ��Ķ������������ˣ�ֻʣ��ͼ�ˣ������������б�ǣ��Ϳ������½�ͼ��
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

    // �������жϴ�С���������0�ˣ�����Ҫ����ͼ��Ǽ�������϶���
    if (m_vecGraphParam.size() <= 0)
    {
        ResetFlag();
    }

    Invalidate(FALSE);
    UpdateWindow();
}

//************************************
// Method:    ������������水ť
// Author:    WangJie
// DateTime:  [7/13/2016]
//************************************
void CScreenShotToolDlg::OnBnClickedBtnSave()
{
    SaveRectToMemDC();          // ��������Ƥ��Ȧס�������ڴ�dc��
    SaveToPicture();            // �������ڴ�DC�����ݵ��ļ���
}

//************************************
// Method:    ����������رհ�ť
// Author:    WangJie
// DateTime:  [7/13/2016]
//************************************
void CScreenShotToolDlg::OnBnClickedBtnClose()
{
    PostQuitMessage(0);
}

//************************************
// Method:    ���������OK��ť�������ͼ�����а�
// Author:    WangJie
// DateTime:  [7/13/2016]
//************************************
void CScreenShotToolDlg::OnBnClickedBtnOK()
{
    SaveRectToMemDC();          // ��������Ƥ��Ȧס�������ڴ�dc��
    SaveToClipboard();
}

//************************************
// Method:    ��ȡ���߽��㣬��ʱû�õ�
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
// Method:    ��ȡ���߽��㷽��ʽ�Ĳ�������ʱû�õ�
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
// Method:    �˳�ʱ����new����Դ�������ڴ�й©
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
// Method:    �����ͼ�еĻ�ͼ
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
// Method:    ���ñ��
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
