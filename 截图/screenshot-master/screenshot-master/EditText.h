
#pragma once
#include "shape.h"

class CEditText :
	public CShape
{
public:
	CEditText(CRect,CString);
	~CEditText(void);
	void Drawing(CDC *pDC);
	CString m_csContent;
	CRect m_rect;
};
