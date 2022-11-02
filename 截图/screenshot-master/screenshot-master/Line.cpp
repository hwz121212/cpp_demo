#include "stdafx.h"
#include "Line.h"
#include "math.h"
CLine::CLine(CPoint ptStart,CPoint ptEnd)
{
	m_startPt = ptStart;
	m_endPt = ptEnd;
}

CLine::~CLine(void)
{
}
void CLine::Drawing(CDC *pDC)
{
	ASSERT(pDC);
	/*pDC->MoveTo(m_startPt);
	pDC->LineTo(m_endPt);*/

	double slopy , cosy , siny;
	double Par = 10.0;  //length of Arrow (>)
	slopy = atan2( double( m_startPt.y - m_endPt.y ),double( m_startPt.x - m_endPt.x ) );
	cosy = cos( slopy );
	siny = sin( slopy ); //need math.h for these functions

	//draw a line between the 2 endpoint
	pDC->MoveTo( m_startPt );
	pDC->LineTo( m_endPt );

	//here is the tough part - actually drawing the arrows
	//a total of 6 lines drawn to make the arrow shape
	/*pdc->MoveTo( m_One);
	pdc->LineTo( m_One.x + int( - Par * cosy - ( Par / 2.0 * siny ) ),
	m_One.y + int( - Par * siny + ( Par / 2.0 * cosy ) ) );
	pdc->LineTo( m_One.x + int( - Par * cosy + ( Par / 2.0 * siny ) ),
	m_One.y - int( Par / 2.0 * cosy + Par * siny ) );
	pdc->LineTo( m_One );*/

	/*/-------------similarly the the other end-------------/*/
	//pDC->MoveTo( m_endPt );
	//pDC->LineTo( m_endPt.x + int( Par * cosy - ( Par / 2.0 * siny ) ),
	//	m_endPt.y + int( Par * siny + ( Par / 2.0 * cosy ) ) );

	//pDC->MoveTo( m_endPt );
	//pDC->LineTo( m_endPt.x + int( Par * cosy + Par / 2.0 * siny ),
	//	m_endPt.y - int( Par / 2.0 * cosy - Par * siny ) );
	CPoint pt1( m_endPt.x + int( Par * cosy - ( Par / 2.0 * siny ) ),m_endPt.y + int( Par * siny + ( Par / 2.0 * cosy ) ));
	CPoint pt2(m_endPt.x + int( Par * cosy + Par / 2.0 * siny ),m_endPt.y - int( Par / 2.0 * cosy - Par * siny ));

	CPoint ptArray[3] = {m_endPt,pt1,pt2};
	pDC->Polygon(ptArray,3);
	HRGN hRgn = CreatePolygonRgn(ptArray,3,ALTERNATE);
	CBrush brush(RGB(255,0,0));//
	CBrush *pBrush = pDC->SelectObject(&brush);
	pDC->FillRgn(CRgn::FromHandle(hRgn),&brush);
	pDC->SelectObject(pBrush);
	//pdc->LineTo( m_Two );
}
//void CLine::DrawArrow(CDC *pdc,CPoint ptStart, CPoint ptEnd)
//{
//	double slopy , cosy , siny;
//	double Par = 10.0;  //length of Arrow (>)
//	slopy = atan2( double( ptStart.y - ptEnd.y ),double( ptStart.x - ptEnd.x ) );
//	cosy = cos( slopy );
//	siny = sin( slopy ); //need math.h for these functions
//
//	//draw a line between the 2 endpoint
//	pdc->MoveTo( ptStart );
//	pdc->LineTo( ptEnd );
//
//	//here is the tough part - actually drawing the arrows
//	//a total of 6 lines drawn to make the arrow shape
//	/*pdc->MoveTo( m_One);
//	pdc->LineTo( m_One.x + int( - Par * cosy - ( Par / 2.0 * siny ) ),
//	m_One.y + int( - Par * siny + ( Par / 2.0 * cosy ) ) );
//	pdc->LineTo( m_One.x + int( - Par * cosy + ( Par / 2.0 * siny ) ),
//	m_One.y - int( Par / 2.0 * cosy + Par * siny ) );
//	pdc->LineTo( m_One );*/
//
//	/*/-------------similarly the the other end-------------/*/
//	pdc->MoveTo( ptEnd );
//	pdc->LineTo( ptEnd.x + int( Par * cosy - ( Par / 2.0 * siny ) ),
//		ptEnd.y + int( Par * siny + ( Par / 2.0 * cosy ) ) );
//
//	pdc->MoveTo( ptEnd );
//	pdc->LineTo( ptEnd.x + int( Par * cosy + Par / 2.0 * siny ),
//		ptEnd.y - int( Par / 2.0 * cosy - Par * siny ) );
//	//pdc->LineTo( m_Two );
//
//}
//
