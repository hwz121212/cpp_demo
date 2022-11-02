#pragma once

#include "define.h"


// CCMAgnifyDlg �Ի���

class CCMagnifyDlg : public CDialog
{
	DECLARE_DYNAMIC(CCMagnifyDlg)

public:
	CCMagnifyDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CCMagnifyDlg();

// �Ի�������
	enum { IDD = IDD_DLG_MAGNIFY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
    virtual BOOL OnInitDialog();
    afx_msg void OnClose();
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    afx_msg void OnPaint();
    virtual BOOL PreTranslateMessage(MSG* pMsg);

    int             m_nDestWidth;                   // ����Ļ���ȡ�
    int             m_nDestHeight;                  // ����Ļ�߶ȡ�

    CDC             m_DeskTopDC;                    // ������DC��
    CBitmap         m_bmpDestTop;                   // ���ڴ��г�����ʱͼ���λͼ��
    
    CRect           m_rect;

    void InitScreen();              // ����ʼ���Ŵ󾵶Ի���dc��

    void DrawMagnify(CDC& dc);      // �����Ŵ󾵡�

    void DrawBorder(CDC *pDC);      // �����߿�
};
