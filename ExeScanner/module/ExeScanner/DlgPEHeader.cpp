/******************************************************************************
*
* PROJECT:		ExeScanner
* FILE NAME:	DlgPEHeader.cpp
*
* DEVELOPER:	Vishalsinh Jhala
* DATE:			2-Apr-2005
*
* DESCRIPTION:	Defination file for class CChildView.
*
* NOTES:		Displays replica of Dos and Win Header and allows user to
*				view their values.
*
******************************************************************************/

#include "stdafx.h"
#include "ExeScanner.h"
#include "DlgPEHeader.h"
#include "MainFrm.h"


// CDlgPEHeader dialog

IMPLEMENT_DYNAMIC(CDlgPEHeader, CDialog)
CDlgPEHeader::CDlgPEHeader(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgPEHeader::IDD, pParent)
{
}

CDlgPEHeader::~CDlgPEHeader()
{
}

void CDlgPEHeader::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgPEHeader, CDialog)
	ON_COMMAND_RANGE(IDC_BUTTON1,IDC_BUTTON60, OnShowFieldDetails)
END_MESSAGE_MAP()


// CDlgPEHeader message handlers

void CDlgPEHeader::OnShowFieldDetails(UINT nID)
{
	HTREEITEM hti = ((CMainFrame*)AfxGetMainWnd())->m_wndView.m_treeExe.GetRootItem();
	PortableExecutable *ppe = (PortableExecutable*)((CMainFrame*)AfxGetMainWnd())->m_wndView.m_treeExe.GetItemData(hti);

	CString strMsg;
	switch(nID)
	{
	case IDC_BUTTON1:
		strMsg.Format("FIELD: Magic Number\nVALUE: MZ");
		break;
	case IDC_BUTTON2:
		strMsg.Format("FIELD: Bytes on last page of file\nVALUE: %d",ppe->dosHeader.e_cblp);
		break;
	case IDC_BUTTON3:
		strMsg.Format("FIELD: Pages in file\nVALUE: %d",ppe->dosHeader.e_cp);
		break;
	case IDC_BUTTON4:
		strMsg.Format("FIELD: Relocations\nVALUE: %d",ppe->dosHeader.e_crlc);
		break;
	case IDC_BUTTON5:
		strMsg.Format("FIELD: Size of header in paragraphs\nVALUE: %d",ppe->dosHeader.e_cparhdr);
		break;
	case IDC_BUTTON6:
		strMsg.Format("FIELD: Minimum extra paragraphs needed\nVALUE: %d",ppe->dosHeader.e_minalloc);
		break;
	case IDC_BUTTON7:
		strMsg.Format("FIELD: Maximum extra paragraphs needed\nVALUE: %d",ppe->dosHeader.e_maxalloc);
		break;
	case IDC_BUTTON8:
		strMsg.Format("FIELD: Initial (relative) SS value\nVALUE: %d",ppe->dosHeader.e_ss);
		break;
	case IDC_BUTTON9:
		strMsg.Format("FIELD: Initial SP value\nVALUE: %d",ppe->dosHeader.e_sp);
		break;
	case IDC_BUTTON10:
		strMsg.Format("FIELD: Checksum\nVALUE: %d",ppe->dosHeader.e_csum);
		break;
	case IDC_BUTTON11:
		strMsg.Format("FIELD: Initial IP value\nVALUE: %d",ppe->dosHeader.e_ip);
		break;
	case IDC_BUTTON12:
		strMsg.Format("FIELD: Initial (relative) CS value\nVALUE: %d",ppe->dosHeader.e_cs);
		break;
	case IDC_BUTTON13:
		strMsg.Format("FIELD: File address of relocation table\nVALUE: %d",ppe->dosHeader.e_lfarlc);
		break;
	case IDC_BUTTON14:
		strMsg.Format("FIELD: Overlay number\nVALUE: %d",ppe->dosHeader.e_ovno);
		break;
	case IDC_BUTTON15:
		strMsg.Format("FIELD: Reserved words\nVALUE: %d",ppe->dosHeader.e_res[0]);
		break;
	case IDC_BUTTON16:
		strMsg.Format("FIELD: OEM identifier (for e_oeminfo)\nVALUE: %d",ppe->dosHeader.e_oemid);
		break;
	case IDC_BUTTON17:
		strMsg.Format("FIELD: OEM information; e_oemid specific\nVALUE: %d",ppe->dosHeader.e_oeminfo);
		break;
	case IDC_BUTTON18:
		strMsg.Format("FIELD: Reserved words\nVALUE: %d",ppe->dosHeader.e_res2[0]);
		break;
	case IDC_BUTTON19:
		strMsg.Format("FIELD: File address of new exe header\nVALUE: %d",ppe->dosHeader.e_lfanew);
		break;
	//win header
	case IDC_BUTTON20:
		strMsg.Format("FIELD: Signature\nVALUE: PE");
		break;
	case IDC_BUTTON22:
		strMsg.Format("FIELD: Machine\nVALUE: %d",ppe->winHeader.FileHeader.Machine);
		break;
	case IDC_BUTTON23:
		strMsg.Format("FIELD: Number Of Sections\nVALUE: %d",ppe->winHeader.FileHeader.NumberOfSections);
		break;
	case IDC_BUTTON24:
		strMsg.Format("FIELD: TimeDateStamp\nVALUE: %d",ppe->winHeader.FileHeader.TimeDateStamp);
		break;
	case IDC_BUTTON27:
		strMsg.Format("FIELD: Pointer To Symbol Table\nVALUE: %d",ppe->winHeader.FileHeader.PointerToSymbolTable);
		break;
	case IDC_BUTTON28:
		strMsg.Format("FIELD: Number Of Symbols\nVALUE: %d",ppe->winHeader.FileHeader.NumberOfSymbols);
		break;
	case IDC_BUTTON30:
		strMsg.Format("FIELD: Size Of Optional Header\nVALUE: %d",ppe->winHeader.FileHeader.SizeOfOptionalHeader);
		break;
	case IDC_BUTTON31:
		strMsg.Format("FIELD: Characteristics\nVALUE: %d",ppe->winHeader.FileHeader.Characteristics);
		break;
	case IDC_BUTTON32:
		strMsg.Format("FIELD: Magic\nVALUE: %d",ppe->winHeader.OptionalHeader.Magic);
		break;
	case IDC_BUTTON33:
		strMsg.Format("FIELD: Major Linker Version\nVALUE: %d",ppe->winHeader.OptionalHeader.MajorLinkerVersion);
		break;
	case IDC_BUTTON34:
		strMsg.Format("FIELD: Minor Linker Version\nVALUE: %d",ppe->winHeader.OptionalHeader.MinorLinkerVersion);
		break;
	case IDC_BUTTON35:
		strMsg.Format("FIELD: Size Of Code\nVALUE: %d",ppe->winHeader.OptionalHeader.SizeOfCode);
		break;
	case IDC_BUTTON36:
		strMsg.Format("FIELD: Size Of Initialized Data\nVALUE: %d",ppe->winHeader.OptionalHeader.SizeOfInitializedData);
		break;
	case IDC_BUTTON38:
		strMsg.Format("FIELD: Size Of Uninitialized Data\nVALUE: %d",ppe->winHeader.OptionalHeader.SizeOfUninitializedData);
		break;
	case IDC_BUTTON25:
		strMsg.Format("FIELD: Address Of Entry Point\nVALUE: %d",ppe->winHeader.OptionalHeader.AddressOfEntryPoint);
		break;
	case IDC_BUTTON29:
		strMsg.Format("FIELD: Base Of Code\nVALUE: %d",ppe->winHeader.OptionalHeader.BaseOfCode);
		break;
	case IDC_BUTTON26:
		//strMsg.Format("FIELD: Base Of Data\nVALUE: %d",ppe->winHeader.OptionalHeader.BaseOfData);
		break;
	case IDC_BUTTON39:
		strMsg.Format("FIELD: Image Base\nVALUE: %d",ppe->winHeader.OptionalHeader.ImageBase);
		break;
	case IDC_BUTTON40:
		strMsg.Format("FIELD: Section Alignment\nVALUE: %d",ppe->winHeader.OptionalHeader.SectionAlignment);
		break;
	case IDC_BUTTON41:
		strMsg.Format("FIELD: File Alignment\nVALUE: %d",ppe->winHeader.OptionalHeader.FileAlignment);
		break;
	case IDC_BUTTON21:
		strMsg.Format("FIELD: Major Operating System Version\nVALUE: %d",ppe->winHeader.OptionalHeader.MajorOperatingSystemVersion);
		break;
	case IDC_BUTTON44:
		strMsg.Format("FIELD: Minor Operating System Version\nVALUE: %d",ppe->winHeader.OptionalHeader.MinorOperatingSystemVersion);
		break;
	case IDC_BUTTON45:
		strMsg.Format("FIELD: Major Image Version\nVALUE: %d",ppe->winHeader.OptionalHeader.MajorImageVersion);
		break;
	case IDC_BUTTON46:
		strMsg.Format("FIELD: Minor Image Version\nVALUE: %d",ppe->winHeader.OptionalHeader.MinorImageVersion);
		break;
	case IDC_BUTTON47:
		strMsg.Format("FIELD: Major Subsystem Version\nVALUE: %d",ppe->winHeader.OptionalHeader.MajorSubsystemVersion);
		break;
	case IDC_BUTTON48:
		strMsg.Format("FIELD: Minor Subsystem Version\nVALUE: %d",ppe->winHeader.OptionalHeader.MinorSubsystemVersion);
		break;
	case IDC_BUTTON49:
		strMsg.Format("FIELD: Win32 Version Value\nVALUE: %d",ppe->winHeader.OptionalHeader.Win32VersionValue);
		break;
	case IDC_BUTTON50:
		strMsg.Format("FIELD: Size Of Image\nVALUE: %d",ppe->winHeader.OptionalHeader.SizeOfImage);
		break;
	case IDC_BUTTON51:
		strMsg.Format("FIELD: Size Of Headers\nVALUE: %d",ppe->winHeader.OptionalHeader.SizeOfHeaders);
		break;
	case IDC_BUTTON52:
		strMsg.Format("FIELD: CheckSum\nVALUE: %d",ppe->winHeader.OptionalHeader.CheckSum);
		break;
	case IDC_BUTTON53:
		strMsg.Format("FIELD: Subsystem\nVALUE: %d",ppe->winHeader.OptionalHeader.Subsystem);
		break;
	case IDC_BUTTON54:
		strMsg.Format("FIELD: Dll Characteristics\nVALUE: %d",ppe->winHeader.OptionalHeader.DllCharacteristics);
		break;
	case IDC_BUTTON55:
		strMsg.Format("FIELD: Size Of Stack Reserve\nVALUE: %d",ppe->winHeader.OptionalHeader.SizeOfStackReserve);
		break;
	case IDC_BUTTON56:
		strMsg.Format("FIELD: Size Of Stack Commit\nVALUE: %d",ppe->winHeader.OptionalHeader.SizeOfStackCommit);
		break;
	case IDC_BUTTON57:
		strMsg.Format("FIELD: Size Of Heap Reserve\nVALUE: %d",ppe->winHeader.OptionalHeader.SizeOfHeapReserve);
		break;
	case IDC_BUTTON58:
		strMsg.Format("FIELD: Size Of Heap Commit\nVALUE: %d",ppe->winHeader.OptionalHeader.SizeOfHeapCommit);
		break;
	case IDC_BUTTON59:
		strMsg.Format("FIELD: Loader Flags\nVALUE: %d",ppe->winHeader.OptionalHeader.LoaderFlags);
		break;
	case IDC_BUTTON60:
		strMsg.Format("FIELD: Number Of Rva And Sizes\nVALUE: %d",ppe->winHeader.OptionalHeader.NumberOfRvaAndSizes);
		break;
	}
	AfxMessageBox(strMsg);

}
