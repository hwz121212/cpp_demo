// Dlg_InterfaceToInvoke.cpp : 实现文件
//

#include "stdafx.h"
#include "CodeGenerateTool.h"
#include "Dlg_InterfaceToInvoke.h"
#include "afxdialogex.h"


// CDlg_InterfaceToInvoke 对话框

IMPLEMENT_DYNAMIC(CDlg_InterfaceToInvoke, CDialogEx)

CDlg_InterfaceToInvoke::CDlg_InterfaceToInvoke(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlg_InterfaceToInvoke::IDD, pParent)
	, m_strInput(_T(""))
	, m_strOutput(_T(""))
{

}

CDlg_InterfaceToInvoke::~CDlg_InterfaceToInvoke()
{
}

void CDlg_InterfaceToInvoke::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_BAD_CODE, m_strInput);
	DDX_Text(pDX, IDC_RICHEDIT_GOOD_CODE, m_strOutput);
}


BEGIN_MESSAGE_MAP(CDlg_InterfaceToInvoke, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR, &CDlg_InterfaceToInvoke::OnBnClickedButtonClear)
	ON_BN_CLICKED(IDC_BUTTON_GENERATE, &CDlg_InterfaceToInvoke::OnBnClickedButtonGenerate)
END_MESSAGE_MAP()


// CDlg_InterfaceToInvoke 消息处理程序


void CDlg_InterfaceToInvoke::OnBnClickedButtonClear()
{
	// TODO:  在此添加控件通知处理程序代码
	m_strInput = _T("");
	UpdateData(FALSE);
}


void CDlg_InterfaceToInvoke::OnBnClickedButtonGenerate()
{
	// TODO:  在此添加控件通知处理程序代码

	UpdateData(TRUE);
	CString strTemp = m_strInput;
	m_strOutput = GenerateFun(strTemp);
	UpdateData(FALSE);
}

CString CDlg_InterfaceToInvoke::GenerateFun(const CString& str)
{
	CString strReturn = str;

	arrNamespace.RemoveAll();
	m_strInterfaceName = _T("");
	m_strInterface = _T("");
	m_vecFun.clear();

	CString strTemp = str;
	CString strBeginTag = _T("//");
	CString strEndTag = _T("\r\n");
	strTemp = GetString_RemovedBeginAndEnd(strTemp, strBeginTag, strEndTag);
	strTemp.Replace(_T("/"), _T(""));

	strTemp.Replace(_T("[\"amd\"]"), _T(""));
	strTemp.Replace(_T("/"), _T(""));
	strTemp.Replace(_T("LXGrid::"), _T(""));
	strTemp.Replace(_T("Common::"), _T(""));
		

	ParseString(strTemp);
	strReturn = GenerateString();
	return strReturn;
}


//输入----------------------------------

//module LXGrid
//{
//	module App
//	{
//		module Questionnaire
//		{
//			const string IObjectQuestionnaireSurveyName = "IObjectQuestionnaireSurvey";
//			interface IObjectQuestionnaireSurvey extends Common::IObjectBase
//			{
//
//				//获取调查基本信息
//				Common::LXReturn GetSurveyBasic(string survey_id, out LXSurveyBasic survey);
//
//				//获取调查发送限制信息
//				Common::LXReturn GetSurveySendLimit(string survey_id, out LXSurveySendLimit sendlimit);
//
//				//获取调查提交限制信息
//				Common::LXReturn GetSurveySubmitLimit(string survey_id, out LXSurveySubmitLimit submit_limit);
//
//				//获取调查问卷的问题
//				Common::LXReturn GetSurveyQuestions(string survey_id, out LXSurveyQuestionSeq question_seq);
//
//				//获取调查问卷的答案
//				Common::LXReturn GetSurveyAnswers(string survey_id, int submit_userid, out LXSurveyQuestionAnswerSeq answer_seq);
//				
//			};
//		};
//	};
//
//};


