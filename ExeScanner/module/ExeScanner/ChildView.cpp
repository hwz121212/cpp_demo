/******************************************************************************
*
* PROJECT:		ExeScanner
* FILE NAME:	ChildView.cpp
*
* DEVELOPER:	Vishalsinh Jhala
* DATE:			2-Apr-2005
*
* DESCRIPTION:	Defination file for class CChildView.
*
* NOTES:		FrameWnd instanciates this class. It is responsible for 
*				handling all user commands and creating child views like
*				left tree view and right object view.
*
******************************************************************************/

#include "stdafx.h"
#include "ExeScanner.h"
#include "ChildView.h"
#include "DlgPEHeader.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CChildView

CChildView::CChildView()
{
	m_PEFile = NULL;
	m_PE = NULL;	
	m_pTreeList = NULL;
	m_pWndRes = NULL;

	m_pDC = NULL;
	m_pBmp = NULL;
	m_bMemDC = FALSE;
	m_strInfo.Format("-No Selection-\n");
	m_bPEHeaderShow = FALSE;

	m_fntVerdana.CreateFont(
		13,                        // nHeight
		0,                         // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		FW_NORMAL,                 // nWeight
		FALSE,                     // bItalic
		FALSE,                     // bUnderline
		0,                         // cStrikeOut
		ANSI_CHARSET,              // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		DEFAULT_QUALITY,           // nQuality
		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		"Verdana");                 // lpszFacename
	
	//To be used for showing custom toolbar.
	m_imglstRes.Create(MAKEINTRESOURCE(IDB_BITMAP_RESOURCE_LIST),16,11,RGB(236,233,216));

}

CChildView::~CChildView()
{
	DeletePEObjects();
	
	if(m_pDC)
		delete m_pDC;
	if(m_pBmp)
		delete m_pBmp;
	if(m_pWndRes)
		delete m_pWndRes;
}


BEGIN_MESSAGE_MAP(CChildView, CWnd)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_COMMAND(ID_FILE_SCANPE, OnFileScanpe)
	ON_COMMAND(ID_ACTION_IMPORT, OnPortableexecutableImport)
	ON_COMMAND(ID_ACTION_EXPORT, OnPortableexecutableExport)
	ON_COMMAND(ID_ACTION_PROPERTIES, OnPortableexecutableProperties)
	ON_COMMAND(ID_HELP_HELP, OnHelpHelp)
	ON_COMMAND(ID_TOOLS_DOS, OnToolsDos)
END_MESSAGE_MAP()



// CChildView message handlers

BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(NULL, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW+1), NULL);

	return TRUE;
}

/******************************************************************************
* Function Name: DeletePEObjects
*
* Note: This function is called when a user selects new Executable file for scan.
*		Responsible for deleting all PEBase derived objects.
*
******************************************************************************/
void CChildView::DeletePEObjects()
{
	m_bMemDC = FALSE;
	m_bPEHeaderShow = FALSE;

	if(m_pDC)
		delete m_pDC;
	if(m_pBmp)
		delete m_pBmp;
	if(m_pWndRes)
	{
		HWND hwnd = m_pWndRes->Detach();
		::DestroyWindow(hwnd);
		delete m_pWndRes;
	}

	m_pDC = NULL;m_pBmp = NULL;m_pWndRes = NULL;
	
	if(m_pTreeList)
	{
		delete m_pTreeList;
		m_pTreeList = NULL;
	}
	if(m_PE)
	{
		delete m_PE;
		m_PE = NULL;
	}
	if(m_PEFile)
	{
		delete m_PEFile;
		m_PEFile = NULL;
	}

}

void CChildView::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
}

