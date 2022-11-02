/******************************************************************************
*
* PROJECT:		ExeScanner
* FILE NAME:	PEFile.cpp
*
* DEVELOPER:	Vishalsinh Jhala
* DATE:			2-Apr-2005
*
* DESCRIPTION:	Defination file for class PEFile.
*
* NOTES:		Represents PE File. Allows client classes to read/write 
*				directly to file by specifying physical offset from Begin.
*
******************************************************************************/
#include "StdAfx.h"
#include "pefile.h"

PEFile::PEFile(CString strPath)
	:m_strPath(strPath)
{
	
	m_pFile = new CFile;
		
	try
	{
		CFileException e;
        
		//Open File
		if( !m_pFile->Open(m_strPath,
			CFile::modeReadWrite|CFile::shareDenyWrite|CFile::typeBinary
			,&e) )
		{
			char *pErrMsg = new char[1024];
			e.GetErrorMessage(pErrMsg,1024);
			throw new PEFileException(pErrMsg);
		}
	}
	catch(PEFileException *e)
	{
		if( m_pFile )
			delete m_pFile;

		throw e;
	}

}

PEFile::~PEFile(void)
{
	if(m_pFile)
		delete m_pFile;
}
/******************************************************************************
* Function Name: Read
*
* Note: Function Reads data at dwImageBase+dwRVA relative to begin of file
*		Memory Allocation & deallocation of pBuff is clients' responsibility.
*
******************************************************************************/
int PEFile::Read(DWORD dwImageBase, DWORD dwRVA, BYTE* pBuff, int iSize)
{
	int iRet = 0;
	
	try
	{
		m_pFile->Seek(dwImageBase+dwRVA,CFile::begin);
		iRet = m_pFile->Read(pBuff,iSize);
	}
	catch(CFileException *e)
	{
		char *szErrMsg = new char[1024];
		e->GetErrorMessage(szErrMsg,1024);
		throw new PEFileException(szErrMsg);
	}
	
	return iRet;

}
/******************************************************************************
* Function Name: Write
*
* Note: Function writes data at dwImageBase+dwRVA relative to begin of file
*		Memory Allocation & deallocation of pBuff is clients' responsibility.
*
******************************************************************************/
void PEFile::Write(DWORD dwImageBase, DWORD dwRVA, BYTE* pBuff, int iSize)
{
	
	try
	{
		m_pFile->Seek(dwImageBase+dwRVA,CFile::begin);
		m_pFile->Write(pBuff,iSize);
	}
	catch(CFileException *e)
	{
		char *szErrMsg = new char[1024];
		e->GetErrorMessage(szErrMsg,1024);
		throw new PEFileException(szErrMsg);
	}

}