#pragma once


// CPointLineDistanceMultiDlg �Ի���

class CPointLineDistanceMultiDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CPointLineDistanceMultiDlg)

public:
	CPointLineDistanceMultiDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CPointLineDistanceMultiDlg();

// �Ի�������
	enum { IDD = IDD_POINTLINEDISTANCE_DIALOG_MULTI };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
