// CMAgnifyDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ScreenShotTool.h"
#include "CMAgnifyDlg.h"


// CCMAgnifyDlg �Ի���

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


// CCMAgnifyDlg ��Ϣ�������

BOOL CCMagnifyDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    // TODO:  �ڴ���Ӷ���ĳ�ʼ��
    InitScreen();

    return TRUE;  // return TRUE unless you set the focus to a control
    // �쳣: OCX ����ҳӦ���� FALSE
}

void CCMagnifyDlg::OnClose()
{
    // TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
    PostQuitMessage(0);

    CDialog::OnClose();
}

BOOL CCMagnifyDlg::OnEraseBkgnd(CDC* pDC)
{
    // TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
    return CDialog::OnEraseBkgnd(pDC);
}

BOOL CCMagnifyDlg::PreTranslateMessage(MSG* pMsg)
{
    // ������ػ�esc��Ϣ���˳���
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
// Method:    ��ʼ����Ļ
// Author:    WangJie
// DateTime:  [7/20/2016]
//************************************
void CCMagnifyDlg::InitScreen()
{
    m_nDestWidth  = GetSystemMetrics(SM_CXSCREEN);  // ���������Ļ��ȡ�
    m_nDestHeight = GetSystemMetrics(SM_CYSCREEN);  // ���������Ļ�߶ȡ�

    CWindowDC dc(GetDesktopWindow());               // ���Ե�ǰ�����洰�崴��һ��DC��

    m_DeskTopDC.CreateCompatibleDC(&dc);                   // ������һ���������洰���dc������bitmap��
    m_bmpDestTop.CreateCompatibleBitmap(&dc, m_nDestWidth, m_nDestHeight);
    m_DeskTopDC.SelectObject(m_bmpDestTop);         // ����λͼѡ���ڴ�DC��

    // ����ԭ���������ͻ�������Ȼ���Ǻ�ɫ��
    m_DeskTopDC.BitBlt(0, 0, m_nDestWidth, m_nDestHeight, &dc, 0, 0, SRCCOPY);    // ��������DC���ݴ���Ӧ�ó����е�dc,֧��ͼ�ο�ĸ��ƣ��ٶȺܿ졿

    SetWindowLong(GetSafeHwnd(),GWL_EXSTYLE,GetWindowLong(GetSafeHwnd(),GWL_EXSTYLE)|WS_EX_LAYERED);
    SetLayeredWindowAttributes(0,255,LWA_ALPHA);
}

//************************************
// Method:    ���Ŵ�
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

    //�Ŵ����
    dc.StretchBlt(  0, 0,                // ��Ŀ��������ʼ���꡿
                    m_rect.Width(),      // ��Ŀ�����ĳ���
                    m_rect.Height(),     // ��Ŀ�����ĳ���
                    &m_DeskTopDC,        // ������Դdc��
                    point.x, point.y,    // ��Դy���꡿
                    m_rect.Width()/MAGNIFY_MULTIPLE,   // ��Դ���εĿ�ȡ�
                    m_rect.Height()/MAGNIFY_MULTIPLE,  // ��Դ���εĸ߶ȡ�
                    SRCCOPY);
}

//************************************
// Method:    ���Ŵ󾵶Ի���߿�
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

        //����䶥����ܡ�
        point.x = rtWnd.Width();
        point.y = GetSystemMetrics(SM_CYFRAME)+1;
        pDC->PatBlt(0, 0, point.x, point.y, PATCOPY);

        //���������ܡ�
        point.x = GetSystemMetrics(SM_CXFRAME);
        point.y = rtWnd.Height();
        pDC->PatBlt(0, 0, point.x, point.y, PATCOPY);

        //�����ײ���ܡ�
        point.x = rtWnd.Width(); 
        point.y = GetSystemMetrics(SM_CYFRAME) + 1;
        pDC->PatBlt(0, rtWnd.Height()-point.y, point.x, point.y, PATCOPY);

        //������Ҳ��ܡ�
        point.x = GetSystemMetrics(SM_CXFRAME);
        point.y = rtWnd.Height();
        pDC->PatBlt(rtWnd.Width()-point.x, 0, point.x, point.y, PATCOPY);
    }
}
