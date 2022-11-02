/******************************************************************************
*
* PROJECT:		ExeScanner
* FILE NAME:	PEBase.h
*
* DEVELOPER:	Vishalsinh Jhala
* DATE:			2-Apr-2005
*
* DESCRIPTION:	Declaration file for Abstract Classes
*						PEBase
*						PEResource
*						IMAGE_RESOURCE
* NOTES:		All current and future classes that represent PE Objects will 
*				be derived from PEBase.
*
******************************************************************************/
#pragma once
#include "PEFile.h"
#include "PETreeList.h"

class PEBase
{
protected:
	PEFile *m_pFile;//PE File handle to load, import, export itself.
	char *m_pszMyName;//Name of Object
	
public:

	PEBase(void){m_pszMyName = new char[256];}
	~PEBase(void){delete m_pszMyName;}

	virtual void IterateElements(PETreeList *pList)=0; 
	virtual void GetPropertyInfo(CString& strInfo)=0;
};

class PEResource : public PEBase
{
protected:
	BYTE *m_pResData;//Binary resource data loaded from file.

public:
	PEResource(void):m_pResData(NULL){}
	~PEResource(void){if(m_pResData) delete m_pResData;}

	virtual const char* GetName()=0;
	virtual void Draw(CDC **pDC,CBitmap **pBmp,CWnd **pWnd = NULL)=0;
	virtual void Import(BOOL bFromFile)=0;
	virtual void Export(BOOL bFromFile)=0;

};
typedef struct
{
	CString strFile;
	int iWidth;
	int iHeight;
	int iBPP;
	int iSize;
	CDC* pDC;
}IMAGE_RESOURCE;