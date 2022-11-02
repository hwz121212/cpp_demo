
// ScreenShotToolDlg.h : 头文件
//

#pragma once

#include "EnumAllWindowsRect.h"     // 【遍历所有子窗口】
#include "define.h"
#include "CMagnifyDlg.h"

// CScreenShotToolDlg 对话框
class CScreenShotToolDlg : public CDialog
{
// 构造
public:
	CScreenShotToolDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_SCREENSHOTTOOL_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

private:
    
    int             m_nDestWidth;                   // 【屏幕长度】
    int             m_nDestHeight;                  // 【屏幕高度】
    int             m_rectRectangleNum;             // 【矩形个数】

    bool            m_bIsDrawArea;                  // 【画区域标记】
    bool            m_bIsDrawLine;                  // 【画线标记】
    bool            m_bIsDrawRectangle;             // 【画矩形标记】
    bool            m_bIsDrawPen;                   // 【画笔不规则】
    bool            m_bIsChooseRect;                // 【是否选中自识别区域标记】
    bool            m_bIsShowMagnify;

    CDC             m_DeskTopDC;                    // 【桌面DC】
    CBitmap         m_bmpDestTop;                   // 【内存中承载临时图像的位图】

    POINT           m_ptAreaStartPoint;             // 【区域截图起始坐标】
    POINT           m_ptAreaEndPoint;               // 【区域截图结束坐标】

    POINT           m_ptLineStartPoint;             // 【线截图起始坐标】
    POINT           m_ptLineEndPoint;               // 【线截图结束坐标】

    POINT           m_ptRectangleStartPoint;        // 【矩形截图起始坐标】
    POINT           m_ptRectangleEndPoint;          // 【矩形截图结束坐标】

    POINT           m_ptPenStartPoint;              // 【不规则画图起始坐标】
    POINT           m_ptPenEndPoint;                // 【不规则画图结束坐标】

    POINT           m_ptLastInPoint;                // 【最后一个在区域内的点，供画线时超出区域用】

    RECT            m_rect;                         // 【子窗口自识别区域】

    CBitmap         m_pBitmap;                      // 【截图时，保存当前桌面，当做截屏窗口的背景】
    HBITMAP         m_pGray;                        // 【灰色画刷位图】

    CBitmap              m_bmpSaveMap;              // 【截图区域位图】

    CToolTipCtrl         m_tToolTip;                // 【气泡功能提示】

    HACCEL               m_hAccel;                  // 【加速键实现热键功能】

    CToolBar             m_tbToolBar;               // 【ToolBar工具条】

    CToolBar             m_tbColorToolBar;          // 【颜色工具条】

    CRectTrackerEx       m_rtAreaTracker;           // 【橡皮筋，用来截图】
    CRectTrackerEx       m_rtCurRectangleTracker;   // 【橡皮筋，用来画矩形】

    vector<GraphParam*>  m_vecGraphParam;           // 【用来保存所有图形】
    vector<CPoint>       m_vecPenPoint;             // 【不规则画笔点坐标保存】

    EnumAllWindowsRect   m_allWindowsRect;          // 【获取所有子窗口】

    CImageList           m_ilImageList;             // 【ico图片列表，工具栏使用】

    CCMagnifyDlg*        m_dlgMagnify;              // 【放大镜对话框】

public:
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
    afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
    afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
    afx_msg void OnMButtonDblClk(UINT nFlags, CPoint point);                // 【鼠标中键双击左上角预览】
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);                // 【双击左键保存位图到内存】
    afx_msg BOOL OnToolBarTip(UINT id, NMHDR * pNMHDR, LRESULT * pResult );
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);

    afx_msg void OnBnClickedBtnSave();                  // 【实现一个按钮，保存位图功能，供热键和ToolBar使用】
    afx_msg void OnBnClickedBtnLine();                  // 【工具栏画线按钮】
    afx_msg void OnBnClickedBtnRectangle();             // 【工具栏画矩形按钮】
    afx_msg void OnBnClickedBtnPen();                   // 【工具栏画不规则按钮】
    afx_msg void OnBnClickedBtnCancel();                // 【工具栏撤销按钮】
    afx_msg void OnBnClickedBtnClose();
    afx_msg void OnBnClickedBtnOK();
    afx_msg void OnClose();

    virtual BOOL PreTranslateMessage(MSG* pMsg);

    void InitGrayBitMap();                          // 【初始化灰色画布】
    void InitParam();                               // 【初始化橡皮筋等】
    void InitScreenDC();                            // 【初始化屏幕】
    void InitToolBar();                             // 【初始化工具条】
    void InitMagnifyDlg();                          // 【初始化放大镜对话框】
    void InitColorToolBar();                        // 【初始化颜色对话框】
    void InitToolTip();                             // 【初始化提示栏】
    void ResetFlag();                               // 【状态复位】

    void DrawAreaStart(CPoint point);               // 【画区域开始】
    void DrawAreaEnd(CPoint point);                 // 【画区域结束】

    void DrawLineStart(CPoint point);               // 【画线开始】
    void DrawLineEnd(CPoint point);                 // 【画线结束】

    void DrawRectangleStart(CPoint point);          // 【画矩形开始】
    void DrawRectangleEnd(CPoint point);            // 【画矩形结束】

    void DrawPenStart(CPoint point);                // 【画笔不规则图形开始】
    void DrawPenEnd(CPoint point);                  // 【画笔不规则图形结束】

    void DrawScreen(CDC& dc);                       // 【画截图】
    void DrawAllGraphParam(CDC& dc);                // 【绘制截图上所有元素】

    void SaveToPicture();                           // 【保存成Bmp图片】
    void SaveRectToMemDC();                         // 【保存截图区域到内存dc中】
    void SaveToClipboard();                         // 【保存到剪切板中】

    void CreateMagnify(CDC& dc);                    // 【创建一个放大镜画布】
    void ShowMagnifyDlg(CDC& dc);                   // 【显示放大镜内容】
    void ShowToolBar();                             // 【显示工具条】
    void ClearGraphParam();                         // 【清理截图区域内画图】

    void ShowLineGo(CDC& dc, int nFlags, CPoint point);      // 【显示画线轨迹】
    void ShowPenGo(CDC& dc, int nFlags, CPoint point);       // 【显示画笔轨迹】
    void ShowAutoArea(CDC& dc, CPoint point);                // 【显示自识别区域】
    void ShowAreaCursor(int nFlags, CPoint point);           // 【截图区域鼠标形状显示】

    void ChangeBackGround(HDC& hdc, CRect rect);             // 【改变背景色】

    bool GetLineToLineNode(LineParam para1,LineParam para2,CPoint &point);      // 【获取线线交点】
    void GetLineParam(long x1,long y1,long x2,long y2,LineParam& para);         // 【获取线线交点方程参数值】
};
