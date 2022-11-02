
// MFCApplication2Dlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MFCApplication2.h"
#include "MFCApplication2Dlg.h"
#include "afxdialogex.h"
#include<TlHelp32.h>
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
//CString CAboutDlg::ctrpath = L"";

// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMFCApplication2Dlg �Ի���



CMFCApplication2Dlg::CMFCApplication2Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_MFCAPPLICATION2_DIALOG, pParent)
	, cpustr(_T(""))
	, ctrpath(_T(""))
	, filestr(_T(""))
	//, mycleactr(_T(""))
	, myclearctring(_T(""))
	, mypidctr(_T(""))
	, mypidheap(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCApplication2Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, cclistctrl);
	//DDX_Text(pDX, IDC_EDIT1, myctr);
	DDX_Text(pDX, IDC_EDIT1, cpustr);
	DDX_Control(pDX, IDC_EDIT1, myCpuEdit);
	DDX_Text(pDX, IDC_EDIT4, ctrpath);
	DDX_Text(pDX, IDC_EDIT3, filestr);
	DDX_Control(pDX, IDC_EDIT3, myfileCEdit);
	DDX_Control(pDX, IDC_EDIT2, myclear);
	//DDX_Text(pDX, IDC_EDIT2, mycleactr);
	DDX_Text(pDX, IDC_EDIT2, myclearctring);
	DDX_Text(pDX, IDC_EDIT5, mypidctr);
	DDX_Text(pDX, IDC_EDIT6, mypidheap);
}

BEGIN_MESSAGE_MAP(CMFCApplication2Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(NM_CLICK, IDC_LIST1, &CMFCApplication2Dlg::OnNMClickList1)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST1, &CMFCApplication2Dlg::OnLvnItemchangedList1)
	ON_BN_CLICKED(IDC_BUTTON1, &CMFCApplication2Dlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CMFCApplication2Dlg::OnBnClickedButton2)
	//ON_EN_CHANGE(IDC_EDIT1, &CMFCApplication2Dlg::OnEnChangeEdit1)
	ON_BN_CLICKED(IDC_BUTTON3, &CMFCApplication2Dlg::OnBnClickedButton3)
	ON_EN_CHANGE(IDC_EDIT1, &CMFCApplication2Dlg::OnEnChangeEdit1)
	ON_BN_CLICKED(IDC_BUTTON4, &CMFCApplication2Dlg::OnBnClickedButton4)
	//ON_BN_CLICKED(IDC_BUTTON5, &CMFCApplication2Dlg::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON6, &CMFCApplication2Dlg::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON7, &CMFCApplication2Dlg::OnBnClickedButton7)
	ON_EN_CHANGE(IDC_EDIT3, &CMFCApplication2Dlg::OnEnChangeEdit3)
	ON_EN_CHANGE(IDC_EDIT4, &CMFCApplication2Dlg::OnEnChangeEdit4)
	//ON_BN_CLICKED(IDC_BUTTON5, &CMFCApplication2Dlg::OnBnClickedButton5)
	ON_EN_CHANGE(IDC_EDIT2, &CMFCApplication2Dlg::OnEnChangeEdit2)
	ON_BN_CLICKED(IDC_BUTTON5, &CMFCApplication2Dlg::OnBnClickedButton5)
	ON_EN_CHANGE(IDC_EDIT5, &CMFCApplication2Dlg::OnEnChangeEdit5)
	ON_EN_CHANGE(IDC_EDIT6, &CMFCApplication2Dlg::OnEnChangeEdit6)
	ON_BN_CLICKED(IDC_BUTTON8, &CMFCApplication2Dlg::OnBnClickedButton8)
	ON_BN_CLICKED(IDC_BUTTON9, &CMFCApplication2Dlg::OnBnClickedButton9)
	ON_BN_CLICKED(IDC_BUTTON10, &CMFCApplication2Dlg::OnBnClickedButton10)
	ON_BN_CLICKED(IDC_BUTTON11, &CMFCApplication2Dlg::OnBnClickedButton11)
	ON_WM_HOTKEY()

