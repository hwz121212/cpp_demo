/******************************************************************************
*
* PROJECT:		ExeScanner
* FILE NAME:	PEResBitmap.h
*
* DEVELOPER:	Vishalsinh Jhala
* DATE:			2-Apr-2005
*
* DESCRIPTION:	Declaration file for class PEResBitmap.
*
* NOTES:		Encapsulates Bitmap Resource. Such resources, when requested
*				by user to be displayed, are shown in View.
*
******************************************************************************/
#pragma once
#include "pebase.h"

class PEResBitmap :
	public PEResource
{
	DWORD m_dwOffset;//Physical Offset
	DWORD m_dwSize;//Size in Bytes

public:
	PEResBitmap(PEFile *pFile, DWORD dwOffset, DWORD dwSize);

	virtual void IterateElements(PETreeList *pList); 
	virtual void GetPropertyInfo(CString& strInfo);

	virtual const char* GetName();
	virtual void Draw(CDC **pDC,CBitmap **pBmp,CWnd **pWnd = NULL);
	virtual void Import(BOOL bFromFile);
	virtual void Export(BOOL bFromFile);

	~PEResBitmap(void);
};
