// Dlg_Select.cpp : 实现文件
//

#include "stdafx.h"
#include "CodeGenerateTool.h"
#include "Dlg_Select.h"
#include "afxdialogex.h"


// CDlg_Select 对话框

IMPLEMENT_DYNAMIC(CDlg_Select, CDialogEx)

CDlg_Select::CDlg_Select(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlg_Select::IDD, pParent)
	, m_strInput(_T(""))
	, m_strOutput(_T(""))
{

}

CDlg_Select::~CDlg_Select()
{
}

void CDlg_Select::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_INPUT, m_strInput);
	DDX_Text(pDX, IDC_RICHEDIT_OUTPUT, m_strOutput);
}


BEGIN_MESSAGE_MAP(CDlg_Select, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR, &CDlg_Select::OnBnClickedButtonClear)
	ON_BN_CLICKED(IDC_BUTTON_GENERATE, &CDlg_Select::OnBnClickedButtonGenerate)
END_MESSAGE_MAP()


// CDlg_Select 消息处理程序


void CDlg_Select::OnBnClickedButtonClear()
{
	// TODO:  在此添加控件通知处理程序代码
	m_strInput = _T("");
	UpdateData(FALSE);
}


void CDlg_Select::OnBnClickedButtonGenerate()
{
	UpdateData(TRUE);
	CString strTemp = m_strInput;
	m_strOutput = GenerateFun(strTemp);
	UpdateData(FALSE);
}


//
//SELECT[task_id]
//, [alarm_type]
//, [notice_type]
//, [host_target]
//, [notice_target]
//, [start_time]
//, [end_time]
//, [in_time]
//FROM[ep_monitor].[dbo].[alarm_task]
//GO

CString CDlg_Select::GenerateFun(const CString& str)
{	
	CString strSource = str;	
	CString strKeyEnd = _T("GO");
	CString strResult = GetStringBeforeStr(strSource, strKeyEnd);
	strResult.Replace(_T("\r\n"), _T(""));
	strResult = RemoveMultiSpace(strResult);
	strResult.Replace(_T(", "), _T(","));
	strResult.Replace(_T(" ,"), _T(","));
	strResult.Trim();

	strResult = _T("const char* sqls = \"") + strResult + _T(" WHERE [xxxxx]=?;\";");


	return strResult;
}