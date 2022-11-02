// Catch.cpp: implementation of the CCatch class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ClipScreen.h"
#include "Catch.h"
#include "CatchScreenDlg.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

int WINAPI  StartCatchScreen(CWnd *pSnapshotWnd)
{
	
	AFX_MANAGE_STATE(AfxGetStaticModuleState());   
	CCatchScreenDlg dlg(pSnapshotWnd);
	if(dlg.DoModal()==IDOK)
		return 1;
	else
		return 0;
	
}

