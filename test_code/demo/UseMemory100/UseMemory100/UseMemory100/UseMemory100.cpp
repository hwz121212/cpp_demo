// UseMemory100.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include <Windows.h>

int _tmain(int argc, _TCHAR* argv[])
{
	while (1)
	{
		new char[1024*1024];
		Sleep(1000);
	}


	return 0;
}

