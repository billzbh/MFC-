// ATM_DEMODlg.cpp : implementation file
//

#include "stdafx.h"
#include "ATM_DEMO.h"
#include "ATM_DEMODlg.h"

#include "KMY350X.h"
#include "KDMexp.h"
#include "des.h"
#include "str.h"
#include "HidFace.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#pragma comment(lib,"KDMexp.lib") 


#define VENDOR_ID		0x0483
#define PRODUCT_ID		0x5750

char g_bComNum[5];		// 端口号"COM1"

struct TMK{
	char termid[21];	// 终端编号
	char kval[49];		// 终端密钥
	int  klen;			// 密钥长度
	char kchk[7];		// 校验值
}tmk,testTMK;

char g_sAtmCode[21];	
char g_strTmk[49];		// 
int  g_sLen = 0;
int  g_POSport = 1;
int  g_MasterIndex = 0;
int g_transfer = 0;
int hidStatus = -1;

unsigned char gl_ucWorking = 0;
unsigned char g_TransKey[24]; // 记录由随机数和终端编号的合成密钥
unsigned char g_tLen = 0;

unsigned char g_ThreadState; // 0 线程关闭 1 线程自动关闭 2线程运行


HANDLE	m_hThreadEvent;
HANDLE	hComHand;
HWND	hWnd;

CEdit* g_editLog = NULL;
CEdit* g_check = NULL;
CEdit* g_rand = NULL;
CEdit* g_term = NULL;
CEdit* g_len = NULL;
CButton* g_selectHidOrCom = NULL;

CWinThread* pThread;

BOOL	b_AtmGoing;


void Log(CString info);
void ShowTmkInfo();


static int iHIDSendReceive(unsigned char *sendBuff, int sendLength, 
						unsigned char *receiveBuff, int *receiveLength, 
						int timeout)
{
	if ( iGetHidHandle() < 0)
		return IMATE_PORT_NOTOPEN;

	int ret = IMATE_COMM_ERROR;

	unsigned char packBuffer[512];
	packBuffer[0] = 0x02;
	packBuffer[1] = sendLength;

	unsigned char ucCheck = 0x03;
	for (int i = 0; i < sendLength; i++) {
		packBuffer[i+2] = sendBuff[i];
		ucCheck ^= sendBuff[i];
	}
	packBuffer[2+sendLength] = 0x03;
	packBuffer[3+sendLength] = ucCheck;

	ucHidReset();
	if (ucHidSendBuf(packBuffer, sendLength + 4))
		return IMATE_COMM_ERROR;

	long t = GetTickCount();
	unsigned char buffer[10];
	while (1) {
		if ( GetTickCount()-t > (ULONG)timeout*1000)
			return IMATE_COMM_ERROR;
		Sleep(1);
		if (ucHidTest() == 0)
			continue;
		if (ucHidGetBufBymillsecond(buffer, 1, 20))
			return IMATE_COMM_ERROR;
		if (buffer[0] != 0x02)
			continue;

		if (ucHidGetBufBymillsecond(buffer, 1, 20))
			return IMATE_COMM_ERROR;

		int length = buffer[0];
		if (ucHidGetBuf(receiveBuff, length, 1))
			return IMATE_COMM_ERROR;

		ucHidReset();
		*receiveLength = length;
		break;
	}
	return 0;
}

int LoadHXpinpadMasterKey(int algorithm,int index,unsigned char * masterKey,int keyLength)
{
	 unsigned char receivedBytes[50];
     unsigned char sendBytes[50];
        
     sendBytes[0] = (char)0x90;
     sendBytes[1] = 0; //dowload master key
     sendBytes[2] = (char)index;
     memcpy(sendBytes + 3, masterKey, keyLength);
     if (keyLength == 8)
         memcpy(sendBytes + 3 + 8, masterKey, keyLength);

	 int outlength=0;
	 int iRet = iHIDSendReceive(sendBytes,3+16,receivedBytes,&outlength,2);
	 if (iRet > 0 && receivedBytes[0]) {
		return -2;//发生错误
	 }
	 if(iRet==-1)
	 {
		return -1;//超时
	 }
	 return 0;
}

