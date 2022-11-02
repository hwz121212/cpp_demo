/******************************************************************************
*
* PROJECT:		ExeScanner
* FILE NAME:	ResourceSection.cpp
*
* DEVELOPER:	Vishalsinh Jhala
* DATE:			2-Apr-2005
*
* DESCRIPTION:	Defination file for class ResourceSection.
*
* NOTES:		Owns all Resource Branches.(BITMAPS,ICONS,CURSORS,STRINGS ETC)
*
******************************************************************************/

#include "StdAfx.h"
#include "ResourceSection.h"

ResourceSection::ResourceSection(PEFile *pFile, DWORD dwImageBase, DWORD dwRVA)
	: m_dwImageBase(dwImageBase), m_dwRVA(dwRVA) 
{
	//Check Input parameters and their validity
	if( !pFile )
	{
		char *szErrMsg = new char[1024];
		strcpy(szErrMsg,"Cannot Proceed with NULL File");
		throw new PEFileException(szErrMsg);
	}
	if( dwImageBase<=0 || dwRVA<=0 )
	{
		char *szErrMsg = new char[1024];
		strcpy(szErrMsg,"Invalid Resource Section Header");
		throw new PEResParseException(szErrMsg);
	}

	m_pFile = pFile;
	//Who am I?
	strcpy(m_pszMyName,"ResourceSection");
	
	//Load the root directory for ResourceSection.
	//This will let us know the resource types burried here in form of branches.
	m_pFile->Read(dwImageBase,0,(BYTE*)&m_resDirRoot,sizeof(IMAGE_RESOURCE_DIRECTORY));

	if(m_resDirRoot.NumberOfIdEntries+m_resDirRoot.NumberOfNamedEntries<=0 ||
		m_resDirRoot.NumberOfIdEntries+m_resDirRoot.NumberOfNamedEntries>=1000)
	{
		char *szErrMsg = new char[1024];
		strcpy(szErrMsg,"Invalid Resource Directory Table");
		throw new PEResParseException(szErrMsg);
	}

	//Create Branch Objects for each resource type
	for(int i = 0; i<m_resDirRoot.NumberOfIdEntries+m_resDirRoot.NumberOfNamedEntries;i++)
	{
		m_pResBranch[i] = new ResourceBranch(m_pFile,m_dwImageBase,m_dwRVA,sizeof(IMAGE_RESOURCE_DIRECTORY)+ i*sizeof(IMAGE_RESOURCE_DIRECTORY_ENTRY));
	}

}

ResourceSection::~ResourceSection(void)
{
	for(int i = 0; i<m_resDirRoot.NumberOfIdEntries+m_resDirRoot.NumberOfNamedEntries;i++)
		if (m_pResBranch[i])
			delete m_pResBranch[i];
}
void ResourceSection::GetPropertyInfo(CString& strInfo)
{

	strInfo.Format("CHARACTERISTICS\n%d\n"
		"TIMESTAMP\n%d\n"
		"VERSION\n%d:%d\n"
		"NAMED-ENTRIES\n%d\n"
		"ID-ENTRIES#\n%d\n",
		m_resDirRoot.Characteristics,
		m_resDirRoot.TimeDateStamp,
		m_resDirRoot.MajorVersion,m_resDirRoot.MinorVersion,
		m_resDirRoot.NumberOfNamedEntries,
		m_resDirRoot.NumberOfIdEntries
		);

}
void ResourceSection::IterateElements(PETreeList *pList)
{
	if( !pList)
	{
		char *szErrMsg = new char[1024];
		strcpy(szErrMsg,"Tree Control Object does not exist");
		throw new PEUIException(szErrMsg);
	}
	pList->AddNode(1,m_pszMyName,this);

	for(int i = 0; i<m_resDirRoot.NumberOfIdEntries+m_resDirRoot.NumberOfNamedEntries;i++)
		m_pResBranch[i]->IterateElements(pList);

}