BOOL CChildView::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext)
{
	int iRet =  CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);

	//Create all necessary window like tree view, main view, legend view, tree ctrl.
	if(iRet)
	{
		//Left Tree View
		m_wndTree.Create(AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(NULL, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW+1), NULL),"",
		WS_CHILD|WS_VISIBLE,rect,this,1357);

		//Right Object View
		m_wndView.Create(AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(NULL, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW+1), NULL),"",
		WS_CHILD|WS_VISIBLE,rect,this,1359);

		//Tree Control inside left tree view
		m_treeExe.Create(WS_VISIBLE | WS_TABSTOP | WS_CHILD /*| WS_BORDER*/
			| TVS_HASBUTTONS | TVS_LINESATROOT /*| TVS_HASLINES*/,
			CRect(10, 10, 10, 10), &m_wndTree, 0x1005);
		
		m_treeExe.SetFont(&m_fntVerdana);

		//Semitransparent legend window.
		m_wndLegend.CreateEx(0x80000,"","Legend",
			WS_VISIBLE|WS_POPUP,CRect(0,0,100,100),&m_wndView,9102);
		
	}
	return iRet;
}

void CChildView::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	//Adjust left tree view on the basis of new size
	if(m_wndTree.GetSafeHwnd())
	{
		m_wndTree.MoveWindow(0,0,270,cy);
		m_treeExe.MoveWindow(28,55,270-56,cy-91);
	}

	//Adjust right object view and its childs on the basis of new size
	if(m_wndView.GetSafeHwnd())
	{
		CRect rect;

		m_wndView.MoveWindow(270,0,cx-270,cy);
		m_wndView.GetClientRect(rect);
		m_wndView.ClientToScreen(rect);
		
		m_wndLegend.MoveWindow(rect.right-120,rect.bottom-22,120,20);
		m_wndLegend.m_bMaximize = TRUE;
		m_wndLegend.Animate();
		
		if(m_pWndRes)
		{
			HTREEITEM htiSel = m_treeExe.GetSelectedItem();
			CString strSel = m_treeExe.GetItemText(htiSel);

			//If the selected resource is string or binary the list
			//control needs to be adjusted
			if(strSel.CompareNoCase("STRING")==0 || strSel.CompareNoCase("BINARY")==0)
			{
				CRect rctWndView;
				m_wndView.GetClientRect(&rctWndView);
				m_pWndRes->MoveWindow(28,55,rctWndView.right-28-28,rctWndView.bottom-55-28);
			}
		}

	}
}

