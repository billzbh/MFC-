
// MFCApplicationForWHNS.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CMFCApplicationForWHNSApp: 
// �йش����ʵ�֣������ MFCApplicationForWHNS.cpp
//

class CMFCApplicationForWHNSApp : public CWinApp
{
public:
	CMFCApplicationForWHNSApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CMFCApplicationForWHNSApp theApp;