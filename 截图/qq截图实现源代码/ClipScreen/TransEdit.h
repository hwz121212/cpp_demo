#pragma once
#include "afxwin.h"
#include <afxglobals.h>

class CTransEdit :
	public CEdit
{
public:
	CTransEdit(COLORREF clr);
	~CTransEdit(void);
	DECLARE_MESSAGE_MAP()
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	afx_msg UINT OnGetDlgCode();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnNcDestroy();
	afx_msg void OnEnChange();

private:
	COLORREF m_clr;
public:

};
