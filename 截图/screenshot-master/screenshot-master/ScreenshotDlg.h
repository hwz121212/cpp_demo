
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

	BOOL m_bDraw;                   //�Ƿ�Ϊ��ȡ״̬
	BOOL m_bFirstDraw;              //�Ƿ�Ϊ�״ν�ȡ
	BOOL m_bQuit;                   //�Ƿ�Ϊ�˳�
	BOOL m_bDrawImage;				//��ͼ���ͣ��������Σ�ֱ�ߣ���ͷ�����ߣ����ֵ�
	DWORD m_nDrawType;
	CPoint m_startPt;				//��ȡ�������Ͻ�
	CPoint m_endPt;             //��ȡ�������½�
	CPoint m_startPt2;			//��ͼ�����Ͻ�
	CPoint m_prePt;             //��ͼ��һ����
	BaseTracker m_rectTracker;     //��Ƥ����
	//CMyTracker m_rectTracker2;
	CBrush m_brush;					//
    HCURSOR m_hCursor;              //���
	CBitmap * m_pBitmap;            //����λͼ
	CList<CShape*,CShape*>m_list;   //�����ͼ
	CList<CPoint,CPoint>m_pointList;
	CDlgBar *m_pDlgBar;
	BOOL m_bBtnState[5];           //�����ͼ��5����ť��״̬ falseΪ����̬��trueΪ��ѹ̬
	BOOL m_bMoveRect;              //��ʶ�ܷ��ƶ���ͼ���򣬳�ʼΪtrue�������������˵���Ϊfalse
	BOOL m_bFisrtDrawArrow;        //��ֹһ��ʼ�ͻ�2��ʵ�ļ�ͷ
	BOOL m_bMenuVisiable;
	CFont m_Font;
	CRgn m_rgn;						//������������
	CString m_csCacheFile;
	CString m_csSaveFileName;

	HWND hMainWnd;
public:
	void CreateDarkBitmapFile();    //������ɫ��BMP�ļ�
	HBITMAP CopyScreenToBitmap(LPRECT lpRect);   //�������浽λͼ
	void SaveBitmapToClipboard(HBITMAP hBmp);	//����λͼ�����а�
	void SaveBitmapToFile(HBITMAP hBmp, LPCWSTR lpfilename); //����λͼ���ļ���
	void DrawImage(CDC *pDC,CPoint point);         //�ڽ�ͼ�����ͼ
	void Reset();                                  //�������ý�ͼ�����Ϣ
	void DrawArrow(CDC *pDC,CPoint ptStart,CPoint ptEnd);  //��ʵ�ļ�ͷ
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
