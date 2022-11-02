// threadmod.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MFCApplication2.h"
#include "threadmod.h"
#include "afxdialogex.h"
#include<TlHelp32.h>

// threadmod �Ի���

IMPLEMENT_DYNAMIC(threadmod, CDialogEx)

threadmod::threadmod(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG2, pParent)
	, threadid(_T(""))
{

}

threadmod::~threadmod()
{
}

void threadmod::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST2, mythreadlistctrl);

	DDX_Text(pDX, IDC_EDIT1, threadid);
}


BEGIN_MESSAGE_MAP(threadmod, CDialogEx)
	//ON_BN_CLICKED(IDOK, &threadmod::OnBnClickedOk)
	ON_WM_PAINT()
	ON_EN_CHANGE(IDC_EDIT1, &threadmod::OnEnChangeEdit1)
	ON_BN_CLICKED(IDC_BUTTON1, &threadmod::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &threadmod::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &threadmod::OnBnClickedButton3)
END_MESSAGE_MAP()


// threadmod ��Ϣ�������


bool threadmod::Enumthread()
{
	// 1. �ȴ�������
	HANDLE hTool32 = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
	if (INVALID_HANDLE_VALUE == hTool32)
	{
		MessageBox(L"Ȩ��̫�ͣ��������Ʋ�ҪϹ�㣡������");
		//printf("����error!\n");
		return false;
	}
	//typedef struct tagTHREADENTRY32 {
	//	DWORD dwSize;
	//	DWORD cntUsage;
	//	DWORD th32ThreadID;
	//	DWORD th32OwnerProcessID;
	//	LONG  tpBasePri;
	//	LONG  tpDeltaPri;
	//	DWORD dwFlags;
	//} THREADENTRY32, *PTHREADENTRY32;
	// 2. ��ʼ��������
	THREADENTRY32 mi = { sizeof(THREADENTRY32) };
	BOOL bRet = Thread32First(hTool32, &mi);
	if (!bRet)
	{
		printf("Thread32First error!\n");
		return false;
	}
	
	mythreadlistctrl.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	CRect rc;
	mythreadlistctrl.GetClientRect(rc);
	DWORD nwidth = rc.Width();
	//	DWORD th32OwnerProcessID;
	mythreadlistctrl.InsertColumn(0, L"th32OwnerProcessID", 0, nwidth / 7);
	//	DWORD th32ThreadID;
	mythreadlistctrl.InsertColumn(1, L"th32ThreadID", 1, nwidth / 7);
	//	DWORD dwSize;
	mythreadlistctrl.InsertColumn(2, L"dwSize", 2, nwidth / 7);
	//	DWORD cntUsage;
	mythreadlistctrl.InsertColumn(3, L"cntUsage", 3, nwidth / 7);
	//	DWORD tpBasePri;
	mythreadlistctrl.InsertColumn(4, L"tpBasePri", 4, nwidth / 7);
	//	DWORD tpDeltaPri;
	mythreadlistctrl.InsertColumn(5, L"tpDeltaPri", 5, nwidth / 7);
	//	DWORD dwFlags;
	mythreadlistctrl.InsertColumn(6, L"dwFlags", 6, nwidth / 7);
	int i = 0;
	do
	{
		//	DWORD th32OwnerProcessID;
		CString th32OwnerProcessID;
		th32OwnerProcessID.Format(L"%4d", mi.th32OwnerProcessID);
		mythreadlistctrl.InsertItem(i, th32OwnerProcessID);
		//	DWORD th32ThreadID;
		CString th32ThreadID;
		th32ThreadID.Format(L"%4d", mi.th32ThreadID);
		mythreadlistctrl.SetItemText(i, 1, th32ThreadID);
		//	DWORD dwSize;
		CString dwSize;
		dwSize.Format(L"%4d", mi.dwSize);
		mythreadlistctrl.SetItemText(i, 2, dwSize);
		//	DWORD cntUsage;
		CString cntUsage;
		cntUsage.Format(L"%4d", mi.cntUsage);
		mythreadlistctrl.SetItemText(i, 3, cntUsage);
		//	DWORD tpBasePri;
		CString tpBasePri;
		tpBasePri.Format(L"%4d", mi.tpBasePri);
		mythreadlistctrl.SetItemText(i, 4, tpBasePri);
		//	DWORD tpDeltaPri;
		CString tpDeltaPri;
		tpDeltaPri.Format(L"%4d", mi.tpDeltaPri);
		mythreadlistctrl.SetItemText(i, 5, tpDeltaPri);
		//	DWORD dwFlags;
		CString dwFlags;
		dwFlags.Format(L"%4d", mi.dwFlags);
		mythreadlistctrl.SetItemText(i, 6, dwFlags);
		++i;

	} while (Thread32Next(hTool32, &mi));
	return false;
}

//void threadmod::OnBnClickedOk()
//{
//	// TODO: �ڴ���ӿؼ�֪ͨ����������
//	CDialogEx::OnOK();
//
//
//}
void threadmod::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: �ڴ˴������Ϣ����������
					   // ��Ϊ��ͼ��Ϣ���� CDialogEx::OnPaint()
	Enumthread();

}

