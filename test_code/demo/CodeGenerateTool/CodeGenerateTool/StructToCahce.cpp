// StructToCahce.cpp : 实现文件
//

#include "stdafx.h"
#include "CodeGenerateTool.h"
#include "StructToCahce.h"
#include "afxdialogex.h"


// CStructToCahce 对话框

IMPLEMENT_DYNAMIC(CStructToCahce, CDialogEx)

CStructToCahce::CStructToCahce(CWnd* pParent /*=NULL*/)
	: CDialogEx(CStructToCahce::IDD, pParent)
	, m_strInput(_T(""))
	, m_strOutput(_T(""))
{

}

CStructToCahce::~CStructToCahce()
{
}

void CStructToCahce::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_INPUT, m_strInput);
	DDX_Text(pDX, IDC_RICHEDIT_OUTPUT, m_strOutput);
}


BEGIN_MESSAGE_MAP(CStructToCahce, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR, &CStructToCahce::OnBnClickedButtonClear)
	ON_BN_CLICKED(IDC_BUTTON_GENERATE, &CStructToCahce::OnBnClickedButtonGenerate)
END_MESSAGE_MAP()


// CStructToCahce 消息处理程序


void CStructToCahce::OnBnClickedButtonClear()
{
	// TODO:  在此添加控件通知处理程序代码
	m_strInput = _T("");
	UpdateData(FALSE);
}


void CStructToCahce::OnBnClickedButtonGenerate()
{
	// TODO:  在此添加控件通知处理程序代码
	UpdateData(TRUE);
	CString strTemp = m_strInput;
	m_strOutput = GenerateFun(strTemp);
	UpdateData(FALSE);
}


//输入---------------------------------------------->
//调查问卷的基本信息
//struct LXSurveyBasic
//{
//	string						title;								//调查标题
//	string						content;							//调查内容描述	
//	int							question_num;						//问题数量	
//	int							publisher_id;						//发布人Id	
//	long						pub_time;							//发布时间								
//	long						start_time;							//调查开始时间
//	long						expire_time;						//调查过期时间									
//};


//输出=================================================>
//
//调查问卷的基本信息缓存
//class SurveyBasicCache : public CCacheRecBase
//{
//public:
//	SurveyBasicCache(){}
//	static std::string MakeKey(const std::string& survey_id) { return strFormat("SurveyBasicCache_%s", survey_id); }
//	virtual const char* GetName() { return "SurveyBasicCache"; }
//	virtual void OnSerialize(BufferStream& bsBuf) const override;
//	virtual void OnUnSerialize(MemoryStream& msBuf) override;
//
//	void CopyFrom(const LXSurveyBasic& object);
//	void CopyTo(LXSurveyBasic& object) const;
//
//private:
//	LXSurveyBasic m_object;
//};
//
//void SurveyBasicCache::OnSerialize(BufferStream& bsBuf) const
//{
//	bsBuf.WriteString<4>(m_object.title);
//	bsBuf.WriteString<4>(m_object.content);
//	bsBuf.WriteFixed(m_object.question_num);
//	bsBuf.WriteFixed(m_object.publisher_id);
//	bsBuf.WriteFixed(m_object.pub_time);
//	bsBuf.WriteFixed(m_object.start_time);
//	bsBuf.WriteFixed(m_object.expire_time);
//}
//
//void SurveyBasicCache::OnUnSerialize(MemoryStream& msBuf)
//{
//	msBuf.ReadString<4>(m_object.title);
//	msBuf.ReadString<4>(m_object.content);
//	msBuf.ReadFixed(m_object.question_num);
//	msBuf.ReadFixed(m_object.publisher_id);
//	msBuf.ReadFixed(m_object.pub_time);
//	msBuf.ReadFixed(m_object.start_time);
//	msBuf.ReadFixed(m_object.expire_time);
//}
//
//void SurveyBasicCache::CopyFrom(const LXSurveyBasic& object)
//{
//	m_object.title = object.title;
//	m_object.content = object.content;
//	m_object.question_num = object.question_num;
//	m_object.publisher_id = object.publisher_id;
//	m_object.pub_time = object.pub_time;
//	m_object.start_time = object.start_time;
//	m_object.expire_time = object.expire_time;
//}
//void SurveyBasicCache::CopyTo(LXSurveyBasic& object) const
//{
//	object.title = m_object.title;
//	object.content = m_object.content;
//	object.question_num = m_object.question_num;
//	object.publisher_id = m_object.publisher_id;
//	object.pub_time = m_object.pub_time;
//	object.start_time = m_object.start_time;
//	object.expire_time = m_object.expire_time;
//}


