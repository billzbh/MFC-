
// MFCApplicationForWHNSDlg.cpp : ʵ���ļ�
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

//����ȫ�ֵĶ���ָ��
CFont myFont;


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���
class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CMFCApplicationForWHNSDlg �Ի���



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


// CMFCApplicationForWHNSDlg ��Ϣ�������

BOOL CMFCApplicationForWHNSDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
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

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CMFCApplicationForWHNSDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CMFCApplicationForWHNSDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CMFCApplicationForWHNSDlg::OnBnClickedButtonConnectdevice()
{
	// TODO: ���豸
	if (!HXiMate_DeviceIsConnecting())
	{
		HXiMate_DeviceClose();
		int ret = HXiMate_DeviceConnect("usb-hid");
		if (ret == 0)
		{
			m_statusLog.SetWindowTextW(_T("���ӳɹ�"));
		}
		else {
			m_statusLog.SetWindowTextW(_T("��ʧ��,�����²��һ��"));
			return;
		}
	}
	//�����̣߳���ȡ�汾�� �� ���к�
//	CreateThread(NULL, 0, ThreadProc, NULL, 0, NULL);//����һ���̣߳�ȥִ��ThreadProc����

	char serialNo[50] = { 0 };
	int ret = HXiMate_DeviceSerialNumber(serialNo);
	if (ret>0) {
		char TermSN[17] = { 0 };
		memcpy(TermSN, serialNo + ret - 10, 10);
		CString TermID(TermSN);
		m_statusLog.SetWindowText(_T("���ӳɹ������кţ�") + TermID);
	}
	else {
		m_statusLog.SetWindowText(_T("���ӳɹ������кŶ�ȡʧ��"));
	}
}


