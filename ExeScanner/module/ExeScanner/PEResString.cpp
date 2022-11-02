/******************************************************************************
*
* PROJECT:		ExeScanner
* FILE NAME:	PEResString.cpp
*
* DEVELOPER:	Vishalsinh Jhala
* DATE:			2-Apr-2005
*
* DESCRIPTION:	Defination file for class PEResString.
*
* NOTES:		Encapsulates String Resource. Such resources, when requested
*				by user to be displayed, are shown in String editor.
*
******************************************************************************/
#include "StdAfx.h"
#include "PEResString.h"
#include "MainFrm.h"

PEResString::PEResString(PEFile *pFile, DWORD dwOffset, DWORD dwSize)
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
	strcpy(m_pszMyName,"STRING");
}

PEResString::~PEResString(void)
{

}
const char* PEResString::GetName()
{
	return (const char*)m_pszMyName;
}

void PEResString::GetPropertyInfo(CString& strInfo)
{
}
void PEResString::IterateElements(PETreeList *pList)
{

}
void PEResString::Draw(CDC **pDC,CBitmap **pBmp,CWnd **pWnd)
{
	//Load the date from File
	m_pResData = new BYTE[m_dwSize];
	m_pFile->Read(m_dwOffset,0,m_pResData,m_dwSize);

	//Create a list view for String editor
	HWND hwnd = CreateWindow("SysListView32",
		"",WS_VISIBLE|WS_CHILD|LVS_REPORT|LVS_NOCOLUMNHEADER,
		0,0,100,100,((CMainFrame*)::AfxGetMainWnd())->m_wndView.m_wndView.GetSafeHwnd(),
		(HMENU)NULL,::AfxGetInstanceHandle(),0);

	//Attach the HWND with CWnd Object that will be returned to UI for display.
	*pWnd = new CWnd();
	(*pWnd)->Attach(hwnd);

	//Litte beautification
	((CListCtrl*)(*pWnd))->InsertColumn(0,"#",LVCFMT_LEFT,30);
	((CListCtrl*)(*pWnd))->InsertColumn(1,"String",LVCFMT_LEFT,1100);
	((CListCtrl*)(*pWnd))->InsertColumn(2,"Size",LVCFMT_LEFT,0);
	((CListCtrl*)(*pWnd))->SetExtendedStyle(LVS_EX_GRIDLINES);

	//Parse the input and populate the String editor
	//iLines decides the number of rows in ListView.
	int iLines = 0;
	short iChars = 0;
	DWORD iStartPos = 0;
	CString strRow,strTmp;

	//Remember you are going to parse UNICODE string
	while(iStartPos<m_dwSize)
	{
		//Start with n number of pairs of bytes with value 0.
		while(!m_pResData[iStartPos] && !m_pResData[iStartPos+1])
			iStartPos+=2;
		
		//Safety check to avoid runtime exceptions and that we dont read
		//anything beyond resource size.
		if(iStartPos>=m_dwSize)
			break;

		//Next 2 bytes give the size of string in characters.
		memcpy(&iChars,m_pResData+iStartPos,2);
		iStartPos+=2;

		//Safety check to avoid runtime exceptions and that we dont read
		//anything beyond resource size.
		if(iStartPos>=m_dwSize)
			break;

		//Safety check to avoid runtime exceptions and that we dont read
		//anything beyond resource size.
		if( iStartPos+iChars*2>m_dwSize )
		{
			char *szErrMsg = new char[1024];
			strcpy(szErrMsg,"Invalid String resource");
			throw new PEResParseException(szErrMsg);
		}

		//Read and format the string.
		BYTE *pB = new BYTE[iChars*2+2];
		memcpy(pB,m_pResData+iStartPos,iChars*2);
		pB[iChars*2] = pB[iChars*2+1] = 0;

		strTmp.Format("%d",iStartPos);
		iStartPos += iChars*2;
		strRow.Format("%S",pB);

		delete []pB;

		//Add it to string editor
		((CListCtrl*)(*pWnd))->InsertItem(iLines,strTmp);
		((CListCtrl*)(*pWnd))->SetItemText(iLines,1,strRow);
		strTmp.Format("%d",iChars);
		((CListCtrl*)(*pWnd))->SetItemText(iLines,2,strTmp);

		iLines++;

	}
	//Release Memory
	delete m_pResData;
	m_pResData = NULL;
}
void PEResString::Import(BOOL bFromFile)
{
	AfxMessageBox("Clicking on string allows you to modify them");
}
void PEResString::Export(BOOL bFromFile)
{
	//Load the data
	m_pResData = new BYTE[m_dwSize];
	m_pFile->Read(m_dwOffset,0,m_pResData,m_dwSize);

	//iLines decides the number of rows in ListView.
	int iLines = 0;
	short iChars = 0;
	DWORD iStartPos = 0;
	CString strRow, str,strTemp;
	
	while(iStartPos<m_dwSize)
	{
		//Start with n number of pairs of bytes with value 0.
		while(!m_pResData[iStartPos] && !m_pResData[iStartPos+1])
			iStartPos+=2;

		//Safety check to avoid runtime exceptions and that we dont read
		//anything beyond resource size.
		if(iStartPos>=m_dwSize)
			break;

		//Next 2 bytes give the size of string in characters.
		memcpy(&iChars,m_pResData+iStartPos,2);
		iStartPos+=2;

		//Safety check to avoid runtime exceptions and that we dont read
		//anything beyond resource size.
		if(iStartPos>=m_dwSize)
			break;

		//Safety check to avoid runtime exceptions and that we dont read
		//anything beyond resource size.
		if( iStartPos+iChars*2>m_dwSize )
		{
			char *szErrMsg = new char[1024];
			strcpy(szErrMsg,"Invalid String resource");
			throw new PEResParseException(szErrMsg);
		}

		//Read and format the string.
		BYTE *pB = new BYTE[iChars*2];
		memcpy(pB,m_pResData+iStartPos,iChars*2);
		iStartPos += iChars*2;
		strRow.Format("%s\n\n%S",str,pB);
		delete []pB;
		str.Format("%s",strRow);
		iLines++;

	}
	
	//Copy the string to clipboard.
	HGLOBAL hglbCopy = NULL;
	
	//Open and empty clipboard.
	OpenClipboard(AfxGetMainWnd()->GetSafeHwnd());
	EmptyClipboard();

	//Allocate global momory
	hglbCopy = GlobalAlloc(GMEM_MOVEABLE, str.GetLength()+1);
	if(hglbCopy==NULL)
	{
		char *szErrMsg = new char[1024];
		strcpy(szErrMsg,"Cannot copy string resource to clipboard");
		throw new PEResParseException(szErrMsg);
	}

	//Lock and transfer the data to global memory
	char* szMem = (char*)GlobalLock(hglbCopy);
	memcpy(szMem,str.GetBuffer(1024),str.GetLength());
	
	//Mark the end of string
	szMem[str.GetLength()] = 0;
	
	//Unlock now
	GlobalUnlock(hglbCopy);

	//transfer global memory to clipboard
	try{
		HANDLE H = SetClipboardData(CF_TEXT,hglbCopy);
	}
	catch(...)
	{
		char *szErrMsg = new char[1024];
		strcpy(szErrMsg,"Cannot copy string resource to clipboard");
		throw new PEResParseException(szErrMsg);

	}
	CloseClipboard();

	//Release Memory
	delete m_pResData;
	m_pResData = NULL;
	AfxMessageBox("Data copied to clipboard. Paste in Notepad or any other text editor");

}

/******************************************************************************
* Function Name: Save
*
* Note: This function is called when a user has modifed a string in string editor.
*		iRelOffset - Offset within Resource Data.
*		strDate - New String to be saved.
*		iOrgSize - Orginal size of the string before it was modified.
*
******************************************************************************/
void PEResString::Save(int iRelOffset, CString strData, int iOrgSize)
{
	//convert string to UNICODE
	BSTR bstr = strData.AllocSysString();
	
	//Write string to file
	int iLen = strData.GetLength();
	m_pFile->Write(m_dwOffset,iRelOffset,(BYTE*)bstr,iLen*2);
	
	//If the new string is smaller than its original size replace the remaining
	//bytes with 0.
	if(iLen<iOrgSize)
	{
		BYTE *b = new BYTE[(iOrgSize-iLen)*2];
		
		ZeroMemory(b,(iOrgSize-iLen)*2);
		m_pFile->Write(m_dwOffset,iRelOffset+iLen*2,b,(iOrgSize-iLen)*2);
		
		delete []b;
	}
	//As always, release memory.
	SysFreeString(bstr);
}