/******************************************************************************
*
* PROJECT:		ExeScanner
* FILE NAME:	PEResMenu.h
*
* DEVELOPER:	Vishalsinh Jhala
* DATE:			2-Apr-2005
*
* DESCRIPTION:	Declaration file for class PEResMenu.
*
* NOTES:		Encapsulates Menu Resource. Such resources, when requested
*				by user to be displayed, are shown in a modalless window.
*
******************************************************************************/
#pragma once
#include "pebase.h"

class PEResMenu :
	public PEResource
{
	DWORD m_dwOffset;//Physical Offset
	DWORD m_dwSize;//Size in Bytes

public:
	PEResMenu(PEFile *pFile, DWORD dwOffset, DWORD dwSize);

	virtual void IterateElements(PETreeList *pList); 
	virtual void GetPropertyInfo(CString& strInfo);

	virtual const char* GetName();
	virtual void Draw(CDC **pDC,CBitmap **pBmp,CWnd **pWnd = NULL);
	virtual void Import(BOOL bFromFile);
	virtual void Export(BOOL bFromFile);

	~PEResMenu(void);
};
