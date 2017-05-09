
// MFCApplicationForWHNSDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"


// CMFCApplicationForWHNSDlg �Ի���
class CMFCApplicationForWHNSDlg : public CDialogEx
{
// ����
public:
	CMFCApplicationForWHNSDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MFCAPPLICATIONFORWHNS_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	BOOL CMFCApplicationForWHNSDlg::PreTranslateMessage(MSG* pMsg)

	{
		if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN) return TRUE;

		else

			return CDialog::PreTranslateMessage(pMsg);

	}

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonConnectdevice();
//	afx_msg void OnBnClickedButtonInitmk();

	// ��ʾ�豸�ն˺�
//	CEdit m_TermID;
	// ����Կ����1
	CEdit m_POWERON_text;
	// ����Կ����2
	CEdit m_TIP_up;
	// ������װ������Կ��������
	CEdit m_TIP_down;
	// ����ԿУ��ֵ
//	CEdit m_MKCHK;
	// ����һ�ε�����Կ���ܺ������Կ��������
//	CEdit m_updateMK;
	// ������Կ����
//	CEdit m_workingKey;
	// �豸��״̬��ʾ
	CStatic m_statusLog;
	// �Ƿ�ֱ��ʹ�����������Կ
//	CButton m_isXorMK;
	// �Ƿ�У��
//	CButton m_isCHK;
	// ����������Ϣ�ĵ���
	int ErrorMessageBox(CString errorMsg);
	int isHEXString(char *s);
	char* cstring2ca(CString str);
	int Xor(unsigned char *inBytes1, unsigned char *inBytes2, int inLength, unsigned char *out);
	// ��������Կ��ť
//	CButton m_ButtonUpdateMK;
//	CEdit m_MKCHK2;
	afx_msg void OnClose();
	afx_msg void OnClickedButtonSetDisplayText();
	afx_msg void OnClickedCheck1();
	afx_msg void OnClickedCheck2();
	afx_msg void OnClickedCheck3();
};
