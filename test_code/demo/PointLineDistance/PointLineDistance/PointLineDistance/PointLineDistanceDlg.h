
// PointLineDistanceDlg.h : ͷ�ļ�
//

#pragma once


// CPointLineDistanceDlg �Ի���
class CPointLineDistanceDlg : public CDialogEx
{
// ����
public:
	CPointLineDistanceDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_POINTLINEDISTANCE_DIALOG };

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
	LONGLONG m_x1;
	LONGLONG m_x2;
	LONGLONG m_x3;
	LONGLONG m_y1;
	LONGLONG m_y2;
	LONGLONG m_y3;
	afx_msg void OnBnClickedButton1();
	CString m_strLine;
	CString m_strA;
	CString m_strB;
	CString m_strC;


	LONGLONG m_lA;
	LONGLONG m_lB;
	LONGLONG m_lC;
	CString m_strDistance;
	double m_dDistance;
	double m_dRes;
	afx_msg void OnBnClickedButton2();
};
