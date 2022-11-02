// DlgBar.cpp : 实现文件
//

#include "stdafx.h"
#include "Screenshot.h"
#include "DlgBar.h"


// CDlgBar 对话框

IMPLEMENT_DYNAMIC(CDlgBar, CDialog)

CDlgBar::CDlgBar(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgBar::IDD, pParent)
{

}

CDlgBar::~CDlgBar()
{
}

void CDlgBar::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON1, m_btnRect);
	DDX_Control(pDX, IDC_BUTTON2, m_arrow);
	DDX_Control(pDX, IDC_BUTTON3, m_btnEllipse);
	DDX_Control(pDX, IDC_BUTTON4, m_btnLine);
	DDX_Control(pDX, IDC_BUTTON5, m_btnStr);
	DDX_Control(pDX, IDC_BUTTON6, m_btnBack);
	DDX_Control(pDX, IDC_BUTTON7, m_btnSave);
	DDX_Control(pDX, IDC_BUTTON8, m_btnEsc);
	DDX_Control(pDX, IDC_BUTTON9, m_btnOk);
}


BEGIN_MESSAGE_MAP(CDlgBar, CDialog)
	ON_COMMAND_RANGE(IDC_BUTTON1,IDC_BUTTON9,OnOutPutStatusButtonUp)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CDlgBar 消息处理程序

BOOL CDlgBar::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化

	LONG   lStyle   =   ::GetWindowLong(GetSafeHwnd(),   GWL_STYLE); 
	lStyle   &=   ~WS_CAPTION; 
	::SetWindowLong(GetSafeHwnd(),   GWL_STYLE,   lStyle); 

	MoveButton(IDC_BUTTON1,9);

	EnableToolTips(TRUE);
	m_toolTip.Create(this);
	m_toolTip.Activate(TRUE);

	m_btnRect.SetImage(IDB_BITMAP_RECT,25,24);
	m_arrow.SetImage(IDB_BITMAP_ARROW,25,24);
	m_btnEllipse.SetImage(IDB_BITMAP_ELLIPSE,25,24);
	m_btnLine.SetImage(IDB_BITMAP_BRUSH,25,24);
	m_btnStr.SetImage(IDB_BITMAP_FONT,25,24);
	m_btnBack.SetImage(IDB_BITMAP_BACKWARD,25,24);
	m_btnSave.SetImage(IDB_BITMAP_SAVE,25,24);
	m_btnEsc.SetImage(IDB_BITMAP_ESC,25,24);
	m_btnOk.SetImage(IDB_BITMAP_OK,60,24);

	
	m_toolTip.AddTool(GetDlgItem(IDC_BUTTON1),_T("矩形工具") );
	m_toolTip.AddTool(GetDlgItem(IDC_BUTTON2),_T("箭头工具") );
	m_toolTip.AddTool(GetDlgItem(IDC_BUTTON3),_T("椭圆工具") );
	m_toolTip.AddTool(GetDlgItem(IDC_BUTTON4),_T("曲线工具") );
	m_toolTip.AddTool(GetDlgItem(IDC_BUTTON5),_T("文字工具") );
	m_toolTip.AddTool(GetDlgItem(IDC_BUTTON6),_T("撤销编辑") );
	m_toolTip.AddTool(GetDlgItem(IDC_BUTTON7),_T("保存") );
	m_toolTip.AddTool(GetDlgItem(IDC_BUTTON8),_T("退出截图") );
	m_toolTip.AddTool(GetDlgItem(IDC_BUTTON9),_T("完成截图") );
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDlgBar::OnOutPutStatusButtonUp(UINT nID)
{
	HWND hWnd = GetParent()->GetSafeHwnd();
	ASSERT(hWnd);

	int nType = 0;
	switch(nID)
	{
	case IDC_BUTTON1:
		//代码1
		nType = 1;
		break;

	case IDC_BUTTON2:
		//代码2
		nType = 2;
		break;

	case IDC_BUTTON3:
		nType = 3;
		break;

	case IDC_BUTTON4:
		nType = 4;
		break;

	case IDC_BUTTON5:
		nType = 5;
		break;

	case IDC_BUTTON6:
		nType = 6;
		break;

	case IDC_BUTTON7:
		nType = 7;
		break;

	case IDC_BUTTON8:
		nType = 8;
		break;

	case IDC_BUTTON9:
		nType = 9;
		break;

	
	}
	
    ::SendMessage(hWnd,WM_GETCOMMAND,0,(LPARAM)&nType);
}

void CDlgBar::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CDialog::OnClose();
}

BOOL CDlgBar::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	m_toolTip.RelayEvent(pMsg);
	if(WM_KEYDOWN   ==   pMsg-> message) 
	{ 
		if( (13 == pMsg->wParam) || 27 == pMsg-> wParam ) //enter || escape 
		{
			CWnd *pParent = GetParent();
			if(pParent)
			{
				pParent->SendMessage(WM_CLOSE);
			}
			return   FALSE; 
		}
	} 
	return CDialog::PreTranslateMessage(pMsg);
}

void CDlgBar::MoveButton(UINT nID,int nCount)
{
	CButton *pButton = (CButton*)GetDlgItem(nID);
	if (NULL == pButton)
	{
		return;
	}

	CRect rc;
	pButton->GetWindowRect(&rc);
	ScreenToClient(&rc);

	

	rc.top = 0;
	rc.left = 0;
	rc.bottom = 24;
	rc.right = 25;
	pButton->MoveWindow(&rc);

	int nWith = rc.Width();
	int nheight = rc.Height();

	CRect rcBtn;
	for (int i = 1;i<nCount - 1;i++)
	{
		pButton = (CButton*)GetDlgItem(nID + i);
		if (NULL == pButton)
		{
			return;
		}
		pButton->GetWindowRect(&rcBtn);
		ScreenToClient(&rcBtn);
		rcBtn.left = rc.left + i*nWith;
		rcBtn.right = rcBtn.left + nWith;
		rcBtn.top = 0;
		rcBtn.bottom = nheight;
		pButton->MoveWindow(&rcBtn);

	}

	//最后一个按钮长度为60，单独处理
	pButton = (CButton*)GetDlgItem(nID + nCount -1);
	if (NULL == pButton)
	{
		return;
	}
	pButton->GetWindowRect(&rcBtn);
	ScreenToClient(&rcBtn);
	rcBtn.left = rc.left + (nCount -1)*nWith;
	rcBtn.right = rcBtn.left + 60;
	rcBtn.top = 0;
	rcBtn.bottom = nheight;
	pButton->MoveWindow(&rcBtn);

}
