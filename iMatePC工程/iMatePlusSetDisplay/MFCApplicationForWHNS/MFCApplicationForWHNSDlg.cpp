
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
	m_POWERON_text.SetLimitText(20);
	m_TIP_up.SetLimitText(20);
	m_TIP_down.SetLimitText(20);

	myFont.CreatePointFont(110, _T("Consolas"));
	m_POWERON_text.SetFont(&myFont);
	m_TIP_up.SetFont(&myFont);
	m_TIP_down.SetFont(&myFont);

	//��ʼ��һЩ����
	m_POWERON_text.SetWindowTextW(_T("���ڻ�������"));
	m_TIP_up.SetWindowTextW(_T("iMate Plus"));
	m_TIP_down.SetWindowTextW(_T("��ӭʹ��"));

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


//void CMFCApplicationForWHNSDlg::OnBnClickedButtonInitmk()
//{
//	
//	// TODO: ��װ��ʼ����Կ
//	if (!HXiMate_DeviceIsConnecting()) {
//		//�豸δ����
//		ErrorMessageBox(_T("�豸δ����"));
//		return;
//	}
//
//	unsigned char masterKey[16] = {0};
//	if (BST_UNCHECKED == IsDlgButtonChecked(IDC_CHECK_isUseXorMK))
//	{
//		// δ��ѡ�Ļ�
//		CString masterkey1, masterkey2;
//		m_MK1.GetWindowTextW(masterkey1);
//		m_MK2.GetWindowTextW(masterkey2);
//
//		// 1 ����������ֵ
//		if (m_MK1.GetWindowTextLengthW() != 32)
//		{
//			ErrorMessageBox(_T("����1�ĳ���ֵ����ӦΪ32��"));
//			return;
//		}
//		char* cstr1 = cstring2ca(masterkey1);
//		if (isHEXString(cstr1) == 0) {
//			ErrorMessageBox(_T("����1���зǷ����ţ�����Ϊ0-9��A-F��a-f"));
//			return;
//		}
//
//		if (m_MK2.GetWindowTextLengthW() != 32)
//		{
//			ErrorMessageBox(_T("����2�ĳ���ֵ����ӦΪ32��"));
//			return;
//		}
//		char* cstr2 = cstring2ca(masterkey2);
//		if (isHEXString(cstr2) == 0) {
//			ErrorMessageBox(_T("����2���зǷ����ţ�����Ϊ0-9��A-F��a-f"));
//			return;
//		}
//
//
//		// 2 ����õ�����Կ��ֵ
//		unsigned char outData1[16] = { 0 };
//		HXiMate_TwoOne((unsigned char *)cstr1, 32, outData1);
//		unsigned char outData2[16] = { 0 };
//		HXiMate_TwoOne((unsigned char *)cstr2, 32, outData2);
//		int ret = Xor(outData1, outData2,16, masterKey);
//		if (ret != 0) {
//			ErrorMessageBox(_T("�������ʧ��"));
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
//			ErrorMessageBox(_T("����Կ����ֵ����ӦΪ32��"));
//			return;
//		}
//		if (isHEXString(cstr) == 0) {
//			ErrorMessageBox(_T("����Կ�к��зǷ����ţ�����Ϊ0-9��A-F��a-f"));
//			return;
//		}
//	    HXiMate_TwoOne((unsigned char *)cstr, 32, masterKey);
//	}
//	
//	int ret = HXiMate_PinPad_PowerOn();
//	if (ret != 0)
//	{
//		ErrorMessageBox(_T("�ϵ�ʧ��"));
//		return ;
//	}
//	//��װ����Կ
//	ret = HXiMate_PinPad_DownloadMasterkey(1,0,masterKey,16);
//	if (ret < 0) {
//		ErrorMessageBox(_T("��װ��ʼ����Կʧ�ܣ�������"));
//		return;
//	}
//
//	//�Ƿ���ҪУ��
//	if (BST_UNCHECKED == IsDlgButtonChecked(IDC_CHECK_MKCHK))
//	{
//		MessageBox(_T("��װ����Կ�ɹ���"), _T("��ʾ��Ϣ"), MB_OK);
//		return;
//	}
//	else
//	{
//		//ʹ������Կ����8���ֽڵ�ȫ0
//		unsigned char data[16] = { 0 };
//		unsigned char outData[100] = { 0 };
//		memset(outData, 0, sizeof(outData));
//		ret = HXiMate_PinPad_Encrypt(1, ALGO_ENCRYPT, 0, -1, data, outData, 16);
//		if (ret < 0)
//		{
//			ErrorMessageBox(_T("����У��ֵʧ�ܣ�������"));
//			return;
//		}
//
//		//�Ƚ��Ƿ�һ��
//		CString chkCode;
//		char* pValue = cstring2ca(chkCode);
//		if (strncmp(pValue, (const char*)outData, 16) != 0)
//		{
//			ErrorMessageBox(_T("����ԿУ��ʧ�ܣ�����"));
//			return ;
//		}
//		MessageBox(_T("��װ����Կ�ɹ���"), _T("��ʾ��Ϣ"), MB_OK);
//	}
//}


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


