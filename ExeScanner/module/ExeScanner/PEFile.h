/******************************************************************************
*
* PROJECT:		ExeScanner
* FILE NAME:	PEFile.h
*
* DEVELOPER:	Vishalsinh Jhala
* DATE:			2-Apr-2005
*
* DESCRIPTION:	Declaration file for class PEFile.
*
* NOTES:		Represents PE File. Allows client classes to read/write 
*				directly to file by specifying physical offset from Begin.
*
******************************************************************************/

#pragma once
#include "Exception.h"

class PEFile
{
	CFile *m_pFile; //File Object
	CString m_strPath;//Path to file system

public:
	PEFile(CString strPath);
	~PEFile(void);
	int Read(DWORD dwImageBase, DWORD dwRVA, BYTE* pBuff, int iSize) ;
	void Write(DWORD dwImageBase, DWORD dwRVA, BYTE* pBuff, int iSize);
	CString GetFileName(){return m_strPath;};

};