int WriteTermId(unsigned char* termId,int length){

	unsigned char receivedBytes[50];
    unsigned char sendBytes[50];
	memset(sendBytes, 0, sizeof(sendBytes));
	sendBytes[0] = 0x68;
    sendBytes[1] = 0x04;
    sendBytes[2] = 0;
    sendBytes[3] = 20;
    sendBytes[4] = 0;
    sendBytes[5] = 13;
    memcpy(sendBytes+6, termId, length);

	int outlength=0;
	int iRet = iHIDSendReceive(sendBytes,6+length,receivedBytes,&outlength,2);
	 if (iRet > 0 && receivedBytes[0]) {
		return -2;//发生错误
	 }
	 if(iRet==-1)
	 {
		return -1;//超时
	 }
	 return 0;
}

int readTermId(unsigned char* termId)
{
	unsigned char receivedBytes[50];
    unsigned char sendBytes[50];
	memset(sendBytes, 0, sizeof(sendBytes));
	sendBytes[0] = 0x68;
    sendBytes[1] = 0x03;
    sendBytes[2] = 0;
    sendBytes[3] = 20;
    sendBytes[4] = 0;
    sendBytes[5] = 13;
	int outlength=0;
	int iRet = iHIDSendReceive(sendBytes,6,receivedBytes,&outlength,2);
	 if (iRet > 0 && receivedBytes[0]) {
		return -2;//发生错误
	 }
	 if(iRet==-1)
	 {
		return -1;//超时
	 }

	 memcpy(termId,receivedBytes+1,outlength-1);
	 return 0;

}


// 字符转换
int StringToHex(unsigned char *hex,unsigned char *str,unsigned int len)
{
	unsigned int i;
	unsigned char tempH,tempL;

	for(i=0; i<len; i++)
	{
	   tempH = *str++;
	   tempL = *str++;
	   if(tempH >= '0' && tempH <= '9') tempH -= '0';
	   else
	   if(tempH >= 'A' && tempH <= 'F') tempH = tempH - 'A' + 0x0a;
	   else return -1;
	   if(tempL >= '0' && tempL <= '9') tempL -= '0';
	   else
	   if(tempL >= 'A' && tempL <= 'F') tempL = tempL - 'A' + 0x0a;
	   else return -1;
	   *hex++ = (tempH << 4) | tempL;
	}
	return 0;
}

void HexToString(unsigned char *hex,unsigned char *str,unsigned int len)
{
	unsigned int i;
	unsigned char temp;
	unsigned char tmp;

	for(i=0; i<len; i++)
	{
		temp = *hex++;
		tmp  = temp >> 4;
		temp&= 0x0f;
		if(tmp < 0x0a) *str++ = tmp + '0';
		else		   *str++ = tmp + 'A'-0x0a;
		if(temp < 0x0a) *str++ = temp + '0';
		else		    *str++ = temp + 'A'-0x0a;
	}
}


