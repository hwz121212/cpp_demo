/******************************************************************************
*
* PROJECT:		ExeScanner
* FILE NAME:	PortableExecutable.h
*
* DEVELOPER:	Vishalsinh Jhala
* DATE:			2-Apr-2005
*
* DESCRIPTION:	Declaration file for class PortableExecutable.
*
* NOTES:		Starting point to enter a PE. Reads and verifies DOS/WIN 
*				headers. Direct Child Objects are ResourceSections. When the 
*				program will deal will all sections ResourceSection will 
*				become a generic class PESection. A PESection with ".rsrc"
*				will be ResourceSection.
*				convert this to a singleton class later on
*
******************************************************************************/

#pragma once

#include "pebase.h"
#include "ResourceSection.h"

class PortableExecutable :
	public PEBase
{
	ResourceSection *m_pRSRC;

	void GetMachine(CString&str);
	void GetCharacteristics(CString&str);
	void GetSubSystem(CString&str);
	void GetDllChars(CString&str);
public:
	IMAGE_DOS_HEADER dosHeader;
	IMAGE_NT_HEADERS winHeader;
	IMAGE_SECTION_HEADER *m_pSectionHeader;
	
	PortableExecutable(PEFile *pFile);
	~PortableExecutable(void);
	virtual void IterateElements(PETreeList *pList);
	virtual void GetPropertyInfo(CString& strInfo);
};
