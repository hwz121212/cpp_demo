
// CodeGenerateToolDlg.h : ͷ�ļ�
//

#pragma once


// CCodeGenerateToolDlg �Ի���
class CCodeGenerateToolDlg : public CDialogEx
{
// ����
public:
	CCodeGenerateToolDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_CODEGENERATETOOL_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedButtonCreate();
	afx_msg void OnBnClickedButtonInsert();
	afx_msg void OnBnClickedButtonStructToCache();
	afx_msg void OnBnClickedButtonCodeToGood();
	afx_msg void OnBnClickedButtonInterfaceToInvoke();
	afx_msg void OnBnClickedButtonSelect();
};
