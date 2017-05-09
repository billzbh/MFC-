
// MFCApplicationForWHNSDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MFCApplicationForWHNS.h"
#include "MFCApplicationForWHNSDlg.h"
#include "afxdialogex.h"
#include "HXiMateWin32.h"
#include "HXiMatePinPadWin32.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//定义全局的对象指针
CFont myFont;


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框
class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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


// CMFCApplicationForWHNSDlg 对话框



CMFCApplicationForWHNSDlg::CMFCApplicationForWHNSDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_MFCAPPLICATIONFORWHNS_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCApplicationForWHNSDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_MK1, m_POWERON_text);
	DDX_Control(pDX, IDC_EDIT_MK2, m_TIP_up);
	DDX_Control(pDX, IDC_EDIT_MKALL, m_TIP_down);
	DDX_Control(pDX, IDC_STATIC_STATUS, m_statusLog);
	//  DDX_Control(pDX, IDC_CHECK_isUseXorMK, m_isXorMK);
}

BEGIN_MESSAGE_MAP(CMFCApplicationForWHNSDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_CONNECTDEVICE, &CMFCApplicationForWHNSDlg::OnBnClickedButtonConnectdevice)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON_INITMK, &CMFCApplicationForWHNSDlg::OnClickedButtonSetDisplayText)
	ON_BN_CLICKED(IDC_CHECK1, &CMFCApplicationForWHNSDlg::OnClickedCheck1)
	ON_BN_CLICKED(IDC_CHECK2, &CMFCApplicationForWHNSDlg::OnClickedCheck2)
	ON_BN_CLICKED(IDC_CHECK3, &CMFCApplicationForWHNSDlg::OnClickedCheck3)
END_MESSAGE_MAP()


// CMFCApplicationForWHNSDlg 消息处理程序

BOOL CMFCApplicationForWHNSDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	m_POWERON_text.SetLimitText(20);
	m_TIP_up.SetLimitText(20);
	m_TIP_down.SetLimitText(20);

	myFont.CreatePointFont(110, _T("Consolas"));
	m_POWERON_text.SetFont(&myFont);
	m_TIP_up.SetFont(&myFont);
	m_TIP_down.SetFont(&myFont);

	//初始化一些文字
	m_POWERON_text.SetWindowTextW(_T("深圳华信智能"));
	m_TIP_up.SetWindowTextW(_T("iMate Plus"));
	m_TIP_down.SetWindowTextW(_T("欢迎使用"));

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CMFCApplicationForWHNSDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMFCApplicationForWHNSDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMFCApplicationForWHNSDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CMFCApplicationForWHNSDlg::OnBnClickedButtonConnectdevice()
{
	// TODO: 打开设备
	if (!HXiMate_DeviceIsConnecting())
	{
		int ret = HXiMate_DeviceConnect("usb-hid");
		if (ret == 0)
		{
			m_statusLog.SetWindowTextW(_T("连接成功"));
		}
		else {
			m_statusLog.SetWindowTextW(_T("打开失败,请重新插拔一次"));
			return;
		}
	}

	char serialNo[50] = { 0 };
	int ret = HXiMate_DeviceSerialNumber(serialNo);
	if (ret>0) {
		char TermSN[17] = { 0 };
		memcpy(TermSN, serialNo + ret - 10, 10);
		CString TermID(TermSN);
		m_statusLog.SetWindowText(_T("连接成功，序列号：") + TermID);
	}
	else {
		m_statusLog.SetWindowText(_T("连接成功，序列号读取失败"));
	}
}


