/******************************************************************************
*
* PROJECT:		ExeScanner
* FILE NAME:	PEResIcon.cpp
*
* DEVELOPER:	Vishalsinh Jhala
* DATE:			2-Apr-2005
*
* DESCRIPTION:	Defination file for class PEResIcon.
*
* NOTES:		Encapsulates Icon Resource. Such resources, when requested
*				by user to be displayed, are shown in View.
*
******************************************************************************/
#include "StdAfx.h"
#include "PEResIcon.h"
#include "ImageImportDlg.h"

PEResIcon::PEResIcon(PEFile *pFile, DWORD dwOffset, DWORD dwSize)
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
	strcpy(m_pszMyName,"ICON");
	
}

PEResIcon::~PEResIcon(void)
{

}
const char* PEResIcon::GetName()
{
	return (const char*)m_pszMyName;
}

void PEResIcon::GetPropertyInfo(CString& strInfo)
{
	BITMAPINFOHEADER bih;
	m_pFile->Read(m_dwOffset,0,(BYTE*)&bih,sizeof(BITMAPINFOHEADER));
	
	int iColors = (int)pow(2,bih.biBitCount);
	strInfo.Format("RESOURCE TYPE\n%s\n"
		"OFFSET\n%d\n"
		"SIZE\n%d\n"
		"WIDTH\n%d\n"
		"HEIGHT\n%d\n"
		"BITS PER PIXEL#\n%d\n"
		"COLORS\n%d\n",
		"Icon Image",
		m_dwOffset,
		m_dwSize,
		bih.biWidth,
		bih.biHeight,
		bih.biBitCount,
		iColors		
		);
}
void PEResIcon::IterateElements(PETreeList *pList)
{

}
void PEResIcon::Draw(CDC **pDC,CBitmap **pBmp,CWnd **pWnd)
{
	//Load the date from File
	m_pResData = new BYTE[m_dwSize];
	m_pFile->Read(m_dwOffset,0,m_pResData,m_dwSize);

	*pDC = new CDC();
	*pBmp = new CBitmap();

	BITMAPINFOHEADER bih;
	BITMAPINFO *bi;
	
	//First thing that appears in data is the BITMAPINFOHEADER
	memcpy(&bih,m_pResData,sizeof(BITMAPINFOHEADER));
	if( bih.biSize !=40 || bih.biWidth <=0 || bih.biHeight <=0 )
	{
		char *szErrMsg = new char[1024];
		strcpy(szErrMsg,"Invalid Icon Header");
		throw new PEResParseException(szErrMsg);
	}

	//Decide the presence of color table in bitmap
	int iColors = (int)pow(2,bih.biBitCount);
	bi = NULL;
	
	if( iColors > 0 && iColors <= 256 )
	{
		if(bih.biBitCount == 1)
		{
			typedef struct tag{
					BITMAPINFOHEADER bmiHeader;
					RGBQUAD bmiColors[2];
					} bi1;
			bi = (BITMAPINFO*)new bi1;

		}
		else if(bih.biBitCount == 2)
		{
			typedef struct tag{
					BITMAPINFOHEADER bmiHeader;
					RGBQUAD bmiColors[4];
					} bi1;
			bi = (BITMAPINFO*)new bi1;

		}
		else if(bih.biBitCount == 3)
		{
			typedef struct tag{
					BITMAPINFOHEADER bmiHeader;
					RGBQUAD bmiColors[8];
					} bi1;
			bi = (BITMAPINFO*)new bi1;

		}
		else if(bih.biBitCount == 4)
		{
			typedef struct tag{
					BITMAPINFOHEADER bmiHeader;
					RGBQUAD bmiColors[16];
					} bi1;
			bi = (BITMAPINFO*)new bi1;

		}
		else if(bih.biBitCount == 8)
		{
			typedef struct tag{
					BITMAPINFOHEADER bmiHeader;
					RGBQUAD bmiColors[256];
					} bi1;
			bi = (BITMAPINFO*)new bi1;

		}
	}
	else
	{
		iColors = 0;
		bi = new BITMAPINFO;
	}
	//Read colortable
	memcpy(bi,m_pResData,sizeof(BITMAPINFOHEADER)+iColors*4);
	
	//Initialize the bitmap and DC object that will be send back to UI.
	(*pDC)->CreateCompatibleDC(CDC::FromHandle(::GetDC(NULL)));
	(*pBmp)->CreateCompatibleBitmap(CDC::FromHandle(::GetDC(NULL)),bih.biWidth,bih.biWidth);
	(*pDC)->SelectObject(*pBmp);

	//Convert data loaded from file to bitmap and copy bitmap bits to DC.
	int iRet = SetDIBitsToDevice((*pDC)->GetSafeHdc(),0,0,bih.biWidth,bih.biWidth,0,0,0,bih.biWidth,m_pResData+sizeof(BITMAPINFOHEADER)+iColors*4,bi,DIB_RGB_COLORS);
	if(iRet == 0)
	{
		int iErr = GetLastError();
		char *szErrMsg = new char[1024];
		strcpy(szErrMsg,"Error while copying Icon to memory:");
		itoa(iErr,szErrMsg+35,10);
		throw new PEResParseException(szErrMsg);
	}

	//Release Memory
	delete m_pResData;
	m_pResData = NULL;
	delete bi;
}
void PEResIcon::Import(BOOL bFromFile)
{
	IMAGE_RESOURCE irSrc,irDest;

	//Get the file to be imported
	CFileDialog dlgOpen(TRUE,0,0,
		OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT|OFN_FILEMUSTEXIST|OFN_PATHMUSTEXIST,
		"Icons (*.ico)|*.ico||");
	
	if(dlgOpen.DoModal() == IDOK)
	{
		irSrc.strFile = dlgOpen.GetPathName();
	}
	else
		return;
	
	CFileStatus fs;
	CFile::GetStatus(irSrc.strFile,fs);

	//Load the file
	HANDLE handle = NULL;
	handle = ::LoadImage(::AfxGetInstanceHandle(),
	irSrc.strFile,IMAGE_ICON,0,0,LR_LOADFROMFILE|LR_DEFAULTCOLOR);
	if(handle == NULL)
	{
		char *szErrMsg = new char[27];
		strcpy(szErrMsg,"Error while Loading Icon");
		throw new PEResParseException(szErrMsg);
	}
	CBitmap bmpSrc,*pbmpDest;
	CDC dcSrc,*pdcDest;
	BITMAP bitmap;

	BITMAPINFOHEADER bih;
	ICONDIR id;
	ICONDIRENTRY ide;

	CFile fileSrc;
	fileSrc.Open(irSrc.strFile,CFile::modeRead|CFile::typeBinary);
	fileSrc.Read(&id,sizeof(ICONDIR));
	fileSrc.Read(&ide,sizeof(ICONDIRENTRY));
	fileSrc.Read(&bih,sizeof(BITMAPINFOHEADER));

	
	//Prepare the source bitmap loaded from file.
	dcSrc.CreateCompatibleDC(CDC::FromHandle(::GetDC(NULL)));
	bmpSrc.CreateCompatibleBitmap(CDC::FromHandle(::GetDC(NULL)),ide.bWidth,ide.bWidth);
	dcSrc.SelectObject(&bmpSrc);
	dcSrc.DrawIcon(0,0,(HICON)handle);

	//Prepare image resource structure.
	irSrc.iWidth = bih.biWidth;
	irSrc.iHeight = bih.biHeight;
	irSrc.iBPP = bih.biBitCount;
	irSrc.iSize = (int)(fs.m_size - sizeof(ICONDIR)- sizeof(ICONDIRENTRY));
	irSrc.pDC = &dcSrc;

	//Load destination image with in PE file for display.
	m_pResData = new BYTE[m_dwSize];
	m_pFile->Read(m_dwOffset,0,m_pResData,m_dwSize);
	memcpy(&bih,m_pResData,sizeof(BITMAPINFOHEADER));
	delete m_pResData;
	m_pResData = NULL;

	Draw(&pdcDest,&pbmpDest);
	pbmpDest->GetBitmap(&bitmap);
	irDest.strFile = "RESOURCE";
	irDest.iWidth = bih.biWidth;
	irDest.iHeight = bih.biHeight;
	irDest.iBPP = bih.biBitCount;
	irDest.iSize = m_dwSize;
	irDest.pDC = pdcDest;

	//Call the Dialog
	CImageImportDlg ImportDlg(irSrc,irDest);
	if( ImportDlg.DoModal() == IDOK )
	{
		ICONDIR idtemp;
		ICONDIRENTRY idetemp;
		//CFile f1;
		m_pResData = new BYTE[m_dwSize];
		//f1.Open(irSrc.strFile,CFile::modeRead|CFile::typeBinary);
		fileSrc.Seek(0,CFile::begin);
		fileSrc.Read(&idtemp,sizeof(ICONDIR));
		fileSrc.Read(&idetemp,sizeof(ICONDIRENTRY));
		fileSrc.Read(m_pResData,m_dwSize);
		//f1.Close();
		
		m_pFile->Write(m_dwOffset,0,m_pResData,m_dwSize);
		AfxMessageBox("Resource Updated");
		delete m_pResData;
		m_pResData = NULL;
	}

	//Release Memory
	fileSrc.Close();
	delete pbmpDest;
	delete pdcDest;

}
void PEResIcon::Export(BOOL bFromFile)
{
	//Load the resource 
	m_pResData = new BYTE[m_dwSize];
	m_pFile->Read(m_dwOffset,0,m_pResData,m_dwSize);

	ICONDIR id;
	ICONDIRENTRY ide;
	BITMAPINFOHEADER bih;
	
	//Determine dimensions
	memcpy(&bih,m_pResData,sizeof(BITMAPINFOHEADER));
	if( bih.biSize !=40 || bih.biWidth <=0 || bih.biHeight <=0 )
	{
		char *szErrMsg = new char[1024];
		strcpy(szErrMsg,"Invalid Icon Header");
		throw new PEResParseException(szErrMsg);
	}

	int iColors = (int)pow(2,bih.biBitCount);
	if( iColors < 0 || iColors > 256 )
		iColors = 0;
	
	//Prepare file header
	id.idReserved = 0;
	id.idType = 1;
	id.idCount = 1;
	ide.bWidth = (BYTE)bih.biWidth;
	ide.bHeight = (BYTE)bih.biHeight;
	ide.bColorCount = (BYTE)iColors;
	if(iColors>=256)
		ide.bColorCount = 0;
	ide.bReserved = 0;
	ide.wPlanes = 0;
	ide.wBitCount = 0;
	ide.dwBytesInRes = m_dwSize;
	ide.dwImageOffset = 22;
	
	//Get the saveas name from user
	CFileDialog fd(FALSE,0,"Untitled.ico",
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_PATHMUSTEXIST,
		"Icons (*.ico)|*.ico;||");

	if(fd.DoModal() == IDOK)
	{
		CString strPath = fd.GetPathName();
		CFile fSave;
		fSave.Open(strPath,CFile::modeCreate|CFile::modeWrite|CFile::typeBinary);
		fSave.Write(&id,sizeof(ICONDIR));
		fSave.Write(&ide,sizeof(ICONDIRENTRY));
		fSave.Write(m_pResData,m_dwSize);
		fSave.Close();
	}

	//Release Memory
	delete m_pResData;
	m_pResData = NULL;

}
