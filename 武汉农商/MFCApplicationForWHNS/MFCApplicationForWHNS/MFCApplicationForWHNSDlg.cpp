
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
	DDX_Control(pDX, IDC_EDIT_IMATE_DEVICETERMID, m_TermID);
	DDX_Control(pDX, IDC_EDIT_MK1, m_MK1);
	DDX_Control(pDX, IDC_EDIT_MK2, m_MK2);
	DDX_Control(pDX, IDC_EDIT_MKALL, m_final_MK);
	DDX_Control(pDX, IDC_EDIT_MKCHK, m_MKCHK);
	DDX_Control(pDX, IDC_EDIT_UPDATEMK, m_updateMK);
	DDX_Control(pDX, IDC_EDIT_WorkKey, m_workingKey);
	DDX_Control(pDX, IDC_STATIC_STATUS, m_statusLog);
	DDX_Control(pDX, IDC_CHECK_isUseXorMK, m_isXorMK);
	DDX_Control(pDX, IDC_CHECK_MKCHK, m_isCHK);
	DDX_Control(pDX, IDC_BUTTON_DOWNLOADMK, m_ButtonUpdateMK);
	DDX_Control(pDX, IDC_EDIT_MKCHK2, m_MKCHK2);
}

BEGIN_MESSAGE_MAP(CMFCApplicationForWHNSDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_CONNECTDEVICE, &CMFCApplicationForWHNSDlg::OnBnClickedButtonConnectdevice)
	ON_BN_CLICKED(IDC_BUTTON_GETTERMID, &CMFCApplicationForWHNSDlg::OnBnClickedButtonGettermid)
	ON_BN_CLICKED(IDC_CHECK_mainMKCHK, &CMFCApplicationForWHNSDlg::OnBnClickedCheckmainmkchk)
	ON_BN_CLICKED(IDC_CHECK_MKCHK, &CMFCApplicationForWHNSDlg::OnBnClickedCheckMkchk)
	ON_BN_CLICKED(IDC_BUTTON_INITMK, &CMFCApplicationForWHNSDlg::OnBnClickedButtonInitmk)
	ON_BN_CLICKED(IDC_BUTTON_DOWNLOADMK, &CMFCApplicationForWHNSDlg::OnBnClickedButtonDownloadmk)
	ON_BN_CLICKED(IDC_BUTTON_DOWNLOADWORKKEY, &CMFCApplicationForWHNSDlg::OnBnClickedButtonDownloadworkkey)
	ON_WM_CLOSE()
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
	m_MK1.SetLimitText(32);
	m_MK2.SetLimitText(32);
	m_final_MK.SetLimitText(32);
	m_MKCHK.SetLimitText(16);
	m_MKCHK2.SetLimitText(16);
	m_updateMK.SetLimitText(32);
	m_workingKey.SetLimitText(32);
	HXiMate_PinPad_SetModel(PINPAD_MODEL_HX);

	myFont.CreatePointFont(110, _T("Consolas"));
	m_MK1.SetFont(&myFont);
	m_MK2.SetFont(&myFont);
	m_final_MK.SetFont(&myFont);
	m_MKCHK.SetFont(&myFont);
	m_MKCHK2.SetFont(&myFont);
	m_updateMK.SetFont(&myFont);
	m_workingKey.SetFont(&myFont);

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
		HXiMate_DeviceClose();
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
	//开启线程，读取版本号 和 序列号
//	CreateThread(NULL, 0, ThreadProc, NULL, 0, NULL);//创建一个线程，去执行ThreadProc函数

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


void CMFCApplicationForWHNSDlg::OnBnClickedButtonGettermid()
{
	// TODO: 获取终端号
	if (!HXiMate_DeviceIsConnecting()) {
		//设备未连接
		CMFCApplicationForWHNSDlg::ErrorMessageBox(_T("设备未连接"));
		return;
	}

	unsigned char serialNo[50] = { 0 };
	int ret = HXiMate_ReadTermId(serialNo);
	if (ret>0) {
		CString TermID(serialNo);
		m_TermID.SetWindowText(TermID);
	}
	else {
		m_TermID.SetWindowText(_T(""));
	}
}


void CMFCApplicationForWHNSDlg::OnBnClickedCheckmainmkchk()
{
	// TODO: 是否使用异或后的主密钥
	if (BST_CHECKED == IsDlgButtonChecked(IDC_CHECK_isUseXorMK))
	{
		// 勾选
		m_final_MK.EnableWindow(TRUE);
		m_MK1.EnableWindow(FALSE);
		m_MK2.EnableWindow(FALSE);
	}
	else
	{
		m_final_MK.EnableWindow(FALSE);
		m_MK1.EnableWindow(TRUE);
		m_MK2.EnableWindow(TRUE);
	}
}


