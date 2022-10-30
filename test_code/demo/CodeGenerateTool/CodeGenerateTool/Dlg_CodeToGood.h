#pragma once


// CDlg_CodeToGood 对话框

class CDlg_CodeToGood : public CDialogEx
{
	DECLARE_DYNAMIC(CDlg_CodeToGood)

public:
	CDlg_CodeToGood(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlg_CodeToGood();

// 对话框数据
	enum { IDD = IDD_DIALOG_CODE_TO_GOOD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_strBadCode;
	CString m_strGoodCode;
	afx_msg void OnBnClickedButtonClear();
	afx_msg void OnBnClickedButtonGenerate();


protected:
	CString GenerateFun(const CString& str);
};
