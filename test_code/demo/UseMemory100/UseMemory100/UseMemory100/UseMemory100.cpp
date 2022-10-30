// UseMemory100.cpp : 定义控制台应用程序的入口点。
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

