#include "stdafx.h"
#include "common.h"








//, ODBC_DATATYPE_SBYTE						// �з��ŵ��ֽ�����
//, ODBC_DATATYPE_BYTE						// �޷��ŵ��ֽ�����
//, ODBC_DATATYPE_SWORD						// �з���˫�ֽ�����
//, ODBC_DATATYPE_WORD						// �޷���˫�ֽ�����
//, ODBC_DATATYPE_INT						// �з������ֽ�����
//, ODBC_DATATYPE_UINT						// �޷������ֽ�����
//, ODBC_DATATYPE_STR						// �ַ���
//, ODBC_DATATYPE_LONGSTR					// �ַ���(��ֵ����)
//, ODBC_DATATYPE_BINARY					// ����������
//, ODBC_DATATYPE_LONGBINARY				// ����������(��ֵ����)
//, ODBC_DATATYPE_LONGLONG					// �з��Ű��ֽ�����
//, ODBC_DATATYPE_ULONGLONG					// �޷��Ű��ֽ�����
//, ODBC_DATATYPE_FLOAT						// ���ֽڸ�����
//, ODBC_DATATYPE_DOUBLE					// ���ֽڸ�����
//// ע�⣺SQLServer��֧�ֺ���ʱ�侫�ȣ����fraction����Ч��ΧΪ(1000000-999000000����)����1-999����
//, ODBC_DATATYPE_DATETIME					// ����ʱ�䣬��ӦC��������ΪSQL_TIMESTAMP_STRUCT							
//, ODBC_DATATYPE_NSTR						// �ַ���(unicode)
//, ODBC_DATATYPE_NLONGSTR					// �ַ���(unicode��ֵ����)
//, ODBC_DATATYPE_GUID						// 16�ֽ�GUID����ӦC��������Ϊ����16���޷����ֽ�����
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
	//ODBC_DATATYPE_END						// ��ǽ���
	CString strTemp = str;
	strTemp.MakeLower();

	if (strTemp.Find(_T("nvarchar")) != -1)return _T("ODBC_DATATYPE_NSTR");							   // �ַ���(unicode)	
	if (strTemp.Find(_T("bit")) != -1)return _T("ODBC_DATATYPE_BOOL");								// ����ֵ(BIT)
	if (strTemp.Find(_T("int")) != -1)return _T("ODBC_DATATYPE_INT");							   // �з������ֽ�����
	if (strTemp.Find(_T("varchar")) != -1)return _T("ODBC_DATATYPE_STR");							   // �ַ���	
	if (strTemp.Find(_T("float")) != -1)return _T("ODBC_DATATYPE_FLOAT");							   // ���ֽڸ�����
	if (strTemp.Find(_T("real")) != -1)return _T("ODBC_DATATYPE_DOUBLE");						   // ���ֽڸ�����
	if (strTemp.Find(_T("datetime")) != -1)return _T("ODBC_DATATYPE_DATETIME");					   // ����ʱ�䣬��ӦC��������ΪSQL_TIMESTAMP_STRUCT			
	
	return _T("");
}


//if (strTemp.Find(_T("bit")) != -1)return _T("ODBC_DATATYPE_BOOL");								// ����ֵ(BIT)
////if (strTemp.Find(_T("char")) != -1)return _T("ODBC_DATATYPE_SBYTE");							   // �з��ŵ��ֽ�����
////if (strTemp.Find(_T("char")) != -1)return _T("ODBC_DATATYPE_BYTE");							   // �޷��ŵ��ֽ�����
////if (strTemp.Find(_T("smallint")) != -1)return _T("ODBC_DATATYPE_SWORD");							   // �з���˫�ֽ�����
////if (strTemp.Find(_T("smallint")) != -1)return _T("ODBC_DATATYPE_WORD");							   // �޷���˫�ֽ�����
//if (strTemp.Find(_T("int")) != -1)return _T("ODBC_DATATYPE_INT");							   // �з������ֽ�����
////if (strTemp.Find(_T("int")) != -1)return _T("ODBC_DATATYPE_UINT");							   // �޷������ֽ�����
//if (strTemp.Find(_T("varchar")) != -1)return _T("ODBC_DATATYPE_STR");							   // �ַ���
////if (strTemp.Find(_T("bit")) != -1)return _T("ODBC_DATATYPE_LONGSTR");						   // �ַ���(��ֵ����)
////if (strTemp.Find(_T("bit")) != -1)return _T("ODBC_DATATYPE_BINARY");						   // ����������
////if (strTemp.Find(_T("bit")) != -1)return _T("ODBC_DATATYPE_LONGBINARY");					   // ����������(��ֵ����)
////if (strTemp.Find(_T("bit")) != -1)return _T("ODBC_DATATYPE_LONGLONG");						   // �з��Ű��ֽ�����
////if (strTemp.Find(_T("bit")) != -1)return _T("ODBC_DATATYPE_ULONGLONG");						   // �޷��Ű��ֽ�����
//if (strTemp.Find(_T("float")) != -1)return _T("ODBC_DATATYPE_FLOAT");							   // ���ֽڸ�����
//if (strTemp.Find(_T("real")) != -1)return _T("ODBC_DATATYPE_DOUBLE");						   // ���ֽڸ�����
//if (strTemp.Find(_T("datetime")) != -1)return _T("ODBC_DATATYPE_DATETIME");					   // ����ʱ�䣬��ӦC��������ΪSQL_TIMESTAMP_STRUCT			
//if (strTemp.Find(_T("nvarchar")) != -1)return _T("ODBC_DATATYPE_NSTR");							   // �ַ���(unicode)
////if (strTemp.Find(_T("bit")) != -1)return _T("ODBC_DATATYPE_NLONGSTR");						   // �ַ���(unicode��ֵ����)
////if (strTemp.Find(_T("bit")) != -1)return _T("ODBC_DATATYPE_GUID");							   // 16�ֽ�GUID����ӦC��������Ϊ����16���޷����ֽ�����


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

