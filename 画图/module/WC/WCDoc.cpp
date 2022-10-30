// WCDoc.cpp : implementation of the CWCDoc class
//

#include "stdafx.h"
#include "WC.h"

#include "WCDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWCDoc

IMPLEMENT_DYNCREATE(CWCDoc, CDocument)

BEGIN_MESSAGE_MAP(CWCDoc, CDocument)
	//{{AFX_MSG_MAP(CWCDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWCDoc construction/destruction

CWCDoc::CWCDoc()
{
	// TODO: add one-time construction code here

}

CWCDoc::~CWCDoc()
{
}

BOOL CWCDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CWCDoc serialization

void CWCDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CWCDoc diagnostics

#ifdef _DEBUG
void CWCDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CWCDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CWCDoc commands
