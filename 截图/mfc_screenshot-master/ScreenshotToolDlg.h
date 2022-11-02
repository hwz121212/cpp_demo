#pragma once

#include "MyTracker.h"
#include "MyEdit.h"
#include "Resource.h"
// CScreenshotToolDlg 对话框

class CScreenshotToolDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CScreenshotToolDlg)

public:
	CScreenshotToolDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CScreenshotToolDlg();
	// Generated message map functions
	//{{AFX_MSG(CCatchScreenDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	
// 对话框数据
	enum { IDD = IDD_SCREENSHOTTOOLDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()


public:
	HICON m_hIcon;

	int m_xScreen;
	int m_yScreen;
	
	BOOL m_bShowMsg;                //显示截取矩形大小信息
	BOOL m_bDraw;                   //是否为截取状态
	BOOL m_bFirstDraw;              //是否为首次截取
	BOOL m_bQuit;                   //是否为退出
	CPoint m_startPt;				//截取矩形左上角
	CMyTracker m_rectTracker;     //像皮筋类
	
	CBrush m_brush;					//
    HCURSOR m_hCursor;              //光标
	CBitmap * m_pBitmap;            //背景位图
	
	CRgn m_rgn;						//背景擦除区域

public:
	HBITMAP CopyScreenToBitmap(LPRECT lpRect,BOOL bSave=FALSE);   //考贝桌面到位图
	void DrawTip();                            //显示操作提示信息
	void DrawMessage(CRect &inRect,CDC * pDC);       //显示截取矩形信息
	void PaintWindow();               //重画窗口
	void ChangeRGB();	
	
	BOOL isShown;
	afx_msg void OnDestroy();

	int m_max_width;
	int m_max_height;
};