// 打开串口
int ComOpen(HANDLE &hComHand,char * Comport,long iBaudRate,unsigned int time_set)
{
	hComHand = CreateFile(Comport,	// 端口号
				GENERIC_READ|GENERIC_WRITE, // 允许读和写
				0, // 独占方式
				NULL,
				OPEN_EXISTING, // 打开而不是创建
				0, // 0同步方式 ; FILE_FLAG_OVERLAPPED 异步方式
				NULL);

	if(hComHand == INVALID_HANDLE_VALUE)
	{
		return -1;  // 打开失败，返回-1
	}
	
	SetupComm(hComHand,19620,3000); //设置接收和发射缓冲区
	
	COMMTIMEOUTS TimeOuts;       //超时参数设置
	//设定读超时
	TimeOuts.ReadIntervalTimeout		= 6;
	TimeOuts.ReadTotalTimeoutMultiplier	= 6;
	TimeOuts.ReadTotalTimeoutConstant	= 100; // time_set
	
	//设定写超时
	TimeOuts.WriteTotalTimeoutMultiplier = 100;
	TimeOuts.WriteTotalTimeoutConstant   = 500;
	SetCommTimeouts(hComHand,&TimeOuts); //设置超时 ms
	
	//通讯参数设置
	DCB dcb;
	GetCommState(hComHand,&dcb);
	dcb.BaudRate = iBaudRate;	//波特率为9600
	dcb.ByteSize = 8;			//每个字节有8位
	dcb.Parity   = NOPARITY;	//无奇偶校验位
	dcb.StopBits = 0;			//两个停止位
	dcb.EvtChar  = (char)0xC0;	//收到0xC0触发事件
	SetCommState(hComHand,&dcb);//设置通讯参数

 	SetCommMask(hComHand,EV_RXCHAR); // 设置串口等待事件
	
	//终止目前正在进行
	PurgeComm(hComHand, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);
	
	return 0;
}

// 判断串口是否打开
BOOL IsOpen(HANDLE hComHand)  
{ 
	if (hComHand == INVALID_HANDLE_VALUE)
		return false; 	
	else
		return true; 
}

// 关闭串口
int ComClose(HANDLE hComHand)
{
	if ( IsOpen(hComHand) )
	{
		PurgeComm(hComHand, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);
		BOOL bSuccess = CloseHandle(hComHand);
		if(bSuccess)
		{ //关闭成功
			hComHand = INVALID_HANDLE_VALUE;
			return 0; 
		}
		else
			return -1;
	}
	else 
		return 0;	// COM口本来就没有打开
}

// 读取一个字节
int ReadByte(HANDLE hComHand,unsigned char * data_buff)
{
	unsigned long fact_RDdata_count = 0;
	BOOL bReadStat;

	if( IsOpen(hComHand) )
	{
		bReadStat = ReadFile(hComHand, data_buff, 1, &fact_RDdata_count, NULL);
		if (!bReadStat || (fact_RDdata_count != 1))
		{
			return -1;	
		}
		return 0;
	}
	else 
	{//串口未打开
		return -2;
	}
}

// 读串口缓冲区
int ReadCom(HANDLE hComHand,unsigned char * data_buff,unsigned int data_count)
{
	unsigned long fact_RDdata_count = 0;
	BOOL bReadStat;
	
	if( IsOpen(hComHand) )
	{
		bReadStat = ReadFile(hComHand, &data_buff[0], data_count, &fact_RDdata_count, NULL);
		if (!bReadStat || (fact_RDdata_count != data_count))
		{
			PurgeComm(hComHand, PURGE_TXABORT | PURGE_RXABORT|PURGE_TXCLEAR|PURGE_RXCLEAR);
			return -1;	
		}

	    PurgeComm(hComHand, PURGE_TXABORT | PURGE_RXABORT|PURGE_TXCLEAR|PURGE_RXCLEAR);
		return 0;
	}
	else 
	{//串口未打开
		return -2;
	}
}

// 写串口缓冲区
int WriteCom(HANDLE hComHand,unsigned char * data_buff,unsigned int data_count)
{
	BOOL bWriteStat;
	unsigned long fact_WRdata_count = 0;
	
	if( IsOpen(hComHand) )
	{	
		//清空收发缓冲区，停止后台读、写操作。
		//PurgeComm(hComHand, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);
		PurgeComm(hComHand, PURGE_TXABORT | PURGE_TXCLEAR);
		bWriteStat = WriteFile(hComHand, data_buff,data_count,&fact_WRdata_count, NULL);
		if (!bWriteStat && ((unsigned int)fact_WRdata_count != data_count))
		{	
			return -1;	
		}
			
		return 0;//成功
	}
	else
		return -2;//串口未打开
}

