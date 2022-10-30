#include "stdafx.h"
#include "StringUtil.h"





/*---------------------------------------------------------
  * 函数介绍： 从原字符串里按照指定的分隔字符串进行分割,将分隔的结果存放到字符串数组里
   
    * 输入参数：
	 source -- 原字符串
	  ch -- 指定的分隔字符串
	   strarr -- 外部引用的一个字符串数组
	    
		   * 输出参数：
		     
			   * 返回值 ：总共分隔了多少段字符串.
			     
				  -----------------------------------------------------------*/
int Split(CString source, CString ch, CStringArray &strarr)
{

	CString TmpStr;
	strarr.RemoveAll();
	if (source.IsEmpty() || ch.IsEmpty())
		return 0;
	int len = ch.GetLength();
	int findi = 0;
	int findn = 0;
	int sum = 0;


	findn = source.Find(ch, findi);
	if (findn != -1)
	{
		TmpStr = source.Mid(0, findn);
		//TmpStr.Trim();
		strarr.Add(TmpStr);
		findi = findn + len;
		sum++;

	}
	else
	{
		//source.Trim();
		strarr.Add(source);
		sum++;
		return sum;

	}


	while (findn != -1)//有发现
	{

		findn = source.Find(ch, findi);
		if (findn != -1)
		{
			TmpStr = source.Mid(findi, findn - findi);
			//TmpStr.Trim();//去除头尾空格
			strarr.Add(TmpStr);

			findi = findn + len;
			sum++;
		}
		else
		{
			TmpStr = source.Mid(findi, source.GetLength() - findi);
			//TmpStr.Trim();
			strarr.Add(TmpStr);

			sum++;

		}
	}


	return sum;
}

CString GetStringAfterStr(CString strSource, CString strAfter)
{
	CString strReturn = strSource;
	int iPos = strSource.Find(strAfter);
	if (iPos >= 0)
	{
		strReturn = strReturn.Right(strReturn.GetLength() - iPos - strAfter.GetLength());
		return strReturn;
	}

	return _T("");	
}

CString GetStringBeforeStr(CString strSource, CString strKey)
{
	CString strReturn = strSource;
	int iPos = strSource.Find(strKey);
	if (iPos >= 0)
	{
		strReturn = strReturn.Left(iPos);
		return strReturn;
	}

	return _T("");	
}

CString GetStringInBeginEnd_WithoutTag(CString strSource, CString strBeginTag, CString strEndTag)
{
	CString strTemp = strSource;	
	CString strAfter = GetStringAfterStr(strTemp, strBeginTag);
	CString strBefore = GetStringBeforeStr(strAfter, strEndTag);
	strBefore.Trim();
	return strBefore;
}

CString GetString_RemovedBeginAndEnd(CString strInput, CString strBeginTag, CString strEndTag)
{
	CString strTemp = strInput;

	CStringArray arrNeedDelete;
	GetStringVecByBeginEndTag(strTemp, strBeginTag, strEndTag, arrNeedDelete);

	for (int index = 0; index < arrNeedDelete.GetSize(); index++)
	{
		CString strArrTemp = arrNeedDelete.GetAt(index);
		strTemp.Replace(strArrTemp, _T(""));
	}

	return strTemp;
}

void GetStringVecByBeginEndTag(CString strSource, CString strBeginTag, CString strEndTag, CStringArray &strArr)
{
	CString strTemp = strSource;
	
	while (strTemp.GetLength() != 0)
	{
		CString strAfter = GetStringAfterStr(strTemp, strBeginTag);
		CString strBefore = GetStringBeforeStr(strAfter, strEndTag);
		if (strBefore.GetLength() != 0)
		{
			strArr.Add(strBefore);
		}
		strTemp = GetStringAfterStr(strAfter, strEndTag);
	}
}

CString RemoveMultiSpace(CString source)
{
	CString strReturn = source;
	while (strReturn.Find(_T("  ")) != -1)
	{
		strReturn.Replace(_T("  "), _T(" "));
	}
	return strReturn;
}