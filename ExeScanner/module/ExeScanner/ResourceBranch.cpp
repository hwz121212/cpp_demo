/******************************************************************************
*
* PROJECT:		ExeScanner
* FILE NAME:	ResourceBranch.cpp
*
* DEVELOPER:	Vishalsinh Jhala
* DATE:			2-Apr-2005
*
* DESCRIPTION:	Defination file for class ResourceBranch.
*
* NOTES:		Represents a resource directory that owns multiple resources
*				of specific type(Called Nodes).
*
******************************************************************************/

#include "StdAfx.h"
#include "ResourceBranch.h"

#ifdef _DEBUG
	extern CFile g_File;
	extern CString g_strDbg;
#endif

ResourceBranch::ResourceBranch(PEFile *pFile ,DWORD dwImageBase, DWORD dwRVA,DWORD dwBranchOffset)
	: m_dwImageBase(dwImageBase), m_dwRVA(dwRVA)
{
	//Validate input parameters
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
	
	//The directoryEntry would lead us to a Directory which owns the node.
	m_pFile->Read(m_dwImageBase,
		dwBranchOffset,
		(BYTE*)&m_resEntryBranch,
		sizeof(IMAGE_RESOURCE_DIRECTORY_ENTRY));
		
	if(! (m_resEntryBranch.OffsetToData & 0X80000000) )
		throw new PEResParseException(NULL);
	
	//If High Order bit of Name is set then its branchname is stored with in 
	//exe otherwise name can be decides on predefined numbers
	AssignNames(m_resEntryBranch.Name);

	if(m_resEntryBranch.Name & 0x80000000)
	{
		//Load the Name of branch
		WORD len;
		m_pFile->Read(m_dwImageBase,
			m_resEntryBranch.Name & 0x7FFFFFFF,
			(BYTE*)&len,
			sizeof(WORD));
	
		if(len>=255)
			len = 254;

		BYTE *b = new BYTE[len*2+2];
		m_pFile->Read(m_dwImageBase,
			(m_resEntryBranch.Name & 0x7FFFFFFF) +2,
			b,
			len*2);
		b[len*2] = b[len*2+1] = 0;
		CString str;
		str.Format("%S",b);
		strcpy(m_pszMyName,str.GetBuffer(1024));
		//m_pszMyName[len] = '\0';

		delete [] b;
	}
	#ifdef _DEBUG

	g_strDbg.Format("---------------------------------------------------------------------");
	g_File.Write(g_strDbg.GetBuffer(1024),g_strDbg.GetLength());
	
	g_strDbg.Format("ResDir (%s) (Named:%02d, ID:%02d) \r\n"
		, m_pszMyName,m_resEntryBranch.Name, m_resEntryBranch.Id);
	g_File.Write(g_strDbg.GetBuffer(1024),g_strDbg.GetLength());

	#endif
	
	//The directory to be loaded will lead us to node
	m_pFile->Read(m_dwImageBase,
		m_resEntryBranch.OffsetToDirectory,
		(BYTE*)&m_resDirBranch,
		sizeof(IMAGE_RESOURCE_DIRECTORY));

	if( m_resDirBranch.NumberOfIdEntries <= 0 && m_resDirBranch.NumberOfNamedEntries <= 0 )
	{
		char *szErrMsg = new char[1024];
		strcpy(szErrMsg,"Invalid Resource Directory");
		throw new PEResParseException(szErrMsg);
	}

	#ifdef _DEBUG

	g_strDbg.Format("      ResDir (NEXT) Entries:%02d (Named:%02d, ID:%02d) \r\n"
		, m_resDirBranch.NumberOfIdEntries+m_resDirBranch.NumberOfNamedEntries, m_resDirBranch.NumberOfIdEntries,m_resDirBranch.NumberOfNamedEntries);
	g_File.Write(g_strDbg.GetBuffer(1024),g_strDbg.GetLength());

	#endif


	//m_pResNode = new ResourceNode[m_resDirBranch.NumberOfIdEntries+m_resDirBranch.NumberOfNamedEntries];
	//Create Nodes for this branch.
	for(int i = 0; i<m_resDirBranch.NumberOfIdEntries+m_resDirBranch.NumberOfNamedEntries;i++)
	{
		m_pResNode[i]= new ResourceNode(m_pFile,m_dwImageBase,m_dwRVA,m_resEntryBranch.OffsetToDirectory+sizeof(IMAGE_RESOURCE_DIRECTORY)+i*sizeof(IMAGE_RESOURCE_DIRECTORY_ENTRY),m_resEntryBranch.Name);
	}
}

