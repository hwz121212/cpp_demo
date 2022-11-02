// InputEdit.cpp : ʵ���ļ�
//

#include "stdafx.h"
/*#include "CatchScreen.h"*/
#include "InputEdit.h"


// CInputEdit

IMPLEMENT_DYNAMIC(CInputEdit, CEdit)

CInputEdit::CInputEdit()
{
	m_bDrag = FALSE;
	m_rect = 0;
	m_pFont = new CFont();
	m_pFont->CreateFont(
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
		_T("����") ) ;                 // lpszFacename
}

CInputEdit::~CInputEdit()
{
	m_pFont->DeleteObject();
	delete m_pFont;
}


BEGIN_MESSAGE_MAP(CInputEdit, CEdit)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_CONTROL_REFLECT(EN_CHANGE, &CInputEdit::OnEnChange)
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR_REFLECT()
END_MESSAGE_MAP()



// CInputEdit ��Ϣ�������



void CInputEdit::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	
	m_bDrag = TRUE;

	CRect rc;
	GetWindowRect(&rc);
	GetParent()->ScreenToClient(&rc);

	CPoint point1 = point;
	ClientToScreen(&point1);
	GetParent()->ScreenToClient(&point1);

	int nWidth = rc.Width();
	int nHeight = rc.Height();

	m_ndist_left=point1.x-rc.left;
	m_ndist_top=point1.y-rc.top;
	m_ndist_right=rc.right-point1.x;
	m_ndist_bottom=rc.bottom-point1.y;
	CEdit::OnLButtonDown(nFlags, point);
}

void CInputEdit::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	m_bDrag = FALSE;
	CEdit::OnLButtonUp(nFlags, point);
}

void CInputEdit::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	if (m_bDrag)
	{
		CRect rc;
		GetWindowRect(&rc);
		GetParent()->ScreenToClient(&rc);

		CPoint point1 = point;
		ClientToScreen(&point1);
		GetParent()->ScreenToClient(&point1);

		int nWidth = rc.Width();
		int nHeight = rc.Height();
		
		//�����ƶ���Χ
		point1.x = point1.x>=m_rect.right?m_rect.right:point1.x;
		point1.x = point1.x<=m_rect.left?m_rect.left:point1.x;
		point1.y = point1.y>=m_rect.bottom?m_rect.bottom:point1.y;
		point1.y = point1.y<=m_rect.top?m_rect.top:point1.y;


		rc.left = point1.x-m_ndist_left;
		rc.top = point1.y-m_ndist_top;
		rc.right = point1.x+m_ndist_right;
		rc.bottom = point1.y+m_ndist_bottom;

		if (rc.left < m_rect.left)
		{
			rc.left = m_rect.left + 1;
			rc.right = nWidth + rc.left;
		}
		if (rc.right > m_rect.right)
		{
			rc.right = m_rect.right -1;
			rc.left = rc.right - nWidth;
		}
		if (rc.top < m_rect.top)
		{
			rc.top = m_rect.top + 1;
			rc.bottom = rc.top + nHeight;
		}
		if (rc.bottom > m_rect.bottom)
		{
			rc.bottom = m_rect.bottom - 1;
			rc.top = rc.bottom - nHeight;
		}


		MoveWindow(&rc);
		GetParent()->InvalidateRect(&m_rect);

		SetCursor(::LoadCursor(NULL,MAKEINTRESOURCE(IDC_SIZEALL) ) );

	}
	else
	{
		SetCursor(::LoadCursor(NULL,MAKEINTRESOURCE(IDC_ARROW) ) );
	}
	CEdit::OnMouseMove(nFlags, point);
}

void CInputEdit::SetMoveRect(CRect rc)
{
	m_rect = rc;
}
void CInputEdit::OnEnChange()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ�������������
	// ���͸�֪ͨ��������д CEdit::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	int nIndex = GetLineCount();
	TCHAR szContent[100] = {'\0'};

	int   n   =  GetLine(nIndex -1,   szContent,100);   
	if   (n> 0)   
	{   
		szContent[n]   =   0;   
	} 

	CDC *pDC = GetDC();
	CFont *pFont = GetFont();
	CFont *pOldFont = pDC->SelectObject(pFont);

	//��CSize����ʽ�����ַ����ĸ߶ȺͿ��
	CSize size = pDC->GetTextExtent(szContent);
	CString str = _T("��");
	CSize szExtra = pDC->GetTextExtent(str);

	// �ͷ�
	pDC->SelectObject(pOldFont);
	ReleaseDC(pDC);

	CRect rect;
	GetWindowRect(&rect);
	GetParent()->ScreenToClient(&rect);

	if (rect.bottom + szExtra.cy<m_rect.bottom)
	{
		rect.bottom=rect.top+(nIndex+1)*szExtra.cy;
	}

	if (size.cx + 2*szExtra.cx >rect.Width() )
	{
		rect.right = rect.left + rect.Width() + 2*szExtra.cx;
	}

	//����EDIT���β�������ͼ���εķ�Χ
	if (rect.right >m_rect.right)
	{
		rect.right = m_rect.right - 3;
	}
	if (rect.bottom >m_rect.bottom)
	{
		rect.bottom = m_rect.bottom;
	}

	MoveWindow(&rect);
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}




BOOL CInputEdit::OnEraseBkgnd(CDC* pDC)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	CRect cliRect,rc;
	GetClientRect(&rc);
	cliRect = rc;
	cliRect.top -=1;
	cliRect.left -=1 ;
	cliRect.right +=1;
	cliRect.bottom +=1;
	//pDC->FillSolidRect(cliRect, /*m_clrBKLine*/RGB(255,0,0) );
	//cliRect.top +=1;
	//cliRect.left +=1;
	//cliRect.right -=1;
	//cliRect.bottom -=1;
	//pDC->FillSolidRect(cliRect, m_clrBKIn);

	CPen pen(PS_DASH,1,RGB(0,0,0) );
	CPen *pOldPen = pDC->SelectObject(&pen);
	pDC->Rectangle(&cliRect);
	pDC->SelectObject(pOldPen);
	pen.DeleteObject();
	return true;

	return CEdit::OnEraseBkgnd(pDC);
}

BOOL CInputEdit::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���

	//��EDIT�е������Ѿ���ʾ����ͼ����ĵײ�ʱ���ٰ��س���ֹ����
	if(WM_KEYDOWN   ==   pMsg-> message) 
	{ 
		if( (13 == pMsg->wParam) ) //enter  
		{
			CDC *pDC = GetDC();
			CFont *pFont = GetFont();
			CFont *pOldFont = pDC->SelectObject(pFont);

			//��CSize����ʽ�����ַ����ĸ߶ȺͿ��
			CString str = _T("��");
			CSize szExtra = pDC->GetTextExtent(str);

			// �ͷ�
			pDC->SelectObject(pOldFont);

			CRect rect;
			GetWindowRect(&rect);
			GetParent()->ScreenToClient(&rect);

			if (rect.bottom + szExtra.cy>m_rect.bottom)
			{
				return   TRUE;  //һ��Ҫ����true
			}
		}
	} 
	return CEdit::PreTranslateMessage(pMsg);
}

HBRUSH CInputEdit::CtlColor(CDC* pDC, UINT nCtlColor)
{
	// TODO:  �ڴ˸��� DC ���κ�����
	if(nCtlColor != CTLCOLOR_EDIT)
	{
		return NULL;
	}
	pDC->SetTextColor(RGB(255,0,0) );
//	CFont *pOldFont = pDC->SelectObject(m_pFont);

	return ::GetSysColorBrush(COLOR_WINDOW);
}