END_MESSAGE_MAP()


// CMFCApplication2Dlg ��Ϣ�������

BOOL CMFCApplication2Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	//MessageBox(L"������̺��г���ǰ���̵�����ģ��!!!!!");


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	myCpuEdit.SetReadOnly(TRUE);
	//myfileCEdit.SetReadOnly(TRUE);
	myfileCEdit.DragAcceptFiles(TRUE);
	HWND myallen = (HWND)::GetModuleHandle(NULL);
	::RegisterHotKey(this->m_hWnd, 0x1234, MOD_CONTROL, 'G');
	EnumProcess();
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CMFCApplication2Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CMFCApplication2Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CMFCApplication2Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


bool CMFCApplication2Dlg::EnumProcess() {
	// 1. �ȴ�������
	HANDLE hTool32 = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
	if (INVALID_HANDLE_VALUE == hTool32)
	{
		printf("����error!\n");
		return false;
	}
	// 2. ��ʼ��������
	PROCESSENTRY32W psi = { sizeof(PROCESSENTRY32W) };
	BOOL bRet = Process32FirstW(hTool32, &psi);
	if (!bRet)
	{
		//printf("Process32FirstW error!\n");
		return false;
	}
	//printf("PID    EXE:\n");
	int i = 0;
	cclistctrl.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	cclistctrl.GetClientRect(rc);
	nwidth = rc.Width();
	cclistctrl.InsertColumn(0, L"����", 0, nwidth / 6);
	cclistctrl.InsertColumn(1, L"dwSize", 1, nwidth / 8);
	cclistctrl.InsertColumn(2, L"cntUsage", 2, nwidth / 8);
	cclistctrl.InsertColumn(3, L"PID", 3, nwidth / 8);
	cclistctrl.InsertColumn(4, L"th32DefaultHeapID", 4, nwidth / 8);
	cclistctrl.InsertColumn(5, L"th32ModuleID", 5, nwidth / 8);
	cclistctrl.InsertColumn(6, L"cntThreads", 6, nwidth / 8);
	cclistctrl.InsertColumn(7, L"th32ParentProcessID", 7, nwidth / 8);
	cclistctrl.InsertColumn(8, L"pcPriClassBase", 8, nwidth / 8);
	cclistctrl.InsertColumn(9, L"dwFlags", 9, nwidth / 8);


	do
	{


		//typedef struct tagPROCESSENTRY32 {
		//	DWORD     dwSize;
		//	DWORD     cntUsage;
		//	DWORD     th32ProcessID;
		//	ULONG_PTR th32DefaultHeapID;
		//	DWORD     th32ModuleID;
		//	DWORD     cntThreads;
		//	DWORD     th32ParentProcessID;
		//	LONG      pcPriClassBase;
		//	DWORD     dwFlags;
		//	TCHAR     szExeFile[MAX_PATH];
		//} PROCESSENTRY32, *PPROCESSENTRY32;
		//printf("%4d   %S\n", psi.th32ProcessID, psi.szExeFile);
		//	TCHAR     szExeFile[MAX_PATH];
		cclistctrl.InsertItem(i, psi.szExeFile);
		//	DWORD     dwSize;
		CString strdwSize;
		strdwSize.Format(L"%4d", psi.dwSize);
		cclistctrl.SetItemText(i, 1, strdwSize);
		//	DWORD     cntUsage;
		CString strcntUsage;
		strcntUsage.Format(L"%4d", psi.cntUsage);
		cclistctrl.SetItemText(i, 2, strcntUsage);
		//	DWORD     th32ProcessID;
		CString strcth32ProcessID;
		strcth32ProcessID.Format(L"%4d", psi.th32ProcessID);
		cclistctrl.SetItemText(i, 3, strcth32ProcessID);
		//	ULONG_PTR th32DefaultHeapID;
		CString strcth32DefaultHeapID;
		strcth32DefaultHeapID.Format(L"%4d", psi.th32DefaultHeapID);
		cclistctrl.SetItemText(i, 4, strcth32DefaultHeapID);
		//	ULONG_PTR th32ModuleID;
		CString strth32ModuleID;
		strth32ModuleID.Format(L"%4d", psi.th32ModuleID);
		cclistctrl.SetItemText(i, 5, strth32ModuleID);
		//	DWORD     cntThreads;
		CString cntThreads;
		cntThreads.Format(L"%4d", psi.cntThreads);
		cclistctrl.SetItemText(i, 6, cntThreads);
		//	DWORD     th32ParentProcessID;
		CString th32ParentProcessID;
		th32ParentProcessID.Format(L"%4d", psi.th32ParentProcessID);
		cclistctrl.SetItemText(i, 7, th32ParentProcessID);
		//	LONG      pcPriClassBase;
		CString pcPriClassBase;
		pcPriClassBase.Format(L"%4d", psi.pcPriClassBase);
		cclistctrl.SetItemText(i, 8, pcPriClassBase);
		//	DWORD     dwFlags;
		CString strcdwFlags;
		strcdwFlags.Format(L"%4d", psi.dwFlags);
		cclistctrl.SetItemText(i, 9, strcdwFlags);

		++i;
	} while (Process32NextW(hTool32, &psi));
	return true;
}

