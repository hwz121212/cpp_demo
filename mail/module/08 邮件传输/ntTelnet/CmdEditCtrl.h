/*
  This project was created and edited using the MFC SelfBanner AppWizard.

  Comment: 远程控制－远程Cmd客户端.

  Project: ntCmdCliennt.
  Author : dhyao
  Date   : 星期一, 二月 02, 2009
*/
/////////////////////////////////////////////////////////////////////
#if !defined(AFX_CMDEDITCTRL_H__EC9CD2A8_7481_422A_8D5F_B94A07CC4E36__INCLUDED_)
#define AFX_CMDEDITCTRL_H__EC9CD2A8_7481_422A_8D5F_B94A07CC4E36__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CmdEditCtrl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCmdEditCtrl window

class CCmdEditCtrl  : public CEdit
{
// Construction
public:
	CCmdEditCtrl();

// Attributes
public:
	typedef struct _RECORD_LIST
	{
		_RECORD_LIST *flink;
		_RECORD_LIST *blink;
		char cmd[];
	} RECORD_LIST, *PRECORD_LIST;
	
	SOCKET slave_sock;
	int nCmdStartPos;
	int nCmdRecord;
	int nMaxCmdRecord;
	PRECORD_LIST history;
	HANDLE hThread;

	void AddCmdRecord(char *cmd);
	char *GetNextCmdRecord();
	char *GetPrevCmdRecord();

// Operations
public:
	void InsertText(int nInsertAfterChar, LPCTSTR lpstrText, BOOL bNoScroll = FALSE, BOOL bCanUndo = FALSE);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCmdEditCtrl)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
public:
	BOOL CloseCmd();
	void ReadWrite();
	void AppendText(LPCTSTR lpstrText, BOOL bNoScroll = FALSE, BOOL bCanUndo = FALSE);
	BOOL StartCmdEditor(SOCKET sock);
	virtual ~CCmdEditCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CCmdEditCtrl)
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG
	LRESULT OnPaste(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);

	void OnSocketRead();
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMDEDITCTRL_H__EC9CD2A8_7481_422A_8D5F_B94A07CC4E36__INCLUDED_)
