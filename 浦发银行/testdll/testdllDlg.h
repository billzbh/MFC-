// testdllDlg.h : header file
//

#if !defined(AFX_TESTDLLDLG_H__E5254D0C_F79C_4472_9E6A_700ED288DDED__INCLUDED_)
#define AFX_TESTDLLDLG_H__E5254D0C_F79C_4472_9E6A_700ED288DDED__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CTestdllDlg dialog

class CTestdllDlg : public CDialog
{
// Construction
public:
	CTestdllDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CTestdllDlg)
	enum { IDD = IDD_TESTDLL_DIALOG };
	CEdit	m_log;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTestdllDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CTestdllDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void Onsearch();
	afx_msg void Ondown();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TESTDLLDLG_H__E5254D0C_F79C_4472_9E6A_700ED288DDED__INCLUDED_)