//解析关键信息
void CDlg_InterfaceToInvoke::ParseString(const CString& str)
{
	CString strTemp = str;
	//获取名空间
	while (strTemp.Find(_T("module")) != -1)
	{
		CString strNameSpaceTemp = GetStringInBeginEnd_WithoutTag(strTemp, _T("module"), _T("{"));
		strNameSpaceTemp.Trim();
		strTemp = GetStringAfterStr(strTemp, _T("{"));
		arrNamespace.Add(strNameSpaceTemp);
	}

	m_strInterfaceName = GetStringInBeginEnd_WithoutTag(strTemp, _T("string"), _T("="));
	m_strInterfaceName.Trim();
	strTemp = GetStringAfterStr(strTemp, _T("="));
	
	m_strInterface = GetStringInBeginEnd_WithoutTag(strTemp, _T("interface"), _T("extend"));
	m_strInterface.Trim();
	strTemp = GetStringAfterStr(strTemp, _T("interface"));

	CString strFun = GetStringInBeginEnd_WithoutTag(strTemp, _T("{"), _T("}"));
	CStringArray arrFun;
	Split(strFun, _T(";"), arrFun);

	for (int index = 0; index < arrFun.GetSize(); index++)
	{
		CString strTemp1 = arrFun.GetAt(index);

		while (strTemp1.Find(_T("  ")) != -1)
		{
			strTemp1.Replace(_T("  "), _T(" "));
		}
		strTemp1.Trim();
		if (strTemp1.IsEmpty())
		{
			continue;
		}


		FunctionStruct funTemp1;
		ConvertStringToFunStruct(strTemp1, funTemp1);
		m_vecFun.push_back(funTemp1);
	}
}


//输出-------------------------------

//
//class IObjectQuestionnaireSurveyInvoker : public LXInvokerBase
//{
//public:
//
//	//获取调查基本信息
//	LXReturnPtr GetSurveyBasic(int survey_id, LXSurveyBasic& survey)
//	{
//		return Invoke(survey_id, LXInvokerCallBack{
//			IObjectQuestionnaireSurveyPrx proxy = IObjectQuestionnaireSurveyPrx::uncheckedCast(prxBase);
//			return proxy->GetSurveyBasic(survey_id, survey);
//		});
//	}
//
//	//获取调查发送限制信息
//	LXReturnPtr GetSurveySendLimit(int survey_id, LXSurveySendLimit& survey)
//	{
//		return Invoke(survey_id, LXInvokerCallBack{
//			IObjectQuestionnaireSurveyPrx proxy = IObjectQuestionnaireSurveyPrx::uncheckedCast(prxBase);
//			return proxy->GetSurveySendLimit(survey_id, survey);
//		});
//	}
//
//	//获取调查提交限制信息
//	LXReturnPtr GetSurveySubmitLimit(int survey_id, LXSurveySubmitLimit& survey)
//	{
//		return Invoke(survey_id, LXInvokerCallBack{
//			IObjectQuestionnaireSurveyPrx proxy = IObjectQuestionnaireSurveyPrx::uncheckedCast(prxBase);
//			return proxy->GetSurveySubmitLimit(survey_id, survey);
//		});
//	}
//
//	//获取调查问卷的问题
//	LXReturnPtr GetSurveyQuestions(int survey_id, LXSurveyQuestionSeq& question_seq)
//	{
//		return Invoke(survey_id, LXInvokerCallBack{
//			IObjectQuestionnaireSurveyPrx proxy = IObjectQuestionnaireSurveyPrx::uncheckedCast(prxBase);
//			return proxy->GetSurveyQuestions(survey_id, question_seq);
//		});
//	}
//
//	//获取调查问卷的答案
//	LXReturnPtr GetSurveyAnswers(int survey_id, ::Ice::Int submit_userid, LXSurveyQuestionAnswerSeq& answer_seq)
//	{
//		return Invoke(survey_id, LXInvokerCallBack{
//			IObjectQuestionnaireSurveyPrx proxy = IObjectQuestionnaireSurveyPrx::uncheckedCast(prxBase);
//			return proxy->GetSurveyAnswers(survey_id, submit_userid, answer_seq);
//		});
//	}
//
//protected:
//	virtual void GetObjectInfo(std::string& objName, std::string& clusterName, std::string& gridName)
//	{
//		objName = IObjectQuestionnaireSurveyName;
//		clusterName = LXGrid::App::ClusterNameQuestionnaire;
//		gridName = LXGrid::App::ThisGridName;
//	}
//};





