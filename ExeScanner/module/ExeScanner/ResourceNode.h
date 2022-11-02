/******************************************************************************
*
* PROJECT:		ExeScanner
* FILE NAME:	ResourceNode.h
*
* DEVELOPER:	Vishalsinh Jhala
* DATE:			2-Apr-2005
*
* DESCRIPTION:	Declaration file for class ResourceNode.
*
* NOTES:		ResourceNode Represent Individual resource. Depending upon the
*				type of Branch, a Derived object of PEResource is created and
*				stored.
*
******************************************************************************/
#pragma once
#include "pebase.h"

class ResourceNode :
	public PEBase
{
	DWORD m_dwImageBase,m_dwRVA;

	IMAGE_RESOURCE_DIRECTORY_ENTRY	m_resEntryNode;
	//IMAGE_RESOURCE_DIRECTORY		m_resDirNode;
	IMAGE_RESOURCE_DIRECTORY_ENTRY	m_resEntryLeaf;
	IMAGE_RESOURCE_DIRECTORY		m_resDirLeaf;
	IMAGE_RESOURCE_DATA_ENTRY		m_resData;
	PEResource *m_pRes;

	
	void CreateObjects(PEFile *pFile,DWORD dwName);

public:
	ResourceNode(PEFile *pFile, DWORD dwImageBase, DWORD dwRVA,DWORD dwNodeOffset,int iResType);
	~ResourceNode(void);
	virtual void GetPropertyInfo(CString& strInfo);
	virtual void IterateElements(PETreeList *pList);
};
