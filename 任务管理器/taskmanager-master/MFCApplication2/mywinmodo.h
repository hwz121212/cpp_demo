#pragma once
#include "afxcmn.h"


// mywinmodo �Ի���

class mywinmodo : public CDialogEx
{
	DECLARE_DYNAMIC(mywinmodo)

public:
	mywinmodo(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~mywinmodo();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG4 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
	afx_msg void OnPaint();
	
public:
	CRect rc;
	void Enumwin();
	DWORD nwidth;
	CListCtrl mywinctrl;
};
extern HWND pWnd;