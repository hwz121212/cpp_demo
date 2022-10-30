; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CWCView
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "WC.h"
LastPage=0

ClassCount=7
Class1=CWCApp
Class2=CWCDoc
Class3=CWCView
Class4=CMainFrame

ResourceCount=4
Resource1=IDR_MAINFRAME
Class5=CAboutDlg
Resource2=IDD_ABOUTBOX
Class6=CFontD
Resource3=IDD_INSERTFONT
Class7=CLinewidthDlg
Resource4=IDD_WIDTHDLG

[CLS:CWCApp]
Type=0
HeaderFile=WC.h
ImplementationFile=WC.cpp
Filter=N

[CLS:CWCDoc]
Type=0
HeaderFile=WCDoc.h
ImplementationFile=WCDoc.cpp
Filter=N

[CLS:CWCView]
Type=0
HeaderFile=WCView.h
ImplementationFile=WCView.cpp
Filter=C
BaseClass=CView
VirtualFilter=VWC
LastObject=CWCView


[CLS:CMainFrame]
Type=0
HeaderFile=MainFrm.h
ImplementationFile=MainFrm.cpp
Filter=T
LastObject=ID_LINEWIDTHDLG




[CLS:CAboutDlg]
Type=0
HeaderFile=WC.cpp
ImplementationFile=WC.cpp
Filter=D
LastObject=CAboutDlg

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[MNU:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_FILE_SAVE_AS
Command5=ID_FILE_PRINT
Command6=ID_FILE_PRINT_PREVIEW
Command7=ID_FILE_PRINT_SETUP
Command8=ID_FILE_MRU_FILE1
Command9=ID_APP_EXIT
Command10=ID_EDIT_UNDO
Command11=ID_EDIT_CUT
Command12=ID_EDIT_COPY
Command13=ID_EDIT_PASTE
Command14=ID_VIEW_TOOLBAR
Command15=ID_VIEW_STATUS_BAR
Command16=ID_APP_ABOUT
Command17=ID_COLORPEN
Command18=ID_COLORBRUSH
Command19=ID_FONT1
Command20=ID_LINEWIDTHDLG
CommandCount=20

[ACL:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_FILE_PRINT
Command5=ID_EDIT_UNDO
Command6=ID_EDIT_CUT
Command7=ID_EDIT_COPY
Command8=ID_EDIT_PASTE
Command9=ID_EDIT_UNDO
Command10=ID_EDIT_CUT
Command11=ID_EDIT_COPY
Command12=ID_EDIT_PASTE
Command13=ID_NEXT_PANE
Command14=ID_PREV_PANE
CommandCount=14

[TB:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_EDIT_CUT
Command5=ID_EDIT_COPY
Command6=ID_EDIT_PASTE
Command7=ID_FILE_PRINT
Command8=ID_APP_ABOUT
Command9=ID_RECTANGLE
Command10=ID_FILLRECTANGLE
Command11=ID_FONT1
CommandCount=11

[DLG:IDD_INSERTFONT]
Type=1
Class=CFontD
ControlCount=5
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STRING,edit,1350631556
Control4=IDC_STATIC,static,1342377985
Control5=IDC_SPIN1,msctls_updown32,1342177312

[CLS:CFontD]
Type=0
HeaderFile=FontD.h
ImplementationFile=FontD.cpp
BaseClass=CDialog
Filter=D
LastObject=IDOK
VirtualFilter=dWC

[DLG:IDD_WIDTHDLG]
Type=1
Class=CLinewidthDlg
ControlCount=13
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_RADIO1,button,1342308356
Control4=IDC_RADIO2,button,1342308356
Control5=IDC_RADIO3,button,1342308356
Control6=IDC_STATIC,static,1350565902
Control7=IDC_STATIC,static,1350565902
Control8=IDC_STATIC,button,1342177287
Control9=IDC_RADIO4,button,1342308356
Control10=IDC_RADIO5,button,1342308356
Control11=IDC_STATIC,static,1350565902
Control12=IDC_STATIC,static,1350566158
Control13=IDC_STATIC,static,1350566158

[CLS:CLinewidthDlg]
Type=0
HeaderFile=LinewidthDlg.h
ImplementationFile=LinewidthDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=IDC_RADIO5
VirtualFilter=dWC

