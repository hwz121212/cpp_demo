// homework11View.h : interface of the CHomework11View class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_HOMEWORK11VIEW_H__8EC3A5FF_BF8F_42FE_802A_55D6DA1A2BF5__INCLUDED_)
#define AFX_HOMEWORK11VIEW_H__8EC3A5FF_BF8F_42FE_802A_55D6DA1A2BF5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CHomework11View : public CScrollView
{
protected: // create from serialization only
	CHomework11View();
	DECLARE_DYNCREATE(CHomework11View)

// Attributes
public:
	int m_down4;
	int getcolour();
	int getshape();
	int getstype();
	double getsize();
	int upordown;
	CList<CPoint,CPoint>m_cpoint;
	CList<CPoint,CPoint>m_point;
	CList<int,int>m_colour;
	CList<int,int>m_shape;
	CHomework11Doc* GetDocument();
CPoint cpoint;CPoint bpoint;
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHomework11View)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual void OnInitialUpdate(); // called first time after construct
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CHomework11View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CHomework11View)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);

	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in homework11View.cpp
inline CHomework11Doc* CHomework11View::GetDocument()
   { return (CHomework11Doc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HOMEWORK11VIEW_H__8EC3A5FF_BF8F_42FE_802A_55D6DA1A2BF5__INCLUDED_)
