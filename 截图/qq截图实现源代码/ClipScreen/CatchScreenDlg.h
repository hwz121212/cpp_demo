#if !defined(AFX_CATCHSCREENDLG_H__4D299E27_713A_41D5_B80C_DD90AD666753__INCLUDED_)
#define AFX_CATCHSCREENDLG_H__4D299E27_713A_41D5_B80C_DD90AD666753__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CatchScreenDlg.h : header file
//
/////////////////////////////////////////////////////////////////////////////
// CCatchScreenDlg dialog
#ifndef MYTRACKER_
#include "MyTracker.h"
#endif
#include "22.h"
#include "CatchToolBar.h"
#include <GdiPlus.h>
#pragma comment(lib,"gdiplus.lib")
#include "PaintWnd.h"



class CCatchScreenDlg : public CDialog
{

//////////////////////////////////////////////////////////////////////////
public:
	HBITMAP CopyScreenToBitmap(LPRECT lpRect,BOOL bSave=FALSE);
	void PaintWindow();
	friend void OnItemClick(CToolItem *pPtem,CWnd *pWnd);
private:
	int m_leftX;                    //截屏左上角位置
	int m_topY;					    //截屏左上角位置
	int m_xScreen;                  //屏幕宽度
	int m_yScreen;                  //屏幕高度
	HCURSOR m_hCursor;              //光标
    CBitmap * m_pBitmap;            //背景位图
	CPoint m_startPt;				//截取矩形左上角
	CMyTracker m_rectTracker;       //像皮筋类
	CCatchToolBar m_toolBar;		//工具栏
	BOOL m_bShowMsg;                //显示截取矩形大小信息
	BOOL m_bDraw;                   //是否为截取状态
	BOOL m_bFirstDraw;              //是否为首次截取
	int  m_nHitPos;                 //点击左键时,鼠标选择框哪个位置
	CWnd *m_pSnapShotWnd;			//指定为某窗口的截取（不是整个屏幕的）
	BOOL m_bUseInitPos;
	BOOL m_IsEnableMove;			//Rect 是否允许调整
	CPaintWnd m_PaintWnd;			// 绘画窗口
	
private:
	Gdiplus::GdiplusStartupInput m_gdipStart;
	ULONG_PTR m_gdiToken;

	BOOL Save();
	BOOL SaveimageToFile(HBITMAP hBitmap,const TCHAR *szFile,const TCHAR* imgtype);
	int GetEncoderClsid(const WCHAR* format, CLSID* pClsid);
	void DrawInfo(CDC *dc,const CRect &rtDraw);////在矩形框具体的绘画
	void SetDrawWay(UINT nWay);
	
//////////////////////////////////////////////////////////////////////////

// Construction
public:
	CCatchScreenDlg(CWnd *pSnapshotWnd = NULL,CWnd* pParent = NULL);   // standard constructor
    ~CCatchScreenDlg();
// Dialog Data
	//{{AFX_DATA(CCatchScreenDlg)
	enum { IDD = IDD_CATCHSCREEN_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCatchScreenDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCatchScreenDlg)
	virtual void OnOK();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	virtual BOOL OnInitDialog();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	virtual void OnCancel();
	afx_msg void OnPaint();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CATCHSCREENDLG_H__4D299E27_713A_41D5_B80C_DD90AD666753__INCLUDED_)