//void CMFCApplicationForWHNSDlg::OnBnClickedButtonInitmk()
//{
//	
//	// TODO: 下装初始主密钥
//	if (!HXiMate_DeviceIsConnecting()) {
//		//设备未连接
//		ErrorMessageBox(_T("设备未连接"));
//		return;
//	}
//
//	unsigned char masterKey[16] = {0};
//	if (BST_UNCHECKED == IsDlgButtonChecked(IDC_CHECK_isUseXorMK))
//	{
//		// 未勾选的话
//		CString masterkey1, masterkey2;
//		m_MK1.GetWindowTextW(masterkey1);
//		m_MK2.GetWindowTextW(masterkey2);
//
//		// 1 检查分量长度值
//		if (m_MK1.GetWindowTextLengthW() != 32)
//		{
//			ErrorMessageBox(_T("分量1的长度值错误，应为32个"));
//			return;
//		}
//		char* cstr1 = cstring2ca(masterkey1);
//		if (isHEXString(cstr1) == 0) {
//			ErrorMessageBox(_T("分量1含有非法符号，必须为0-9，A-F，a-f"));
//			return;
//		}
//
//		if (m_MK2.GetWindowTextLengthW() != 32)
//		{
//			ErrorMessageBox(_T("分量2的长度值错误，应为32个"));
//			return;
//		}
//		char* cstr2 = cstring2ca(masterkey2);
//		if (isHEXString(cstr2) == 0) {
//			ErrorMessageBox(_T("分量2含有非法符号，必须为0-9，A-F，a-f"));
//			return;
//		}
//
//
//		// 2 异或后得到主密钥的值
//		unsigned char outData1[16] = { 0 };
//		HXiMate_TwoOne((unsigned char *)cstr1, 32, outData1);
//		unsigned char outData2[16] = { 0 };
//		HXiMate_TwoOne((unsigned char *)cstr2, 32, outData2);
//		int ret = Xor(outData1, outData2,16, masterKey);
//		if (ret != 0) {
//			ErrorMessageBox(_T("分量异或失败"));
//			return;
//		}else {
//			unsigned char outData[33] = { 0 };
//			HXiMate_OneTwo0(masterKey,16,outData);
//			CString finalData(outData);
//			m_final_MK.SetWindowTextW(finalData);
//		}
//	}
//	else {
//
//		CString masterkey;
//		m_final_MK.GetWindowTextW(masterkey);
//		char* cstr = cstring2ca(masterkey);
//		if (m_final_MK.GetWindowTextLengthW() != 32)
//		{
//			ErrorMessageBox(_T("主密钥长度值错误，应为32个"));
//			return;
//		}
//		if (isHEXString(cstr) == 0) {
//			ErrorMessageBox(_T("主密钥中含有非法符号，必须为0-9，A-F，a-f"));
//			return;
//		}
//	    HXiMate_TwoOne((unsigned char *)cstr, 32, masterKey);
//	}
//	
//	int ret = HXiMate_PinPad_PowerOn();
//	if (ret != 0)
//	{
//		ErrorMessageBox(_T("上电失败"));
//		return ;
//	}
//	//下装主密钥
//	ret = HXiMate_PinPad_DownloadMasterkey(1,0,masterKey,16);
//	if (ret < 0) {
//		ErrorMessageBox(_T("下装初始主密钥失败，请重试"));
//		return;
//	}
//
//	//是否需要校验
//	if (BST_UNCHECKED == IsDlgButtonChecked(IDC_CHECK_MKCHK))
//	{
//		MessageBox(_T("下装主密钥成功！"), _T("提示信息"), MB_OK);
//		return;
//	}
//	else
//	{
//		//使用主密钥加密8个字节的全0
//		unsigned char data[16] = { 0 };
//		unsigned char outData[100] = { 0 };
//		memset(outData, 0, sizeof(outData));
//		ret = HXiMate_PinPad_Encrypt(1, ALGO_ENCRYPT, 0, -1, data, outData, 16);
//		if (ret < 0)
//		{
//			ErrorMessageBox(_T("计算校验值失败，请重试"));
//			return;
//		}
//
//		//比较是否一致
//		CString chkCode;
//		char* pValue = cstring2ca(chkCode);
//		if (strncmp(pValue, (const char*)outData, 16) != 0)
//		{
//			ErrorMessageBox(_T("主密钥校验失败，请检查"));
//			return ;
//		}
//		MessageBox(_T("下装主密钥成功！"), _T("提示信息"), MB_OK);
//	}
//}


