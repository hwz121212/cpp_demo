#pragma once


// CStructToCahce �Ի���






class CStructToCahce : public CDialogEx
{
	DECLARE_DYNAMIC(CStructToCahce)
	
public:
	CStructToCahce(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CStructToCahce();

// �Ի�������
	enum { IDD = IDD_DIALOG_STRCUCT_TO_CHCHE };

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


private:
	//����
	CString m_strStructName;	
	vector<KeyValue>  m_vecKeyValue;

	//���
	CString m_strCacheName;
	CString m_strHeader;
	CString m_strCPP;
};
