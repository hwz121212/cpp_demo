
// MFCApplication2Dlg.h : ͷ�ļ�
//

#pragma once
#include "afxcmn.h"
#include "ccmymodo.h"
#include "threadmod.h"
#include "myduimodo.h"
#include "mywinmodo.h"
#include "afxwin.h"
#include "myCEdit.h"
#include "myclearCE.h"
// CMFCApplication2Dlg �Ի���
class CMFCApplication2Dlg : public CDialogEx
{
// ����
public:
	CMFCApplication2Dlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MFCAPPLICATION2_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CRect rc;
	int nwidth;
	bool EnumProcess();
	CListCtrl cclistctrl;
	ccmymodo* ccmymodo1;
	threadmod* ccmythreadmod;
	myduimodo * ccmyduimodo;
	mywinmodo* ccmywinmodo;
	
	afx_msg void OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnItemchangedList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	//CString myctr;
	//afx_msg void OnEnChangeEdit1();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnEnChangeEdit1();
	afx_msg void OnBnClickedButton4();
	//afx_msg void OnBnClickedButton5();
	CString cpustr;
	CEdit myCpuEdit;
	bool bo = true;
	afx_msg void OnBnClickedButton6();
	afx_msg void OnBnClickedButton7();
    CString ctrpath;
	CString filestr;
	myCEdit myfileCEdit;
	afx_msg void OnEnChangeEdit3();
	afx_msg void OnEnChangeEdit4();
	//afx_msg void OnBnClickedButton5();
	myclearCE myclear;
	//CString mycleactr;
	afx_msg void OnEnChangeEdit2();
	CString myclearctring;
	bool KillProcess(DWORD pid);
	afx_msg void OnBnClickedButton5();
	CString mypidctr;
	afx_msg void OnEnChangeEdit5();
	CString mypidheap;
	afx_msg void OnEnChangeEdit6();
	afx_msg void OnBnClickedButton8();
	afx_msg void OnBnClickedButton9();
	afx_msg void OnBnClickedButton10();
	afx_msg void OnBnClickedButton11();
	afx_msg void OnHotKey(UINT nHotKeyId, UINT nKey1, UINT nKey2);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
