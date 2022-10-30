#pragma once


// CCreateDlg 对话框

class CCreateDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CCreateDlg)

public:
	CCreateDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CCreateDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_CREATE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_strSQLCreate;
	CString m_strResult;
	afx_msg void OnBnClickedButtonGenerate();
	afx_msg void OnBnClickedButtonClear();


protected:
	CString GenerateFun(const CString& str);
};