bool threadmod::Enumthread(DWORD pid)
{
	// 1. �ȴ�������
	HANDLE hTool32 = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
	if (INVALID_HANDLE_VALUE == hTool32)
	{
		MessageBox(L"Ȩ��̫�ͣ��������Ʋ�ҪϹ�㣡������");
		//printf("����error!\n");
		return false;
	}
	//typedef struct tagTHREADENTRY32 {
	//	DWORD dwSize;
	//	DWORD cntUsage;
	//	DWORD th32ThreadID;
	//	DWORD th32OwnerProcessID;
	//	LONG  tpBasePri;
	//	LONG  tpDeltaPri;
	//	DWORD dwFlags;
	//} THREADENTRY32, *PTHREADENTRY32;
	// 2. ��ʼ��������
	THREADENTRY32 mi = { sizeof(THREADENTRY32) };
	BOOL bRet = Thread32First(hTool32, &mi);
	if (!bRet)
	{
		printf("Thread32First error!\n");
		return false;
	}

	mythreadlistctrl.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	CRect rc;
	mythreadlistctrl.GetClientRect(rc);
	DWORD nwidth = rc.Width();
	//	DWORD th32OwnerProcessID;
	mythreadlistctrl.InsertColumn(0, L"th32OwnerProcessID", 0, nwidth / 7);
	//	DWORD th32ThreadID;
	mythreadlistctrl.InsertColumn(1, L"th32ThreadID", 1, nwidth / 7);
	//	DWORD dwSize;
	mythreadlistctrl.InsertColumn(2, L"dwSize", 2, nwidth / 7);
	//	DWORD cntUsage;
	mythreadlistctrl.InsertColumn(3, L"cntUsage", 3, nwidth / 7);
	//	DWORD tpBasePri;
	mythreadlistctrl.InsertColumn(4, L"tpBasePri", 4, nwidth / 7);
	//	DWORD tpDeltaPri;
	mythreadlistctrl.InsertColumn(5, L"tpDeltaPri", 5, nwidth / 7);
	//	DWORD dwFlags;
	mythreadlistctrl.InsertColumn(6, L"dwFlags", 6, nwidth / 7);
	int i = 0;
	do
	{
		if (mi.th32OwnerProcessID == pid) {
			//	DWORD th32OwnerProcessID;
			CString th32OwnerProcessID;
			th32OwnerProcessID.Format(L"%4d", mi.th32OwnerProcessID);
			mythreadlistctrl.InsertItem(i, th32OwnerProcessID);
			//	DWORD th32ThreadID;
			CString th32ThreadID;
			th32ThreadID.Format(L"%4d", mi.th32ThreadID);
			mythreadlistctrl.SetItemText(i, 1, th32ThreadID);
			//	DWORD dwSize;
			CString dwSize;
			dwSize.Format(L"%4d", mi.dwSize);
			mythreadlistctrl.SetItemText(i, 2, dwSize);
			//	DWORD cntUsage;
			CString cntUsage;
			cntUsage.Format(L"%4d", mi.cntUsage);
			mythreadlistctrl.SetItemText(i, 3, cntUsage);
			//	DWORD tpBasePri;
			CString tpBasePri;
			tpBasePri.Format(L"%4d", mi.tpBasePri);
			mythreadlistctrl.SetItemText(i, 4, tpBasePri);
			//	DWORD tpDeltaPri;
			CString tpDeltaPri;
			tpDeltaPri.Format(L"%4d", mi.tpDeltaPri);
			mythreadlistctrl.SetItemText(i, 5, tpDeltaPri);
			//	DWORD dwFlags;
			CString dwFlags;
			dwFlags.Format(L"%4d", mi.dwFlags);
			mythreadlistctrl.SetItemText(i, 6, dwFlags);
		}
		++i;

	} while (Thread32Next(hTool32, &mi));
	return false;
}


void threadmod::OnEnChangeEdit1()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
}

//�����߳�
void threadmod::OnBnClickedButton1()
{

	//MessageBox(threadid);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (threadid != "") {
		DWORD hid = _ttoi(threadid);
		HANDLE hihan =  OpenThread(THREAD_ALL_ACCESS, false,hid);
		if (SuspendThread(hihan)) {
			
			MessageBox(L"�ɹ���");
		}
		else {
			MessageBox(L"ʧ�ܣ�");
		}

	}
	else {
		MessageBox(L"������pid");
	}
}

//�ָ��߳�
void threadmod::OnBnClickedButton2()
{
	//MessageBox(threadid);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (threadid != "") {
		DWORD hid = _ttoi(threadid);
		HANDLE hihan = OpenThread(THREAD_ALL_ACCESS, false, hid);
		if (ResumeThread(hihan)) {

			MessageBox(L"�ɹ���");
		}
		else {
			MessageBox(L"ʧ�ܣ�");
		}

	}
	else {
		MessageBox(L"������pid");
	}
}

//�����߳�
void threadmod::OnBnClickedButton3()
{
	if (threadid != "") {
		DWORD hid = _ttoi(threadid);
		HANDLE hihan = OpenThread(THREAD_ALL_ACCESS, false, hid);
		if (TerminateThread(hihan,0)) {

			MessageBox(L"�ɹ���");
		}
		else {
			MessageBox(L"ʧ�ܣ�");
		}

	}
	else {
		MessageBox(L"������pid");
	}
}
