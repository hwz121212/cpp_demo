
// MonitorDemo.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CMonitorDemoApp:
// �йش����ʵ�֣������ MonitorDemo.cpp
//

class CMonitorDemoApp : public CWinAppEx
{
public:
	CMonitorDemoApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CMonitorDemoApp theApp;