// 弹出错误信息的弹窗
int CMFCApplicationForWHNSDlg::ErrorMessageBox(CString errorMsg)
{
	return MessageBox(errorMsg, _T("发生错误"), MB_OK | MB_ICONSTOP);
}


//判断该输入串是否为16进制
//为16进制返回1,否则返回0
int CMFCApplicationForWHNSDlg::isHEXString(char *s)
{
	while (*s)
	{
		if (*s >= '0'&&*s <= '9' || *s >= 'a'&&*s <= 'f' || *s >= 'A'&&*s <= 'F')
				//16进制的范围0~9和a~f或A~F
			s++;
		else
			return 0;
	}
	return 1;
}


char* CMFCApplicationForWHNSDlg::cstring2ca(CString str)
{
	char *ptr;
#ifdef _UNICODE
	LONG len;
	len = WideCharToMultiByte(CP_ACP, 0, str, -1, NULL, 0, NULL, NULL);
	ptr = new char[len + 1];
	memset(ptr, 0, len + 1);
	WideCharToMultiByte(CP_ACP, 0, str, -1, ptr, len + 1, NULL, NULL);
#else
	ptr = new char[str.GetAllocLength() + 1];
	sprintf(ptr, _T("%s"), str);
#endif
	return ptr;
}


int CMFCApplicationForWHNSDlg::Xor(unsigned char *inBytes1, unsigned char *inBytes2,int inLength, unsigned char *out)
{
	for (int i = 0; i < inLength; i++) {
		out[i] = inBytes1[i] ^ inBytes2[i];
	}
	return 0;
}


void CMFCApplicationForWHNSDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	myFont.DeleteObject();
	HXiMate_DeviceClose();
	CDialogEx::OnClose();
}


