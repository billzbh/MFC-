
// MFCApplicationForWHNSDlg.h : 头文件
//

#pragma once
#include "afxwin.h"


// CMFCApplicationForWHNSDlg 对话框
class CMFCApplicationForWHNSDlg : public CDialogEx
{
// 构造
public:
	CMFCApplicationForWHNSDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MFCAPPLICATIONFORWHNS_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	BOOL CMFCApplicationForWHNSDlg::PreTranslateMessage(MSG* pMsg)

	{
		if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN) return TRUE;

		else

			return CDialog::PreTranslateMessage(pMsg);

	}

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonConnectdevice();
//	afx_msg void OnBnClickedButtonInitmk();

	// 显示设备终端号
//	CEdit m_TermID;
	// 主密钥分量1
	CEdit m_POWERON_text;
	// 主密钥分量2
	CEdit m_TIP_up;
	// 真正下装的主密钥明文数据
	CEdit m_TIP_down;
	// 主密钥校验值
//	CEdit m_MKCHK;
	// 以上一次的主密钥加密后的主密钥密文数据
//	CEdit m_updateMK;
	// 工作密钥数据
//	CEdit m_workingKey;
	// 设备的状态显示
	CStatic m_statusLog;
	// 是否直接使用异或后的主密钥
//	CButton m_isXorMK;
	// 是否校验
//	CButton m_isCHK;
	// 弹出错误信息的弹窗
	int ErrorMessageBox(CString errorMsg);
	int isHEXString(char *s);
	char* cstring2ca(CString str);
	int Xor(unsigned char *inBytes1, unsigned char *inBytes2, int inLength, unsigned char *out);
	// 更新主密钥按钮
//	CButton m_ButtonUpdateMK;
//	CEdit m_MKCHK2;
	afx_msg void OnClose();
	afx_msg void OnClickedButtonSetDisplayText();
	afx_msg void OnClickedCheck1();
	afx_msg void OnClickedCheck2();
	afx_msg void OnClickedCheck3();
};