CString CDlg_InterfaceToInvoke::GenerateString()
{
	//CStringArray arrNamespace;
	//CString m_strInterfaceName;
	//CString m_strInterface;

	//vector<FunctionStruct> m_vecFun;



	CString strTemp = _T("class ");
	strTemp += m_strInterface;
	strTemp += _T("Invoker : public LXInvokerBase\r\n{public:\r\n\r\n");

	for (int index = 0; index < m_vecFun.size(); index++)
	{
		FunctionStruct funstruct = m_vecFun[index];
		CString strFunTemp = funstruct.strResult;
		strFunTemp += _T(" ");
		strFunTemp += funstruct.strFunName;
		strFunTemp += _T("(");
		for (int iTemp = 0; iTemp < funstruct.vecFunPara.size(); iTemp++)
		{
			FunPara tempAAAAA = funstruct.vecFunPara[iTemp];
			CString  strTemp22 = ConvertFunParaToString(tempAAAAA);
			strFunTemp += strTemp22;

			if (iTemp != funstruct.vecFunPara.size() -1)
			{
				strFunTemp += _T(", ");
			}
		}		
		strFunTemp += _T(")");

		strTemp += _T("\t") + strFunTemp;
		strTemp += _T("\t{\r\n\t\t");
		strTemp += _T("return Invoke(0, LXInvokerCallBack{\r\n");
		strTemp += _T("\t\t\t");
		strTemp += m_strInterface + _T("Prx proxy = ");
		strTemp += m_strInterface + _T("Prx::uncheckedCast(prxBase);\r\n\t\t\t");
		strTemp += _T("return proxy->");
		strTemp += funstruct.strFunName;
		strTemp += _T("(");

		for (int iTemp = 0; iTemp < funstruct.vecFunPara.size(); iTemp++)
		{
			FunPara tempAAAAA = funstruct.vecFunPara[iTemp];
			strTemp += tempAAAAA.strParaName;

			if (iTemp != funstruct.vecFunPara.size() - 1)
			{
				strTemp += _T(", ");
			}
		}
		strTemp += _T(");\r\n\t\t});\r\n\t}\r\n\r\n");
	}
	

	strTemp += _T("protected:\r\n\tvirtual void GetObjectInfo(std::string& objName, std::string& clusterName, std::string& gridName)\r\n");
	strTemp += _T("\t{\r\n\t\tobjName = ");
	strTemp += m_strInterfaceName;
	strTemp += _T(";\r\n\t\tclusterName = ");
	for (int iArrTemp = 0; iArrTemp < arrNamespace.GetSize(); iArrTemp++)
	{
		if (iArrTemp != arrNamespace.GetSize() - 1)
		{
			strTemp += arrNamespace.GetAt(iArrTemp);
			strTemp += _T("::");
		}
		else
		{
			strTemp += _T("ClusterName");
			strTemp += arrNamespace.GetAt(iArrTemp);
		}
	}
	strTemp += _T(";\r\n\t\tgridName = ");

	for (int iArrTemp = 0; iArrTemp < arrNamespace.GetSize(); iArrTemp++)
	{
		if (iArrTemp != arrNamespace.GetSize() - 1)
		{
			strTemp += arrNamespace.GetAt(iArrTemp);
			strTemp += _T("::");
		}
		else
		{
			strTemp += _T("XXXXX");		
		}
	}

	strTemp += _T(";\r\n\t}\r\n};\r\n");

	return strTemp;
}



