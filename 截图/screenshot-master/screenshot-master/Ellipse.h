
#pragma once
#include "shape.h"

class CEllipse :
	public CShape
{
public:
	CEllipse(CPoint,CPoint);
	void Drawing(CDC *pDC);
	virtual ~CEllipse(void);
};
