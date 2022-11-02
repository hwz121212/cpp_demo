#include "stdafx.h"
#include "EditText.h"

CEditText::CEditText(CRect rc,CString str)
{
	m_rect = rc;
	m_csContent = str;
}

CEditText::~CEditText(void)
{
}
void CEditText::Drawing(CDC *pDC)
{
	ASSERT(pDC);
	pDC->SetBkMode(TRANSPARENT);
	
	CFont font;
	VERIFY(font.CreateFont(
		-12,                        // nHeight
		0,                         // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		FW_NORMAL,                 // nWeight
		FALSE,                     // bItalic
		FALSE,                     // bUnderline
		0,                         // cStrikeOut
		GB2312_CHARSET,              // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		DEFAULT_QUALITY,           // nQuality
		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		_T("����") ));                 // lpszFacename

	CFont *pOldFont = pDC->SelectObject(&font);
	pDC->SetTextColor(RGB(255,0,0) );
	pDC->DrawText(m_csContent,&m_rect,DT_LEFT|DT_NOPREFIX|DT_WORDBREAK);
	pDC->SelectObject(pOldFont);

	font.DeleteObject();

}
