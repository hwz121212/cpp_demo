
// ScreenShotTool.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CScreenShotToolApp:
// �йش����ʵ�֣������ ScreenShotTool.cpp
//

class CScreenShotToolApp : public CWinAppEx
{
public:
	CScreenShotToolApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CScreenShotToolApp theApp;