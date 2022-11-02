/******************************************************************************
*
* PROJECT:		ExeScanner
* FILE NAME:	PortableExecutable.cpp
*
* DEVELOPER:	Vishalsinh Jhala
* DATE:			2-Apr-2005
*
* DESCRIPTION:	Definition file for class PortableExecutable.
*
* NOTES:		Starting point to enter a PE. Reads and verifies DOS/WIN 
*				headers. Direct Child Objects are ResourceSections. When the 
*				program will deal will all sections ResourceSection will 
*				become a generic class PESection. A PESection with ".rsrc"
*				will be ResourceSection.
*
******************************************************************************/
#include "StdAfx.h"
#include "portableexecutable.h"

#ifdef _DEBUG
	extern CFile g_File;
	extern CString g_strDbg;
#endif

PortableExecutable::PortableExecutable(PEFile *pFile)
{
	//Is file valid?
	if( !pFile )
	{
		char *szErrMsg = new char[1024];
		strcpy(szErrMsg,"Cannot Proceed with NULL File");
		throw new PEFileException(szErrMsg);
	}

	m_pRSRC = NULL;
	m_pSectionHeader = NULL;

	m_pFile = pFile;
	
	//Whats my name?
	int iPos = m_pFile->GetFileName().ReverseFind('\\');
	CString strN = m_pFile->GetFileName().Mid(iPos+1);
	strcpy(m_pszMyName,strN.GetBuffer(1024));
		
	//Read and verify DOS header, the first thing in PE.
	m_pFile->Read(0,0,(BYTE*)&dosHeader,sizeof(IMAGE_DOS_HEADER));
	if(dosHeader.e_magic != 0x5A4D)
	{
		char* szErrMsg = new char[1024];
		strcpy(szErrMsg,"Not a valid Executable File.(Invalid DOS Header)"); 
		throw new PEFormatException(szErrMsg);
	}
	
	//Read and verify WINDOWS header.
	m_pFile->Read(0,dosHeader.e_lfanew,(BYTE*)&winHeader,sizeof(IMAGE_NT_HEADERS));
	if(winHeader.Signature != 0x4550)
	{
		char* szErrMsg = new char[1024];
		strcpy(szErrMsg,"Not a valid Executable File.(Invalid WIN Header)"); 
		throw new PEFormatException(szErrMsg);
	}

	//allocate memory for sections as we know how many they are
	m_pSectionHeader = new IMAGE_SECTION_HEADER[winHeader.FileHeader.NumberOfSections];
	
	//Load all the section(Headers).
	for(int i = 0; i < winHeader.FileHeader.NumberOfSections; i++)
	{
		m_pFile->Read(0, 
			dosHeader.e_lfanew + sizeof(IMAGE_NT_HEADERS)+ i*sizeof(IMAGE_SECTION_HEADER),
			(BYTE*)&m_pSectionHeader[i],
			sizeof(IMAGE_SECTION_HEADER));
			
			#ifdef _DEBUG
			g_strDbg.Format("%d %s VirtSize: %08X  VirtAddr:  %08X\r\n"
				"raw data offs:   %08X  raw data size: %08X\r\n"
				"relocation offs: %08X  relocations:   %08X\r\n"
				"line # offs:     %08X  line #'s:      %08X\r\n"
				"characteristics: %08X\r\n\r\n", i+1,m_pSectionHeader[i].Name, m_pSectionHeader[i].Misc.VirtualSize,m_pSectionHeader[i].VirtualAddress,
				m_pSectionHeader[i].PointerToRawData,m_pSectionHeader[i].SizeOfRawData,
				m_pSectionHeader[i].PointerToRelocations,m_pSectionHeader[i].NumberOfRelocations,
				m_pSectionHeader[i].PointerToLinenumbers,m_pSectionHeader[i].PointerToLinenumbers,
				m_pSectionHeader[i].Characteristics);

			g_File.Write(g_strDbg.GetBuffer(1024),g_strDbg.GetLength());
			#endif

		//If its .rsrc, its the section of our interest.
		if( !strcmp((char*)m_pSectionHeader[i].Name,".rsrc") )
		{
			m_pRSRC = new ResourceSection(m_pFile,m_pSectionHeader[i].PointerToRawData,m_pSectionHeader[i].VirtualAddress);
		}
	}
	
}

