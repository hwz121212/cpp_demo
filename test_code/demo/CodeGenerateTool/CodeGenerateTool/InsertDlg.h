#pragma once


// CInsertDlg 对话框

class CInsertDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CInsertDlg)

public:
	CInsertDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CInsertDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_INSERT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

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
