#pragma once


// CDlg_Select �Ի���

class CDlg_Select : public CDialogEx
{
	DECLARE_DYNAMIC(CDlg_Select)

public:
	CDlg_Select(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlg_Select();

// �Ի�������
	enum { IDD = IDD_DIALOG_SELECT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CString m_strInput;
	CString m_strOutput;
	afx_msg void OnBnClickedButtonClear();
	afx_msg void OnBnClickedButtonGenerate();


protected:
	CString GenerateFun(const CString& str);



};
