; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CGoTestDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "GoTest.h"

ClassCount=3
Class1=CGoTestApp
Class2=CGoTestDlg
Class3=CAboutDlg

ResourceCount=3
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Resource3=IDD_GOTEST_DIALOG

[CLS:CGoTestApp]
Type=0
HeaderFile=GoTest.h
ImplementationFile=GoTest.cpp
Filter=N

[CLS:CGoTestDlg]
Type=0
HeaderFile=GoTestDlg.h
ImplementationFile=GoTestDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC
LastObject=IDC_STA_SERVER_RETURN

[CLS:CAboutDlg]
Type=0
HeaderFile=GoTestDlg.h
ImplementationFile=GoTestDlg.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_GOTEST_DIALOG]
Type=1
Class=CGoTestDlg
ControlCount=7
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,button,1342177287
Control4=IDC_STATIC,button,1342177287
Control5=IDC_EDIT_SERVERRETURN,edit,1350631556
Control6=IDC_STATIC,button,1342177287
Control7=IDC_EDIT_SENDREQUEST,edit,1350631556

