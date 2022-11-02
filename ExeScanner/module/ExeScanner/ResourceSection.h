/******************************************************************************
*
* PROJECT:		ExeScanner
* FILE NAME:	ResourceSection.h
*
* DEVELOPER:	Vishalsinh Jhala
* DATE:			2-Apr-2005
*
* DESCRIPTION:	Declaration file for class ResourceSection.
*
* NOTES:		Owns all Resource Branches.(BITMAPS,ICONS,CURSORS,STRINGS ETC)
*
******************************************************************************/
#pragma once
#include "pebase.h"
#include "ResourceBranch.h"

class ResourceSection :
	public PEBase
{
	DWORD m_dwImageBase,m_dwRVA;
	IMAGE_RESOURCE_DIRECTORY m_resDirRoot;
	ResourceBranch *m_pResBranch[1000];

public:
	ResourceSection(PEFile *pFile, DWORD dwImageBase, DWORD dwRVA);
	~ResourceSection(void);
	virtual void GetPropertyInfo(CString& strInfo);
	virtual void IterateElements(PETreeList *pList);
};
