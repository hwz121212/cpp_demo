/******************************************************************************
*
* PROJECT:		ExeScanner
* FILE NAME:	PETreeList.cpp
*
* DEVELOPER:	Vishalsinh Jhala
* DATE:			2-Apr-2005
*
* DESCRIPTION:	Definition file for class PETreeList.
*
* NOTES:		Stores all PE Objects(pointers) in Tree Structure.
*				Primarily used for iterating child objects for each PE Object.
*				Its a Double linked list.
*
******************************************************************************/
#include "StdAfx.h"
#include "petreelist.h"
#include "Exception.h"

PETreeList::PETreeList(void)
	:m_pRoot(NULL),m_pCurrent(NULL), m_iNodes(0)
{
	
}

PETreeList::~PETreeList(void)
{
	DeleteAll();
}
/******************************************************************************
* Function Name: GetTotalNodes
*
* Note: Returns total nodes in the tree.
*
******************************************************************************/
int PETreeList::GetTotalNodes()
{
	return m_iNodes;
}

/******************************************************************************
* Function Name: AddNode
*
* Note: Adds a node at the tail of dubly linked list
*
******************************************************************************/
PETreeData* PETreeList::AddNode(int iLevel,char* szName,PEBase *pObj)
{
	if( !szName || !pObj )
	{
		char *szErrMsg = new char[1024];
		strcpy(szErrMsg,"Cannot create TreeNode with without a valid name or Object");
		throw new PEUIException(szErrMsg);

	}
	PETreeData *pTemp = new PETreeData();
	
	if( !m_iNodes )
	{
		m_pRoot = m_pCurrent = pTemp;
		m_pRoot->pPrev = NULL;
	}
	else
	{
		m_pCurrent->pNext = pTemp;
		pTemp->pPrev = m_pCurrent;
	}

	pTemp->iNo = m_iNodes;
	pTemp->iLevel = iLevel;
	pTemp->szDispText = szName;
	pTemp->pPEObj = pObj;
	pTemp->pNext = NULL;
	
	m_pCurrent = pTemp;
	
	m_iNodes++;
	return m_pCurrent;
}
void PETreeList::DeleteAll()
{
	PETreeData *pNextDel = m_pCurrent;
	for(int i = 0; i < m_iNodes; i++)
	{
		pNextDel = m_pCurrent->pPrev;
		delete m_pCurrent;
		m_pCurrent = pNextDel;
	}
	
}

/******************************************************************************
* Function Name: GetFirstItem
*
* Note: Returns the root.
*
******************************************************************************/
PETreeData* PETreeList::GetFirstItem()
{
	if(m_iNodes)
		return m_pRoot;
	else
		return NULL;
}
PETreeData* PETreeList::GetNextItem(PETreeData *pLast)
{
	if(pLast)
	{
		return pLast->pNext;
	}
	else
		return NULL;
}
PETreeData* PETreeList::GetItem(int iNode)
{
	PETreeData *pTemp;

	if( iNode<0 || iNode>m_iNodes )
		return NULL;
	
	pTemp = m_pRoot;
	for(int i=0;i<=iNode;i++)
	{
		if(pTemp && pTemp->iNo == iNode)
			return pTemp;
		if(pTemp) pTemp = pTemp->pNext;
	}
	return NULL;
}