CString CStructToCahce::GenerateFun(const CString& str)
{
	//分析struct //1. 去掉注释 2. 提取结构体名字 3. 提取变量类型和变量名字

	m_vecKeyValue.clear();

	//输入---------------------------------------------->
	//调查问卷的基本信息
	//struct LXSurveyBasic
	//{
	//	string						title;								//调查标题
	//	string						content;							//调查内容描述	
	//	int							question_num;						//问题数量	
	//	int							publisher_id;						//发布人Id	
	//	long						pub_time;							//发布时间								
	//	long						start_time;							//调查开始时间
	//	long						expire_time;						//调查过期时间									
	//};

	CString strTemp = str;
	CString strBeginTag = _T("//");
	CString strEndTag = _T("\r\n");
	strTemp = GetString_RemovedBeginAndEnd(strTemp, strBeginTag, strEndTag);
	
	//获取结构体名字
	m_strStructName = GetStringInBeginEnd_WithoutTag(strTemp, _T("struct"), _T("{"));

	//获取变量数组
	CString strKeyArray = GetStringInBeginEnd_WithoutTag(strTemp, _T("{"), _T("}"));
	strKeyArray.Replace(_T("/"), _T(" "));
	strKeyArray.Trim();
	CStringArray arrKeyValue;
	Split(strKeyArray, _T(";"), arrKeyValue);
	for (int index = 0; index < arrKeyValue.GetSize(); index++)
	{
		CString strArrTemp = arrKeyValue.GetAt(index);
		strArrTemp.Replace(_T("\t"), _T(" "));
		strArrTemp.Trim();
		if (strArrTemp.IsEmpty())
		{
			continue;
		}

		KeyValue keyvalue1;
		keyvalue1.strKey = GetStringBeforeStr(strArrTemp, _T(" "));
		keyvalue1.strKey.Trim();	
		keyvalue1.strValue = GetStringAfterStr(strArrTemp, _T(" "));
		keyvalue1.strValue.Trim();	
		m_vecKeyValue.push_back(keyvalue1);
	}
	


	//生成转换后的代码

	m_strCacheName = m_strStructName;
	m_strCacheName.Replace(_T("LX"), _T(""));
	m_strCacheName += _T("Cache");			//SurveyBasicCache
		
	//调查问卷的基本信息缓存
	//class SurveyBasicCache : public CCacheRecBase
	//{
	//public:
	//	SurveyBasicCache(){}
	//	static std::string MakeKey(const std::string& survey_id) { return strFormat("SurveyBasicCache_%s", survey_id); }
	//	virtual const char* GetName() { return "SurveyBasicCache"; }
	//	virtual void OnSerialize(BufferStream& bsBuf) const override;
	//	virtual void OnUnSerialize(MemoryStream& msBuf) override;
	//
	//	void CopyFrom(const LXSurveyBasic& object);
	//	void CopyTo(LXSurveyBasic& object) const;
	//
	//private:
	//	LXSurveyBasic m_object;
	//};

	//生成类定义
	m_strHeader = _T("class ");
	m_strHeader += m_strCacheName;
	m_strHeader += _T(" : public CCacheRecBase\r\n{\r\npublic:\r\n\t");
	m_strHeader += m_strCacheName;
	m_strHeader += _T("(){}\r\n\tstatic std::string MakeKey(const std::string& SSSSS, int IIIII) { return strFormat(\"");
	m_strHeader += m_strCacheName;
	m_strHeader += _T("_%s_%d\", SSSSS, IIIII);}\r\n\t");

	m_strHeader += _T("virtual const char* GetName() { return \"");
	m_strHeader += m_strCacheName;
	m_strHeader += _T("\";}\r\n");
	m_strHeader += _T("\tvirtual void OnSerialize(BufferStream& bsBuf) const override;\r\n");
	m_strHeader += _T("\tvirtual void OnUnSerialize(MemoryStream& msBuf) override;\r\n");
	m_strHeader += _T("\r\n");

	m_strHeader += _T("\tvoid CopyFrom(const ");
	m_strHeader += m_strStructName;				//	LXSurveyBasic
	m_strHeader += _T("& object);\r\n");

	m_strHeader += _T("\tvoid CopyTo(");
	m_strHeader += m_strStructName;				//	LXSurveyBasic
	m_strHeader += _T("& object) const;\r\n");
	m_strHeader += _T("\r\n");
	m_strHeader += _T("public:\r\n");
	m_strHeader += _T("\t");
	m_strHeader += m_strStructName;
	m_strHeader += _T(" m_object;\r\n");
	m_strHeader += _T("};\r\n\r\n\r\n");



	
	

	//void SurveyBasicCache::OnSerialize(BufferStream& bsBuf) const
	//{
	//	bsBuf.WriteString<4>(m_object.title);
	//	bsBuf.WriteString<4>(m_object.content);
	//	bsBuf.WriteFixed(m_object.question_num);
	//	bsBuf.WriteFixed(m_object.publisher_id);
	//	bsBuf.WriteFixed(m_object.pub_time);
	//	bsBuf.WriteFixed(m_object.start_time);
	//	bsBuf.WriteFixed(m_object.expire_time);
	//}
	//
	//void SurveyBasicCache::OnUnSerialize(MemoryStream& msBuf)
	//{
	//	msBuf.ReadString<4>(m_object.title);
	//	msBuf.ReadString<4>(m_object.content);
	//	msBuf.ReadFixed(m_object.question_num);
	//	msBuf.ReadFixed(m_object.publisher_id);
	//	msBuf.ReadFixed(m_object.pub_time);
	//	msBuf.ReadFixed(m_object.start_time);
	//	msBuf.ReadFixed(m_object.expire_time);
	//}
	//
	//void SurveyBasicCache::CopyFrom(const LXSurveyBasic& object)
	//{
	//	m_object.title = object.title;
	//	m_object.content = object.content;
	//	m_object.question_num = object.question_num;
	//	m_object.publisher_id = object.publisher_id;
	//	m_object.pub_time = object.pub_time;
	//	m_object.start_time = object.start_time;
	//	m_object.expire_time = object.expire_time;
	//}
	//void SurveyBasicCache::CopyTo(LXSurveyBasic& object) const
	//{
	//	object.title = m_object.title;
	//	object.content = m_object.content;
	//	object.question_num = m_object.question_num;
	//	object.publisher_id = m_object.publisher_id;
	//	object.pub_time = m_object.pub_time;
	//	object.start_time = m_object.start_time;
	//	object.expire_time = m_object.expire_time;
	//}

	//生成类实现

	
	m_strCPP = _T("void ");
	m_strCPP += m_strCacheName;
	m_strCPP += _T("::OnSerialize(BufferStream& bsBuf) const\r\n{\r\n");
	for (unsigned int index = 0; index < m_vecKeyValue.size(); index++)
	{
		KeyValue keyvaue = m_vecKeyValue[index];
		if (keyvaue.strKey == _T("string"))
		{
			m_strCPP += _T("\tbsBuf.WriteString<4>(m_object.");
			m_strCPP += keyvaue.strValue;
			m_strCPP += _T(");\r\n");
		}
		else
		{
			m_strCPP += _T("\tbsBuf.WriteFixed(m_object.");
			m_strCPP += keyvaue.strValue;
			m_strCPP += _T(");\r\n");
		}
	}
	m_strCPP += _T("}\r\n\r\n");
		
	m_strCPP += _T("void ");
	m_strCPP += m_strCacheName;
	m_strCPP += _T("::OnUnSerialize(MemoryStream& msBuf)\r\n{\r\n");
	for (unsigned int index = 0; index < m_vecKeyValue.size(); index++)
	{
		KeyValue keyvaue = m_vecKeyValue[index];
		if (keyvaue.strKey == _T("string"))
		{
			m_strCPP += _T("\tmsBuf.ReadString<4>(m_object.");
			m_strCPP += keyvaue.strValue;
			m_strCPP += _T(");\r\n");
		}
		else
		{
			m_strCPP += _T("\tmsBuf.ReadFixed(m_object.");
			m_strCPP += keyvaue.strValue;
			m_strCPP += _T(");\r\n");
		}
	}
	m_strCPP += _T("}\r\n\r\n");

	
	m_strCPP += _T("void ");
	m_strCPP += m_strCacheName;
	m_strCPP += _T("::CopyFrom(const ");
	m_strCPP += m_strStructName;
	m_strCPP += _T("& object)\r\n{\r\n");
	for (unsigned int index = 0; index < m_vecKeyValue.size(); index++)
	{
		KeyValue keyvaue = m_vecKeyValue[index];
		m_strCPP += _T("\tm_object.");
		m_strCPP += keyvaue.strValue;
		m_strCPP += _T(" = object.");
		m_strCPP += keyvaue.strValue;
		m_strCPP += _T(";\r\n");
	}
	m_strCPP += _T("}\r\n\r\n");


	m_strCPP += _T("void ");
	m_strCPP += m_strCacheName;
	m_strCPP += _T("::CopyTo(");
	m_strCPP += m_strStructName;
	m_strCPP += _T("& object) const\r\n{\r\n");
	for (unsigned int index = 0; index < m_vecKeyValue.size(); index++)
	{
		KeyValue keyvaue = m_vecKeyValue[index];
		m_strCPP += _T("\tobject.");
		m_strCPP += keyvaue.strValue;
		m_strCPP += _T(" = m_object.");
		m_strCPP += keyvaue.strValue;
		m_strCPP += _T(";\r\n");
	}
	m_strCPP += _T("}\r\n\r\n");


	m_strOutput = m_strHeader + m_strCPP;
	return m_strOutput;
}