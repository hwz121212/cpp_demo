
#pragma once

// CShape ÃüÁîÄ¿±ê
class CShape : public CObject
{
public:
	CShape();
	virtual ~CShape();
	virtual void Drawing(CDC *pDC) = 0;

	CPoint m_startPt;
	CPoint m_endPt;
	COLORREF m_col;
};
