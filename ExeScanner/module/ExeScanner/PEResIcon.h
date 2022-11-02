/******************************************************************************
*
* PROJECT:		ExeScanner
* FILE NAME:	PEResIcon.h
*
* DEVELOPER:	Vishalsinh Jhala
* DATE:			2-Apr-2005
*
* DESCRIPTION:	Declaration file for class PEResIcon.
*
* NOTES:		Encapsulates Icon Resource. Such resources, when requested
*				by user to be displayed, are shown in View.
*
******************************************************************************/
#pragma once
#include "pebase.h"

class PEResIcon :
	public PEResource
{
	DWORD m_dwOffset;//Physical Offset
	DWORD m_dwSize;//Size in Bytes

public:
	PEResIcon(PEFile *pFile, DWORD dwOffset, DWORD dwSize);

	virtual void IterateElements(PETreeList *pList); 
	virtual void GetPropertyInfo(CString& strInfo);

	virtual const char* GetName();
	virtual void Draw(CDC **pDC,CBitmap **pBmp,CWnd **pWnd = NULL);
	virtual void Import(BOOL bFromFile);
	virtual void Export(BOOL bFromFile);

	~PEResIcon(void);
};
typedef struct
{
    BYTE        bWidth;          // Width, in pixels, of the image
    BYTE        bHeight;         // Height, in pixels, of the image
    BYTE        bColorCount;     // Number of colors in image (0 if >=8bpp)
    BYTE        bReserved;       // Reserved ( must be 0)
    WORD        wPlanes;         // Color Planes
    WORD        wBitCount;       // Bits per pixel
    DWORD       dwBytesInRes;    // How many bytes in this resource?
    DWORD       dwImageOffset;   // Where in the file is this image?
} ICONDIRENTRY, *LPICONDIRENTRY;

typedef struct
{
    WORD           idReserved;   // Reserved (must be 0)
    WORD           idType;       // Resource Type (1 for icons)
    WORD           idCount;      // How many images?
    //ICONDIRENTRY   idEntries[1]; // An entry for each image (idCount of 'em)
} ICONDIR, *LPICONDIR;
