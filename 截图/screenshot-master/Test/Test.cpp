// Test.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include <Windows.h>

int _tmain(int argc, _TCHAR* argv[])
{
	HINSTANCE hInst = LoadLibrary(_T("Screenshot.dll"));
	if (hInst)
	{
		int(*p)(LPCTSTR) = (int (*)(LPCTSTR))GetProcAddress(hInst, "StartScreenshot");
		p(L"123");
	}
	return 0;
}

