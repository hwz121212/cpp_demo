#pragma once


// CCreateDlg �Ի���

class CCreateDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CCreateDlg)

public:
	CCreateDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CCreateDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_CREATE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CString m_strSQLCreate;
	CString m_strResult;
	afx_msg void OnBnClickedButtonGenerate();
	afx_msg void OnBnClickedButtonClear();


protected:
	CString GenerateFun(const CString& str);
};
