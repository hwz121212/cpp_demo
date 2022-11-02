// myCEdit.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MFCApplication2.h"
#include "myCEdit.h"


// myCEdit

IMPLEMENT_DYNAMIC(myCEdit, CEdit)

myCEdit::myCEdit()
{

}

myCEdit::~myCEdit()
{
}


BEGIN_MESSAGE_MAP(myCEdit, CEdit)
	ON_WM_DROPFILES()
END_MESSAGE_MAP()



// myCEdit ��Ϣ�������




void myCEdit::OnDropFiles(HDROP hDropInfo)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	int DropCount = DragQueryFile(hDropInfo, -1, NULL, 0);//ȡ�ñ��϶��ļ�����Ŀ 
	WCHAR wcStr[MAX_PATH];
	DragQueryFile(hDropInfo, 0, wcStr, MAX_PATH);//�����ҷ�ĵ�i���ļ����ļ���
	//��һ��·��
	CString ctr2;
	ctr2.Format(L"%s", wcStr);
	
	SYSTEMTIME time;
	WIN32_FILE_ATTRIBUTE_DATA lpinf;
	CString TestDate;
	GetFileAttributesEx(ctr2, GetFileExInfoStandard, &lpinf);
	//����ʱ��
	FileTimeToSystemTime(&lpinf.ftCreationTime, &time);
	TestDate.Format(_T("����ʱ�䣺%d-%d-%d\r\n"), time.wYear, time.wMonth, time.wDay);
	ctr += TestDate;
	//����޸�ʱ��
	FileTimeToSystemTime(&lpinf.ftLastWriteTime, &time);
	TestDate.Format(_T("����޸�ʱ�䣺%d-%d-%d\r\n"), time.wYear, time.wMonth, time.wDay);
	ctr += TestDate;
	//������ʱ��
	FileTimeToSystemTime(&lpinf.ftLastAccessTime, &time);
	TestDate.Format(_T("������ʱ�䣺%d-%d-%d\r\n"), time.wYear, time.wMonth, time.wDay);
	ctr += TestDate;


	double size;
	CFileStatus fileStatus;

	if (CFile::GetStatus(ctr2, fileStatus))
	{
		size = fileStatus.m_size;
	}
	CString temp;
	temp.Format(L"��С��%f M\r\n", size / 1024.0 / 1024.0);
	ctr += temp;
	temp.Format(L"·����%s\r\n", fileStatus.m_szFullName);
	ctr += temp;
	














	DragFinish(hDropInfo);  //�ϷŽ�����,�ͷ��ڴ�  
	//HWND myHWND = ::GetDlgItem((HWND)GetModuleHandle(NULL), IDC_EDIT3);
	//::SetWindowTextA((::GetDlgItem((HWND)GetModuleHandle(NULL), IDC_EDIT3)), (LPCSTR)wcStr);
	SetWindowText(ctr);
	ctr = "";
	CEdit::OnDropFiles(hDropInfo);
}