//DWORD WINAPI 
UINT ListenThread(
	 LPVOID lpParameter // thread data
);

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
// CATM_DEMODlg dialog

CATM_DEMODlg::CATM_DEMODlg(CWnd* pParent /*=NULL*/)
	: CDialog(CATM_DEMODlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CATM_DEMODlg)
	m_rand = _T("");
	m_Check = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CATM_DEMODlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CATM_DEMODlg)
	DDX_Control(pDX, IDC_selectHIDorCOM, m_selectHIDorCOM);
	DDX_Control(pDX, IDC_COMBO2, m_Masterindex);
	DDX_Control(pDX, IDC_COMOPEN, m_open);
	DDX_Control(pDX, IDC_COMBO1, m_mposCom);
	DDX_Control(pDX, IDC_TMK_LEN, m_tmklen);
	DDX_Control(pDX, IDC_TERM_CODE, m_TermCode);
	DDX_Control(pDX, IDC_CHECK, m_check);
	DDX_Control(pDX, IDC_RAND, m_Rand);
	DDX_Control(pDX, IDC_COMBO_COMM, m_Comm);
	DDX_Control(pDX, IDC_EXCUTE, m_bntExcute);
	DDX_Control(pDX, IDC_LOG, m_editLog);
	DDX_Text(pDX, IDC_CHECK, m_Check);
	DDV_MaxChars(pDX, m_Check, 6);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CATM_DEMODlg, CDialog)
	//{{AFX_MSG_MAP(CATM_DEMODlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_CBN_SELCHANGE(IDC_COMBO_COMM, OnSelchangeComm)
	ON_BN_CLICKED(IDC_EXCUTE, OnExcute)
	ON_BN_CLICKED(IDC_CLEAR_LOG, OnClearLog)
	ON_CBN_SELCHANGE(IDC_COMBO1, OnSelectComPort)
	ON_BN_CLICKED(IDC_COMOPEN, OnComopen)
	ON_CBN_SELCHANGE(IDC_COMBO2, OnSelchangeCombo2)
	ON_BN_CLICKED(IDC_selectHIDorCOM, OnselectHIDorCOM)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CATM_DEMODlg message handlers

BOOL CATM_DEMODlg::OnInitDialog()
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
	m_Comm.SetCurSel(1);
	m_mposCom.SetCurSel(0);
	m_Masterindex.SetCurSel(0);
	GetCommNum(g_bComNum);
	g_editLog = &m_editLog;
	g_check = &m_check;
	g_rand = &m_Rand;
	g_term = &m_TermCode;
	g_len = &m_tmklen;
	g_selectHidOrCom = &m_selectHIDorCOM;

	SetTimer(1,500,NULL);//启动定时器1,定时时间是1秒

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CATM_DEMODlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CATM_DEMODlg::OnPaint() 
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
HCURSOR CATM_DEMODlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CATM_DEMODlg::GetCommNum(char *com_buff)
{
	CString str;

	int index = m_Comm.GetCurSel();
	m_Comm.GetLBText(index,str);

	strncpy(com_buff,(char *)(LPCTSTR)str,4);
}

void CATM_DEMODlg::OnSelchangeComm() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	GetCommNum(g_bComNum);
}


void Write_Log(char filename[], char log[])
{
	HANDLE hFile;

	hFile = CreateFile(filename,GENERIC_WRITE,FILE_SHARE_READ|FILE_SHARE_WRITE,NULL,OPEN_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);

	DWORD dwHigh;
	DWORD dwPos = GetFileSize(hFile,&dwHigh);
	SetFilePointer(hFile,dwPos,0,FILE_BEGIN);

	DWORD dwWrites;
	WriteFile(hFile,log,strlen(log),&dwWrites,NULL);
	WriteFile(hFile,"\r\n",strlen("\r\n"),&dwWrites,NULL);
	CloseHandle(hFile);
}


