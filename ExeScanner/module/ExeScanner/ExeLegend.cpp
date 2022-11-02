/******************************************************************************
*
* PROJECT:		ExeScanner
* FILE NAME:	ExeLegend.cpp
*
* DEVELOPER:	Vishalsinh Jhala
* DATE:			2-Apr-2005
*
* DESCRIPTION:	Defination file for class ExeLegend.
*
* NOTES:		Represents property window. Its a semi-transparent, popup window
*				with non-standard GUI. Allows users to view the properties of
*				each object.
*
******************************************************************************/

#include "stdafx.h"
#include "ExeScanner.h"
#include "ExeLegend.h"
#include "ChildView.h"


// CExeLegend

IMPLEMENT_DYNAMIC(CExeLegend, CWnd)
CExeLegend::CExeLegend()
{
	m_brGrey.CreateSolidBrush(RGB(40,40,40));
	m_brDarkGrey.CreateSolidBrush(RGB(20,20,20));

	m_rgnRoundRect.CreateRoundRectRgn(0,0,120,20,20,20);
	m_rgnBig.CreateRoundRectRgn(0,0,300,150,20,20);

	m_penWhite.CreatePen(PS_SOLID,1,RGB(255,255,255));

	m_dcPointer.CreateCompatibleDC(CDC::FromHandle(::GetDC(NULL)));
	m_bmpPointer.LoadBitmap(MAKEINTRESOURCE(IDB_BITMAP_POINTER));
	m_dcPointer.SelectObject(&m_bmpPointer);

	m_bMaximize = FALSE;
}

CExeLegend::~CExeLegend()
{
}


BEGIN_MESSAGE_MAP(CExeLegend, CWnd)
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

/******************************************************************************
* Function Name: Transparent
*
* Note: Makes the window semi-transparent
*
******************************************************************************/
void CExeLegend::Transparent()
{
	HMODULE hModule = ::GetModuleHandle("User32");
	WNDPROC	 _SetLayeredWindowAttributes;
	if (hModule)
	{
		_SetLayeredWindowAttributes = (WNDPROC)GetProcAddress(hModule, "SetLayeredWindowAttributes");
		//::SetWindowLong(this->GetSafeHwnd(), GWL_EXSTYLE, ::GetWindowLong(this->GetSafeHwnd(), GWL_EXSTYLE) | /*WS_EX_LAYERED*/0x00080000);
		if(_SetLayeredWindowAttributes)
			_SetLayeredWindowAttributes(this->GetSafeHwnd(), 0, (255 * 70) / 100, 0x00000002);
	}
}


// CExeLegend message handlers


BOOL CExeLegend::CreateEx(DWORD dwExStyle, LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, LPVOID lpParam)
{

	CString className = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(NULL, IDC_ARROW), (HBRUSH) m_brGrey.GetSafeHandle(), NULL);

	int iRet = CWnd::CreateEx(dwExStyle, className, lpszWindowName, dwStyle, rect, pParentWnd, 0, lpParam);
	if(iRet)
	{
		Transparent();
		SetWindowRgn((HRGN)m_rgnRoundRect.GetSafeHandle(),TRUE);
		
		//create the list view
		m_lstProperty.Create(WS_CHILD|WS_VISIBLE|LVS_REPORT|LVS_NOCOLUMNHEADER,
			CRect(20,21,280,150), this, 1);

		//decorate
		ListView_SetExtendedListViewStyle(m_lstProperty.GetSafeHwnd(),LVS_EX_FULLROWSELECT);
		m_lstProperty.SetBkColor(RGB(40,40,40));
		m_lstProperty.SetTextBkColor(RGB(40,40,40));
		m_lstProperty.SetTextColor(RGB(255,255,255));

		//colonize
		m_lstProperty.InsertColumn(0,"Property",LVCFMT_LEFT,100);
		m_lstProperty.InsertColumn(1,"ESC -> Exit",LVCFMT_LEFT,160);

	}
	
	return iRet;
}

