// Dlg_CodeToGood.cpp : 实现文件
//

#include "stdafx.h"
#include "CodeGenerateTool.h"
#include "Dlg_CodeToGood.h"
#include "afxdialogex.h"


// CDlg_CodeToGood 对话框

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


// CDlg_CodeToGood 消息处理程序


void CDlg_CodeToGood::OnBnClickedButtonClear()
{
	// TODO:  在此添加控件通知处理程序代码

	m_strBadCode = _T("");
	UpdateData(FALSE);

}


void CDlg_CodeToGood::OnBnClickedButtonGenerate()
{
	// TODO:  在此添加控件通知处理程序代码
	UpdateData(TRUE);
	CString strTemp = m_strBadCode;
	m_strGoodCode = GenerateFun(strTemp);
	UpdateData(FALSE);
}

//移除tab符号和多余的空格
CString CDlg_CodeToGood::GenerateFun(const CString& str)
{

	//发送范围
	//struct LXSurveySendRange
	//{
	//	int							sendrange_type;								//问卷发放范围的类型(地区、群组、人员)
	//	int							sendrange_id;									//问卷发放范围的ID(地区id、群组id、人员id)
	//	int							fliter_type;									//调查范围过滤条件（老师、学生、家长）
	//};

	// 先按行分解
	CString strReturn = str;
	CStringArray  arr;
	Split(strReturn, _T("\r\n"), arr);

	CStringArray  arrResult;
	//移除分解后的串中的制表符，多余的空格
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


	//按行组合
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