/******************************************************************************
* Function Name: AssignNames
*
* Note: Depending on the Name value branch name is decided.
*
******************************************************************************/
void ResourceBranch::AssignNames(DWORD dwName)
{
	switch(dwName)
	{
	case 0:
		strcpy(m_pszMyName,"???_0");
		break;
	case 1:
		strcpy(m_pszMyName,"CURSOR");
		break;
	case 2:
		strcpy(m_pszMyName,"BITMAP");
		break;
	case 3:
		strcpy(m_pszMyName,"ICON");
		break;
	case 4:
		strcpy(m_pszMyName,"MENU");
		break;
	case 5:
		strcpy(m_pszMyName,"DIALOG");
		break;
	case 6:
		strcpy(m_pszMyName,"STRING");
		break;
	case 7:
		strcpy(m_pszMyName,"FONTDIR");
		break;
	case 8:
		strcpy(m_pszMyName,"FONT");
		break;
	case 9:
		strcpy(m_pszMyName,"ACCELERATORS");
		break;
	case 10:
		strcpy(m_pszMyName,"RCDATA");
		break;
	case 11:
		strcpy(m_pszMyName,"MESSAGETABLE");
		break;
	case 12:
		strcpy(m_pszMyName,"GROUP_CURSOR");
		break;
	case 13:
		strcpy(m_pszMyName,"???_13");
		break;
	case 14:
		strcpy(m_pszMyName,"GROUP_ICON");
		break;
	case 15:
		strcpy(m_pszMyName,"???_15");
		break;
	case 16:
		strcpy(m_pszMyName,"VERSION");
		break;
	case 17:
		strcpy(m_pszMyName,"DLGINCLUDE");
		break;
	case 18:
		strcpy(m_pszMyName,"???_18");
		break;
	case 19:
		strcpy(m_pszMyName,"PLUGPLAY");
		break;
	case 20:
		strcpy(m_pszMyName,"VXD");
		break;
	case 21:
		strcpy(m_pszMyName,"ANICURSOR");
		break;
	case 22:
		strcpy(m_pszMyName,"ANIICON");
		break;
	default:
		itoa(dwName,m_pszMyName,10);
		break;
	}
}
ResourceBranch::~ResourceBranch(void)
{
	for(int i = 0; i<m_resDirBranch.NumberOfIdEntries+m_resDirBranch.NumberOfNamedEntries;i++)
		if (m_pResNode[i])
			delete m_pResNode[i];

}
void ResourceBranch::GetPropertyInfo(CString& strInfo)
{
	strInfo.Format("NAME\n%s\n"
		"OFFSET TO DATA\n%d\n"
		"CHARACTERISTICS\n%d\n"
		"TIMESTAMP\n%d\n"
		"VERSION\n%d:%d\n"
		"NAMED-ENTRIES\n%d\n"
		"ID-ENTRIES#\n%d\n",
		m_pszMyName,
		(m_resEntryBranch.OffsetToData&0X7FFFFFFF),
		m_resDirBranch.Characteristics,
		m_resDirBranch.TimeDateStamp,
		m_resDirBranch.MajorVersion,m_resDirBranch.MinorVersion,
		m_resDirBranch.NumberOfNamedEntries,
		m_resDirBranch.NumberOfIdEntries
		);


}
void ResourceBranch::IterateElements(PETreeList *pList)
{
	if( !pList)
	{
		char *szErrMsg = new char[1024];
		strcpy(szErrMsg,"Tree Control Object does not exist");
		throw new PEUIException(szErrMsg);
	}
	pList->AddNode(2,m_pszMyName,this);

	for(int i = 0; i<m_resDirBranch.NumberOfIdEntries+m_resDirBranch.NumberOfNamedEntries;i++)
		m_pResNode[i]->IterateElements(pList);

}