// bmView.h : interface of the CBmView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_BMVIEW_H__23EA2312_13B9_4B5E_9735_936394358512__INCLUDED_)
#define AFX_BMVIEW_H__23EA2312_13B9_4B5E_9735_936394358512__INCLUDED_

#include "Begin.h"	// Added by ClassView
#include "Winmethod.h"
#include "Again.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CBmView : public CView
{
protected: // create from serialization only
	CBmView();
	DECLARE_DYNCREATE(CBmView)

// Attributes
public:
	CBmDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBmView)
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
	CPoint tip;
	int timer_counter;
	int time_limit;
	bool attackmode;
	CAgain again_dial;
	int UpdateLeft(int x,int y,bool computer);//����computer_left,player_left
	//CDC mdc;
	int ChessOrderCounter;//��¼����
	BOOL game_begin;//��Ϸ��ʼ
	BOOL player_first;//�������
	int game_finish;//��Ϸ����
	int jiange;//���̼��
	//BOOL turn;
	BOOL dapu;//����
	CBegin begin_dial;//��Ϸ���öԻ���
	int **Chessboard;//����״��
	CWinmethod *winmethod;//����ʤ��
	CPoint *ChessOrder;//����˳��
	int **score;//ÿ��λ�÷���
	int win_step[5][2];//ʤ��ʱ��5������һ����
	int *player_left;//���ÿ��ʤ�����м���  6Ϊ��Զ����ʤ
	int *computer_left;//ͬ��
	virtual ~CBmView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CBmView)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnBegingame();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnRegrret();
	afx_msg void OnUpdateRegrret(CCmdUI* pCmdUI);
	afx_msg void OnDapu();
	afx_msg void OnUpdateDapu(CCmdUI* pCmdUI);
	afx_msg void OnTimemode();
	afx_msg void OnUpdateTimemode(CCmdUI* pCmdUI);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void Onzhizhao();
	afx_msg void OnUpdatezhizhao(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in bmView.cpp
inline CBmDoc* CBmView::GetDocument()
   { return (CBmDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BMVIEW_H__23EA2312_13B9_4B5E_9735_936394358512__INCLUDED_)