void CMFCApplicationForWHNSDlg::OnBnClickedButtonGettermid()
{
	// TODO: ��ȡ�ն˺�
	if (!HXiMate_DeviceIsConnecting()) {
		//�豸δ����
		CMFCApplicationForWHNSDlg::ErrorMessageBox(_T("�豸δ����"));
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
	// TODO: �Ƿ�ʹ�����������Կ
	if (BST_CHECKED == IsDlgButtonChecked(IDC_CHECK_isUseXorMK))
	{
		// ��ѡ
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
	// TODO: �Ƿ��������Կ��У��ֵ
	if (BST_CHECKED == IsDlgButtonChecked(IDC_CHECK_MKCHK))
	{
		// ��ѡ
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
	
	// TODO: ��װ��ʼ����Կ
	if (!HXiMate_DeviceIsConnecting()) {
		//�豸δ����
		ErrorMessageBox(_T("�豸δ����"));
		return;
	}

	unsigned char masterKey[16] = {0};
	if (BST_UNCHECKED == IsDlgButtonChecked(IDC_CHECK_isUseXorMK))
	{
		// δ��ѡ�Ļ�
		CString masterkey1, masterkey2;
		m_MK1.GetWindowTextW(masterkey1);
		m_MK2.GetWindowTextW(masterkey2);

		// 1 ����������ֵ
		if (m_MK1.GetWindowTextLengthW() != 32)
		{
			ErrorMessageBox(_T("����1�ĳ���ֵ����ӦΪ32��"));
			return;
		}
		char* cstr1 = cstring2ca(masterkey1);
		if (isHEXString(cstr1) == 0) {
			ErrorMessageBox(_T("����1���зǷ����ţ�����Ϊ0-9��A-F��a-f"));
			return;
		}

		if (m_MK2.GetWindowTextLengthW() != 32)
		{
			ErrorMessageBox(_T("����2�ĳ���ֵ����ӦΪ32��"));
			return;
		}
		char* cstr2 = cstring2ca(masterkey2);
		if (isHEXString(cstr2) == 0) {
			ErrorMessageBox(_T("����2���зǷ����ţ�����Ϊ0-9��A-F��a-f"));
			return;
		}


		// 2 ����õ�����Կ��ֵ
		unsigned char outData1[16] = { 0 };
		HXiMate_TwoOne((unsigned char *)cstr1, 32, outData1);
		unsigned char outData2[16] = { 0 };
		HXiMate_TwoOne((unsigned char *)cstr2, 32, outData2);
		int ret = Xor(outData1, outData2,16, masterKey);
		if (ret != 0) {
			ErrorMessageBox(_T("�������ʧ��"));
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
			ErrorMessageBox(_T("����Կ����ֵ����ӦΪ32��"));
			return;
		}
		if (isHEXString(cstr) == 0) {
			ErrorMessageBox(_T("����Կ�к��зǷ����ţ�����Ϊ0-9��A-F��a-f"));
			return;
		}
	    HXiMate_TwoOne((unsigned char *)cstr, 32, masterKey);
	}
	
	int ret = HXiMate_PinPad_PowerOn();
	if (ret != 0)
	{
		ErrorMessageBox(_T("�ϵ�ʧ��"));
		return ;
	}
	//��װ����Կ
	ret = HXiMate_PinPad_DownloadMasterkey(1,0,masterKey,16);
	if (ret < 0) {
		ErrorMessageBox(_T("��װ��ʼ����Կʧ�ܣ�������"));
		return;
	}

	//�Ƿ���ҪУ��
	if (BST_UNCHECKED == IsDlgButtonChecked(IDC_CHECK_MKCHK))
	{
		MessageBox(_T("��װ����Կ�ɹ���"), _T("��ʾ��Ϣ"), MB_OK);
		return;
	}
	else
	{
		//ʹ������Կ����8���ֽڵ�ȫ0
		unsigned char data[16] = { 0 };
		unsigned char outData[100] = { 0 };
		memset(outData, 0, sizeof(outData));
		ret = HXiMate_PinPad_Encrypt(1, ALGO_ENCRYPT, 0, -1, data, outData, 16);
		if (ret < 0)
		{
			ErrorMessageBox(_T("����У��ֵʧ�ܣ�������"));
			return;
		}

		//�Ƚ��Ƿ�һ��
		CString chkCode;
		m_MKCHK.GetWindowTextW(chkCode);
		char* pValue = cstring2ca(chkCode);
		if (strncmp(pValue, (const char*)outData, 16) != 0)
		{
			ErrorMessageBox(_T("����ԿУ��ʧ�ܣ�����"));
			return ;
		}
		MessageBox(_T("��װ����Կ�ɹ���"), _T("��ʾ��Ϣ"), MB_OK);
	}
}


void CMFCApplicationForWHNSDlg::OnBnClickedButtonDownloadmk()
{
	// TODO: ��������Կ
	if (!HXiMate_DeviceIsConnecting()) {
		//�豸δ����
		CMFCApplicationForWHNSDlg::ErrorMessageBox(_T("�豸δ����"));
		return;
	}

	CString mainEncodeKEY;
	m_updateMK.GetWindowTextW(mainEncodeKEY);
	char* cstr = cstring2ca(mainEncodeKEY);
	if (m_updateMK.GetWindowTextLengthW() != 32)
	{
		ErrorMessageBox(_T("����Կ����ֵ����ӦΪ32��"));
		return;
	}
	if (isHEXString(cstr) == 0) {
		ErrorMessageBox(_T("����Կ�к��зǷ����ţ�����Ϊ0-9��A-F��a-f"));
		return;
	}

	unsigned char outMKey[16] = { 0 };
	HXiMate_TwoOne((unsigned char *)cstr, 32, outMKey);
	
	//��ȡУ��ֵ
	CString chkCode;
	m_MKCHK2.GetWindowTextW(chkCode);
	char* pValue = cstring2ca(chkCode);

	//ʹ������Կ����8���ֽڵ�ȫ0
	unsigned char data[16] = { 0 };
	unsigned char outData[100] = { 0 };
	memset(outData, 0, sizeof(outData));
	int ret = HXiMate_PinPad_Encrypt(1, ALGO_ENCRYPT, 0, -1, data, outData, 16);
	if (ret < 0)
	{
		ErrorMessageBox(_T("�豸����У��ֵʧ�ܣ�������"));
		return;
	}
	if (strncmp(pValue, (const char*)outData, 16) != 0)
	{
		ErrorMessageBox(_T("У��ֵ��������"));
		return;
	}

	ret = HXiMate_PinPad_PowerOn();
	if (ret != 0)
	{
		ErrorMessageBox(_T("�ϵ�ʧ��"));
		return;
	}

	//����
	memset(outData, 0, sizeof(outData));
	ret = HXiMate_PinPad_Encrypt(1, ALGO_DECRYPT, 0, -1, outMKey, outData, 16);
	if (ret < 0)
	{
		ErrorMessageBox(_T("����Կ����ʧ�ܣ�������"));
		return;
	}

	//��װ����Կ
	memset(data, 0, sizeof(data));
	HXiMate_TwoOne((unsigned char *)outData, 32, data);
	ret = HXiMate_PinPad_DownloadMasterkey(1, 0, data, 16);
	if (ret < 0) {
		ErrorMessageBox(_T("��������Կʧ�ܣ�������"));
		return;
	}

	MessageBox(_T("��������Կ�ɹ���"), _T("��ʾ��Ϣ"), MB_OK);
}


void CMFCApplicationForWHNSDlg::OnBnClickedButtonDownloadworkkey()
{
	// TODO: ��װ������Կ
	if (!HXiMate_DeviceIsConnecting()) {
		//�豸δ����
		CMFCApplicationForWHNSDlg::ErrorMessageBox(_T("�豸δ����"));
		return;
	}

	CString workingKEY;
	m_workingKey.GetWindowTextW(workingKEY);
	char* cstr = cstring2ca(workingKEY);
	if (m_workingKey.GetWindowTextLengthW() != 32)
	{
		ErrorMessageBox(_T("������Կ����ֵ����ӦΪ32��"));
		return;
	}
	if (isHEXString(cstr) == 0) {
		ErrorMessageBox(_T("������Կ�к��зǷ����ţ�����Ϊ0-9��A-F��a-f"));
		return;
	}

	unsigned char outWorkKey[16] = {0};
	HXiMate_TwoOne((unsigned char *)cstr, 32, outWorkKey);
	int ret = HXiMate_PinPad_PowerOn();
	if (ret != 0)
	{
		ErrorMessageBox(_T("�ϵ�ʧ��"));
		return;
	}

	ret = HXiMate_PinPad_DownloadWorkingkey(1, 0, 0, outWorkKey, 16);
	if (ret != 0)
	{
		ErrorMessageBox(_T("��װ������Կʧ��"));
	}
	else {
		MessageBox(_T("��װ������Կ�ɹ���"), _T("��ʾ��Ϣ"), MB_OK);
	}
	return;
}


// ����������Ϣ�ĵ���
int CMFCApplicationForWHNSDlg::ErrorMessageBox(CString errorMsg)
{
	return MessageBox(errorMsg, _T("��������"), MB_OK | MB_ICONSTOP);
}


//�жϸ����봮�Ƿ�Ϊ16����
//Ϊ16���Ʒ���1,���򷵻�0
int CMFCApplicationForWHNSDlg::isHEXString(char *s)
{
	while (*s)
	{
		if (*s >= '0'&&*s <= '9' || *s >= 'a'&&*s <= 'f' || *s >= 'A'&&*s <= 'F')
				//16���Ƶķ�Χ0~9��a~f��A~F
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
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	myFont.DeleteObject();
	HXiMate_DeviceClose();
	CDialogEx::OnClose();
}
