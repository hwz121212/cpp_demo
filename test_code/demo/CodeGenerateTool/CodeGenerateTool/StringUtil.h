#pragma once



int Split(CString source, CString ch, CStringArray &strarr);
CString GetStringAfterStr(CString strSource, CString strAfter);
CString GetStringBeforeStr(CString strSource, CString strKey);

CString GetStringInBeginEnd_WithoutTag(CString strSource, CString strBeginTag, CString strEndTag);

CString GetString_RemovedBeginAndEnd(CString strInput, CString strBeginTag, CString strEndTag);	//移除字符串中所有的开始标签和结束标签之间（包含标签）的文本


void GetStringVecByBeginEndTag(CString strSource, CString strBeginTag, CString strEndTag, CStringArray &strArr);

CString RemoveMultiSpace(CString source);