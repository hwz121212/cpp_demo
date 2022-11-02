
#pragma once
#include "shape.h"

class CRectangle :
	public CShape
{
public:
	CRectangle(CPoint,CPoint);
	void Drawing(CDC *pDC);
	virtual ~CRectangle(void);
};
