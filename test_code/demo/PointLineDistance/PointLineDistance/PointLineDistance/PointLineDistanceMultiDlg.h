#pragma once


// CPointLineDistanceMultiDlg 对话框

class CPointLineDistanceMultiDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CPointLineDistanceMultiDlg)

public:
	CPointLineDistanceMultiDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPointLineDistanceMultiDlg();

// 对话框数据
	enum { IDD = IDD_POINTLINEDISTANCE_DIALOG_MULTI };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
