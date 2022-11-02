/******************************************************************************
*
* PROJECT:		ExeScanner
* FILE NAME:	PEResString.h
*
* DEVELOPER:	Vishalsinh Jhala
* DATE:			2-Apr-2005
*
* DESCRIPTION:	Declaration file for class PEResString.
*
* NOTES:		Encapsulates String Resource. Such resources, when requested
*				by user to be displayed, are shown in String editor.
*
******************************************************************************/
#pragma once
#include "pebase.h"

class PEResString :
	public PEResource
{
	DWORD m_dwOffset;//Physical Offset
	DWORD m_dwSize;//Size in Bytes

public:
	PEResString(PEFile *pFile, DWORD dwOffset, DWORD dwSize);

	virtual void IterateElements(PETreeList *pList); 
	virtual void GetPropertyInfo(CString& strInfo);

	virtual const char* GetName();
	virtual void Draw(CDC **pDC,CBitmap **pBmp,CWnd **pWnd = NULL);
	virtual void Import(BOOL bFromFile);
	virtual void Export(BOOL bFromFile);
	void Save(int iRelOffset, CString strData, int iOrgSize);

	~PEResString(void);
};