/******************************************************************************
* Function Name: OnFileScanpe
*
* Note: Menu command Handler
*
******************************************************************************/
void CChildView::OnFileScanpe()
{
	CString strPath;
	CFileDialog fd(TRUE,0,0,
		OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT|OFN_FILEMUSTEXIST|OFN_PATHMUSTEXIST,
		"PortableExecutables (*.exe;*.dll;*.ocx)|*.exe;*.dll;*.ocx||");
	
	if( fd.DoModal() == IDOK)
	{
		//If user selected a valid executable file lets scan it.
		strPath = fd.GetPathName();
		try
		{
			//If previously selected file is open then delete its objects
			DeletePEObjects();

			//Create fresh ones for newly selected file
			m_PEFile = new PEFile(strPath);
			m_PE = new PortableExecutable(m_PEFile);
			m_pTreeList = new PETreeList();

			//enumerate the objects of PE and get them under the tree.
			PopulateTreeCtrl();
			
			m_wndView.Invalidate(TRUE);

			m_wndLegend.m_bMaximize = TRUE;
			m_wndLegend.Animate();
		}
		catch(PEException *e)
		{
			e->Report();
			DeletePEObjects();
			delete e;
		}
	}

}
/******************************************************************************
* Function Name: PopulateTreeCtrl
*
* Note: Enumerates the PE Objects and adds them to tree control. 
*
******************************************************************************/
void CChildView::PopulateTreeCtrl()
{
	HTREEITEM treeItem[15];
		
	//The root PE object is PortableExecutable. It then creates child objects
	//like ResourceSection etc. Each Object in PE knows its childs.
	//On calling the IterateElements of one root object automatically ensures
	//that the function for its child objects is called and that all the elements
	//are populated in the tree.
    m_PE->IterateElements(m_pTreeList);
	PETreeData *peData = m_pTreeList->GetFirstItem();
	
	m_treeExe.DeleteAllItems();
	m_treeExe.SetImageList(&m_imglstRes,TVSIL_NORMAL);

	//Transfer the data from PETreeList to Tree Control
	while(peData)
	{
		if(!peData->iLevel)
		{
			treeItem[peData->iLevel] = m_treeExe.InsertItem(peData->szDispText,TVI_ROOT,TVI_LAST);
			m_treeExe.SetItemImage(treeItem[peData->iLevel],10,10);
		}
		else
			treeItem[peData->iLevel] = m_treeExe.InsertItem(peData->szDispText,treeItem[peData->iLevel-1],TVI_LAST);
		
		m_treeExe.SetItemData(treeItem[peData->iLevel],(DWORD_PTR)peData->pPEObj);
		
		SetImageFromList(treeItem[peData->iLevel],peData->szDispText);

		peData = m_pTreeList->GetNextItem(peData);
	}

}
/******************************************************************************
* Function Name: SetImageFromList
*
* Note: Depending on the type of resource, we set the icon image for them.
*
******************************************************************************/
void CChildView::SetImageFromList(HTREEITEM hItem,char *pszName)
{
	if( !strcmp(pszName,"ResourceSection") )
	{
		m_treeExe.SetItemImage(hItem,0,0);
	}
	else if( !strcmp(pszName,"CURSOR") )
	{
		m_treeExe.SetItemImage(hItem,9,9);
	}
	else if( !strcmp(pszName,"BITMAP"))
	{
		m_treeExe.SetItemImage(hItem,2,2);
	}
	else if( !strcmp(pszName,"ICON"))
	{
		m_treeExe.SetItemImage(hItem,4,4);
	}
	else if( !strcmp(pszName,"MENU"))
	{
		m_treeExe.SetItemImage(hItem,5,5);
	}
	else if( !strcmp(pszName,"DIALOG"))
	{
		m_treeExe.SetItemImage(hItem,3,3);
	}
	else if( !strcmp(pszName,"STRING"))
	{
		m_treeExe.SetItemImage(hItem,7,7);
	}
	else if( !strcmp(pszName,"ACCELERATORS"))
	{
		m_treeExe.SetItemImage(hItem,1,1);
	}
	else if( !strcmp(pszName,"VERSION"))
	{
		m_treeExe.SetItemImage(hItem,8,8);
	}
	else if( !strcmp(pszName,"Binary"))
	{
		m_treeExe.SetItemImage(hItem,6,6);
	}
}
/******************************************************************************
* Function Name: OnSelection
*
* Note: Command Handler, when user changes selection on tree control
*
******************************************************************************/
void CChildView::OnSelection(LPNMTREEVIEW pTV)
{
	CWaitCursor wc;
	
	//Get the object of newly selected item
	PEBase *pB = (PEBase *)m_treeExe.GetItemData(pTV->itemNew.hItem);
	PEResource *pR = dynamic_cast<PEResource*>(pB);

	//Get Property info so that legend window could be shown to user 
	//as and when requested.
	pB->GetPropertyInfo(m_wndLegend.strInfo);

	Load(pR);

	m_wndView.Invalidate(TRUE);
	m_wndLegend.m_bMaximize = TRUE;
	m_wndLegend.Animate();
}
/******************************************************************************
* Function Name: OnPortableexecutableExport
*
* Note: Menu command handler
*
******************************************************************************/
void CChildView::OnPortableexecutableExport()
{
	CWaitCursor wc;
	HTREEITEM h = m_treeExe.GetSelectedItem();
	if(!h)
		return;

	PEBase *pB = (PEBase *)m_treeExe.GetItemData(h);
	PEResource *pR = dynamic_cast<PEResource*>(pB);
	if(pR)
	{
		pR->Export(TRUE);
	}
}
/******************************************************************************
* Function Name: Load
*
* Note: Loads the resource from PE file
*
******************************************************************************/
void CChildView::Load(PEResource *pR)
{
	//Delete DC and window objects of previous resource.
	m_bMemDC = FALSE;
	m_bPEHeaderShow = FALSE;

	if(m_pDC)
		delete m_pDC;
	if(m_pBmp)
		delete m_pBmp;
	if(m_pWndRes)
	{
		HWND hwnd = m_pWndRes->Detach();
		::DestroyWindow(hwnd);
		delete m_pWndRes;
	}

	m_pDC = NULL;m_pBmp = NULL;m_pWndRes = NULL;

	//Create New ones if its the leaf node.
	if(pR)
	{

		try
		{
			//This function ensures that Draw fn. of the 
			//selected object is called and that dc or window is
			//created and returned.
			pR->Draw(&m_pDC,&m_pBmp,&m_pWndRes);
		}
		catch(PEException *e)
		{
			e->Report();
			delete e;
		}

		if(m_pDC)
		{
			BITMAP bmp;
			m_pBmp->GetBitmap(&bmp);
			m_iWidth = bmp.bmWidth;
			m_iHeight = bmp.bmHeight;

			m_bMemDC = TRUE;
		}
		else if(m_pWndRes)
		{

			//Depend on the type of window returned place it properly 
			if(strcmp(pR->GetName(),"STRING")==0)
			{
				CRect rctWndView;
				m_wndView.GetClientRect(&rctWndView);
				m_pWndRes->MoveWindow(28,55,rctWndView.right-28-28,rctWndView.bottom-55-28);
			}
			else if(strcmp(pR->GetName(),"BINARY")==0)
			{
				CRect rctWndView;
				m_wndView.GetClientRect(&rctWndView);
				m_pWndRes->MoveWindow(28,55,rctWndView.right-28-28,rctWndView.bottom-55-28);
			}
			else
			{

				CRect rect,rect1;
				m_wndView.GetClientRect(&rect);
				ClientToScreen(&rect);

				m_pWndRes->GetClientRect(&rect1);
				m_pWndRes->MoveWindow(rect.left+300+20,rect.top+50,rect1.right,rect1.bottom+50);
				m_pWndRes->ShowWindow(SW_SHOW);
			}

		}
	}

}
/******************************************************************************
* Function Name: OnPortableexecutableImport
*
* Note: Menu command Handler
*
******************************************************************************/
void CChildView::OnPortableexecutableImport()
{
	HTREEITEM h = m_treeExe.GetSelectedItem();
	if(!h)
		return;
	PEBase *pB = (PEBase *)m_treeExe.GetItemData(h);
	PEResource *pR = dynamic_cast<PEResource*>(pB);
	if(pR)
	{
		try
		{
			pR->Import(TRUE);

			Load(pR);

			m_wndView.Invalidate(TRUE);
			m_wndLegend.m_bMaximize = TRUE;
			m_wndLegend.Animate();
		}
		catch(PEException *e)
		{
			e->Report();
			delete e;
		}
	}
	
}

