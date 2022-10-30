// homework11Doc.h : interface of the CHomework11Doc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_HOMEWORK11DOC_H__5EAEB0AD_DD28_4B5C_BCDC_E1C93FFCEC69__INCLUDED_)
#define AFX_HOMEWORK11DOC_H__5EAEB0AD_DD28_4B5C_BCDC_E1C93FFCEC69__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include<afxtempl.h>

#pragma once
class CHomework11Doc : public CDocument
{
protected: // create from serialization only
	CHomework11Doc();
	DECLARE_DYNCREATE(CHomework11Doc)

// Attributes
public:

// Operations
public:
CList<CPoint,CPoint>m_List;
CList<int,int>m_width;
CList<int,int>m_colour;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHomework11Doc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CHomework11Doc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CHomework11Doc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HOMEWORK11DOC_H__5EAEB0AD_DD28_4B5C_BCDC_E1C93FFCEC69__INCLUDED_)
