#pragma once


// CDlg_CodeToGood �Ի���

class CDlg_CodeToGood : public CDialogEx
{
	DECLARE_DYNAMIC(CDlg_CodeToGood)

public:
	CDlg_CodeToGood(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlg_CodeToGood();

// �Ի�������
	enum { IDD = IDD_DIALOG_CODE_TO_GOOD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CString m_strBadCode;
	CString m_strGoodCode;
	afx_msg void OnBnClickedButtonClear();
	afx_msg void OnBnClickedButtonGenerate();


protected:
	CString GenerateFun(const CString& str);
};