void CMFCApplication2Dlg::OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int ccol = pNMItemActivate->iSubItem;
	int crow = pNMItemActivate->iItem;
	if (ccol == 3) {
		CString str = cclistctrl.GetItemText(crow, ccol);
		this->ShowWindow(SW_HIDE);
		//modo::pid = _ttoi(str);
	    pid = _ttoi(str);
		ccmymodo1  = new ccmymodo();
		ccmymodo1->DoModal();
		delete ccmymodo1;
		ccmymodo1 = nullptr;
		this->ShowWindow(SW_SHOW);
	}

	*pResult = 0;
}


void CMFCApplication2Dlg::OnLvnItemchangedList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
}


//��ȡϵͳ�����߳�
void CMFCApplication2Dlg::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	this->ShowWindow(SW_HIDE);
	ccmythreadmod = new threadmod();
	ccmythreadmod->DoModal();
	delete ccmythreadmod;
	ccmythreadmod = nullptr;
	this->ShowWindow(SW_SHOW);

}


void CMFCApplication2Dlg::OnBnClickedButton2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (mypidheap != "") {
		this->ShowWindow(SW_HIDE);
		ccmyduimodo = new myduimodo();
		ccmyduimodo->pid = _ttoi(mypidheap);
		ccmyduimodo->DoModal();
		delete ccmyduimodo;
		ccmyduimodo = nullptr;
		this->ShowWindow(SW_SHOW);
	}
	else {
		MessageBox(L"����pid!");
	}
//	MessageBox(myctr);
}



void CMFCApplication2Dlg::OnBnClickedButton3()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//MessageBox(L"allen");
	//mywinmodo* ccmywinmodo;
	this->ShowWindow(SW_HIDE);
	ccmywinmodo = new mywinmodo();
	ccmywinmodo->DoModal();
	delete ccmywinmodo;
	ccmyduimodo = nullptr;
	this->ShowWindow(SW_SHOW);
}


void CMFCApplication2Dlg::OnEnChangeEdit1()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	/*UpdateData(FALSE);
	cpustr += "\n";*/
	//UpdateData(TRUE);
}