void CMFCApplicationForWHNSDlg::OnBnClickedCheckMkchk()
{
	// TODO: 是否检验主密钥的校验值
	if (BST_CHECKED == IsDlgButtonChecked(IDC_CHECK_MKCHK))
	{
		// 勾选
		m_MKCHK.EnableWindow(TRUE);
//		m_updateMK.EnableWindow(TRUE);
//		m_ButtonUpdateMK.EnableWindow(TRUE);
	}
	else
	{
		m_MKCHK.EnableWindow(FALSE);
	}
}


void CMFCApplicationForWHNSDlg::OnBnClickedButtonInitmk()
{
	
	// TODO: 下装初始主密钥
	if (!HXiMate_DeviceIsConnecting()) {
		//设备未连接
		ErrorMessageBox(_T("设备未连接"));
		return;
	}

	unsigned char masterKey[16] = {0};
	if (BST_UNCHECKED == IsDlgButtonChecked(IDC_CHECK_isUseXorMK))
	{
		// 未勾选的话
		CString masterkey1, masterkey2;
		m_MK1.GetWindowTextW(masterkey1);
		m_MK2.GetWindowTextW(masterkey2);

		// 1 检查分量长度值
		if (m_MK1.GetWindowTextLengthW() != 32)
		{
			ErrorMessageBox(_T("分量1的长度值错误，应为32个"));
			return;
		}
		char* cstr1 = cstring2ca(masterkey1);
		if (isHEXString(cstr1) == 0) {
			ErrorMessageBox(_T("分量1含有非法符号，必须为0-9，A-F，a-f"));
			return;
		}

		if (m_MK2.GetWindowTextLengthW() != 32)
		{
			ErrorMessageBox(_T("分量2的长度值错误，应为32个"));
			return;
		}
		char* cstr2 = cstring2ca(masterkey2);
		if (isHEXString(cstr2) == 0) {
			ErrorMessageBox(_T("分量2含有非法符号，必须为0-9，A-F，a-f"));
			return;
		}


		// 2 异或后得到主密钥的值
		unsigned char outData1[16] = { 0 };
		HXiMate_TwoOne((unsigned char *)cstr1, 32, outData1);
		unsigned char outData2[16] = { 0 };
		HXiMate_TwoOne((unsigned char *)cstr2, 32, outData2);
		int ret = Xor(outData1, outData2,16, masterKey);
		if (ret != 0) {
			ErrorMessageBox(_T("分量异或失败"));
			return;
		}else {
			unsigned char outData[33] = { 0 };
			HXiMate_OneTwo0(masterKey,16,outData);
			CString finalData(outData);
			m_final_MK.SetWindowTextW(finalData);
		}
	}
	else {

		CString masterkey;
		m_final_MK.GetWindowTextW(masterkey);
		char* cstr = cstring2ca(masterkey);
		if (m_final_MK.GetWindowTextLengthW() != 32)
		{
			ErrorMessageBox(_T("主密钥长度值错误，应为32个"));
			return;
		}
		if (isHEXString(cstr) == 0) {
			ErrorMessageBox(_T("主密钥中含有非法符号，必须为0-9，A-F，a-f"));
			return;
		}
	    HXiMate_TwoOne((unsigned char *)cstr, 32, masterKey);
	}
	
	int ret = HXiMate_PinPad_PowerOn();
	if (ret != 0)
	{
		ErrorMessageBox(_T("上电失败"));
		return ;
	}
	//下装主密钥
	ret = HXiMate_PinPad_DownloadMasterkey(1,0,masterKey,16);
	if (ret < 0) {
		ErrorMessageBox(_T("下装初始主密钥失败，请重试"));
		return;
	}

	//是否需要校验
	if (BST_UNCHECKED == IsDlgButtonChecked(IDC_CHECK_MKCHK))
	{
		MessageBox(_T("下装主密钥成功！"), _T("提示信息"), MB_OK);
		return;
	}
	else
	{
		//使用主密钥加密8个字节的全0
		unsigned char data[16] = { 0 };
		unsigned char outData[100] = { 0 };
		memset(outData, 0, sizeof(outData));
		ret = HXiMate_PinPad_Encrypt(1, ALGO_ENCRYPT, 0, -1, data, outData, 16);
		if (ret < 0)
		{
			ErrorMessageBox(_T("计算校验值失败，请重试"));
			return;
		}

		//比较是否一致
		CString chkCode;
		m_MKCHK.GetWindowTextW(chkCode);
		char* pValue = cstring2ca(chkCode);
		if (strncmp(pValue, (const char*)outData, 16) != 0)
		{
			ErrorMessageBox(_T("主密钥校验失败，请检查"));
			return ;
		}
		MessageBox(_T("下装主密钥成功！"), _T("提示信息"), MB_OK);
	}
}


