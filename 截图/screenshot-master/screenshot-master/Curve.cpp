#include "stdafx.h"
#include "Curve.h"

CCurve::CCurve(CList<CPoint,CPoint>*pointList)
{
	ASSERT(pointList);
	POSITION pos = pointList->GetHeadPosition();
	while (NULL != pos)
	{
		m_pointList.AddTail(pointList->GetNext(pos) );

	}
}

CCurve::~CCurve(void)
{
}
void CCurve::Drawing(CDC *pDC)
{
	ASSERT(pDC);
	POSITION pos = m_pointList.GetHeadPosition();
	CPoint pt = m_pointList.GetNext(pos);
	pDC->MoveTo(pt);      //移动到起点
	for (int i = 0;i<m_pointList.GetCount() -1;i++)
	{
		pt = m_pointList.GetNext(pos);
		pDC->LineTo(pt);
	}

}