void CATM_DEMODlg::OnExcute() 
{
	// TODO: Add your control notification handler code here
	CString str;
 
	UpdateData(TRUE);
	if(GetDlgItemText(IDC_EXCUTE,str),str == "停止模拟")
	{
		g_ThreadState = 1;
		Log("串口监听线程结束！");
		if(ComClose(hComHand) == 0)
		{ // 成功关闭串口
			b_AtmGoing = FALSE;
			SetDlgItemText(IDC_EXCUTE,"开始模拟");
			GetDlgItem(IDC_COMBO_COMM)->EnableWindow(TRUE);
			Log("ATM终端模拟程序已被终止！");
		}
	}
	else
	{
		if( ComOpen(hComHand,g_bComNum,9600,9000) !=0)
		{
			Log("串口打开失败!");
			return;
		}
		b_AtmGoing = TRUE;
		SetDlgItemText(IDC_EXCUTE,"停止模拟");
		GetDlgItem(IDC_COMBO_COMM)->EnableWindow(FALSE);

		str = "开始监听端口：";
		str += g_bComNum;
		Log(str);

		ClearRand(); // 清编号和终端密钥等

		m_hThreadEvent = CreateEvent(NULL,FALSE,FALSE,NULL);

		hWnd = GetSafeHwnd();
		pThread = AfxBeginThread(ListenThread,hWnd);
		//pThread->m_bAutoDelete = FALSE;
		g_ThreadState = 2;
	}
}



void CATM_DEMODlg::ClearRand(void)
{
	CString str = "";

	g_tLen = 0;

	g_term->SetWindowText(str);
	g_len->SetWindowText(str);

	g_rand->GetWindowText(str);
	str.Empty();
	g_rand->SetWindowText(str);

	g_check->GetWindowText(str);
	str.Empty();
	g_check->SetWindowText(str);
}

void ClearRand(void)
{
	CATM_DEMODlg::ClearRand();
}

