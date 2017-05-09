// TestDllForImateAPIDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TestDllForImateAPI.h"
#include "TestDllForImateAPIDlg.h"
#include "HXiMateWin32.h"
#include "HXiMatePinPadWin32.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CStatic* g_SNLog = NULL;
CStatic* g_VersionLog = NULL;
CStatic* g_DownloadResultLog = NULL;
CStatic* g_BigResultLog =NULL;
CStatic* g_BigResultLog2 =NULL;

CEdit* g_MK1 =NULL;
CEdit* g_MK2 =NULL;
CEdit* g_MKCRC =NULL;
CEdit* g_SNCRC =NULL;


static int isWorkInThread = 0;

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTestDllForImateAPIDlg dialog

CTestDllForImateAPIDlg::CTestDllForImateAPIDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTestDllForImateAPIDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTestDllForImateAPIDlg)
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTestDllForImateAPIDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTestDllForImateAPIDlg)
	DDX_Control(pDX, IDC_BigResultLog2, m_biglogFailTip);
	DDX_Control(pDX, IDC_mkLength2, m_mk2Lenght);
	DDX_Control(pDX, IDC_mkLength1, m_mk1Length);
	DDX_Control(pDX, IDC_BigResultLog, m_BigResultLog);
	DDX_Control(pDX, IDC_masterkeyCheckCode, m_MasterKeyCheckCode);
	DDX_Control(pDX, IDC_DeviceSNCheckCode, m_snCheckCode);
	DDX_Control(pDX, IDC_masterkey2EDIT, m_MK2edit);
	DDX_Control(pDX, IDC_masterkey1EDIT, m_MK1edit);
	DDX_Control(pDX, IDC_RESULT, m_resultLog);
	DDX_Control(pDX, IDC_VersionLog, m_VerisonLog);
	DDX_Control(pDX, IDC_SNLog, m_SNLog);
	DDX_Control(pDX, IDC_STATUSLOG, m_DeviceStatusLog);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CTestDllForImateAPIDlg, CDialog)
	//{{AFX_MSG_MAP(CTestDllForImateAPIDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTON1, OnDownloadMasterKey)
	ON_EN_CHANGE(IDC_masterkey2EDIT, OnChangemasterkey2EDIT)
	ON_EN_CHANGE(IDC_masterkey1EDIT, OnChangemasterkey1EDIT)
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTestDllForImateAPIDlg message handlers

BOOL CTestDllForImateAPIDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	SetTimer(1,2000,NULL);//������ʱ��1,��ʱʱ����1��

	g_SNLog = &m_SNLog;
	g_VersionLog = &m_VerisonLog;
	g_DownloadResultLog = &m_resultLog;

	g_MK1 = &m_MK1edit;
	g_MK2 = &m_MK2edit;
	g_MKCRC = &m_MasterKeyCheckCode;
	g_SNCRC = &m_snCheckCode;
	g_BigResultLog = &m_BigResultLog;
	g_BigResultLog2 = &m_biglogFailTip;

	myFont.CreatePointFont(480,"Terminal");
	m_BigResultLog.SetFont(&myFont);
	m_BigResultLog.SetWindowText("");
	m_biglogFailTip.SetFont(&myFont);
	m_biglogFailTip.SetWindowText("");

	myFont2.CreatePointFont(150,"Courier New");
	m_MK1edit.SetFont(&myFont2);
	m_MK2edit.SetFont(&myFont2);
	m_MasterKeyCheckCode.SetFont(&myFont2);
	m_snCheckCode.SetFont(&myFont2);

	m_MK1edit.SetLimitText(16);
	m_MK2edit.SetLimitText(16);
	m_MasterKeyCheckCode.SetLimitText(8);
	m_snCheckCode.SetLimitText(8);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CTestDllForImateAPIDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CTestDllForImateAPIDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

DWORD _stdcall ThreadProc(LPVOID lpParameter)//�߳�ִ�к���
{
	Sleep(100);
	char version[50]={0};
	char serialNo[50]={0};
	int ret = HXiMate_DeviceVersions(version);
	if(ret>0){
		g_VersionLog->SetWindowText(version);
	}else{
		g_VersionLog->SetWindowText("");
	}
	Sleep(100);
	ret = HXiMate_DeviceSerialNumber(serialNo);
	if(ret>0){
		char TermSN[17]={0};
		memcpy(TermSN,serialNo+ret-10,10);
		g_SNLog->SetWindowText(TermSN);
	}else{
		g_SNLog->SetWindowText("");
	}
	return 0;
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CTestDllForImateAPIDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CTestDllForImateAPIDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if(!HXiMate_DeviceIsConnecting())
	{
		int ret = HXiMate_DeviceConnect("usb-hid");
		if(ret==0)
		{
			m_DeviceStatusLog.SetWindowText("���ӳɹ�");
			//�����̣߳���ȡ�汾�� �� ���к�
			CreateThread(NULL,0,ThreadProc,NULL,0,NULL);//����һ���̣߳�ȥִ��ThreadProc����
		}else{
			m_DeviceStatusLog.SetWindowText("δ����");
			m_SNLog.SetWindowText("");
			m_VerisonLog.SetWindowText("");
		}
	}
	CDialog::OnTimer(nIDEvent);
}

