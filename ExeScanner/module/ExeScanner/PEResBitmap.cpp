/******************************************************************************
*
* PROJECT:		ExeScanner
* FILE NAME:	PEResBitmap.cpp
*
* DEVELOPER:	Vishalsinh Jhala
* DATE:			2-Apr-2005
*
* DESCRIPTION:	Defination file for class PEResBitmap.
*
* NOTES:		Encapsulates Bitmap Resource. Such resources, when requested
*				by user to be displayed, are shown in View.
*
******************************************************************************/
#include "StdAfx.h"
#include "peresbitmap.h"
#include "ImageImportDlg.h"

PEResBitmap::PEResBitmap(PEFile *pFile, DWORD dwOffset, DWORD dwSize)
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
	strcpy(m_pszMyName,"BITMAP");
}

PEResBitmap::~PEResBitmap(void)
{

}
const char* PEResBitmap::GetName()
{
	return (const char*)m_pszMyName;
}

void PEResBitmap::GetPropertyInfo(CString& strInfo)
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
		"Bitmap Image",
		m_dwOffset,
		m_dwSize,
		bih.biWidth,
		bih.biHeight,
		bih.biBitCount,
		iColors		
		);

}
void PEResBitmap::IterateElements(PETreeList *pList)
{

}
void PEResBitmap::Draw(CDC **pDC,CBitmap **pBmp,CWnd **pWnd)
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
		strcpy(szErrMsg,"Invalid Bitmap Header");
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
	(*pBmp)->CreateCompatibleBitmap(CDC::FromHandle(::GetDC(NULL)),bih.biWidth,bih.biHeight);
	(*pDC)->SelectObject(*pBmp);

	//Convert data loaded from file to bitmap and copy bitmap bits to DC.
	int iRet = SetDIBitsToDevice((*pDC)->GetSafeHdc(),0,0,bih.biWidth,bih.biHeight,0,0,0,bih.biHeight,m_pResData+sizeof(BITMAPINFOHEADER)+iColors*4,bi,DIB_RGB_COLORS);
	if(iRet == 0)
	{
		int iErr = GetLastError();
		char *szErrMsg = new char[1024];
		strcpy(szErrMsg,"Error while copying Bitmap to memory:");
		itoa(iErr,szErrMsg+37,10);
		throw new PEResParseException(szErrMsg);
	}

	//Release Memory
	delete m_pResData;
	m_pResData = NULL;
	delete bi;
}
void PEResBitmap::Import(BOOL bFromFile)
{
	IMAGE_RESOURCE irSrc,irDest;
	
	//Get the file to be imported
	CFileDialog dlgOpen(TRUE,0,0,
		OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT|OFN_FILEMUSTEXIST|OFN_PATHMUSTEXIST,
		"Bitmaps (*.bmp)|*.bmp||");
	
	if(dlgOpen.DoModal() == IDOK)
	{
		irSrc.strFile = dlgOpen.GetPathName();
	}
	else
		return;
	
	CFileStatus fs;
	CFile::GetStatus(irSrc.strFile,fs);

	HANDLE handle = NULL;

	//Load the file
	handle = ::LoadImage(::AfxGetInstanceHandle(),
		irSrc.strFile,IMAGE_BITMAP,0,0,LR_LOADFROMFILE|LR_DEFAULTCOLOR);
	if(handle == NULL)
	{
		char *szErrMsg = new char[27];
		strcpy(szErrMsg,"Error while Loading bitmap");
		throw new PEResParseException(szErrMsg);
	}

	CBitmap bmpSrc,*pbmpDest;
	CDC dcSrc,*pdcDest;
	BITMAP bitmap;

	//Prepare the source bitmap loaded from file.
	bmpSrc.Attach(handle);
	bmpSrc.GetBitmap(&bitmap);
	
	dcSrc.CreateCompatibleDC(CDC::FromHandle(::GetDC(NULL)));
	dcSrc.SelectObject(&bmpSrc);
	
	BITMAPINFOHEADER bih;
	BITMAPFILEHEADER bfh;

	//Get the info headers of source file
	CFile fileSrc;
	
	fileSrc.Open(irSrc.strFile,CFile::modeRead|CFile::typeBinary);
	fileSrc.Read(&bfh,sizeof(BITMAPFILEHEADER));
	fileSrc.Read(&bih,sizeof(BITMAPINFOHEADER));
	

	//Prepare image resource structure.
	irSrc.iWidth = bitmap.bmWidth;
	irSrc.iHeight = bitmap.bmHeight;
	irSrc.iBPP = bih.biBitCount;
	irSrc.iSize = (int)(fs.m_size - sizeof(BITMAPFILEHEADER));
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
	irDest.iWidth = bitmap.bmWidth;
	irDest.iHeight = bitmap.bmHeight;
	irDest.iBPP = bih.biBitCount;
	irDest.iSize = m_dwSize;
	irDest.pDC = pdcDest;

	//Call the Dialog
	CImageImportDlg ImportDlg(irSrc,irDest);
	if( ImportDlg.DoModal() == IDOK )
	{
		//CFile f1;
		m_pResData = new BYTE[m_dwSize];
		//f1.Open(irSrc.strFile,CFile::modeRead|CFile::typeBinary);
		
		fileSrc.Seek(0,CFile::begin);
		fileSrc.Read(&bfh,sizeof(BITMAPFILEHEADER));
		fileSrc.Read(m_pResData,m_dwSize);
		
		//f1.Close();

		m_pFile->Write(m_dwOffset,0,m_pResData,m_dwSize);
		
		AfxMessageBox("Resource Updated");
		delete m_pResData;
		m_pResData = NULL;
	}
	fileSrc.Close();

	delete pbmpDest;
	delete pdcDest;
	//DeleteObject(bmpSrc.Detach());

}
void PEResBitmap::Export(BOOL bFromFile)
{
	//Load the resource 
	m_pResData = new BYTE[m_dwSize];
	m_pFile->Read(m_dwOffset,0,m_pResData,m_dwSize);

	BITMAPFILEHEADER bfh;
	BITMAPINFOHEADER bih;
	
	//Determine dimensions
	memcpy(&bih,m_pResData,sizeof(BITMAPINFOHEADER));
	if( bih.biSize !=40 || bih.biWidth <=0 || bih.biHeight <=0 )
	{
		char *szErrMsg = new char[1024];
		strcpy(szErrMsg,"Invalid Bitmap Header");
		throw new PEResParseException(szErrMsg);
	}

	int iColors = (int)pow(2,bih.biBitCount);
	if( iColors < 0 || iColors > 256 )
		iColors = 0;

	//Prepare file header
	ZeroMemory(&bfh,sizeof(BITMAPFILEHEADER));
	bfh.bfType = 0x4D42;
	bfh.bfSize = m_dwSize + sizeof(BITMAPFILEHEADER);
	bfh.bfOffBits = sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)+iColors*4;
	
	//Get the saveas name from user
	CFileDialog fd(FALSE,0,"Untitled.bmp",
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_PATHMUSTEXIST,
		"Bitmaps (*.bmp)|*.bmp;||");

	if(fd.DoModal() == IDOK)
	{
		CString strPath = fd.GetPathName();
		if(strPath.Right(4).CompareNoCase(".bmp") != 0)
			strPath.Append(".bmp");

		CFile fSave;
		
		fSave.Open(strPath,CFile::modeCreate|CFile::modeWrite|CFile::typeBinary);
		fSave.Write(&bfh,sizeof(BITMAPFILEHEADER));
		fSave.Write(m_pResData,m_dwSize);
		
		fSave.Close();
	}
	
	//Release Memory
	delete m_pResData;
	m_pResData = NULL;

}