UINT ListenThread(
	 LPVOID lpParameter // thread data
)
{
	unsigned char buff[100];
	unsigned char cnt = 0;
	unsigned char temp;
	unsigned char f_slipHead = 0;
	unsigned char pack[100];
	unsigned char len = 0;

	CString s;
	CString str;
	CTime time;

	for(;;)
	{
		if(g_ThreadState == 1)
		{
			g_ThreadState = 0;
			return 0;
		}

		if(ReadByte(hComHand,&temp) != 0) continue;

		if(temp == 0xC0)
		{
			if(f_slipHead == 0 || cnt == 1)
			{	// 第一个C0或连续两个C0
				f_slipHead = 1;
				cnt = 0;
				buff[cnt++] = temp;
				continue;
			}
			else
			{	// 收到完整数据包
				buff[cnt++] = temp;
			}
		}
		else
		{
			if(f_slipHead == 1)
			{	// 有效数据
				buff[cnt++] = temp;
			}
			continue;
		}

		memset(&tmk,0,sizeof(struct TMK));
		temp = dispatchData(cnt,buff,tmk.termid,tmk.kval);
		
		f_slipHead = 0; // 清数据包标识
		cnt = 0;

		if(temp == 0)
		{
			Log("收到终端主密钥数据包有误!");
		}
		else
		{
			tmk.klen = strlen(tmk.kval);

			Log("收到终端主密钥!");
			str = "密钥明文值：";
			str += tmk.kval;
			Log(str);

			StringToHex(pack,(unsigned char *)tmk.kval,tmk.klen);
			CalculateCheck(pack,tmk.klen/2,(unsigned char *)tmk.kchk);
			str = "密钥校验值：";
			str += tmk.kchk;
			Log(str);

			Log("密钥分发完成,等待灌主密钥...");

			int iret=-1;
			if(g_transfer==0){
				iret = OpenPort(g_POSport,9600);
			}else if(g_transfer==1){
				iret = ucHidTestOnLine();
			}

			if(iret==0){

				Log("密码键盘串口打开成功");
				Log("密钥长度："+tmk.klen);
				if(tmk.klen%16==0)
				{
					char szTemp[49];
					unsigned char UcReData[100]={0};
					memset(szTemp,0,sizeof(szTemp));
					sprintf(szTemp,"%s",tmk.kval);
					
					int iret2=-1;
					if(g_transfer==0){
						iret2 = LoadMainKey(g_MasterIndex,(unsigned char*)(LPCTSTR)szTemp,UcReData); //oneTwo后的数据
					}else if(g_transfer==1){
						unsigned char pack3[100];
						StringToHex(pack3,(unsigned char*)szTemp,tmk.klen);
						iret2 = LoadHXpinpadMasterKey(1,0,pack3,tmk.klen/2);//3des
					}

					if (iret2 == 0)
					{
						Log("下载主密钥成功");
					}
					else{
						Log("下载主密钥失败");
						Log("终止下载主密钥");
					}
				}else{
					Log("主密钥长度不符合为8的倍数");
					Log("终止下载主密钥");
				}


				char szTermId[9];
				int length = strlen(tmk.termid);
				if(length>8)
					Log("终端号长度大于8,更新终端序列号失败");
				else{
					memset(szTermId,0,sizeof(szTermId));
					sprintf(szTermId,"%s",tmk.termid);
					int iret3=-1;
					if(g_transfer==0){
						iret3 = SetClientSN((unsigned char*)(LPCTSTR)szTermId);
					}else if(g_transfer==1){
						iret3 = WriteTermId((unsigned char*)(LPCTSTR)szTermId,length);
					}

					if(iret3==0)
					{
						Log("更新终端序列号成功！");
					}else{
						Log("更新终端序列号失败！");
					}
				}
                
			}else{
				Log("打开失败");
			}

            ClosePort();

			time = CTime::GetCurrentTime();
			s = time.Format(TEXT("%Y%m%d%H%M%S"));
			str = "发送应答时间：";
			str += s;
			Log(str);

			memcpy(buff,(char *)(LPCTSTR)s+2,12);
			len = assembleMsg(1,12,buff,pack);
			if(WriteCom(hComHand,pack,len) != 0)
			{
				Log("应答发送失败！");
			}



			ShowTmkInfo();
		}
	}

	return 0;
}


void CATM_DEMODlg::Log(CString info)
{
	CString str;

	if(g_editLog)
	{
		g_editLog->GetWindowText(str);
		str += "--> ";
		str += info;
		str += "\r\n";
		g_editLog->SetWindowText(str);
	}
}


void Log(CString info)
{
	CATM_DEMODlg::Log(info);
}

void CATM_DEMODlg::OnClearLog() 
{
	// TODO: Add your control notification handler code here
	CString str;

	if(g_editLog)
	{
		g_editLog->GetWindowText(str);
		str.Empty();
		g_editLog->SetWindowText(str);
	}

	g_term->SetWindowText(str);
	g_rand->SetWindowText(str);
	g_check->SetWindowText(str);
	g_len->SetWindowText(str);
}

void CATM_DEMODlg::ShowTmkInfo()
{
	CString str;

	str.Format("%s",tmk.termid);
	g_term->SetWindowText(str);
	str.Format("%s",tmk.kval);
	g_rand->SetWindowText(str);
	str.Format("%s",tmk.kchk);
	g_check->SetWindowText(str);
	str.Format("%d",tmk.klen);
	g_len->SetWindowText(str);
}

void ShowTmkInfo()
{
	CATM_DEMODlg::ShowTmkInfo();
}

