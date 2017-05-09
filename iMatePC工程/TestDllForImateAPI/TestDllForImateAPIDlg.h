// TestDllForImateAPIDlg.h : header file
//



#if !defined(AFX_TESTDLLFORIMATEAPIDLG_H__53A7F943_A70C_44A4_A21F_FB2233C7E289__INCLUDED_)
#define AFX_TESTDLLFORIMATEAPIDLG_H__53A7F943_A70C_44A4_A21F_FB2233C7E289__INCLUDED_
#include "HexStringEdit.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CTestDllForImateAPIDlg dialog

class CTestDllForImateAPIDlg : public CDialog
{
// Construction
public:
	CTestDllForImateAPIDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CTestDllForImateAPIDlg)
	enum { IDD = IDD_TESTDLLFORIMATEAPI_DIALOG };
	CStatic	m_biglogFailTip;
	CStatic	m_mk2Lenght;
	CStatic	m_mk1Length;
	CStatic	m_BigResultLog;
	CHexStringEdit	m_MasterKeyCheckCode;
	CHexStringEdit	m_snCheckCode;
	CHexStringEdit	m_MK2edit;
	CHexStringEdit	m_MK1edit;
	CStatic	m_resultLog;
	CStatic	m_VerisonLog;
	CStatic	m_SNLog;
	CStatic	m_DeviceStatusLog;
	//}}AFX_DATA
	CFont myFont;
	CFont myFont2;

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTestDllForImateAPIDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	virtual BOOL PreTranslateMessage(MSG* pMsg);  // PreTranslateMessage是消息在送给TranslateMessage函数之前被调用的
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CTestDllForImateAPIDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnDownloadMasterKey();
	afx_msg void OnChangemasterkey2EDIT();
	afx_msg void OnChangemasterkey1EDIT();
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TESTDLLFORIMATEAPIDLG_H__53A7F943_A70C_44A4_A21F_FB2233C7E289__INCLUDED_)
