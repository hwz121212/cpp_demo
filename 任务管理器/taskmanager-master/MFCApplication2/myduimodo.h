#pragma once
#include "afxcmn.h"


// myduimodo �Ի���

class myduimodo : public CDialogEx
{
	DECLARE_DYNAMIC(myduimodo)

public:
	myduimodo(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~myduimodo();
	DWORD pid;
	bool Enumheap(DWORD PID);

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG3 };
#endif

protected:
	
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
	afx_msg void OnPaint();
public:
	CListCtrl myduictrl;
};
