
// ScreenShotToolDlg.h : ͷ�ļ�
//

#pragma once

#include "EnumAllWindowsRect.h"     // �����������Ӵ��ڡ�
#include "define.h"
#include "CMagnifyDlg.h"

// CScreenShotToolDlg �Ի���
class CScreenShotToolDlg : public CDialog
{
// ����
public:
	CScreenShotToolDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_SCREENSHOTTOOL_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

private:
    
    int             m_nDestWidth;                   // ����Ļ���ȡ�
    int             m_nDestHeight;                  // ����Ļ�߶ȡ�
    int             m_rectRectangleNum;             // �����θ�����

    bool            m_bIsDrawArea;                  // ���������ǡ�
    bool            m_bIsDrawLine;                  // �����߱�ǡ�
    bool            m_bIsDrawRectangle;             // �������α�ǡ�
    bool            m_bIsDrawPen;                   // �����ʲ�����
    bool            m_bIsChooseRect;                // ���Ƿ�ѡ����ʶ�������ǡ�
    bool            m_bIsShowMagnify;

    CDC             m_DeskTopDC;                    // ������DC��
    CBitmap         m_bmpDestTop;                   // ���ڴ��г�����ʱͼ���λͼ��

    POINT           m_ptAreaStartPoint;             // �������ͼ��ʼ���꡿
    POINT           m_ptAreaEndPoint;               // �������ͼ�������꡿

    POINT           m_ptLineStartPoint;             // ���߽�ͼ��ʼ���꡿
    POINT           m_ptLineEndPoint;               // ���߽�ͼ�������꡿

    POINT           m_ptRectangleStartPoint;        // �����ν�ͼ��ʼ���꡿
    POINT           m_ptRectangleEndPoint;          // �����ν�ͼ�������꡿

    POINT           m_ptPenStartPoint;              // ��������ͼ��ʼ���꡿
    POINT           m_ptPenEndPoint;                // ��������ͼ�������꡿

    POINT           m_ptLastInPoint;                // �����һ���������ڵĵ㣬������ʱ���������á�

    RECT            m_rect;                         // ���Ӵ�����ʶ������

    CBitmap         m_pBitmap;                      // ����ͼʱ�����浱ǰ���棬�����������ڵı�����
    HBITMAP         m_pGray;                        // ����ɫ��ˢλͼ��

    CBitmap              m_bmpSaveMap;              // ����ͼ����λͼ��

    CToolTipCtrl         m_tToolTip;                // �����ݹ�����ʾ��

    HACCEL               m_hAccel;                  // �����ټ�ʵ���ȼ����ܡ�

    CToolBar             m_tbToolBar;               // ��ToolBar��������

    CToolBar             m_tbColorToolBar;          // ����ɫ��������

    CRectTrackerEx       m_rtAreaTracker;           // ����Ƥ�������ͼ��
    CRectTrackerEx       m_rtCurRectangleTracker;   // ����Ƥ����������Ρ�

    vector<GraphParam*>  m_vecGraphParam;           // ��������������ͼ�Ρ�
    vector<CPoint>       m_vecPenPoint;             // �������򻭱ʵ����걣�桿

    EnumAllWindowsRect   m_allWindowsRect;          // ����ȡ�����Ӵ��ڡ�

    CImageList           m_ilImageList;             // ��icoͼƬ�б�������ʹ�á�

    CCMagnifyDlg*        m_dlgMagnify;              // ���Ŵ󾵶Ի���

public:
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
    afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
    afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
    afx_msg void OnMButtonDblClk(UINT nFlags, CPoint point);                // ������м�˫�����Ͻ�Ԥ����
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);                // ��˫���������λͼ���ڴ桿
    afx_msg BOOL OnToolBarTip(UINT id, NMHDR * pNMHDR, LRESULT * pResult );
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);

    afx_msg void OnBnClickedBtnSave();                  // ��ʵ��һ����ť������λͼ���ܣ����ȼ���ToolBarʹ�á�
    afx_msg void OnBnClickedBtnLine();                  // �����������߰�ť��
    afx_msg void OnBnClickedBtnRectangle();             // �������������ΰ�ť��
    afx_msg void OnBnClickedBtnPen();                   // ����������������ť��
    afx_msg void OnBnClickedBtnCancel();                // ��������������ť��
    afx_msg void OnBnClickedBtnClose();
    afx_msg void OnBnClickedBtnOK();
    afx_msg void OnClose();

    virtual BOOL PreTranslateMessage(MSG* pMsg);

    void InitGrayBitMap();                          // ����ʼ����ɫ������
    void InitParam();                               // ����ʼ����Ƥ��ȡ�
    void InitScreenDC();                            // ����ʼ����Ļ��
    void InitToolBar();                             // ����ʼ����������
    void InitMagnifyDlg();                          // ����ʼ���Ŵ󾵶Ի���
    void InitColorToolBar();                        // ����ʼ����ɫ�Ի���
    void InitToolTip();                             // ����ʼ����ʾ����
    void ResetFlag();                               // ��״̬��λ��

    void DrawAreaStart(CPoint point);               // ��������ʼ��
    void DrawAreaEnd(CPoint point);                 // �������������

    void DrawLineStart(CPoint point);               // �����߿�ʼ��
    void DrawLineEnd(CPoint point);                 // �����߽�����

    void DrawRectangleStart(CPoint point);          // �������ο�ʼ��
    void DrawRectangleEnd(CPoint point);            // �������ν�����

    void DrawPenStart(CPoint point);                // �����ʲ�����ͼ�ο�ʼ��
    void DrawPenEnd(CPoint point);                  // �����ʲ�����ͼ�ν�����

    void DrawScreen(CDC& dc);                       // ������ͼ��
    void DrawAllGraphParam(CDC& dc);                // �����ƽ�ͼ������Ԫ�ء�

    void SaveToPicture();                           // �������BmpͼƬ��
    void SaveRectToMemDC();                         // �������ͼ�����ڴ�dc�С�
    void SaveToClipboard();                         // �����浽���а��С�

    void CreateMagnify(CDC& dc);                    // ������һ���Ŵ󾵻�����
    void ShowMagnifyDlg(CDC& dc);                   // ����ʾ�Ŵ����ݡ�
    void ShowToolBar();                             // ����ʾ��������
    void ClearGraphParam();                         // �������ͼ�����ڻ�ͼ��

    void ShowLineGo(CDC& dc, int nFlags, CPoint point);      // ����ʾ���߹켣��
    void ShowPenGo(CDC& dc, int nFlags, CPoint point);       // ����ʾ���ʹ켣��
    void ShowAutoArea(CDC& dc, CPoint point);                // ����ʾ��ʶ������
    void ShowAreaCursor(int nFlags, CPoint point);           // ����ͼ���������״��ʾ��

    void ChangeBackGround(HDC& hdc, CRect rect);             // ���ı䱳��ɫ��

    bool GetLineToLineNode(LineParam para1,LineParam para2,CPoint &point);      // ����ȡ���߽��㡿
    void GetLineParam(long x1,long y1,long x2,long y2,LineParam& para);         // ����ȡ���߽��㷽�̲���ֵ��
};
