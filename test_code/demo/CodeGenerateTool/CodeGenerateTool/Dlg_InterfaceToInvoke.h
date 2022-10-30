#pragma once


// CDlg_InterfaceToInvoke 对话框









class CDlg_InterfaceToInvoke : public CDialogEx
{
	DECLARE_DYNAMIC(CDlg_InterfaceToInvoke)

public:
	CDlg_InterfaceToInvoke(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlg_InterfaceToInvoke();

// 对话框数据
	enum { IDD = IDD_DIALOG_INTERFACE_TO_INVOKE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

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
