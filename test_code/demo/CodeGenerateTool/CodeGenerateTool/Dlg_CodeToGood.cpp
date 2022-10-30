// Dlg_CodeToGood.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "CodeGenerateTool.h"
#include "Dlg_CodeToGood.h"
#include "afxdialogex.h"


// CDlg_CodeToGood �Ի���

IMPLEMENT_DYNAMIC(CDlg_CodeToGood, CDialogEx)

CDlg_CodeToGood::CDlg_CodeToGood(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlg_CodeToGood::IDD, pParent)
	, m_strBadCode(_T(""))
	, m_strGoodCode(_T(""))
{

}

CDlg_CodeToGood::~CDlg_CodeToGood()
{
}

void CDlg_CodeToGood::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_STRUCT, m_strBadCode);
	DDX_Text(pDX, IDC_RICHEDIT_RESULT, m_strGoodCode);
}


BEGIN_MESSAGE_MAP(CDlg_CodeToGood, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR, &CDlg_CodeToGood::OnBnClickedButtonClear)
	ON_BN_CLICKED(IDC_BUTTON_GENERATE, &CDlg_CodeToGood::OnBnClickedButtonGenerate)
END_MESSAGE_MAP()


// CDlg_CodeToGood ��Ϣ�������


void CDlg_CodeToGood::OnBnClickedButtonClear()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������

	m_strBadCode = _T("");
	UpdateData(FALSE);

}


void CDlg_CodeToGood::OnBnClickedButtonGenerate()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	CString strTemp = m_strBadCode;
	m_strGoodCode = GenerateFun(strTemp);
	UpdateData(FALSE);
}

//�Ƴ�tab���źͶ���Ŀո�
CString CDlg_CodeToGood::GenerateFun(const CString& str)
{

	//���ͷ�Χ
	//struct LXSurveySendRange
	//{
	//	int							sendrange_type;								//�ʾ��ŷ�Χ������(������Ⱥ�顢��Ա)
	//	int							sendrange_id;									//�ʾ��ŷ�Χ��ID(����id��Ⱥ��id����Աid)
	//	int							fliter_type;									//���鷶Χ������������ʦ��ѧ�����ҳ���
	//};

	// �Ȱ��зֽ�
	CString strReturn = str;
	CStringArray  arr;
	Split(strReturn, _T("\r\n"), arr);

	CStringArray  arrResult;
	//�Ƴ��ֽ��Ĵ��е��Ʊ��������Ŀո�
	for (int index = 0; index < arr.GetSize(); index++)
	{
		CString strTemp = arr.GetAt(index);
		strTemp.Trim();
		if (strTemp.IsEmpty())
		{
			continue;
		}
		
		strTemp.Replace(_T("\t"), _T(" "));

		while (strTemp.Find(_T("  ")) != -1)
		{
			strTemp.Replace(_T("  "), _T(" "));
		}

		strTemp.Trim();
		arrResult.Add(strTemp);
	}


	//�������
	strReturn.Empty();
	for (int index = 0; index < arrResult.GetSize(); index++)
	{
		CString strTemp = arrResult.GetAt(index);

		if (index == 0 || index == 1)
		{
			strReturn += strTemp + _T("\r\n");
		}
		else if (index == arrResult.GetSize() - 1 )
		{
			strReturn += strTemp + _T("\r\n");
		}
		else
		{
			strReturn += _T("\t") + strTemp + _T("\r\n");
		}
	}

	return strReturn;
}