void CExeLegend::OnPaint()
{
	CPaintDC dc(this); 
	
	CRect rect;
	GetClientRect(rect);

	dc.SelectObject(&m_penWhite);
	dc.SelectObject(&m_brDarkGrey);
	dc.SetTextColor(RGB(174,213,217));
	dc.SetBkMode(TRANSPARENT);

	dc.RoundRect(0,0,rect.right,rect.bottom,20,20);
	
	dc.TextOut(10,0,"Legend");

	dc.MoveTo(rect.right-20,rect.top);
	dc.LineTo(rect.right-20,20);
	
	if(m_bMaximize)
	{
		dc.SelectObject(&m_brGrey);
		dc.Rectangle(0,20,rect.right,rect.bottom);
	}
	if(m_bMaximize)
	{
		dc.BitBlt(285,5,9,9,&m_dcPointer,9,0,SRCCOPY);
	}
	else
	{
		dc.BitBlt(105,7,9,9,&m_dcPointer,0,0,SRCCOPY);
	}

}

void CExeLegend::OnMouseMove(UINT nFlags, CPoint point)
{
	CRect rect;
	GetClientRect(rect);

	if(point.x>rect.right-20 && point.y < 20)
	{
		SetCursor(AfxGetApp()->LoadStandardCursor(MAKEINTRESOURCE(32649)));
	}

	CWnd::OnMouseMove(nFlags, point);

}

void CExeLegend::OnLButtonDown(UINT nFlags, CPoint point)
{
	CRect rect;
	GetClientRect(rect);

	if(point.x>rect.right-20 && point.y < 20)
	{
		Animate();
	}
	CWnd::OnLButtonDown(nFlags, point);
}
/******************************************************************************
* Function Name: Animate
*
* Note: Minimizes or maximizes the window with a effect
*
******************************************************************************/
void CExeLegend::Animate()
{
	CRect rect;
	GetClientRect(rect);
	ClientToScreen(rect);

	//Minimize if already maximized
	if(m_bMaximize)
	{
		MoveWindow(rect.right-120,rect.bottom-20,120,20);
		
		m_rgnRoundRect.DeleteObject();
		m_rgnRoundRect.CreateRoundRectRgn(0,0,120,20,20,20);
		SetWindowRgn((HRGN)m_rgnRoundRect.GetSafeHandle(),TRUE);
		
		m_bMaximize = FALSE;
		Invalidate(TRUE);
	}
	//Maximize if already minimized
	else
	{
		for(int i=1;i<=20;i++)
		{
			MoveWindow(rect.right-i*15,rect.bottom-i*7,i*15,i*7);
			Invalidate(FALSE);
			Sleep(1);
		}
		MoveWindow(rect.right-300,rect.bottom-150,300,150);
		
		m_rgnBig.DeleteObject();
		m_rgnBig.CreateRoundRectRgn(0,0,300,150,20,20);
		SetWindowRgn((HRGN)m_rgnBig.GetSafeHandle(),TRUE);
		
		m_bMaximize = TRUE;
		Invalidate(FALSE);
		Populate();

	}
}
/******************************************************************************
* Function Name: Populate
*
* Note: Populates the list control
*
******************************************************************************/
void CExeLegend::Populate()
{
	//Gets the property from selected object
	m_lstProperty.DeleteAllItems();

	int iItem = 0;
	int curpos=0;
	CString strTok;	

	strTok= strInfo.Tokenize("\n",curpos);
	while (strTok != "")
	{
		m_lstProperty.InsertItem(iItem,strTok);
		Type(iItem,0,strTok);

		strTok= strInfo.Tokenize("\n",curpos);
		if(strTok == "")
			break;
		
		m_lstProperty.SetItemText(iItem,1,strTok);
		Type(iItem,1,strTok);
		
		strTok= strInfo.Tokenize("\n",curpos);
		
		iItem++;
	}
	//delete szProp;
}
/******************************************************************************
* Function Name: Type
*
* Note: Generates the typing effect of characters when being added to list control
*
******************************************************************************/
void CExeLegend::Type(int i, int j,CString str)
{
	for(int i1=1;i1<=str.GetLength();i1+=3)
	{
		if(i1>=str.GetLength()-3)
			i1 = str.GetLength();
		
		if(j==0)
			m_lstProperty.SetTextColor(RGB(209,200,123));
		else
			m_lstProperty.SetTextColor(RGB(255,255,255));
		
		m_lstProperty.SetItemText(i,j,str.Mid(0,i1));
		
		m_lstProperty.UpdateWindow();
		Sleep(2);
	}

}

BOOL CExeLegend::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->message == WM_SYSCOMMAND)
	{
		CWnd::PreTranslateMessage(pMsg);
		return TRUE;
	}
	return CWnd::PreTranslateMessage(pMsg);
}