void CATM_DEMODlg::OnSelectComPort() 
{
    CString strSelect;

	int index = m_mposCom.GetCurSel();
	m_mposCom.GetLBText(index,strSelect);
	
	CString str;
    str = "选择MPOS串口为：" + strSelect;
	Log(str);

	g_POSport = index+1;
}



DWORD _stdcall ThreadProc(LPVOID lpParameter)//线程执行函数
{
    CTime time;
	CString s;

    //测试数据
	// 定义密码键盘接收到密钥分发器的数据包(注：数据包长度不定长)
	// 编号：	term012
	// 主密钥：	12345678901234567890123456789012
	unsigned char pack[77] = {0xc0,'2',0x52,64,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,'t','e','r','m','0','1','2',0,0,0,0,0,0,0,0,0,0,0,0,0,
0x01,0x23,0x45,0x67,0x89,0xab,0xcd,0xef,0x01,0x23,0x45,0x67,0x89,0xab,0xcd,0xef,0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88,
16,0x7D,0x44,0xCB,0xE5,0xD7,0x82,0xCA,0xC8,0x19,0xAB,0xAE,0x1C,0xD6,0x11,0x03,0x0A,0x60,0xC1,0x52,0xc0};

	
	memset(&testTMK,0,sizeof(struct TMK));
	unsigned char buff[100];
    unsigned char pack2[100];
	int temp = dispatchData(77,pack,testTMK.termid,testTMK.kval);
    if(temp == 0)
	{
		Log("收到终端主密钥数据包有误!");
	}else{

			Log("收到终端主密钥!");
			CString str = "密钥明文值：";
			str += testTMK.kval;
			Log(str);

			char lengh[10]={0};
			testTMK.klen = strlen(testTMK.kval);
			str = "密钥长度:";
			DecToAscii2(lengh,testTMK.klen);// 32 个字节
			str += lengh;
			Log(str);

			StringToHex(pack2,(unsigned char *)testTMK.kval,testTMK.klen);
			CalculateCheck(pack2,testTMK.klen/2,(unsigned char *)testTMK.kchk);
			str = "密钥校验值：";
			str += testTMK.kchk;
			Log(str);
            str = "终端号：";
            str += testTMK.termid;
			Log(str);



			Log("密钥分发完成,等待灌主密钥...");

			int iret=-1;
			if(g_transfer==0){
				iret = OpenPort(g_POSport,9600);
			}else if(g_transfer==1){
				iret = ucHidTestOnLine();
			}

			if(iret==0){

				CString str2;
				str2.Format("%s",testTMK.kchk);
				g_check->SetWindowText(str2);
				str2.Format("%d",testTMK.klen);
				g_len->SetWindowText(str2);
				
				Log("密码键盘串口打开成功");
				if(testTMK.klen%16==0)
				{
					char szTemp[49];
					unsigned char UcReData[100]={0};
					memset(szTemp,0,sizeof(szTemp));
					sprintf(szTemp,"%s",testTMK.kval);

					int iret2=-1;
					if(g_transfer==0){
						iret2 = LoadMainKey(g_MasterIndex,(unsigned char*)(LPCTSTR)szTemp,UcReData); //oneTwo后的数据
					}else if(g_transfer==1){
						unsigned char pack3[100];
						StringToHex(pack3,(unsigned char*)szTemp,testTMK.klen);
						iret2 = LoadHXpinpadMasterKey(1,0,pack3,testTMK.klen/2);//3des
					}

					if (iret2 == 0)
					{
						Log("下载主密钥成功");
						str2.Format("%s",testTMK.kval);
						g_rand->SetWindowText(str2);
					}
					else{
						Log("下载主密钥失败");
						Log("终止下载主密钥");
					}
				}else{
					Log("主密钥长度不符合为8的倍数");
					Log("终止下载主密钥");
				}


				char szTermId[9];
				int length = strlen(testTMK.termid);
				if(length>8)
					Log("终端号长度大于8,更新终端序列号失败");
				else{
					memset(szTermId,0,sizeof(szTermId));
					sprintf(szTermId,"%s",testTMK.termid);
					
					int iret3=-1;
					if(g_transfer==0){
						iret3 = SetClientSN((unsigned char*)(LPCTSTR)szTermId);
					}else if(g_transfer==1){
						iret3 = WriteTermId((unsigned char*)(LPCTSTR)szTermId,length);
					}

					if(iret3==0)
					{
						Log("更新终端序列号成功！");
						str2.Format("%s",testTMK.termid);
						g_term->SetWindowText(str2);

						//测试读取序列号
						unsigned char snbuf[60] = {0};
						unsigned char snText[9] = {0};
						int iret4 = -1;
						if(g_transfer==0){
							iret4 = ReadClientSN(snbuf);
						}else if(g_transfer==1){
							iret4 = readTermId(snbuf);
						}

						if(iret4==0){
						
							DES_StringToHex((char*)(LPCTSTR)snbuf,16,snText);
							CString SNtip;
							SNtip = "读取终端序列号：";
							SNtip += snText;
							Log(SNtip);

						}else
						{
							Log("读取终端序列号失败！");
						}
					}else{
						Log("更新终端序列号失败！");
					}
				}
                
			}else{
				Log("打开失败");
			}

            ClosePort();

			time = CTime::GetCurrentTime();
			s = time.Format(TEXT("%Y%m%d%H%M%S"));
			str = "发送应答时间：";
			str += s;
			Log(str);

			memcpy(buff,(char *)(LPCTSTR)s+2,12);
			int len = assembleMsg(1,12,buff,pack);
			if(WriteCom(hComHand,pack,len) != 0)
			{
				Log("应答发送失败！模拟时没有分发器，发送失败是正常的");
			}

	}

	int iret = ClosePort();
   	if(iret == 0)
		Log("串口关闭成功");
	else
		Log("串口关闭失败");

	Log("测试完成");
	return 0;
}