HBRUSH CTestDllForImateAPIDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	
	if (pWnd->GetDlgCtrlID() == IDC_SNLog)
	{
//		pDC->SetBkColor(RGB(0,255,0));//����ɫΪ��ɫ
		pDC->SetTextColor(RGB(0, 0, 255));//����Ϊ��ɫ
//		pDC->SelectObject(&m_font);//����Ϊ15�����壬�����п�
	}else if(pWnd->GetDlgCtrlID() == IDC_RESULT){
		pDC->SetBkColor(RGB(0,255,0));//����ɫΪ��ɫ
		pDC->SetTextColor(RGB(255, 0, 0));//����Ϊ��ɫ
//		pDC->SelectObject(&m_font);//����Ϊ15�����壬�����п�
	}else if(pWnd->GetDlgCtrlID() == IDC_BigResultLog){
		pDC->SetTextColor(RGB(0, 255, 0));
		pDC->SetBkColor(RGB(0,0,255));//����ɫ
//		pDC->SelectObject(&m_font);//����Ϊ15�����壬�����п�
	}else if(pWnd->GetDlgCtrlID() == IDC_BigResultLog2){
		pDC->SetTextColor(RGB(255, 0, 0));
		pDC->SetBkColor(RGB(0,0,255));//����ɫΪ��ɫ
//		pDC->SelectObject(&m_font);//����Ϊ15�����壬�����п�
	}

	// TODO: Return a different brush if the default is not desired
	return hbr;
}


// �����������ֽ����
// In       : psVect1  : Ŀ�괮
//            psVect2  : Դ��
//            iLength  : �ֽ���
void vXor(unsigned char *psVect1, const unsigned char*psVect2, int iLength)
{
    int i;
    for(i=0; i<iLength; i++)
        psVect1[i] ^= psVect2[i];
}


