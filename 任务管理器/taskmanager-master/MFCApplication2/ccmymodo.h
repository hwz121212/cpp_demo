#pragma once
#include "afxcmn.h"


// ccmymodo �Ի���

class ccmymodo : public CDialogEx
{
	DECLARE_DYNAMIC(ccmymodo)

public:
	ccmymodo(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~ccmymodo();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	bool EnumModule(DWORD dwPID);
	CListCtrl myctrl;
	afx_msg void OnPaint();
};


extern DWORD pid;