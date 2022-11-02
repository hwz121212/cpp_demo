/******************************************************************************
*
* PROJECT:		ExeScanner
* FILE NAME:	PEResDialog.cpp
*
* DEVELOPER:	Vishalsinh Jhala
* DATE:			2-Apr-2005
*
* DESCRIPTION:	Defination file for class PEResDialog.
*
* NOTES:		Encapsulates Dialog Resource. Such resources, when requested
*				by user to be displayed, are shown in a modalless window.
*
******************************************************************************/
#include "StdAfx.h"
#include "peresDialog.h"

PEResDialog::PEResDialog(PEFile *pFile, DWORD dwOffset, DWORD dwSize)
	:m_dwOffset(dwOffset),m_dwSize(dwSize)
{
	//Validate the inputs
	if( !pFile )
	{
		char *szErrMsg = new char[1024];
		strcpy(szErrMsg,"Cannot Proceed with NULL File");
		throw new PEFileException(szErrMsg);
	}
	m_pFile = pFile;

	//who am i?
	strcpy(m_pszMyName,"DIALOG");

}

PEResDialog::~PEResDialog(void)
{

}
const char* PEResDialog::GetName()
{
	return (const char*)m_pszMyName;
}

void PEResDialog::GetPropertyInfo(CString& strInfo)
{
	DLGTEMPLATE dlgtemp;
	m_pFile->Read(m_dwOffset,0,(BYTE*)&dlgtemp,sizeof(DLGTEMPLATE));

	CString strStyle;
	//GetDlgStyle(strStyle,dlgtemp.style);

	strInfo.Format("RESOURCE TYPE\n%s\n"
		"OFFSET\n%d\n"
		"SIZE\n%d\n"
		"WIDTH\n%d\n"
		"HEIGHT\n%d\n"
		"CONTROL #\n%d\n",
		/*"STYLE\n%s\n",*/
		"Dialog Box",
		m_dwOffset,
		m_dwSize,
		dlgtemp.cx,
		dlgtemp.cy,
		dlgtemp.cdit/*,
		dlgtemp.style*/		
		);
}
void PEResDialog::IterateElements(PETreeList *pList)
{

}
void PEResDialog::Draw(CDC **pDC,CBitmap **pBmp,CWnd **pWnd)
{
	//Load the date from File
	m_pResData = new BYTE[m_dwSize];
	m_pFile->Read(m_dwOffset,0,m_pResData,m_dwSize);

	//Determind the width and height of dialog
	short iWidth,iHeight;
	memcpy(&iWidth, m_pResData+14,2);
	memcpy(&iHeight, m_pResData+16,2);

	//Even though we get invalid width or height, dialogs are created. 
	//So commenting this code.
	//In the next version, resource scripting engine will properly decompile the size etc.
	/*
	if( iWidth <=0 || iHeight <=0 )
	{
		char *szErrMsg = new char[1024];
		strcpy(szErrMsg,"Invalid Dialog Header");
		throw new PEResParseException(szErrMsg);
	}*/

	
	//Create a modeless window
	//CRect rect(0,0,iWidth,iHeight);
	HWND hwnd = NULL;
	hwnd= CreateDialogIndirectParam(::AfxGetInstanceHandle(),
		(LPDLGTEMPLATE)m_pResData,
		::AfxGetMainWnd()->GetSafeHwnd(),
		NULL,
		/*WS_VISIBLE|*/WS_POPUP|WS_CAPTION);

	//width/height we got for resource data should be converted to Window dimensions.
	//MapDialogRect(hwnd,&rect);
	
	if(!hwnd)
	{
		int iErr = GetLastError();
		char *szErrMsg = new char[1024];
		strcpy(szErrMsg,"Error while Creating Dailog Resource:");
		itoa(iErr,szErrMsg+37,10);
		throw new PEResParseException(szErrMsg);
	}

	//Attach the HWND with CWnd Object that will be returned to UI for display.
	*pWnd = new CWnd();
	(*pWnd)->Attach(hwnd);

	//Release Memory
	delete m_pResData;
	m_pResData = NULL;

}
void PEResDialog::Import(BOOL bFromFile)
{
	AfxMessageBox("Current Version does not support export/import of dialog resource.");
}
void PEResDialog::Export(BOOL bFromFile)
{
	AfxMessageBox("Current Version does not support export/import of dialog resource.");

}
void PEResDialog::GetDlgStyle(CString& strStyle,DWORD dwStyle)
{
	/*if(dwStyle|DS_3DLOOK
	{
	#define DS_ABSALIGN         0x01L
#define DS_SYSMODAL         0x02L
#define DS_LOCALEDIT        0x20L   
#define DS_SETFONT          0x40L   
#define DS_MODALFRAME       0x80L   
#define DS_NOIDLEMSG        0x100L  
#define DS_SETFOREGROUND    0x200L  


#if(WINVER >= 0x0400)
#define DS_3DLOOK           0x0004L
#define DS_FIXEDSYS         0x0008L
#define DS_NOFAILCREATE     0x0010L
#define DS_CONTROL          0x0400L
#define DS_CENTER           0x0800L
#define DS_CENTERMOUSE      0x1000L
#define DS_CONTEXTHELP      0x2000L
	
	}*/
}