/******************************************************************************
* Function Name: OnPortableexecutableProperties
*
* Note: Menu command Handler
*
******************************************************************************/
void CChildView::OnPortableexecutableProperties()
{
	m_wndLegend.Animate();
}

/******************************************************************************
* Function Name: OnHelpHelp
*
* Note: Menu command Handler
*
******************************************************************************/
void CChildView::OnHelpHelp()
{
	AfxMessageBox("Refer ExeScanner.doc for HELP.");
	int iRet = system("winword ExeScanner.doc");
	
}

/******************************************************************************
* Function Name: OnMoveWnd
*
* Note: When the application is dragged, Legend window needs to be adjusted
*		accordingly. But the message is received in CMainFrame. So it calls
*		this function in case of move.
*
******************************************************************************/
void CChildView::OnMoveWnd()
{
	CRect rect;
	if(m_wndView.GetSafeHwnd())
	{
		m_wndView.GetClientRect(rect);
		m_wndView.ClientToScreen(rect);

		m_wndLegend.MoveWindow(rect.right-120,rect.bottom-22,120,20);
		m_wndLegend.m_bMaximize = TRUE;
		m_wndLegend.Animate();
	}
}

/******************************************************************************
* Function Name: OnToolsDos
*
* Note: Menu command handler
*
******************************************************************************/
void CChildView::OnToolsDos()
{
	if(!m_PE)
		return;

	CDlgPEHeader dlgHdr;
	dlgHdr.DoModal();
}
