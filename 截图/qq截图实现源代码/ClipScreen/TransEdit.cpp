#include "StdAfx.h"
#include "TransEdit.h"

CTransEdit::CTransEdit(COLORREF clr)
{
	m_clr = clr;
}

CTransEdit::~CTransEdit(void)
{
}
BEGIN_MESSAGE_MAP(CTransEdit, CEdit)
	ON_WM_CTLCOLOR_REFLECT()
	ON_WM_GETDLGCODE()
	ON_WM_ERASEBKGND()
	ON_WM_NCDESTROY()
	ON_CONTROL_REFLECT(EN_CHANGE, &CTransEdit::OnEnChange)
END_MESSAGE_MAP()

HBRUSH CTransEdit::CtlColor(CDC* pDC, UINT nCtlColor)
{
	//HBRUSH hbr = CEdit::OnCtlColor(pDC,nCtlColor);

	// TODO:  在此更改 DC 的任何属性

	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	//pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(m_clr);
	return (HBRUSH)GetStockObject(WHITE_BRUSH);
}

UINT CTransEdit::OnGetDlgCode()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	RedrawWindow(NULL, NULL,RDW_INVALIDATE|RDW_ERASE);
	return CEdit::OnGetDlgCode();
}

BOOL CTransEdit::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	return TRUE;//CEdit::OnEraseBkgnd(pDC);
}

void CTransEdit::OnNcDestroy()
{
	CEdit::OnNcDestroy();
	delete this;
	// TODO: 在此处添加消息处理程序代码
}

void CTransEdit::OnEnChange()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CEdit::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}
