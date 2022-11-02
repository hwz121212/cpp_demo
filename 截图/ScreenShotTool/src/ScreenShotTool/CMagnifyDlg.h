#pragma once

#include "define.h"


// CCMAgnifyDlg 对话框

class CCMagnifyDlg : public CDialog
{
	DECLARE_DYNAMIC(CCMagnifyDlg)

public:
	CCMagnifyDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CCMagnifyDlg();

// 对话框数据
	enum { IDD = IDD_DLG_MAGNIFY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
    virtual BOOL OnInitDialog();
    afx_msg void OnClose();
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    afx_msg void OnPaint();
    virtual BOOL PreTranslateMessage(MSG* pMsg);

    int             m_nDestWidth;                   // 【屏幕长度】
    int             m_nDestHeight;                  // 【屏幕高度】

    CDC             m_DeskTopDC;                    // 【桌面DC】
    CBitmap         m_bmpDestTop;                   // 【内存中承载临时图像的位图】
    
    CRect           m_rect;

    void InitScreen();              // 【初始化放大镜对话框dc】

    void DrawMagnify(CDC& dc);      // 【画放大镜】

    void DrawBorder(CDC *pDC);      // 【画边框】
};
