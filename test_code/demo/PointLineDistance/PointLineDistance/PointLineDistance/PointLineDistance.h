
// PointLineDistance.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CPointLineDistanceApp: 
// �йش����ʵ�֣������ PointLineDistance.cpp
//

class CPointLineDistanceApp : public CWinApp
{
public:
	CPointLineDistanceApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CPointLineDistanceApp theApp;