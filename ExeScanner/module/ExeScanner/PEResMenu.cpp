/******************************************************************************
*
* PROJECT:		ExeScanner
* FILE NAME:	PEResMenu.cpp
*
* DEVELOPER:	Vishalsinh Jhala
* DATE:			2-Apr-2005
*
* DESCRIPTION:	Defination file for class PEResMenu.
*
* NOTES:		Encapsulates Menu Resource. Such resources, when requested
*				by user to be displayed, are shown in a modaless window.
*
******************************************************************************/
#include "StdAfx.h"
#include "PEResMenu.h"

PEResMenu::PEResMenu(PEFile *pFile, DWORD dwOffset, DWORD dwSize)
	:m_dwOffset(dwOffset),m_dwSize(dwSize)
{
	//Validate Inputs
	if( !pFile )
	{
		char *szErrMsg = new char[1024];
		strcpy(szErrMsg,"Cannot Proceed with NULL File");
		throw new PEFileException(szErrMsg);
	}
	m_pFile = pFile;

	//Who am I?
	strcpy(m_pszMyName,"Menu");
	
}

PEResMenu::~PEResMenu(void)
{

}
const char* PEResMenu::GetName()
{
	return (const char*)m_pszMyName;
}

void PEResMenu::GetPropertyInfo(CString& strInfo)
{

	strInfo.Format("RESOURCE TYPE\n%s\n"
		"OFFSET\n%d\n"
		"SIZE\n%d\n",
		"Menu Resource",
		m_dwOffset,
		m_dwSize
		);
}
void PEResMenu::IterateElements(PETreeList *pList)
{

}
void PEResMenu::Draw(CDC **pDC,CBitmap **pBmp,CWnd **pWnd)
{
	//Load the date from File
	m_pResData = new BYTE[m_dwSize];
	m_pFile->Read(m_dwOffset,0,m_pResData,m_dwSize);

	//Create the menu from data loaded.
	CMenu menu;
	if( !menu.LoadMenuIndirect(m_pResData) )
	{
		int iErr = GetLastError();
		char *szErrMsg = new char[1024];
		strcpy(szErrMsg,"Error while copying Menu to memory:");
		itoa(iErr,szErrMsg+35,10);
		throw new PEResParseException(szErrMsg);
	}
	

	//Create a modeless window
	HWND hwnd = CreateWindow(::AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW,NULL,NULL,NULL),
		"",WS_VISIBLE|WS_CAPTION|WS_OVERLAPPEDWINDOW,
		100,100,500,50,::AfxGetMainWnd()->GetSafeHwnd(),
		menu.GetSafeHmenu(),::AfxGetInstanceHandle(),0);

	SetMenu(hwnd,menu.GetSafeHmenu());

	//Attach the HWND with CWnd Object that will be returned to UI for display.
	*pWnd = new CWnd();
	(*pWnd)->Attach(hwnd);

	//Release Memory
	delete m_pResData;
	m_pResData = NULL;

}
void PEResMenu::Import(BOOL bFromFile)
{
	AfxMessageBox("Current Version does not support export/import of Menu resource.");

}
void PEResMenu::Export(BOOL bFromFile)
{
	AfxMessageBox("Current Version does not support export/import of Menu resource.");

}
