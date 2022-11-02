/******************************************************************************
*
* PROJECT:		ExeScanner
* FILE NAME:	ResourceNode.cpp
*
* DEVELOPER:	Vishalsinh Jhala
* DATE:			2-Apr-2005
*
* DESCRIPTION:	Defination file for class ResourceNode.
*
* NOTES:		ResourceNode Represent Individual resource. Depending upon the
*				type of Branch, a Derived object of PEResource is created and
*				stored.
*
******************************************************************************/
#include "StdAfx.h"
#include "ResourceNode.h"
#include "PEResCursor.h"
#include "PEResBitmap.h"
#include "PEResIcon.h"
#include "PEResDialog.h"
#include "PEResString.h"
#include "PEResBinary.h"
#include "PEResMenu.h"

#ifdef _DEBUG
	extern CFile g_File;
	extern CString g_strDbg;
#endif

ResourceNode::ResourceNode(PEFile *pFile, DWORD dwImageBase, DWORD dwRVA,DWORD dwNodeOffset,int iResType)
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
	strcpy(m_pszMyName,"Resource");
	
	m_pFile->Read(m_dwImageBase,
		dwNodeOffset,
		(BYTE*)&m_resEntryNode,
		sizeof(IMAGE_RESOURCE_DIRECTORY_ENTRY));
		
	if(! (m_resEntryNode.OffsetToData & 0X80000000) )
		throw new PEResParseException(NULL);

	m_pFile->Read(m_dwImageBase,
		m_resEntryNode.OffsetToDirectory,
		(BYTE*)&m_resDirLeaf,
		sizeof(IMAGE_RESOURCE_DIRECTORY));

	if( m_resDirLeaf.NumberOfIdEntries != 1 && m_resDirLeaf.NumberOfNamedEntries != 1 )
		throw new PEResParseException(NULL);

	//This entry gives us the exact address where data is stored.
	m_pFile->Read(m_dwImageBase,
		m_resEntryNode.OffsetToDirectory+sizeof(IMAGE_RESOURCE_DIRECTORY),
		(BYTE*)&m_resEntryLeaf,
		sizeof(IMAGE_RESOURCE_DIRECTORY_ENTRY));
		
	#ifdef _DEBUG

	g_strDbg.Format("            ID: %08X  DataEntryOffs: %08X\r\n"
		,m_resEntryNode.Id,m_resEntryNode.OffsetToData );
	g_File.Write(g_strDbg.GetBuffer(1024),g_strDbg.GetLength());

	#endif

	if( (m_resEntryLeaf.OffsetToData & 0X80000000) )
		throw new PEResParseException(NULL);

	m_pFile->Read(m_dwImageBase,
		m_resEntryLeaf.OffsetToDirectory,
		(BYTE*)&m_resData,
		sizeof(IMAGE_RESOURCE_DATA_ENTRY));
		
	#ifdef _DEBUG

	g_strDbg.Format("            DataRVA: %X  DataSize: %X  CodePage: %X\r\n"
		,m_resData.OffsetToData,m_resData.Size,m_resData.CodePage );
	g_File.Write(g_strDbg.GetBuffer(1024),g_strDbg.GetLength());

	#endif
	//Create appropriate object derived from PEResource
	CreateObjects(m_pFile,iResType);

}

ResourceNode::~ResourceNode(void)
{
	if(m_pRes)
		delete m_pRes;
}
void ResourceNode::GetPropertyInfo(CString& strInfo)
{

	strInfo.Format("SIZE OF DATA\n%d\n",
		m_resData.Size
		);


}
void ResourceNode::IterateElements(PETreeList *pList)
{
	if( !pList)
	{
		char *szErrMsg = new char[1024];
		strcpy(szErrMsg,"Tree Control Object does not exist");
		throw new PEUIException(szErrMsg);
	}
	pList->AddNode(3,m_pszMyName,m_pRes);
}
/******************************************************************************
* Function Name: CreateObjects
*
* Note: Creates objects depending upon resource type
*
******************************************************************************/
void ResourceNode::CreateObjects(PEFile *pFile,DWORD dwName)
{
	switch(dwName)
	{
	/*case 0://"???_0"
		m_pRes = NULL;
		break;*/
	case 1://"CURSOR"
		m_pRes = new PEResCursor(pFile,m_dwImageBase -m_dwRVA + m_resData.OffsetToData,m_resData.Size);
		strcpy(m_pszMyName,"CURSOR");
		break;
	case 2://"BITMAP"
		m_pRes = new PEResBitmap(pFile,m_dwImageBase -m_dwRVA + m_resData.OffsetToData,m_resData.Size);
		strcpy(m_pszMyName,"BITMAP");
		break;
	case 3://"ICON"
		m_pRes = new PEResIcon(pFile,m_dwImageBase -m_dwRVA + m_resData.OffsetToData,m_resData.Size);
		strcpy(m_pszMyName,"ICON");
		break;
	case 4://"MENU"
		m_pRes = new PEResMenu(pFile,m_dwImageBase -m_dwRVA + m_resData.OffsetToData,m_resData.Size);
		strcpy(m_pszMyName,"MENU");
		break;
	case 5://"DIALOG"
		m_pRes = new PEResDialog(pFile,m_dwImageBase -m_dwRVA + m_resData.OffsetToData,m_resData.Size);
		strcpy(m_pszMyName,"DIALOG");
		break;
	case 6://"STRING"
		m_pRes = new PEResString(pFile,m_resData.OffsetToData+(m_dwImageBase -m_dwRVA) ,m_resData.Size);
		strcpy(m_pszMyName,"STRING");
		break;
	/*case 7://"FONTDIR"
		m_pRes = NULL;
		break;
	case 8://"FONT"
		m_pRes = NULL;
		break;
	case 9://"ACCELERATORS"
		m_pRes = NULL;
		break;
	case 10://"RCDATA"
		m_pRes = NULL;
		break;
	case 11://"MESSAGETABLE"
		m_pRes = NULL;
		break;
	case 12://"GROUP_CURSOR"
		m_pRes = NULL;
		break;
	case 13://"???_13"
		m_pRes = NULL;
		break;
	case 14://"GROUP_ICON"
		m_pRes = NULL;
		break;
	case 15://"???_15"
		m_pRes = NULL;
		break;
	case 16://"VERSION"
		m_pRes = NULL;
		break;
	case 17://"DLGINCLUDE"
		m_pRes = NULL;
		break;
	case 18://"???_18"
		m_pRes = NULL;
		break;
	case 19://"PLUGPLAY"
		m_pRes = NULL;
		break;
	case 20://"VXD"
		m_pRes = NULL;
		break;
	case 21://"ANICURSOR"
		m_pRes = NULL;
		break;
	case 22://"ANIICON"
		m_pRes = NULL;
		break;*/
	default://Binary Data
		m_pRes = new PEResBinary(pFile,m_resData.OffsetToData+(m_dwImageBase -m_dwRVA) ,m_resData.Size);
		strcpy(m_pszMyName,"Binary");
		break;
	}
}