void CMFCApplicationForWHNSDlg::OnBnClickedButtonDownloadmk()
{
	// TODO: 更新主密钥
	if (!HXiMate_DeviceIsConnecting()) {
		//设备未连接
		CMFCApplicationForWHNSDlg::ErrorMessageBox(_T("设备未连接"));
		return;
	}

	CString mainEncodeKEY;
	m_updateMK.GetWindowTextW(mainEncodeKEY);
	char* cstr = cstring2ca(mainEncodeKEY);
	if (m_updateMK.GetWindowTextLengthW() != 32)
	{
		ErrorMessageBox(_T("主密钥长度值错误，应为32个"));
		return;
	}
	if (isHEXString(cstr) == 0) {
		ErrorMessageBox(_T("主密钥中含有非法符号，必须为0-9，A-F，a-f"));
		return;
	}

	unsigned char outMKey[16] = { 0 };
	HXiMate_TwoOne((unsigned char *)cstr, 32, outMKey);
	
	//获取校验值
	CString chkCode;
	m_MKCHK2.GetWindowTextW(chkCode);
	char* pValue = cstring2ca(chkCode);

	//使用主密钥加密8个字节的全0
	unsigned char data[16] = { 0 };
	unsigned char outData[100] = { 0 };
	memset(outData, 0, sizeof(outData));
	int ret = HXiMate_PinPad_Encrypt(1, ALGO_ENCRYPT, 0, -1, data, outData, 16);
	if (ret < 0)
	{
		ErrorMessageBox(_T("设备计算校验值失败，请重试"));
		return;
	}
	if (strncmp(pValue, (const char*)outData, 16) != 0)
	{
		ErrorMessageBox(_T("校验值错误，请检查"));
		return;
	}

	ret = HXiMate_PinPad_PowerOn();
	if (ret != 0)
	{
		ErrorMessageBox(_T("上电失败"));
		return;
	}

	//解密
	memset(outData, 0, sizeof(outData));
	ret = HXiMate_PinPad_Encrypt(1, ALGO_DECRYPT, 0, -1, outMKey, outData, 16);
	if (ret < 0)
	{
		ErrorMessageBox(_T("主密钥解析失败，请重试"));
		return;
	}

	//下装主密钥
	memset(data, 0, sizeof(data));
	HXiMate_TwoOne((unsigned char *)outData, 32, data);
	ret = HXiMate_PinPad_DownloadMasterkey(1, 0, data, 16);
	if (ret < 0) {
		ErrorMessageBox(_T("更新主密钥失败，请重试"));
		return;
	}

	MessageBox(_T("更新主密钥成功！"), _T("提示信息"), MB_OK);
}


void CMFCApplicationForWHNSDlg::OnBnClickedButtonDownloadworkkey()
{
	// TODO: 下装工作密钥
	if (!HXiMate_DeviceIsConnecting()) {
		//设备未连接
		CMFCApplicationForWHNSDlg::ErrorMessageBox(_T("设备未连接"));
		return;
	}

	CString workingKEY;
	m_workingKey.GetWindowTextW(workingKEY);
	char* cstr = cstring2ca(workingKEY);
	if (m_workingKey.GetWindowTextLengthW() != 32)
	{
		ErrorMessageBox(_T("工作密钥长度值错误，应为32个"));
		return;
	}
	if (isHEXString(cstr) == 0) {
		ErrorMessageBox(_T("工作密钥中含有非法符号，必须为0-9，A-F，a-f"));
		return;
	}

	unsigned char outWorkKey[16] = {0};
	HXiMate_TwoOne((unsigned char *)cstr, 32, outWorkKey);
	int ret = HXiMate_PinPad_PowerOn();
	if (ret != 0)
	{
		ErrorMessageBox(_T("上电失败"));
		return;
	}

	ret = HXiMate_PinPad_DownloadWorkingkey(1, 0, 0, outWorkKey, 16);
	if (ret != 0)
	{
		ErrorMessageBox(_T("下装工作密钥失败"));
	}
	else {
		MessageBox(_T("下装工作密钥成功！"), _T("提示信息"), MB_OK);
	}
	return;
}


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
