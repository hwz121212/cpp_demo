#pragma once



int Split(CString source, CString ch, CStringArray &strarr);
CString GetStringAfterStr(CString strSource, CString strAfter);
CString GetStringBeforeStr(CString strSource, CString strKey);

CString GetStringInBeginEnd_WithoutTag(CString strSource, CString strBeginTag, CString strEndTag);

CString GetString_RemovedBeginAndEnd(CString strInput, CString strBeginTag, CString strEndTag);	//�Ƴ��ַ��������еĿ�ʼ��ǩ�ͽ�����ǩ֮�䣨������ǩ�����ı�


void GetStringVecByBeginEndTag(CString strSource, CString strBeginTag, CString strEndTag, CStringArray &strArr);

CString RemoveMultiSpace(CString source);