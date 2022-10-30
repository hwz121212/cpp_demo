// WCView.h : interface of the CWCView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_WCVIEW_H__515D39F6_F973_406A_AC54_2B392A9BC789__INCLUDED_)
#define AFX_WCVIEW_H__515D39F6_F973_406A_AC54_2B392A9BC789__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CWCView : public CView
{
protected: // create from serialization only
	CWCView();
	DECLARE_DYNCREATE(CWCView)

// Attributes
public:
	CWCDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWCView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	CDC dcmomory;
	CBitmap bmp;
	CString flag;
	bool m_isbuttondown;
	int x1,x2,y1,y2;
	int width;
	COLORREF colorpen;
	COLORREF colorbrush;
	COLORREF fontcolor;
	CFont fontstyle;
	virtual ~CWCView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CWCView)
	afx_msg void OnRectangle();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnColorpen();
	afx_msg void OnFillrectangle();
	afx_msg void OnColorbrush();
	afx_msg void OnFont();
	afx_msg void OnFont1();
	afx_msg void OnLinewidthdlg();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in WCView.cpp
inline CWCDoc* CWCView::GetDocument()
   { return (CWCDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WCVIEW_H__515D39F6_F973_406A_AC54_2B392A9BC789__INCLUDED_)
