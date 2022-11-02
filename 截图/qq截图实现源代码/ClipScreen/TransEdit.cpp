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

	// TODO:  �ڴ˸��� DC ���κ�����

	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
	//pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(m_clr);
	return (HBRUSH)GetStockObject(WHITE_BRUSH);
}

UINT CTransEdit::OnGetDlgCode()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	RedrawWindow(NULL, NULL,RDW_INVALIDATE|RDW_ERASE);
	return CEdit::OnGetDlgCode();
}

BOOL CTransEdit::OnEraseBkgnd(CDC* pDC)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	return TRUE;//CEdit::OnEraseBkgnd(pDC);
}

void CTransEdit::OnNcDestroy()
{
	CEdit::OnNcDestroy();
	delete this;
	// TODO: �ڴ˴������Ϣ����������
}

void CTransEdit::OnEnChange()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CEdit::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}