PortableExecutable::~PortableExecutable(void)
{
	if(m_pSectionHeader)
		delete [] m_pSectionHeader;
	if(m_pRSRC)
		delete m_pRSRC;
}
void PortableExecutable::GetPropertyInfo(CString& strInfo)
{
	CString strSec,strTemp;
	CFileStatus fs;
	
	CString strFileName(m_pFile->GetFileName());
	CFile::GetStatus(strFileName,fs);
	
	CString strMachine,strChars,strDllChars,strSubsys;
	
	GetMachine(strMachine);
	GetSubSystem(strSubsys);
	GetDllChars(strDllChars);
	GetCharacteristics(strChars);

	strInfo.Format("FILE NAME\n%s\n"
		"FILE SIZE\n%I64d\n"
		"DOS CHECKSUM\n%hd\n"
		"MACHINE\n%s\n"
		"SECTIONS#\n%d\n"
		"TIMESTAMP\n%d\n"
		"CHARACTERISTICS\n%s \n"
		"LINKER-VERSION\n%d:%d\n"
		"CODE SIZE\n%d\n"
		"OS VERSION\n%d:%d\n"
		"IMAGE VERSION\n%d:%d\n"
		"SUBSYSTEM\n%s\n"
		"DLL CHARACTERISTICS\n%s\n",
		m_pFile->GetFileName(),
		fs.m_size,
		dosHeader.e_csum,
		strMachine,
		winHeader.FileHeader.NumberOfSections,
		winHeader.FileHeader.TimeDateStamp,
		strChars,
		winHeader.OptionalHeader.MajorLinkerVersion,winHeader.OptionalHeader.MinorLinkerVersion,
		winHeader.OptionalHeader.SizeOfCode,
		winHeader.OptionalHeader.MajorOperatingSystemVersion,winHeader.OptionalHeader.MinorOperatingSystemVersion,
		winHeader.OptionalHeader.MajorImageVersion,winHeader.OptionalHeader.MinorImageVersion,
		strSubsys,
		strDllChars
		);

	for(int i = 0; i < winHeader.FileHeader.NumberOfSections; i++)
	{
		strSec.Format("SECTION: %s\n%d\n",m_pSectionHeader[i].Name,m_pSectionHeader[i].SizeOfRawData);
		strTemp.Format("%s%s",strInfo,strSec);
		strInfo.Format("%s",strTemp);
	}

}
void PortableExecutable::IterateElements(PETreeList *pList)
{
	if( !pList)
	{
		char *szErrMsg = new char[1024];
		strcpy(szErrMsg,"Tree Control Object does not exist");
		throw new PEUIException(szErrMsg);
	}
	pList->AddNode(0,m_pszMyName,this);
	
	if(m_pRSRC)
		m_pRSRC->IterateElements(pList);
}
void PortableExecutable::GetMachine(CString &str)
{
	switch(winHeader.FileHeader.Machine)
	{
	case IMAGE_FILE_MACHINE_I386:
		str.Format("%s","Intel "); 
	case IMAGE_FILE_MACHINE_IA64:
		str.Format("%s","64-bit Windows: Intel ");
	case IMAGE_FILE_MACHINE_AMD64:
		str.Format("%s","64-bit Windows: AMD ");
	case IMAGE_FILE_MACHINE_ALPHA:
		str.Format("%s","Windows NT 4.0 and earlier: DEC Alpha ");
	case IMAGE_FILE_MACHINE_POWERPC:
		str.Format("%s","Windows NT 4.0 and earlier: Power PC ");
	default:
		str.Format("%s","Unknown");
	}
}
void PortableExecutable::GetCharacteristics(CString &str)
{
	CString strTemp,strChar("");
	if(winHeader.FileHeader.Characteristics & IMAGE_FILE_RELOCS_STRIPPED)
	{
		strTemp.Format("%sRelocation information is stripped from the file.\n \n",strChar);
		strChar.Format("%s",strTemp);
	}
	if(winHeader.FileHeader.Characteristics & IMAGE_FILE_EXECUTABLE_IMAGE)
	{
		strTemp.Format("%sThe file is executable (there are no unresolved external references). \n \n",strChar);
		strChar.Format("%s",strTemp);
	}
	if(winHeader.FileHeader.Characteristics & IMAGE_FILE_LINE_NUMS_STRIPPED)
	{
		strTemp.Format("%sLine numbers are stripped from the file. \n \n",strChar);
		strChar.Format("%s",strTemp);
	}
	if(winHeader.FileHeader.Characteristics & IMAGE_FILE_LOCAL_SYMS_STRIPPED)
	{
		strTemp.Format("%sLocal symbols are stripped from file. \n \n",strChar);
		strChar.Format("%s",strTemp);
	}
	if(winHeader.FileHeader.Characteristics & IMAGE_FILE_AGGRESIVE_WS_TRIM )
	{
		strTemp.Format("%sAggressively trim the working set. \n \n",strChar);
		strChar.Format("%s",strTemp);
	}
	if(winHeader.FileHeader.Characteristics & IMAGE_FILE_LARGE_ADDRESS_AWARE )
	{
		strTemp.Format("%sThe application can handle addresses larger than 2 GB. \n \n",strChar);
		strChar.Format("%s",strTemp);
	}
	if(winHeader.FileHeader.Characteristics & IMAGE_FILE_BYTES_REVERSED_LO)
	{
		strTemp.Format("%sBytes of the word are reversed. \n \n",strChar);
		strChar.Format("%s",strTemp);
	}
	if(winHeader.FileHeader.Characteristics & IMAGE_FILE_32BIT_MACHINE )
	{
		strTemp.Format("%sComputer supports 32-bit words. \n \n",strChar);
		strChar.Format("%s",strTemp);
	}
	if(winHeader.FileHeader.Characteristics & IMAGE_FILE_DEBUG_STRIPPED )
	{
		strTemp.Format("%sDebugging information is stored separately in a .dbg file. \n \n",strChar);
		strChar.Format("%s",strTemp);
	}
	if(winHeader.FileHeader.Characteristics & IMAGE_FILE_REMOVABLE_RUN_FROM_SWAP)
	{
		strTemp.Format("%sIf the image is on removable media, copy and run from the swap file. \n \n",strChar);
		strChar.Format("%s",strTemp);
	}
	if(winHeader.FileHeader.Characteristics & IMAGE_FILE_NET_RUN_FROM_SWAP)
	{
		strTemp.Format("%sIf the image is on the network, copy and run from the swap file. \n \n",strChar);
		strChar.Format("%s",strTemp);
	}
	if(winHeader.FileHeader.Characteristics & IMAGE_FILE_SYSTEM )
	{
		strTemp.Format("%sSystem file. \n \n",strChar);
		strChar.Format("%s",strTemp);
	}
	if(winHeader.FileHeader.Characteristics & IMAGE_FILE_DLL)
	{
		strTemp.Format("%sDLL file. \n \n",strChar);
		strChar.Format("%s",strTemp);
	}
	if(winHeader.FileHeader.Characteristics & IMAGE_FILE_UP_SYSTEM_ONLY )
	{
		strTemp.Format("%sFile should be run only on a uniprocessor computer. \n \n",strChar);
		strChar.Format("%s",strTemp);
	}
	if(winHeader.FileHeader.Characteristics & IMAGE_FILE_BYTES_REVERSED_HI)
	{
		strTemp.Format("%sBytes of word are reversed. \n \n",strChar);
		strChar.Format("%s",strTemp);
	}
	str.Format("%s",strChar);
}
void PortableExecutable::GetSubSystem(CString &str)
{
	switch(winHeader.OptionalHeader.Subsystem)
	{
	case IMAGE_SUBSYSTEM_UNKNOWN:
		str.Format("%s","Unknown subsystem. ");
	case IMAGE_SUBSYSTEM_NATIVE:
		str.Format("%s","No subsystem required. ");
	case IMAGE_SUBSYSTEM_WINDOWS_CUI:
		str.Format("%s","Windows character-mode subsystem. ");
	case IMAGE_SUBSYSTEM_POSIX_CUI:
		str.Format("%s","POSIX character-mode subsystem. ");
	case IMAGE_SUBSYSTEM_WINDOWS_CE_GUI:
		str.Format("%s","Windows CE. ");
	default:
		str.Format("%s","Unknown subsystem. ");
	}
}
void PortableExecutable::GetDllChars(CString &str)
{
	if(winHeader.OptionalHeader.DllCharacteristics ==0x2000)
		str.Format("%s","A WDM driver. ");
	else
		str.Format("%s","-NONE-");
}