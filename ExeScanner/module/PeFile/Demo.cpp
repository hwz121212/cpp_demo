// Demo.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "CPeFile.h"


int main()
{
	const char* lpszPath = "C:\\test.exe";
	CPeFile PE;
	if (PE.Attach(lpszPath) != IMAGE_NT_SIGNATURE)
		return -1;

	//Dos Header
	const IMAGE_DOS_HEADER* lpDosHeader = PE.GetDosHeader();
	//lpDosHeader->e_xxx


	//NT Header
	const IMAGE_NT_HEADERS32* lpNtHeader32 = PE.GetNtHeader();
	const IMAGE_NT_HEADERS64* lpNtHeader64 = (IMAGE_NT_HEADERS64*)lpNtHeader32;
	BOOL b64Bit = PE.Is64Bit(); //�����64λ��PE��Ҫʹ��lpNtHeader64
	//.......


	//Section Header
	WORD wSectionNum;
	const IMAGE_SECTION_HEADER* lpSectionHeader = PE.GetSectionHeader(&wSectionNum);
	for (WORD i = 0; i < wSectionNum; ++i)
	{
		//lpSectionHeader[i]  
	}


	//�����
	if (!PE.ReadImport())
		return -1;
	DWORD nImport;
	const IMAGE_IMPORT_DESCRIPTOR* lpImport = PE.GetImportDescriptor(&nImport);
	if (lpImport)
	{
		for (DWORD i = 0UL; i < nImport; ++i)
		{
			//lpImport[i]

			DWORD nThunk;
			const IMAGE_THUNK_DATA32* lpThunk = PE.GetImportThunkData(i, &nThunk);
			for (DWORD j = 0UL; j < nThunk; ++j)
			{
				//����Thunk
			}
		}
	}


	//��ַ�ض�λ��
	if (!PE.ReadBaseRelocation())
		return -1;
	DWORD nRelocation;
	const IMAGE_BASE_RELOCATION* const* lpRelocation = PE.GetBaseRelocation(&nRelocation);
	if (lpRelocation)
	{
		for (DWORD i = 0UL; i < nRelocation; ++i)
		{
			//lpRelocation[i]
			DWORD dwAddress = lpRelocation[i]->VirtualAddress;
			DWORD nCount;
			const WORD* lpItem = PE.GetBaseRelocationBlock(lpRelocation[i], &nCount);
			for (DWORD j = 0; j < nCount; ++j)
			{
				//lpItem[j]
			}
		}
	}


	//��������ͬ�����ⲻ�г���
	//��Դ����Ҫһ��һ���ȡ�����忴��ʵ�ִ�����ע��

	return 0;
}