void CMFCApplicationForWHNSDlg::OnClickedButtonSetDisplayText()
{
	if (!HXiMate_DeviceIsConnecting()) {
		//�豸δ����
		ErrorMessageBox(_T("�豸δ����"));
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
			ErrorMessageBox(_T("������ʾ���ֹ��������8�����Ļ���16��Ӣ��"));
			return;
		}

		//�������־���
		int blankCount = (16 - strlen(cstr1)) / 2;
		char CenterChar[20] = { 0 };
		memset(CenterChar, 0x20, blankCount);//���ո�
		memcpy(CenterChar + blankCount, cstr1, strlen(cstr1));

		//���ÿ�������
		unsigned char receivedBytes[50];
		unsigned char sendBytes[50];
		memset(sendBytes, 0, sizeof(sendBytes));
		sendBytes[0] = 0x68;
		sendBytes[1] = 0x04;
		sendBytes[2] = 1; //��λƫ����
		sendBytes[3] = 44;//��λ
		sendBytes[4] = 0;
		sendBytes[5] = 20;
		memset(sendBytes + 6, 0, 20);
		memcpy(sendBytes + 6, CenterChar, 20);

		int outlength = 0;
		int iRet = HXiMate_SendReceive(sendBytes, 6 + 20, receivedBytes, &outlength, 3);
		if (iRet > 0 && receivedBytes[0]) {
			ErrorMessageBox(_T("ͨѶ��������"));
			return;
		}
		if (iRet == -1)
		{
			ErrorMessageBox(_T("ͨѶ��ʱ"));
			return;//��ʱ
		}

	}

	
	if (BST_CHECKED == IsDlgButtonChecked(IDC_CHECK2))
	{
		char* cstr2 = cstring2ca(freeTextUp);
		if (strlen(cstr2) > 16)
		{
			ErrorMessageBox(_T("������ʾ����(��)�����8�����Ļ���16��Ӣ��"));
			return;
		}

		//���ô������� ���ϣ�
		int blankCount = (16 - strlen(cstr2)) / 2;
		char CenterChar[20] = { 0 };
		memset(CenterChar, 0x20, blankCount);//���ո�
		memcpy(CenterChar + blankCount, cstr2, strlen(cstr2));

		unsigned char receivedBytes[50];
		unsigned char sendBytes[50];
		memset(sendBytes, 0, sizeof(sendBytes));
		sendBytes[0] = 0x68;
		sendBytes[1] = 0x04;
		sendBytes[2] = 1; //��λƫ����
		sendBytes[3] = 44 + 20;//��λ
		sendBytes[4] = 0;
		sendBytes[5] = 20;
		memset(sendBytes + 6, 0, 20);
		memcpy(sendBytes + 6, CenterChar, 20);

		int outlength = 0;
		int iRet = HXiMate_SendReceive(sendBytes, 6 + 20, receivedBytes, &outlength, 3);
		if (iRet > 0 && receivedBytes[0]) {
			ErrorMessageBox(_T("ͨѶ��������"));
			return;
		}
		if (iRet == -1)
		{
			ErrorMessageBox(_T("ͨѶ��ʱ"));
			return;//��ʱ
		}

	}
	
	if (BST_CHECKED == IsDlgButtonChecked(IDC_CHECK3)) {
		char* cstr3 = cstring2ca(freeTextDown);
		if (strlen(cstr3) > 16)
		{
			ErrorMessageBox(_T("������ʾ����(��)�����8�����Ļ���16��Ӣ��"));
			return;
		}

		//���ô������� ���£�
		int blankCount = (16 - strlen(cstr3)) / 2;
		char CenterChar[20] = { 0 };
		memset(CenterChar, 0x20, blankCount);//���ո�
		memcpy(CenterChar + blankCount, cstr3, strlen(cstr3));

		unsigned char receivedBytes[50];
		unsigned char sendBytes[50];
		memset(sendBytes, 0, sizeof(sendBytes));
		sendBytes[0] = 0x68;
		sendBytes[1] = 0x04;
		sendBytes[2] = 1; //��λƫ����
		sendBytes[3] = 44 + 20 + 20 + 20;//��λ
		sendBytes[4] = 0;
		sendBytes[5] = 20;
		memset(sendBytes + 6, 0, 20);
		memcpy(sendBytes + 6, CenterChar, 20);

		int outlength = 0;
		int iRet = HXiMate_SendReceive(sendBytes, 6 + 20, receivedBytes, &outlength, 3);
		if (iRet > 0 && receivedBytes[0]) {
			ErrorMessageBox(_T("ͨѶ��������"));
			return;
		}
		if (iRet == -1)
		{
			ErrorMessageBox(_T("ͨѶ��ʱ"));
			return;//��ʱ
		}
	}

	MessageBox(_T("����������ʾ�ɹ��������豸��Ч��"), _T("��ʾ��Ϣ"), MB_OK);
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
