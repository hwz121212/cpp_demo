#include "stdafx.h"
#include "Ellipse.h"

CEllipse::CEllipse(CPoint ptStart,CPoint ptEnd)
{
	m_startPt = ptStart;
	m_endPt = ptEnd;
}

CEllipse::~CEllipse(void)
{
}
void CEllipse::Drawing(CDC *pDC)
{
	CBrush *pOldBrush = (CBrush*)pDC->SelectStockObject(NULL_BRUSH);
	pDC->Ellipse(CRect(m_startPt,m_endPt) );
	pDC->SelectObject(pOldBrush);
	return ;
}