DWORD _stdcall ThreadDownloadMK(LPVOID lpParameter)//�߳�ִ�к���
{
	
	if(isWorkInThread==1)
	{
		g_DownloadResultLog->SetWindowText("��ȴ���һ�β����Ľ��");
		return 0;
	}
	isWorkInThread = 1;
	g_BigResultLog->SetWindowText("");
	g_BigResultLog2->SetWindowText("");

	if(!HXiMate_DeviceIsConnecting()){
		g_DownloadResultLog->SetWindowText("�豸δ����");
		g_BigResultLog2->SetWindowText("FAIL");
		isWorkInThread = 0;
		return 0;
	}

	if(HXiMate_DeviceIsWorking())
	{
		g_DownloadResultLog->SetWindowText("�豸æ...���Ժ�����");
		g_BigResultLog2->SetWindowText("FAIL");
		isWorkInThread = 0;
		return 0;
	}

	char serialNo[50]={0};
	int ret;
	ret = HXiMate_DeviceSerialNumber(serialNo);
	if(ret<=0){
		// ��ȡ�ն����к�ʧ��
		g_DownloadResultLog->SetWindowText("��ȡ�ն����к�ʧ��");
		isWorkInThread = 0;
		g_BigResultLog2->SetWindowText("FAIL");
		return 0;
	}

	char TermSN[20]={0};
	memcpy(TermSN,serialNo+ret-10,10);

	
	//0. ����������(����)
	if(g_MK1->GetWindowTextLength()!=16)
	{
		//�������ȴ���
		g_DownloadResultLog->SetWindowText("����һ���ȴ���");
		isWorkInThread = 0;
		g_BigResultLog2->SetWindowText("FAIL");
		return 0;
	}

	if(g_MK2->GetWindowTextLength()!=16)
	{
		//�������ȴ���
		g_DownloadResultLog->SetWindowText("���������ȴ���");
		isWorkInThread = 0;
		g_BigResultLog2->SetWindowText("FAIL");
		return 0;
	}

	if(g_MKCRC->GetWindowTextLength()!=8)
	{
		//�������ȴ���
		g_DownloadResultLog->SetWindowText("��ԿУ��ֵ���ȴ���");
		isWorkInThread = 0;
		g_BigResultLog2->SetWindowText("FAIL");
		return 0;
	}

	if(g_SNCRC->GetWindowTextLength()!=8)
	{
		//�������ȴ���
		g_DownloadResultLog->SetWindowText("�豸У��ֵ���ȴ���");
		isWorkInThread = 0;
		g_BigResultLog2->SetWindowText("FAIL");
		return 0;
	}

	CString masterkey1,masterkey2,snCRC,mkCRC;
	g_MK1->GetWindowText(masterkey1);
	g_MK2->GetWindowText(masterkey2);
	g_MKCRC->GetWindowText(mkCRC);
	g_SNCRC->GetWindowText(snCRC);

	//1. ��װ����Կ
	CString masterKey = masterkey1 + masterkey2;
	HXiMate_PinPad_SetModel(PINPAD_MODEL_HX);
	ret = HXiMate_PinPad_PowerOn();
	if(ret!=0)
	{
		g_DownloadResultLog->SetWindowText("�ϵ�ʧ��");
		isWorkInThread = 0;
		g_BigResultLog2->SetWindowText("FAIL");
		return 0;	
	}

	int strLength = masterKey.GetLength() + 1;
    char *pValue = new char[strLength];
	unsigned char outData[100] = {0};
    strncpy(pValue, masterKey, strLength);
	HXiMate_TwoOne((unsigned char *)pValue, 32, outData);
	ret = HXiMate_PinPad_DownloadMasterkey(2, 1, outData, (strLength-1)/2);
	if(ret!=0)
	{
		g_DownloadResultLog->SetWindowText("��װ��Կʧ��");
		isWorkInThread = 0;
		g_BigResultLog2->SetWindowText("FAIL");
		return 0;
	}

	//2. ����ԿУ��ֵ���Ƚ�
	unsigned char data[16]={0};
	memset(outData,0,sizeof(outData));
	ret = HXiMate_PinPad_Encrypt(2, ALGO_ENCRYPT, 1, -1, data , outData, 16);
	if(ret < 0)
	{
		g_DownloadResultLog->SetWindowText("����У��ֵʧ��");
		isWorkInThread = 0;
		g_BigResultLog2->SetWindowText("FAIL");
		return 0;
	}

	strLength = mkCRC.GetLength() + 1;
    pValue = new char[strLength];
	strncpy(pValue, mkCRC, strLength);
	if(strncmp(pValue,(const char*)outData,8)!=0)
	{
		g_DownloadResultLog->SetWindowText("��ԿУ��ֵ����");
		isWorkInThread = 0;
		g_BigResultLog2->SetWindowText("FAIL");
		return 0;
	}


	//3. ������Կ�� ���к���SM3 ɢ��
	strLength = strlen(TermSN);
	memset(outData,0,sizeof(outData));
	ret = HXiMate_PinPad_Hash(2, (unsigned char*)TermSN, strLength, outData);
	if(ret < 0)
	{
		g_DownloadResultLog->SetWindowText("����SM3ɢ��ʧ��");
		isWorkInThread = 0;
		g_BigResultLog2->SetWindowText("FAIL");
		return 0;
	}

	//4. ��� ���Ҳ��ֵ�ɢ��ֵ
	unsigned char tmpData[32];
	unsigned char leftData[16];
	unsigned char rightData[16];
	HXiMate_TwoOne(outData, 64, tmpData);
	memcpy(leftData,tmpData,16);
	memcpy(rightData,tmpData+16,16);
	vXor(leftData,rightData,16);


	//5. ������Կ���� 4 �Ľ�����õ��豸У��ֵ
	memset(outData,0,sizeof(outData));
	ret = HXiMate_PinPad_Encrypt(2, ALGO_ENCRYPT, 1, -1, leftData , outData, 16);
	if(ret < 0)
	{
		g_DownloadResultLog->SetWindowText("�����豸У��ֵʧ��");
		isWorkInThread = 0;
		g_BigResultLog2->SetWindowText("FAIL");
		return 0;
	}

	strLength = snCRC.GetLength() + 1;
    pValue = new char[strLength];
	strncpy(pValue, snCRC, strLength);
	if(strncmp(pValue,(const char*)outData,8)!=0)
	{
		g_DownloadResultLog->SetWindowText("�豸У��ֵ����");
		isWorkInThread = 0;
		g_BigResultLog2->SetWindowText("FAIL");
		return 0;
	}

	//6. �Ƚ�У��ֵ�Ƿ���ȷ��
	g_DownloadResultLog->SetWindowText("��װ����Կ�ɹ�");
	isWorkInThread = 0;
	g_BigResultLog->SetWindowText("OK");
	return 0;
}


void CTestDllForImateAPIDlg::OnDownloadMasterKey() 
{
	m_resultLog.SetWindowText("");
	//�����̣߳���������Կ
	CreateThread(NULL,0,ThreadDownloadMK,NULL,0,NULL);//����һ���̣߳�ȥִ��ThreadDownloadMK����
}

BOOL CTestDllForImateAPIDlg::PreTranslateMessage(MSG* pMsg)
 {
	//����ESC�رմ���/
	if(pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_ESCAPE ){
		return TRUE;
	}
	//���λس��رմ���,���ᵼ�»س��ڴ�����ʧЧ.�� OnOk()
	if(pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_RETURN && pMsg->wParam) 
		return TRUE;

	return CDialog::PreTranslateMessage(pMsg);
 }

void CTestDllForImateAPIDlg::OnChangemasterkey2EDIT() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	int length = m_MK2edit.GetWindowTextLength();
	char buff[10]={0};
	sprintf(buff,"%d",length);
	m_mk2Lenght.SetWindowText(buff);
}

void CTestDllForImateAPIDlg::OnChangemasterkey1EDIT() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	int length = m_MK1edit.GetWindowTextLength();
	char buff[10]={0};
	sprintf(buff,"%d",length);
	m_mk1Length.SetWindowText(buff);
}

void CTestDllForImateAPIDlg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	//�رն�ʱ��
	KillTimer(1);
	HXiMate_DeviceClose();
	CDialog::OnClose();
}
