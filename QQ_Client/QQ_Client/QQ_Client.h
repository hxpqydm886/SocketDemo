
// QQ_Client.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CQQ_ClientApp: 
// �йش����ʵ�֣������ QQ_Client.cpp
//

class CQQ_ClientApp : public CWinApp
{
public:
	CQQ_ClientApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CQQ_ClientApp theApp;