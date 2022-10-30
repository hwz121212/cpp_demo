#pragma once


// CDlg_InterfaceToInvoke �Ի���









class CDlg_InterfaceToInvoke : public CDialogEx
{
	DECLARE_DYNAMIC(CDlg_InterfaceToInvoke)

public:
	CDlg_InterfaceToInvoke(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlg_InterfaceToInvoke();

// �Ի�������
	enum { IDD = IDD_DIALOG_INTERFACE_TO_INVOKE };

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
	void ParseString(const CString& str);
	CString GenerateString();


private:
	CStringArray arrNamespace;
	CString m_strInterfaceName;
	CString m_strInterface;

	vector<FunctionStruct> m_vecFun;


	


};
