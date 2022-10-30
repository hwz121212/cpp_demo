#pragma once


// CStructToCahce 对话框






class CStructToCahce : public CDialogEx
{
	DECLARE_DYNAMIC(CStructToCahce)
	
public:
	CStructToCahce(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CStructToCahce();

// 对话框数据
	enum { IDD = IDD_DIALOG_STRCUCT_TO_CHCHE };

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


private:
	//输入
	CString m_strStructName;	
	vector<KeyValue>  m_vecKeyValue;

	//输出
	CString m_strCacheName;
	CString m_strHeader;
	CString m_strCPP;
};
