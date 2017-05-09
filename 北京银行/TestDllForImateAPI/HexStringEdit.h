#if !defined(AFX_HEXSTRINGEDIT_H__10249BF7_02D4_4D1A_B37F_E770BEFA7ADC__INCLUDED_)
#define AFX_HEXSTRINGEDIT_H__10249BF7_02D4_4D1A_B37F_E770BEFA7ADC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// HexStringEdit.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CHexStringEdit window

class CHexStringEdit : public CEdit
{
// Construction
public:
	CHexStringEdit();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHexStringEdit)
//public:
//	virtual BOOL PreTranslateMessage(MSG* pMsg);
//protected:
	
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CHexStringEdit();

	// Generated message map functions
protected:
	//{{AFX_MSG(CHexStringEdit)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnPaste(WPARAM wParam, LPARAM lParam);
	// for translating Windows messages in main message pump
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HEXSTRINGEDIT_H__10249BF7_02D4_4D1A_B37F_E770BEFA7ADC__INCLUDED_)