//�鿴cpu��Ϣ
void CMFCApplication2Dlg::OnBnClickedButton4()
{
		this->myCpuEdit.Clear();
		SYSTEM_INFO systemInfo;
		GetSystemInfo(&systemInfo);
		CString str;
		str.Format(L"����������:%d \r\n", systemInfo.dwActiveProcessorMask);
		cpustr += str;
		str.Format(L"����������:%d \r\n", systemInfo.dwNumberOfProcessors);
		cpustr += str;
		str.Format(L"��������ҳ��С:%d\r\n", systemInfo.dwPageSize);
		cpustr += str;
		str.Format(L"����������:%d\r\n", systemInfo.dwProcessorType);
		cpustr += str;
		str.Format(L"���Ѱַ��Ԫ: %d\r\n", systemInfo.lpMaximumApplicationAddress);
		cpustr += str;
		str.Format(L"��СѰַ��Ԫ: %d\r\n", systemInfo.lpMinimumApplicationAddress);
		cpustr += str;
		str.Format(L"�������ȼ�: %d\r\n", systemInfo.wProcessorLevel);
		cpustr += str;
		str.Format(L"�������汾: %d\r\n", systemInfo.wProcessorRevision);
		MEMORYSTATUS stat;
		GlobalMemoryStatus(&stat);
		str.Format(L"�ڴ��ʹ����: %d\r\n", stat.dwMemoryLoad);
		cpustr += str;
		double neic = stat.dwTotalPhys/1024.0/1024.0/1024.0;

		str.Format(L"�ܵ������ڴ��С: %f G\r\n", neic);
		cpustr += str;
		str.Format(L"���õ������ڴ��С: %f G\r\n", stat.dwAvailPhys / 1024.0 / 1024.0 / 1024.0);
		cpustr += str;
		str.Format(L"�����ַ�ռ�: %f G\r\n", stat.dwTotalVirtual / 1024.0 / 1024.0 / 1024.0);
		cpustr += str;
		str.Format(L"���������ַ�ռ�: %f G\r\n", stat.dwAvailVirtual / 1024.0 / 1024.0 / 1024.0);

		cpustr += str;
		CWnd* pWnd = GetDlgItem(IDC_EDIT1);
		pWnd->SetWindowText(cpustr);
		cpustr = "";
		

		//dwLength ����MEMORYSTATUS�ṹ�Ĵ�С���ڵ�GlobalMemoryStatus����ǰ��sizeof()������ã��������������ṹ�İ汾��
		//	dwMemoryLoad �� ����һ������0��100֮���ֵ������ָʾ��ǰϵͳ�ڴ��ʹ���ʡ�
		//	dwTotalPhys ���������ܵ������ڴ��С�����ֽ�(byte)Ϊ��λ��
		//	dwAvailPhys �������ؿ��õ������ڴ��С�����ֽ�(byte)Ϊ��λ��
		//	dwTotalPageFile  ��ʾ���Դ���ҳ���ļ��е��ֽ�����ע�������ֵ������ʾ��ҳ���ļ��ڴ����ϵ���ʵ�����С��
		//	dwAvailPageFile  ���ؿ��õ�ҳ���ļ���С�����ֽ�(byte)Ϊ��λ��
		//	dwTotalVirtual �����ص��ý��̵��û�ģʽ���ֵ�ȫ�����������ַ�ռ䣬���ֽ�(byte)Ϊ��λ��
		//	dwAvailVirtual �����ص��ý��̵��û�ģʽ���ֵ�ʵ�����ɿ��õ������ַ�ռ䣬���ֽ�(byte)Ϊ��λ��
	/*	typedef struct _MEMORYSTATUS {
			DWORD dwLength;
			DWORD dwMemoryLoad;
			SIZE_T dwTotalPhys;
			SIZE_T dwAvailPhys;
			SIZE_T dwTotalPageFile;
			SIZE_T dwAvailPageFile;
			SIZE_T dwTotalVirtual;
			SIZE_T dwAvailVirtual;
		} MEMORYSTATUS, *LPMEMORYSTATUS;*/
		
	/*typedef struct _SYSTEM_INFO {
		union {
			DWORD  dwOemId;
			struct {
				WORD wProcessorArchitecture;
				WORD wReserved;
			};
		};
		DWORD     dwPageSize;
		LPVOID    lpMinimumApplicationAddress;
		LPVOID    lpMaximumApplicationAddress;
		DWORD_PTR dwActiveProcessorMask;
		DWORD     dwNumberOfProcessors;
		DWORD     dwProcessorType;
		DWORD     dwAllocationGranularity;
		WORD      wProcessorLevel;
		WORD      wProcessorRevision;
	} SYSTEM_INFO;*/

	// TODO: �ڴ���ӿؼ�֪ͨ����������
	
	//cout << setw(20) << "����������: " << systemInfo.dwActiveProcessorMask << endl
	//	<< setw(20) << "����������: " << systemInfo.dwNumberOfProcessors << endl
	//	<< setw(20) << "��������ҳ��С: " << systemInfo.dwPageSize << endl
	//	<< setw(20) << "����������: " << systemInfo.dwProcessorType << endl
	//	<< setw(20) << "���Ѱַ��Ԫ: " << systemInfo.lpMaximumApplicationAddress << endl
	//	<< setw(20) << "��СѰַ��Ԫ: " << systemInfo.lpMinimumApplicationAddress << endl
	//	<< setw(20) << "�������ȼ�: " << systemInfo.wProcessorLevel << endl
	//	<< setw(20) << "�������汾: " << systemInfo.wProcessorRevision << endl;
	//GetSystemInfo
}


