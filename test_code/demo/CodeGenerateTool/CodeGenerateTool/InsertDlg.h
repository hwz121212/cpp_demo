#pragma once


// CInsertDlg �Ի���

class CInsertDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CInsertDlg)

public:
	CInsertDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CInsertDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_INSERT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonGenerate();
	CString m_strSQLInsert;
	CString m_strInsertResult;



protected:
	CString GenerateFun(const CString& str);

public:
	afx_msg void OnBnClickedButtonClear();
};
