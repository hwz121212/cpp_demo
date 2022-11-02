/******************************************************************************
*
* PROJECT:		ExeScanner
* FILE NAME:	ResourceBranch.h
*
* DEVELOPER:	Vishalsinh Jhala
* DATE:			2-Apr-2005
*
* DESCRIPTION:	Declaration file for class ResourceBranch.
*
* NOTES:		Represents a resource directory that owns multiple resources
*				of specific type(Called Nodes).
*
******************************************************************************/
#pragma once
#include "pebase.h"
#include "ResourceNode.h"

class ResourceBranch :
	public PEBase
{
	DWORD m_dwImageBase,m_dwRVA;
	IMAGE_RESOURCE_DIRECTORY_ENTRY m_resEntryBranch;
	IMAGE_RESOURCE_DIRECTORY	m_resDirBranch;
	IMAGE_RESOURCE_DIRECTORY_STRING m_resString;
	ResourceNode *m_pResNode[1000];
	void AssignNames(DWORD dwName);

public:
	ResourceBranch(PEFile *pFile, DWORD dwImageBase, DWORD dwRVA,DWORD dwBranchOffset);
	~ResourceBranch(void);
	virtual void GetPropertyInfo(CString& strInfo);
	virtual void IterateElements(PETreeList *pList);
};
