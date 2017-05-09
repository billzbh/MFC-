// testdllDlg.cpp : implementation file
//

#include "stdafx.h"
#include "testdll.h"
#include "testdllDlg.h"

typedef unsigned char(*DisPatchData)(char*,char*, char*, char*);
typedef unsigned char(*DetectDevice)(char*);
typedef int(__stdcall *AKP_GetDevStatu )( int iComPort , int iBaudRate , int iComTimeout ,char *pszDevStatu , int *piDevStatuLen );

DetectDevice g_device;		
DisPatchData g_dispatchData;
CEdit* g_editLog = NULL;
HINSTANCE g_hDllInst;
AKP_GetDevStatu getDevStatus;


void Log(CString info,int send){
	CString str;
	if(g_editLog)
	{
		
		g_editLog->GetWindowText(str);
		if(send==1)
			str += "����: ";
		else if(send==2)
			str += "����:";
		else
			str += "--> ";
		str += info;
		str += "\r\n";
		g_editLog->SetWindowText(str);

		/*
		if(send==1)
			str += "����: ";
		else if(send==2)
			str += "����:";
		else
			str += "--> ";
		str += info;
		str += "\r\n";

		int nCount    = 0;
		int nLastLine = 0;
		nCount  =  g_editLog->GetLineCount();              //��ȡ�����������س���
		nLastLine   =  g_editLog->LineIndex( nCount - 1 );   //��ȡ�ַ�������ɶ���
		g_editLog->SetSel( nLastLine + 1, nLastLine + 2 );      //�趨���ѡ�е�����
		g_editLog->ReplaceSel(str);                 //�����滻
		*/
	}
}

void Log(CString info)
{
	Log(info,0);
}

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

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
// CTestdllDlg dialog

CTestdllDlg::CTestdllDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTestdllDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTestdllDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTestdllDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTestdllDlg)
	DDX_Control(pDX, IDC_EDIT1, m_log);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CTestdllDlg, CDialog)
	//{{AFX_MSG_MAP(CTestdllDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, Onsearch)
	ON_BN_CLICKED(IDC_BUTTON2, Ondown)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTestdllDlg message handlers

BOOL CTestdllDlg::OnInitDialog()
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
	g_editLog = &m_log;

	
	g_hDllInst = LoadLibrary(_T("HXiMate401.dll"));
	if(g_hDllInst){
		Log("����dll�ɹ�");
        
	}else{
		Log("����dllʧ��");
		FreeLibrary(g_hDllInst);
	}
	

	return TRUE;  // return TRUE  unless you set the focus to a control
}


DWORD _stdcall ThreadDetectdevice(LPVOID lpParameter)//��ѯ�豸�߳�ִ�к���
{

	
	char buff[25];
	memset(buff,0,25);
	char ret= g_device(buff);
	if(ret!=0)
		Log("��ѯʧ��");
	else{
		Log("���к�:");
		Log(buff);
	}
	return 0;
}

DWORD _stdcall ThreadDownloadKey(LPVOID lpParameter)//����Կ�߳�ִ�к���
{
	char key[16]={0x00, 0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f};
	Log("���صĲ�������Կ���ģ�");
	Log("000102030405060708090A0B0C0D0E0F");
	int ret = g_dispatchData("0000","PF131420","66",key);
	if(ret==1)
		Log("����OK");
	else
		Log("����ʧ��");
	return 0;
}

void CTestdllDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CTestdllDlg::OnPaint() 
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

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CTestdllDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CTestdllDlg::Onsearch() 
{
	// TODO: Add your control notification handler code here

	g_device = (DetectDevice)GetProcAddress(g_hDllInst, "detectClientDev");
	//�ж��Ƿ��ȡ�ɹ�
	if(g_device != NULL)
	{
		//Log("��ȡdll�е�detectClientDev�����ɹ�");
	}else{
		Log("��ȡdll�е�detectClientDev����ʧ��");
		return;
	}
	
	CreateThread(NULL,0,ThreadDetectdevice,NULL,0,NULL);//����һ���̣߳�ȥִ��detectClientDev����
}

void CTestdllDlg::Ondown() 
{
	// TODO: Add your control notification handler code here
	//���ݺ����������Ѽ��ص�DLL�л�ȡ������ַ����ֵ
	
	getDevStatus = (AKP_GetDevStatu)GetProcAddress(g_hDllInst, "AKP_GetDevStatu");

    //�ж��Ƿ��ȡ�ɹ�
	if(getDevStatus != NULL)
	{
		Log("��ȡdll�е�AKP_GetDevStatu�����ɹ�");
	}else{
		Log("��ȡdll�е�AKP_GetDevStatu����ʧ��");
		return;
	}
	
	CreateThread(NULL,0,ThreadDownloadKey,NULL,0,NULL);//����һ���̣߳�ȥִ��dispatchKeyData����
}