//�鿴�ļ���Ϣ
void CMFCApplication2Dlg::OnBnClickedButton6()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//typedef struct _WIN32_FILE_ATTRIBUTE_DATA {
	//	DWORD    dwFileAttributes;
	   //����ʱ��
	//	FILETIME ftCreationTime;
		//������ʱ��
	//	FILETIME ftLastAccessTime;
		//����޸�ʱ��
	//	FILETIME ftLastWriteTime;
	//	DWORD    nFileSizeHigh;
	//	DWORD    nFileSizeLow;
	//} WIN32_FILE_ATTRIBUTE_DATA, *LPWIN32_FILE_ATTRIBUTE_DATA;
	SYSTEMTIME time;
	WIN32_FILE_ATTRIBUTE_DATA lpinf;
	CString TestDate;
	GetFileAttributesEx(ctrpath, GetFileExInfoStandard, &lpinf);
	//����ʱ��
	FileTimeToSystemTime(&lpinf.ftCreationTime, &time);
	TestDate.Format(_T("����ʱ�䣺%d-%d-%d\r\n"), time.wYear, time.wMonth, time.wDay);
	filestr += TestDate;
	//����޸�ʱ��
	FileTimeToSystemTime(&lpinf.ftLastWriteTime, &time);
	TestDate.Format(_T("����޸�ʱ�䣺%d-%d-%d\r\n"), time.wYear, time.wMonth, time.wDay);
	filestr += TestDate;
	//������ʱ��
	FileTimeToSystemTime(&lpinf.ftLastAccessTime, &time);
	TestDate.Format(_T("������ʱ�䣺%d-%d-%d\r\n"), time.wYear, time.wMonth, time.wDay);
	filestr += TestDate;


	double size;
	CFileStatus fileStatus;

	if (CFile::GetStatus(ctrpath, fileStatus))
	{
		size = fileStatus.m_size;
	}
	CString str;
	str.Format(L"��С��%f M\r\n", size/1024.0/1024.0);
	filestr += str;
	str.Format(L"·����%s\r\n", fileStatus.m_szFullName);
	filestr += str;
	CWnd* pWnd = GetDlgItem(IDC_EDIT3);
	pWnd->SetWindowText(filestr);
	filestr = "";

	//CFileStatus��һ���ṹ�壬�˽ṹ��洢���ļ���״̬��Ϣ��
	//	����������������ṹ���Ա��
	//	CTime m_ctime   The date and time the file was created.
	//	��ʾ�ļ��Ĵ���ʱ��
	//	CTime m_mtime   The date and time the file was last modified.
	//	��ʾ�ļ����޸�ʱ��
	//	CTime m_atime   The date and time the file was last accessed for reading.
	//	��ʾ�ļ���������ʱ��
	//	ULONGLONG m_size   The logical size of the file in bytes, as reported by the DIR command.
	//	��ʾ�ļ����߼���С
	//	BYTE m_attribute   The attribute byte of the file.
	//	��ʾ�ļ���ϵͳ����
	//	char m_szFullName[_MAX_PATH]   The absolute filename in the Windows character set.
	//	��ʾ�ļ��ľ���·��
}


