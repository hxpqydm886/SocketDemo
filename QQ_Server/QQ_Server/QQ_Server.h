
// QQ_Server.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CQQ_ServerApp: 
// �йش����ʵ�֣������ QQ_Server.cpp
//

class CQQ_ServerApp : public CWinApp
{
public:
	CQQ_ServerApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CQQ_ServerApp theApp;