void CMFCApplicationForWHNSDlg::OnClickedButtonSetDisplayText()
{
	if (!HXiMate_DeviceIsConnecting()) {
		//设备未连接
		ErrorMessageBox(_T("设备未连接"));
		return;
	}

	CString powerOnText, freeTextUp,freeTextDown;
	m_POWERON_text.GetWindowTextW(powerOnText);
	m_TIP_up.GetWindowTextW(freeTextUp);
	m_TIP_down.GetWindowTextW(freeTextDown);


	if (BST_CHECKED == IsDlgButtonChecked(IDC_CHECK1))
	{
		char* cstr1 = cstring2ca(powerOnText);
		if (strlen(cstr1) > 16)
		{
			ErrorMessageBox(_T("开机提示文字过长，最多8个中文或者16个英文"));
			return;
		}

		//处理文字居中
		int blankCount = (16 - strlen(cstr1)) / 2;
		char CenterChar[20] = { 0 };
		memset(CenterChar, 0x20, blankCount);//填充空格
		memcpy(CenterChar + blankCount, cstr1, strlen(cstr1));

		//设置开机文字
		unsigned char receivedBytes[50];
		unsigned char sendBytes[50];
		memset(sendBytes, 0, sizeof(sendBytes));
		sendBytes[0] = 0x68;
		sendBytes[1] = 0x04;
		sendBytes[2] = 1; //高位偏移量
		sendBytes[3] = 44;//低位
		sendBytes[4] = 0;
		sendBytes[5] = 20;
		memset(sendBytes + 6, 0, 20);
		memcpy(sendBytes + 6, CenterChar, 20);

		int outlength = 0;
		int iRet = HXiMate_SendReceive(sendBytes, 6 + 20, receivedBytes, &outlength, 3);
		if (iRet > 0 && receivedBytes[0]) {
			ErrorMessageBox(_T("通讯发生错误"));
			return;
		}
		if (iRet == -1)
		{
			ErrorMessageBox(_T("通讯超时"));
			return;//超时
		}

	}

	
	if (BST_CHECKED == IsDlgButtonChecked(IDC_CHECK2))
	{
		char* cstr2 = cstring2ca(freeTextUp);
		if (strlen(cstr2) > 16)
		{
			ErrorMessageBox(_T("待机提示文字(上)，最多8个中文或者16个英文"));
			return;
		}

		//设置待机文字 （上）
		int blankCount = (16 - strlen(cstr2)) / 2;
		char CenterChar[20] = { 0 };
		memset(CenterChar, 0x20, blankCount);//填充空格
		memcpy(CenterChar + blankCount, cstr2, strlen(cstr2));

		unsigned char receivedBytes[50];
		unsigned char sendBytes[50];
		memset(sendBytes, 0, sizeof(sendBytes));
		sendBytes[0] = 0x68;
		sendBytes[1] = 0x04;
		sendBytes[2] = 1; //高位偏移量
		sendBytes[3] = 44 + 20;//低位
		sendBytes[4] = 0;
		sendBytes[5] = 20;
		memset(sendBytes + 6, 0, 20);
		memcpy(sendBytes + 6, CenterChar, 20);

		int outlength = 0;
		int iRet = HXiMate_SendReceive(sendBytes, 6 + 20, receivedBytes, &outlength, 3);
		if (iRet > 0 && receivedBytes[0]) {
			ErrorMessageBox(_T("通讯发生错误"));
			return;
		}
		if (iRet == -1)
		{
			ErrorMessageBox(_T("通讯超时"));
			return;//超时
		}

	}
	
	if (BST_CHECKED == IsDlgButtonChecked(IDC_CHECK3)) {
		char* cstr3 = cstring2ca(freeTextDown);
		if (strlen(cstr3) > 16)
		{
			ErrorMessageBox(_T("待机提示文字(下)，最多8个中文或者16个英文"));
			return;
		}

		//设置待机文字 （下）
		int blankCount = (16 - strlen(cstr3)) / 2;
		char CenterChar[20] = { 0 };
		memset(CenterChar, 0x20, blankCount);//填充空格
		memcpy(CenterChar + blankCount, cstr3, strlen(cstr3));

		unsigned char receivedBytes[50];
		unsigned char sendBytes[50];
		memset(sendBytes, 0, sizeof(sendBytes));
		sendBytes[0] = 0x68;
		sendBytes[1] = 0x04;
		sendBytes[2] = 1; //高位偏移量
		sendBytes[3] = 44 + 20 + 20 + 20;//低位
		sendBytes[4] = 0;
		sendBytes[5] = 20;
		memset(sendBytes + 6, 0, 20);
		memcpy(sendBytes + 6, CenterChar, 20);

		int outlength = 0;
		int iRet = HXiMate_SendReceive(sendBytes, 6 + 20, receivedBytes, &outlength, 3);
		if (iRet > 0 && receivedBytes[0]) {
			ErrorMessageBox(_T("通讯发生错误"));
			return;
		}
		if (iRet == -1)
		{
			ErrorMessageBox(_T("通讯超时"));
			return;//超时
		}
	}

	MessageBox(_T("设置文字提示成功，重启设备生效！"), _T("提示信息"), MB_OK);
}


void CMFCApplicationForWHNSDlg::OnClickedCheck1()
{
	if (BST_CHECKED == IsDlgButtonChecked(IDC_CHECK1)) {
		m_POWERON_text.EnableWindow(TRUE);
	}
	else {
		m_POWERON_text.EnableWindow(FALSE);
	}
}


void CMFCApplicationForWHNSDlg::OnClickedCheck2()
{
	if (BST_CHECKED == IsDlgButtonChecked(IDC_CHECK2)) {
		m_TIP_up.EnableWindow(TRUE);
	}
	else {
		m_TIP_up.EnableWindow(FALSE);
	}
}


void CMFCApplicationForWHNSDlg::OnClickedCheck3()
{
	if (BST_CHECKED == IsDlgButtonChecked(IDC_CHECK2)) {
		m_TIP_down.EnableWindow(TRUE);
	}
	else {
		m_TIP_down.EnableWindow(FALSE);
	}
}
