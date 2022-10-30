#include "stdafx.h"
#include "common.h"








//, ODBC_DATATYPE_SBYTE						// 有符号单字节整数
//, ODBC_DATATYPE_BYTE						// 无符号单字节整数
//, ODBC_DATATYPE_SWORD						// 有符号双字节整数
//, ODBC_DATATYPE_WORD						// 无符号双字节整数
//, ODBC_DATATYPE_INT						// 有符号四字节整数
//, ODBC_DATATYPE_UINT						// 无符号四字节整数
//, ODBC_DATATYPE_STR						// 字符串
//, ODBC_DATATYPE_LONGSTR					// 字符串(大值数据)
//, ODBC_DATATYPE_BINARY					// 二进制数据
//, ODBC_DATATYPE_LONGBINARY				// 二进制数据(大值数据)
//, ODBC_DATATYPE_LONGLONG					// 有符号八字节整数
//, ODBC_DATATYPE_ULONGLONG					// 无符号八字节整数
//, ODBC_DATATYPE_FLOAT						// 四字节浮点数
//, ODBC_DATATYPE_DOUBLE					// 八字节浮点数
//// 注意：SQLServer仅支持毫秒时间精度，因此fraction的有效范围为(1000000-999000000纳秒)，即1-999毫秒
//, ODBC_DATATYPE_DATETIME					// 日期时间，对应C数据类型为SQL_TIMESTAMP_STRUCT							
//, ODBC_DATATYPE_NSTR						// 字符串(unicode)
//, ODBC_DATATYPE_NLONGSTR					// 字符串(unicode大值数据)
//, ODBC_DATATYPE_GUID						// 16字节GUID，对应C数据类型为长度16的无符号字节数组
//
//


CDBDataType::CDBDataType()
{
	
}

CDBDataType::~CDBDataType()
{

}

CString CDBDataType::GetDataTypeString(CString str)
{
	//ODBC_DATATYPE_END						// 标记结束
	CString strTemp = str;
	strTemp.MakeLower();

	if (strTemp.Find(_T("nvarchar")) != -1)return _T("ODBC_DATATYPE_NSTR");							   // 字符串(unicode)	
	if (strTemp.Find(_T("bit")) != -1)return _T("ODBC_DATATYPE_BOOL");								// 布尔值(BIT)
	if (strTemp.Find(_T("int")) != -1)return _T("ODBC_DATATYPE_INT");							   // 有符号四字节整数
	if (strTemp.Find(_T("varchar")) != -1)return _T("ODBC_DATATYPE_STR");							   // 字符串	
	if (strTemp.Find(_T("float")) != -1)return _T("ODBC_DATATYPE_FLOAT");							   // 四字节浮点数
	if (strTemp.Find(_T("real")) != -1)return _T("ODBC_DATATYPE_DOUBLE");						   // 八字节浮点数
	if (strTemp.Find(_T("datetime")) != -1)return _T("ODBC_DATATYPE_DATETIME");					   // 日期时间，对应C数据类型为SQL_TIMESTAMP_STRUCT			
	
	return _T("");
}


//if (strTemp.Find(_T("bit")) != -1)return _T("ODBC_DATATYPE_BOOL");								// 布尔值(BIT)
////if (strTemp.Find(_T("char")) != -1)return _T("ODBC_DATATYPE_SBYTE");							   // 有符号单字节整数
////if (strTemp.Find(_T("char")) != -1)return _T("ODBC_DATATYPE_BYTE");							   // 无符号单字节整数
////if (strTemp.Find(_T("smallint")) != -1)return _T("ODBC_DATATYPE_SWORD");							   // 有符号双字节整数
////if (strTemp.Find(_T("smallint")) != -1)return _T("ODBC_DATATYPE_WORD");							   // 无符号双字节整数
//if (strTemp.Find(_T("int")) != -1)return _T("ODBC_DATATYPE_INT");							   // 有符号四字节整数
////if (strTemp.Find(_T("int")) != -1)return _T("ODBC_DATATYPE_UINT");							   // 无符号四字节整数
//if (strTemp.Find(_T("varchar")) != -1)return _T("ODBC_DATATYPE_STR");							   // 字符串
////if (strTemp.Find(_T("bit")) != -1)return _T("ODBC_DATATYPE_LONGSTR");						   // 字符串(大值数据)
////if (strTemp.Find(_T("bit")) != -1)return _T("ODBC_DATATYPE_BINARY");						   // 二进制数据
////if (strTemp.Find(_T("bit")) != -1)return _T("ODBC_DATATYPE_LONGBINARY");					   // 二进制数据(大值数据)
////if (strTemp.Find(_T("bit")) != -1)return _T("ODBC_DATATYPE_LONGLONG");						   // 有符号八字节整数
////if (strTemp.Find(_T("bit")) != -1)return _T("ODBC_DATATYPE_ULONGLONG");						   // 无符号八字节整数
//if (strTemp.Find(_T("float")) != -1)return _T("ODBC_DATATYPE_FLOAT");							   // 四字节浮点数
//if (strTemp.Find(_T("real")) != -1)return _T("ODBC_DATATYPE_DOUBLE");						   // 八字节浮点数
//if (strTemp.Find(_T("datetime")) != -1)return _T("ODBC_DATATYPE_DATETIME");					   // 日期时间，对应C数据类型为SQL_TIMESTAMP_STRUCT			
//if (strTemp.Find(_T("nvarchar")) != -1)return _T("ODBC_DATATYPE_NSTR");							   // 字符串(unicode)
////if (strTemp.Find(_T("bit")) != -1)return _T("ODBC_DATATYPE_NLONGSTR");						   // 字符串(unicode大值数据)
////if (strTemp.Find(_T("bit")) != -1)return _T("ODBC_DATATYPE_GUID");							   // 16字节GUID，对应C数据类型为长度16的无符号字节数组


//[1] [bigint] NULL,
//[2][binary](50) NULL,
//[3][bit] NULL,
//[4][char](10) NULL,
//[5][date] NULL,
//[6][datetime] NULL,
//[7][datetime2](7) NULL,
//[8][datetimeoffset](7) NULL,
//[9][decimal](18, 0) NULL,
//[10][float] NULL,
//[11][geography] NULL,
//[12][hierarchyid] NULL,
//[13][image] NULL,
//[14][int] NULL,
//[15][money] NULL,
//[16][nchar](10) NULL,
//[17][ntext] NULL,
//[18][numeric](18, 0) NULL,
//[19][nvarchar](50) NULL,
//[20][nvarchar](max) NULL,
//[21][real] NULL,
//[22][smalldatetime] NULL,
//[23][smallint] NULL,
//[24][smallmoney] NULL,
//[25][sql_variant] NULL,
//[26][text] NULL,
//[27][time](7) NULL,
//[28][timestamp] NULL,
//[29][tinyint] NULL,
//[30][uniqueidentifier] NULL,
//[31][varbinary](50) NULL,
//[32][varbinary](max) NULL,
//[33][varchar](50) NULL,
//[34][varchar](max) NULL,
//[35][xml] NULL

