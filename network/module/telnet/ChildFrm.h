// ChildFrm.h : interface of the CChildFrame class
//


#pragma once


class CChildFrame : public CMDIChildWnd
{
        DECLARE_DYNCREATE(CChildFrame)
public:
        CChildFrame();

// Attributes
public:

// Operations
public:

// Overrides
        virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// Implementation
public:
        virtual ~CChildFrame();
#ifdef _DEBUG
        virtual void AssertValid() const;
        virtual void Dump(CDumpContext& dc) const;
#endif

// Generated message map functions
protected:
        CStatusBar  m_wndStatusBar;
        DECLARE_MESSAGE_MAP()
public:
        afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
        afx_msg void OnPane1(CCmdUI* pCmdUI);
        CString m_pane1;
};