void CATM_DEMODlg::OnComopen() 
{
	
	CreateThread(NULL,0,ThreadProc,NULL,0,NULL);//创建一个线程，去执行ThreadProc函数
}

BOOL CATM_DEMODlg::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	ClosePort();
	ComClose(hComHand);
	return CDialog::DestroyWindow();
}

void CATM_DEMODlg::OnSelchangeCombo2() 
{
	g_MasterIndex = m_Masterindex.GetCurSel();
}


DWORD WINAPI ThreadConnect (PVOID pParam)
{
	HWND hWnd = (HWND)pParam;
	gl_ucWorking = 1;

	ucHidClose();
	ucHidOpen(VENDOR_ID, PRODUCT_ID);

	gl_ucWorking = 0;

	return 0;
}


void CATM_DEMODlg::OnselectHIDorCOM() 
{
	// TODO: Add your control notification handler code here

	if ( BST_CHECKED == IsDlgButtonChecked( IDC_selectHIDorCOM ) )
	{
		// 勾选
		g_transfer = 1;
		Log("选择HID-USB通讯");
		//关闭KMY串口
		ClosePort();
	}
	else
	{
		g_transfer = 0;
		hidStatus = -1;
		Log("选择COM通讯");
		//关闭HID
		ucHidClose();
	}
}

void CATM_DEMODlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if(g_transfer==1){

		if (ucHidTestOnLine() == 0) {		
			if (hidStatus != 1) {
				Log("设备状态：HID-USB已连接");
				hidStatus = 1;
			}		
		}
		else {
			if (gl_ucWorking == 0) {
				HANDLE hThread;
				hThread=CreateThread(NULL,0,ThreadConnect,m_hWnd,0,NULL);
			}

			if (hidStatus != 0) {
				Log("设备状态：HID-USB未连接");
				hidStatus = 0;
			}
		}	
	}
	CDialog::OnTimer(nIDEvent);
}
