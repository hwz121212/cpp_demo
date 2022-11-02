/******************************************************************************
*
* PROJECT:		ExeScanner
* FILE NAME:	PEResBinary.cpp
*
* DEVELOPER:	Vishalsinh Jhala
* DATE:			2-Apr-2005
*
* DESCRIPTION:	Defination file for class PEResBinary.
*
* NOTES:		Encapsulates Binary Resource. Such resources, when requested
*				by user to be displayed, are shown in a hex editor. Individual
*				bytes can be manipulated upon users request.
*
******************************************************************************/
#include "StdAfx.h"
#include "PEResBinary.h"
#include "MainFrm.h"

PEResBinary::PEResBinary(PEFile *pFile, DWORD dwOffset, DWORD dwSize)
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
	//Who am I?
	strcpy(m_pszMyName,"BINARY");

}

PEResBinary::~PEResBinary(void)
{

}
const char* PEResBinary::GetName()
{
	return (const char*)m_pszMyName;
}
void PEResBinary::GetPropertyInfo(CString& strInfo)
{
	strInfo.Format("RESOURCE TYPE\n%s\n"
		"OFFSET\n%d\n"
		"SIZE\n%d\n",
		"Binary Data",
		m_dwOffset,
		m_dwSize
		);
	
}
void PEResBinary::IterateElements(PETreeList *pList)
{

}
void PEResBinary::Draw(CDC **pDC,CBitmap **pBmp,CWnd **pWnd)
{
	DWORD m_dwSizeNew = m_dwSize;
	if(m_dwSize>1024*1024)
	{	
		AfxMessageBox("Binary Data Bigger that 1 MB. Only first MB of data will be shown");
		m_dwSizeNew = 1024*1024;
	}
	
	//Load the date from File
	m_pResData = new BYTE[m_dwSizeNew];
	m_pFile->Read(m_dwOffset,0,m_pResData,m_dwSizeNew);

	//Create a list view for hex editor
	HWND hwnd = CreateWindow("SysListView32",
		"",WS_VISIBLE|WS_CHILD|LVS_REPORT|LVS_NOCOLUMNHEADER|LVS_SHOWSELALWAYS|LVS_SINGLESEL ,
		0,0,100,100,((CMainFrame*)::AfxGetMainWnd())->m_wndView.m_wndView.GetSafeHwnd(),
		(HMENU)NULL,::AfxGetInstanceHandle(),0);

	//Attach the HWND with CWnd Object that will be returned to UI for display.
	*pWnd = new CWnd();
	(*pWnd)->Attach(hwnd);
	
	//Beauify the list view.
	Beautify(pWnd);

	//Parse the input and populate the hex editor
	//iLines decides the number of rows in ListView.
	int iLines = (int)ceil(double(m_dwSizeNew /16)) + 1;
	
	CString strRow,strCol;
	char *chCol = new char[17]; //Ascii Display of 16 chars displayed as hex

	for(int i=0;i<iLines;i++)
	{
		strRow.Format("%08X  ",i*16);//Address of row

		((CListCtrl*)(*pWnd))->InsertItem(iLines,strRow);

		ZeroMemory(chCol,16);
		//Insert all the 16 chars of a row in hex format
		for(int j=0;j<16;j++)
		{
			if(i*16+j>=m_dwSizeNew)
				break;
			strCol.Format("%02X",m_pResData[i*16+j]);
			if(j<8)
				((CListCtrl*)(*pWnd))->SetItemText(i,j+1,strCol);
			else
				((CListCtrl*)(*pWnd))->SetItemText(i,j+2,strCol);
			if(isalnum(m_pResData[i*16+j]))
				chCol[j] = m_pResData[i*16+j];
			else
				chCol[j] = '.';
		}
		chCol[16] = '\0';
		((CListCtrl*)(*pWnd))->SetItemText(i,18,chCol);

	}

	//Release Memory
	delete [] chCol;
	delete m_pResData;
	m_pResData = NULL;
}

void PEResBinary::Import(BOOL bFromFile)
{
	AfxMessageBox("Click on the byte you want to modify.");

}
void PEResBinary::Export(BOOL bFromFile)
{
	CFileDialog dlgFileSave(FALSE,0,"Untitled.bin",
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_PATHMUSTEXIST);
	
	if(dlgFileSave.DoModal() == IDOK)
	{
		CString strPath = dlgFileSave.GetPathName();
		
		CFile fileExport;
		
		fileExport.Open(strPath,CFile::modeCreate|CFile::modeWrite|CFile::typeBinary);

		m_pResData = new BYTE[m_dwSize];
		m_pFile->Read(m_dwOffset,0,m_pResData,m_dwSize);
		
		fileExport.Write(m_pResData,m_dwSize);
		
		delete m_pResData;
		m_pResData = NULL;

		fileExport.Close();
	}

}

/******************************************************************************
* Function Name: Save
*
* Note: This function is called when a user has modifed a byte in hex editor.
*		iPos - Offset within Resource Data.
*		bChar - New byte value to be saved.
******************************************************************************/
void PEResBinary::Save(BYTE bChar, int iPos)
{
	CListCtrl *pCtrl = (CListCtrl*)((CMainFrame*)::AfxGetMainWnd())->m_wndView.m_pWndRes;
	
	m_pResData = new BYTE[1];
	m_pResData[0] = bChar;
	
	m_pFile->Write(m_dwOffset,iPos,m_pResData,1);
	
	delete m_pResData;
	m_pResData = NULL;
}
void PEResBinary::Beautify(CWnd **pWnd)
{
	((CListCtrl*)(*pWnd))->InsertColumn(0,"Address",LVCFMT_LEFT,70);
	((CListCtrl*)(*pWnd))->InsertColumn(1,"",LVCFMT_LEFT,25);
	((CListCtrl*)(*pWnd))->InsertColumn(2,"",LVCFMT_LEFT,25);
	((CListCtrl*)(*pWnd))->InsertColumn(3,"",LVCFMT_LEFT,25);
	((CListCtrl*)(*pWnd))->InsertColumn(4,"",LVCFMT_LEFT,25);
	((CListCtrl*)(*pWnd))->InsertColumn(5,"",LVCFMT_LEFT,25);
	((CListCtrl*)(*pWnd))->InsertColumn(6,"",LVCFMT_LEFT,25);
	((CListCtrl*)(*pWnd))->InsertColumn(7,"",LVCFMT_LEFT,25);
	((CListCtrl*)(*pWnd))->InsertColumn(8,"",LVCFMT_LEFT,25);
	((CListCtrl*)(*pWnd))->InsertColumn(9,"",LVCFMT_LEFT,25);
	((CListCtrl*)(*pWnd))->InsertColumn(10,"",LVCFMT_LEFT,25);
	((CListCtrl*)(*pWnd))->InsertColumn(11,"",LVCFMT_LEFT,25);
	((CListCtrl*)(*pWnd))->InsertColumn(12,"",LVCFMT_LEFT,25);
	((CListCtrl*)(*pWnd))->InsertColumn(13,"",LVCFMT_LEFT,25);
	((CListCtrl*)(*pWnd))->InsertColumn(14,"",LVCFMT_LEFT,25);
	((CListCtrl*)(*pWnd))->InsertColumn(15,"",LVCFMT_LEFT,25);
	((CListCtrl*)(*pWnd))->InsertColumn(16,"",LVCFMT_LEFT,25);
	((CListCtrl*)(*pWnd))->InsertColumn(17,"",LVCFMT_LEFT,25);
	((CListCtrl*)(*pWnd))->InsertColumn(18,"Char View",LVCFMT_LEFT,200);

}
