
// stdafx.cpp : ֻ������׼�����ļ���Դ�ļ�
// CodeGenerateTool.pch ����ΪԤ����ͷ
// stdafx.obj ������Ԥ����������Ϣ

#include "stdafx.h"






//�Կո����Ĵ�������Ϊkey-value     string survey_id
void ConvertStringToKeyValue(CString strInput, KeyValue& keyValue)
{
	CString strTemp = strInput;
	keyValue.strKey = GetStringBeforeStr(strTemp, _T(" "));
	strTemp = GetStringAfterStr(strTemp, _T(" "));
	keyValue.strValue = strTemp;
	keyValue.strKey.Trim();
	keyValue.strValue.Trim();
}

//out LXSurveyBasic survey
void ConvertStringToFunPara(CString strInput, FunPara& keyValue)
{
	CString strTemp = strInput;
	if (strTemp.Find(_T("out")) != -1)
	{
		keyValue.isOutput = TRUE;
		strTemp.Replace(_T("out"), _T(""));		
	}
	strTemp.Trim();

	keyValue.strParaType = GetStringBeforeStr(strTemp, _T(" "));
	strTemp = GetStringAfterStr(strTemp, _T(" "));
	keyValue.strParaName = strTemp;
	keyValue.strParaType.Trim();
	keyValue.strParaName.Trim();
}


CString ConvertKeyValueToString(const KeyValue& keyValue)
{
	CString strAAAAA = keyValue.strKey;
	strAAAAA += _T(" ");
	strAAAAA += keyValue.strValue;

	return strAAAAA;
}

CString ConvertFunParaToString(const FunPara& keyValue)
{
	CString strKey = keyValue.strParaType;
	CString strAAAAA;
	if (keyValue.isOutput)
	{				
		if (strKey == _T("string"))
		{
			strAAAAA = _T("std::string");
		}
		else 
		{
			strAAAAA = keyValue.strParaType;

			if (strKey.Find(_T("int")) != -1
				|| strKey.Find(_T("string")) != -1
				|| strKey.Find(_T("Seq")) != -1)
			{
				
			}
			else
			{
				strAAAAA += _T("Ptr");
			}
			
		}

		strAAAAA += _T("&");
	}
	else
	{
		if (strKey == _T("string"))
		{
			strAAAAA = _T("const std::string&");
		}
		else if (strKey == _T("int"))
		{
			strAAAAA = _T("int");
		}
		else
		{
			strAAAAA = _T("const ");

			if (strKey.Find(_T("int")) != -1
				|| strKey.Find(_T("string")) != -1
				|| strKey.Find(_T("Seq")) != -1)
			{
				strAAAAA += keyValue.strParaType;
			}
			else
			{
				strAAAAA += keyValue.strParaType;
				strAAAAA += _T("Ptr");
			}
			strAAAAA += _T("&");
		}		
	}
	
	strAAAAA += _T(" ");
	strAAAAA += keyValue.strParaName;

	return strAAAAA;
}

//
//
//typedef struct _FunParaStruct
//{
//	CString strParaType;
//	CString strParaName;
//	BOOL	isOutput;
//	_FunParaStruct()
//	{
//		strParaType = _T("");
//		strParaName = _T("");
//		isOutput = FALSE;
//	}
//}FunParaStruct;
//
//typedef struct _FunctionStruct
//{
//	CString strResult;
//	CString strFunName;
//	vector<FunParaStruct> vecFunPara;
//}FunctionStruct;


////�����ı�׼��ʽ
//				Common::LXReturn GetSurveyBasic(string survey_id, out LXSurveyBasic survey)

void ConvertStringToFunStruct(CString strInput, FunctionStruct& fun)
{
	//���кͷֺ�֮����Ǻ���������
	CString strTemp = strInput;
	strTemp.Trim();

	fun.strResult = GetStringBeforeStr(strTemp, _T(" "));
	strTemp = GetStringAfterStr(strTemp, _T(" "));
	fun.strResult.Trim();

	fun.strResult.Replace(_T("LXReturn"), _T("LXReturnPtr"));

	fun.strFunName = GetStringBeforeStr(strTemp, _T("("));
	strTemp = GetStringAfterStr(strTemp, _T("("));
	fun.strFunName.Trim();

	strTemp = GetStringBeforeStr(strTemp, _T(")"));

	CStringArray arrTemp;
	Split(strTemp, _T(","), arrTemp);

	for (int index = 0; index < arrTemp.GetSize(); index++)
	{
		CString strAAA = arrTemp.GetAt(index);			
		FunPara keyValue;
		ConvertStringToFunPara(strAAA, keyValue);
		
		fun.vecFunPara.push_back(keyValue);
	}
}

