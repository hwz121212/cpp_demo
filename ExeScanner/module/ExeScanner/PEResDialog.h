/******************************************************************************
*
* PROJECT:		ExeScanner
* FILE NAME:	PEResDialog.h
*
* DEVELOPER:	Vishalsinh Jhala
* DATE:			2-Apr-2005
*
* DESCRIPTION:	Declaration file for class PEResDialog.
*
* NOTES:		Encapsulates Dialog Resource. Such resources, when requested
*				by user to be displayed, are shown in a modalless window.
*
******************************************************************************/
#pragma once
#include "pebase.h"

class PEResDialog :
	public PEResource
{
	DWORD m_dwOffset;//Physical Offset
	DWORD m_dwSize;//Size in Bytes

	void GetDlgStyle(CString& strStyle,DWORD dwStyle);

public:
	PEResDialog(PEFile *pFile, DWORD dwOffset, DWORD dwSize);

	virtual void IterateElements(PETreeList *pList); 
	virtual void GetPropertyInfo(CString& strInfo);

	virtual const char* GetName();
	virtual void Draw(CDC **pDC,CBitmap **pBmp,CWnd **pWnd = NULL);
	virtual void Import(BOOL bFromFile);
	virtual void Export(BOOL bFromFile);

	~PEResDialog(void);
};
