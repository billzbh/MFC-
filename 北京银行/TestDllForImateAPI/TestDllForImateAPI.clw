; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CTestDllForImateAPIDlg
LastTemplate=CEdit
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "TestDllForImateAPI.h"

ClassCount=4
Class1=CTestDllForImateAPIApp
Class2=CTestDllForImateAPIDlg
Class3=CAboutDlg

ResourceCount=3
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Class4=CHexStringEdit
Resource3=IDD_TESTDLLFORIMATEAPI_DIALOG

[CLS:CTestDllForImateAPIApp]
Type=0
HeaderFile=TestDllForImateAPI.h
ImplementationFile=TestDllForImateAPI.cpp
Filter=N

[CLS:CTestDllForImateAPIDlg]
Type=0
HeaderFile=TestDllForImateAPIDlg.h
ImplementationFile=TestDllForImateAPIDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC
LastObject=IDC_BigResultLog2

[CLS:CAboutDlg]
Type=0
HeaderFile=TestDllForImateAPIDlg.h
ImplementationFile=TestDllForImateAPIDlg.cpp
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

[DLG:IDD_TESTDLLFORIMATEAPI_DIALOG]
Type=1
Class=CTestDllForImateAPIDlg
ControlCount=23
Control1=IDC_masterkey1EDIT,edit,1350631432
Control2=IDC_STATIC,static,1342308352
Control3=IDC_STATIC,static,1342308352
Control4=IDC_masterkey2EDIT,edit,1350631432
Control5=IDC_STATIC,button,1342177287
Control6=IDC_BUTTON1,button,1342242816
Control7=IDC_STATIC,static,1342308876
Control8=IDC_RESULT,static,1342308864
Control9=IDC_STATIC,static,1342308864
Control10=IDC_STATIC,button,1342177287
Control11=IDC_STATIC,static,1342308352
Control12=IDC_STATIC,static,1342308352
Control13=IDC_STATUSLOG,static,1342308864
Control14=IDC_VersionLog,static,1342308864
Control15=IDC_SNLog,static,1342308864
Control16=IDC_masterkeyCheckCode,edit,1350631432
Control17=IDC_STATIC,static,1342308352
Control18=IDC_STATIC,static,1342308352
Control19=IDC_DeviceSNCheckCode,edit,1350631432
Control20=IDC_BigResultLog,static,1342308352
Control21=IDC_mkLength1,static,1342308353
Control22=IDC_mkLength2,static,1342308353
Control23=IDC_BigResultLog2,static,1342308352

[CLS:CHexStringEdit]
Type=0
HeaderFile=HexStringEdit.h
ImplementationFile=HexStringEdit.cpp
BaseClass=CEdit
Filter=W

