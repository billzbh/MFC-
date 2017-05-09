// TestDllForImateAPI.h : main header file for the TESTDLLFORIMATEAPI application
//

#if !defined(AFX_TESTDLLFORIMATEAPI_H__D1A5BC6F_6A18_44D5_A31F_E4E043684907__INCLUDED_)
#define AFX_TESTDLLFORIMATEAPI_H__D1A5BC6F_6A18_44D5_A31F_E4E043684907__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CTestDllForImateAPIApp:
// See TestDllForImateAPI.cpp for the implementation of this class
//

class CTestDllForImateAPIApp : public CWinApp
{
public:
	CTestDllForImateAPIApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTestDllForImateAPIApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CTestDllForImateAPIApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TESTDLLFORIMATEAPI_H__D1A5BC6F_6A18_44D5_A31F_E4E043684907__INCLUDED_)
