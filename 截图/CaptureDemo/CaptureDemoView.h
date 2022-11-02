// CaptureDemoView.h : interface of the CCaptureDemoView class
//


#pragma once


class CCaptureDemoView : public CScrollView
{
protected: // create from serialization only
	CCaptureDemoView();
	DECLARE_DYNCREATE(CCaptureDemoView)

// Attributes
public:
	CCaptureDemoDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void OnInitialUpdate(); // called first time after construct

// Implementation
public:
	virtual ~CCaptureDemoView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
   virtual void OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/);
};

#ifndef _DEBUG  // debug version in CaptureDemoView.cpp
inline CCaptureDemoDoc* CCaptureDemoView::GetDocument() const
   { return reinterpret_cast<CCaptureDemoDoc*>(m_pDocument); }
#endif

