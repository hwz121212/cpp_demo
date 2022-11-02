/******************************************************************************
*
* PROJECT:		ExeScanner
* FILE NAME:	PETreeList.h
*
* DEVELOPER:	Vishalsinh Jhala
* DATE:			2-Apr-2005
*
* DESCRIPTION:	Declaration file for class PETreeList.
*
* NOTES:		Stores all PE Objects(pointers) in Tree Structure.
*				Primarily used for iterating child objects for each PE Object.
*				Its a Doubly linked list.
*
******************************************************************************/
#pragma once

class PEBase;

typedef struct tagPETreeData{
	tagPETreeData * pPrev;
	int iNo;
	int iLevel;
	char * szDispText;
	PEBase *pPEObj;
	tagPETreeData *pNext;
}PETreeData;

class PETreeList
{
	PETreeData *m_pRoot,*m_pCurrent;//Tree Node
	int m_iNodes; //Total Nodes

	void DeleteAll();
public:
	PETreeList(void);
	~PETreeList(void);
	PETreeData* AddNode(int iLevel,char* szName,PEBase *pObj);
	PETreeData* PETreeList::GetFirstItem();
	PETreeData* PETreeList::GetNextItem(PETreeData *pLast);
	PETreeData* PETreeList::GetItem(int iNode);
	int GetTotalNodes();

};
