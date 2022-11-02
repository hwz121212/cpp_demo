
#pragma once
#include "shape.h"

class CLine :
	public CShape
{
public:
	CLine(CPoint,CPoint);
	void Drawing(CDC *pDC);
	virtual ~CLine(void);
};
