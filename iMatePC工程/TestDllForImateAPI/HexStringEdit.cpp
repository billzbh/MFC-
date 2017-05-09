// HexStringEdit.cpp : implementation file
//

#include "stdafx.h"
#include "TestDllForImateAPI.h"
#include "HexStringEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHexStringEdit

CHexStringEdit::CHexStringEdit()
{
}

CHexStringEdit::~CHexStringEdit()
{
}


BEGIN_MESSAGE_MAP(CHexStringEdit, CEdit)
	//{{AFX_MSG_MAP(CHexStringEdit)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
	ON_WM_CHAR()
	ON_MESSAGE(WM_PASTE, OnPaste)
END_MESSAGE_MAP()

/*
BOOL CHexStringEdit::PreTranslateMessage(MSG* pMsg)
{
	UINT  nKeyCode = pMsg->wParam; // virtual key code of the key pressed  
   
	if (pMsg->message == WM_KEYDOWN)  
	{     
	if ( (nKeyCode == _T('C') || nKeyCode == _T('X')   
			|| nKeyCode == _T('V')) &&   
										(::GetKeyState(VK_CONTROL) & 0x8000) )  
	{  
		::TranslateMessage(pMsg);  
		::DispatchMessage(pMsg);  
		return(TRUE);    
	}  
 }  
 return CEdit::PreTranslateMessage(pMsg);  
}
*/

void CHexStringEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	BOOL bConitue;
	bConitue = (nChar >= '0' && nChar <= '9') // 数字
		|| nChar == VK_BACK || nChar == VK_DELETE
		||(nChar >= 'a' && nChar <= 'f')     // a-f
	    ||(nChar >= 'A' && nChar <= 'F');     // a-f

	if(!bConitue) return;                     // 限定输入
	CEdit::OnChar(  nChar,  nRepCnt,  nFlags);
}

void CHexStringEdit::OnPaste(WPARAM wParam, LPARAM lParam)
{
// do nothing
}
/////////////////////////////////////////////////////////////////////////////
// CHexStringEdit message handlers
