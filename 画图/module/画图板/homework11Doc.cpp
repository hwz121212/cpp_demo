// homework11Doc.cpp : implementation of the CHomework11Doc class
//

#include "stdafx.h"
#include "homework11.h"

#include "homework11Doc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHomework11Doc

IMPLEMENT_DYNCREATE(CHomework11Doc, CDocument)

BEGIN_MESSAGE_MAP(CHomework11Doc, CDocument)
	//{{AFX_MSG_MAP(CHomework11Doc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHomework11Doc construction/destruction

CHomework11Doc::CHomework11Doc()
{
	// TODO: add one-time construction code here

}

CHomework11Doc::~CHomework11Doc()
{
}

BOOL CHomework11Doc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	m_List.RemoveAll();
	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CHomework11Doc serialization

void CHomework11Doc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		POSITION ps=m_List.GetHeadPosition();
		POSITION pc=m_colour.GetHeadPosition();
		POSITION pw=m_width.GetHeadPosition();
		
		CPoint pt;
		int a,b;
		while(ps)
		{	
		pt=m_List.GetNext(ps);
		a=m_colour.GetNext(pc);
		b=m_width.GetNext(pw);
		ar<<pt;
		ar<<a;
		ar<<b;	
		}
		pt=CPoint(-1,-1);
		ar<<pt;
	}
	else
	{
	POSITION ps =NULL;
	CPoint pt;int a,b;
	ar>>pt;
	while(pt!=CPoint(-1,-1))
	{
		m_List.AddTail(pt);
		ar>>a;
		m_colour.AddTail(a);
		ar>>b;
		m_width.AddTail(b);	
	
	}
	}
}

/////////////////////////////////////////////////////////////////////////////
// CHomework11Doc diagnostics

#ifdef _DEBUG
void CHomework11Doc::AssertValid() const
{
	CDocument::AssertValid();
}

void CHomework11Doc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CHomework11Doc commands
