// InsertDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "CodeGenerateTool.h"
#include "InsertDlg.h"
#include "afxdialogex.h"


// CInsertDlg 对话框

IMPLEMENT_DYNAMIC(CInsertDlg, CDialogEx)

CInsertDlg::CInsertDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CInsertDlg::IDD, pParent)
	, m_strSQLInsert(_T(""))
	, m_strInsertResult(_T(""))
{

}

CInsertDlg::~CInsertDlg()
{
}

void CInsertDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_INSERT, m_strSQLInsert);
	DDX_Text(pDX, IDC_RICHEDIT2_RESULT, m_strInsertResult);
}


BEGIN_MESSAGE_MAP(CInsertDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_GENERATE, &CInsertDlg::OnBnClickedButtonGenerate)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR, &CInsertDlg::OnBnClickedButtonClear)
END_MESSAGE_MAP()


// CInsertDlg 消息处理程序


void CInsertDlg::OnBnClickedButtonGenerate()
{
	// TODO:  在此添加控件通知处理程序代码
	UpdateData(TRUE);
	CString strTemp = m_strSQLInsert;
	m_strInsertResult = GenerateFun(strTemp);
	UpdateData(FALSE);
}

void CInsertDlg::OnBnClickedButtonClear()
{
	// TODO:  在此添加控件通知处理程序代码

	m_strSQLInsert = _T("");
	UpdateData(FALSE);
}



//
//INSERT INTO[ep_survey].[dbo].[survey]
//([survey_id]
//, [title]
//, [content]
//, [question_num]
//, [pub_time]
//, [publisher_id]
//, [tag_open]
//, [start_time]
//, [expire_time]
//, [family_submit_onetime]
//, [unique_submit_onetime]
//, [send_num]
//, [submit_num]
//, [status]
//, [have_prize]
//, [is_anonymous]
//, [sendrange_type]
//, [sendrange]
//, [publish_fliter])
//VALUES
//(<survey_id, varchar(50), >
//, <title, varchar(500), >
//, <content, varchar, >
//, <question_num, int, >
//, <pub_time, datetime, >
//, <publisher_id, int, >
//, <tag_open, int, >
//, <start_time, datetime, >
//, <expire_time, datetime, >
//, <family_submit_onetime, int, >
//, <unique_submit_onetime, int, >
//, <send_num, int, >
//, <submit_num, int, >
//, <status, int, >
//, <have_prize, int, >
//, <is_anonymous, int, >
//, <sendrange_type, int, >
//, <sendrange, varchar, >
//, <publish_fliter, varchar, >)




//解析Insert SQL语句，生成Insert代码，以及输入参数定义
CString CInsertDlg::GenerateFun(const CString& str)
{
	CString strSource = str;
	CString strSplit = _T(",");
	CString strResult;

	
	CString strKey = _T("VALUES");
	CString strKeyEnd = _T("GO");

	//VALUES之前的变换
	CString strValuesBefore = GetStringBeforeStr(strSource, strKey);
	strValuesBefore.Replace(_T("\r\n"), _T(""));	
	strValuesBefore = RemoveMultiSpace(strValuesBefore);
	strValuesBefore.Replace(_T(", "), _T(","));
	strValuesBefore.Replace(_T(" ,"), _T(","));

	CStringArray arrSplit;
	int iNum = Split(strValuesBefore, strSplit, arrSplit);
	CString strQuestionMark = _T("(");
	//生成这样的结构字符串   (?,?,?,?,?);
	for (int index = 0; index < iNum; index++)
	{
		if (index != iNum - 1)
		{
			strQuestionMark = strQuestionMark + _T("?,");
		}
		else
		{
			strQuestionMark = strQuestionMark + _T("?);");
		}		
	}
	strQuestionMark.Replace(_T(" "), _T(""));
	strValuesBefore = strValuesBefore + strKey + strQuestionMark;

	strValuesBefore = _T("const char* sqli = \"") + strValuesBefore + _T("\";");
	


	//VALUES之后的变换
	CString strValuesAfter = GetStringAfterStr(strSource, strKey);
	strValuesAfter = GetStringBeforeStr(strValuesAfter, strKeyEnd);

	vector<KeyValue> vec_KeyValue;

	CString strBeginTag = _T("<");
	CString strEndTag = _T(">");
	CStringArray arrValues;
	GetStringVecByBeginEndTag(strValuesAfter, strBeginTag, strEndTag, arrValues);
	for (unsigned int iTemp = 0; iTemp < arrValues.GetSize(); iTemp++)
	{
		CString strKeyValue = arrValues.GetAt(iTemp);
		CStringArray strKeyValueArr;
		Split(strKeyValue, _T(","), strKeyValueArr);
		KeyValue keyValueTemp;
		keyValueTemp.strKey = strKeyValueArr.GetAt(0);
		keyValueTemp.strValue = strKeyValueArr.GetAt(1);
		vec_KeyValue.push_back(keyValueTemp);
	}

	strValuesAfter = _T("ODBC_PARAM api[] =\r\n{\r\n");
	for (unsigned int iNum = 0; iNum < vec_KeyValue.size(); iNum++)
	{
		CString strODBCTemp = _T("{ ODBC_IOTYPE_INPUT, ");
		CString strKey = vec_KeyValue[iNum].strKey;
		CString strValue = vec_KeyValue[iNum].strValue;
		CString strType = CDBDataType::GetDataTypeString(strValue);
		strODBCTemp += strType;
		strODBCTemp += _T(", (void*)");

		if (strType != _T("ODBC_DATATYPE_STR") && strType != _T("ODBC_DATATYPE_NSTR") )
		{
			strODBCTemp += _T("&");
		}
		strODBCTemp += strKey;
		strODBCTemp += _T(", 0, 0 },\r\n");
		strValuesAfter += strODBCTemp;
	}

	strValuesAfter = strValuesAfter + _T("{ ODBC_IOTYPE_INPUT, ODBC_DATATYPE_END, NULL, 0, 0 }\r\n};");
	strResult = strValuesBefore + _T("\r\n\r\n") + strValuesAfter;
	return strResult;
}