void CMFCApplication2Dlg::OnBnClickedButton7()
{
	//IDC_EDIT4, ctrpath);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString gReadFilePathName;
	CFileDialog fileDlg(true, _T("mp3"), _T("*.mp3"), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("mp3 Files (*.mp3)|*.mp3|wav File(*.wav)|*.wav|All File (*.*)|*.*||"), NULL);
	if (fileDlg.DoModal() == IDOK)    //�����Ի���  
	{
		gReadFilePathName = fileDlg.GetPathName();//�õ��������ļ�����Ŀ¼����չ��  
		GetDlgItem(IDC_EDIT4)->SetWindowText(gReadFilePathName);//��·����ʾ  
		ctrpath = gReadFilePathName;
	}
	//MessageBox(gReadFilePathName);
}


void CMFCApplication2Dlg::OnEnChangeEdit3()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}


void CMFCApplication2Dlg::OnEnChangeEdit4()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}


void CMFCApplication2Dlg::OnEnChangeEdit2()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}

//��������
void CMFCApplication2Dlg::OnBnClickedButton5()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (mypidctr != "") {
		DWORD pid = _ttoi(mypidctr);
		//KillProcess(pid);
		if (KillProcess(pid)) {
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


bool CMFCApplication2Dlg::KillProcess(DWORD pid)
{
	// When the all operation fail this function terminate the "winlogon" Process for force exit the system.
	HANDLE hYourTargetProcess = OpenProcess(PROCESS_QUERY_INFORMATION |   // Required by Alpha
		PROCESS_CREATE_THREAD |   // For CreateRemoteThread
		PROCESS_VM_OPERATION |   // For VirtualAllocEx/VirtualFreeEx
		PROCESS_TERMINATE |
		PROCESS_VM_WRITE,             // For WriteProcessMemory
		FALSE, pid);

	if (hYourTargetProcess == NULL)
	{
		return FALSE;
	}

	if (TerminateProcess(hYourTargetProcess, 0) != 0)
	{
		::Sleep(1000);
		return TRUE;
	}
	else
		return FALSE;
}

void CMFCApplication2Dlg::OnEnChangeEdit5()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������

	UpdateData(TRUE);
}

//heap
void CMFCApplication2Dlg::OnEnChangeEdit6()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
}

//�ػ�
void CMFCApplication2Dlg::OnBnClickedButton8()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	ExitWindowsEx(EWX_POWEROFF | EWX_FORCE, 0);
}

//����
void CMFCApplication2Dlg::OnBnClickedButton9()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	ExitWindows(EWX_REBOOT | EWX_FORCE, 0);
}

//ע��
void CMFCApplication2Dlg::OnBnClickedButton10()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	ExitWindows(EWX_LOGOFF | EWX_FORCE, 0);
}

//����
void CMFCApplication2Dlg::OnBnClickedButton11()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//SetSuspendState(TRUE, FALSE, FALSE);
}


void CMFCApplication2Dlg::OnHotKey(UINT nHotKeyId, UINT nKey1, UINT nKey2)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	
	CDialogEx::OnHotKey(nHotKeyId, nKey1, nKey2);
}

//::RegisterHotKey(myallen, 0x1234, MOD_CONTROL, 'G');
//::RegisterHotKey(myallen, 0x1235, MOD_CONTROL, 'J');

BOOL CMFCApplication2Dlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
	if ((pMsg->message == WM_HOTKEY) && (pMsg->wParam == 0x1234))
	{
	//	HWND myallen = (HWND)::GetModuleHandle(NULL);
		if (::IsWindowVisible(m_hWnd) == TRUE  ){
			ShowWindow(SW_HIDE);
		}
		else {
			ShowWindow(SW_SHOW);
		}

		

	}
	return CDialogEx::PreTranslateMessage(pMsg);
}
