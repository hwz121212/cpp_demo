/******************************************************************************
*
* PROJECT:		ExeScanner
* FILE NAME:	PEResBinary.h
*
* DEVELOPER:	Vishalsinh Jhala
* DATE:			2-Apr-2005
*
* DESCRIPTION:	Declaration file for class PEResBinary.
*
* NOTES:		Encapsulates Binary Resource. Such resources, when requested
*				by user to be displayed, are shown in a hex editor. Individual
*				bytes can be manipulated upon users request.
*
******************************************************************************/
#pragma once
#include "pebase.h"

class PEResBinary :
	public PEResource
{
	DWORD m_dwOffset;//Physical Offset
	DWORD m_dwSize;//Size in Bytes
	
	void Beautify(CWnd**pWnd);

public:
	PEResBinary(PEFile *pFile, DWORD dwOffset, DWORD dwSize);

	virtual void IterateElements(PETreeList *pList); 
	virtual void GetPropertyInfo(CString& strInfo);

	virtual const char* GetName();
	virtual void Draw(CDC **pDC,CBitmap **pBmp,CWnd **pWnd = NULL);
	virtual void Import(BOOL bFromFile);
	virtual void Export(BOOL bFromFile);
	void Save(BYTE bChar, int iPos);

	~PEResBinary(void);
};
