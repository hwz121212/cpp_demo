// CreateDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "CodeGenerateTool.h"
#include "CreateDlg.h"
#include "afxdialogex.h"


// CCreateDlg 对话框

IMPLEMENT_DYNAMIC(CCreateDlg, CDialogEx)

CCreateDlg::CCreateDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCreateDlg::IDD, pParent)
	, m_strSQLCreate(_T(""))
	, m_strResult(_T(""))
{

}

CCreateDlg::~CCreateDlg()
{
}

void CCreateDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_CREATE, m_strSQLCreate);
	DDX_Text(pDX, IDC_RICHEDIT2_RESULT, m_strResult);
}


BEGIN_MESSAGE_MAP(CCreateDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_GENERATE, &CCreateDlg::OnBnClickedButtonGenerate)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR, &CCreateDlg::OnBnClickedButtonClear)
END_MESSAGE_MAP()


// CCreateDlg 消息处理程序


void CCreateDlg::OnBnClickedButtonGenerate()
{
	// TODO:  在此添加控件通知处理程序代码
	UpdateData(TRUE);

	CString strTemp = m_strSQLCreate;
	m_strResult = GenerateFun(strTemp);
	UpdateData(FALSE);
}


void CCreateDlg::OnBnClickedButtonClear()
{
	// TODO:  在此添加控件通知处理程序代码
	m_strSQLCreate = _T("");
	UpdateData(FALSE);
}



//CREATE TABLE[dbo].[survey](
//	[survey_id][varchar](50) NOT NULL,
//	[title][varchar](500) NOT NULL,
//	[content][varchar](max) NOT NULL,
//	[question_num][int] NOT NULL,
//	[pub_time][datetime] NOT NULL,
//	[publisher_id][int] NOT NULL,
//	[tag_open][int] NOT NULL,
//	[start_time][datetime] NOT NULL,
//	[expire_time][datetime] NOT NULL,
//	[family_submit_onetime][int] NOT NULL,
//	[unique_submit_onetime][int] NOT NULL,
//	[send_num][int] NULL,
//	[submit_num][int] NULL,
//	[status][int] NOT NULL,
//	[have_prize][int] NOT NULL,
//	[is_anonymous][int] NOT NULL,
//	[sendrange_type][int] NOT NULL,
//	[sendrange][varchar](max) NOT NULL,
//	[publish_fliter][varchar](max) NULL,
//	CONSTRAINT[PK_survey_main] PRIMARY KEY CLUSTERED


CString CCreateDlg::GenerateFun(const CString& str)
{
	CString strResult;




	return strResult;
}



