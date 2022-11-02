
#pragma once

#include "Resource.h"
#include "Shape.h"
#include "BaseTracker.h"
#include "Rectangle.h"
#include "Line.h"
#include "Ellipse.h"
#include "Curve.h"
#include "DlgBar.h"
#include "EditText.h"
#include "InputEdit.h"

#define WM_GETCOMMAND WM_USER + 1988
/////////////////////////////////////////////////////////////////////////////
// CScreenshotDlg dialog

class CScreenshotDlg : public CDialog
{
	// Construction
public:

	CScreenshotDlg(CWnd* pParent = NULL);	// standard constructor
	~CScreenshotDlg();

	// Dialog Data
	//{{AFX_DATA(CScreenshotDlg)
	enum { IDD = IDD_SCREENSHOT_DIALOG };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CScreenshotDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

//**********************************************************************
public:
	HBITMAP m_hBitmap;
	int m_xScreen;
	int m_yScreen;

	BOOL m_bDraw;                   //是否为截取状态
	BOOL m_bFirstDraw;              //是否为首次截取
	BOOL m_bQuit;                   //是否为退出
	BOOL m_bDrawImage;				//绘图类型，包括矩形，直线，箭头，曲线，文字等
	DWORD m_nDrawType;
	CPoint m_startPt;				//截取矩形左上角
	CPoint m_endPt;             //截取矩形右下角
	CPoint m_startPt2;			//绘图的左上角
	CPoint m_prePt;             //绘图上一个点
	BaseTracker m_rectTracker;     //像皮筋类
	//CMyTracker m_rectTracker2;
	CBrush m_brush;					//
    HCURSOR m_hCursor;              //光标
	CBitmap * m_pBitmap;            //背景位图
	CList<CShape*,CShape*>m_list;   //保存绘图
	CList<CPoint,CPoint>m_pointList;
	CDlgBar *m_pDlgBar;
	BOOL m_bBtnState[5];           //保存绘图中5个按钮的状态 false为正常态，true为下压态
	BOOL m_bMoveRect;              //标识能否移动截图区域，初始为true，点击工具条或菜单后为false
	BOOL m_bFisrtDrawArrow;        //防止一开始就画2个实心箭头
	BOOL m_bMenuVisiable;
	CFont m_Font;
	CRgn m_rgn;						//背景擦除区域
	CString m_csCacheFile;
	CString m_csSaveFileName;

	HWND hMainWnd;
public:
	void CreateDarkBitmapFile();    //产生暗色调BMP文件
	HBITMAP CopyScreenToBitmap(LPRECT lpRect);   //考贝桌面到位图
	void SaveBitmapToClipboard(HBITMAP hBmp);	//保存位图到剪切板
	void SaveBitmapToFile(HBITMAP hBmp, LPCWSTR lpfilename); //保存位图到文件中
	void DrawImage(CDC *pDC,CPoint point);         //在截图区域绘图
	void Reset();                                  //重新设置截图相关信息
	void DrawArrow(CDC *pDC,CPoint ptStart,CPoint ptEnd);  //画实心箭头
//**********************************************************************

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CScreenshotDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnMenuCommand(UINT);
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
	afx_msg LRESULT OnUpdateToolBar(WPARAM wParam, LPARAM lParam); 
	afx_msg LRESULT OnGetCommond(WPARAM wParam, LPARAM lParam); 


	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	CInputEdit m_Edit;
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#include "afxwin.h"
