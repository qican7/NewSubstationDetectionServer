
// NewSubstationDetectionServer.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CNewSubstationDetectionServerApp:
// �йش����ʵ�֣������ NewSubstationDetectionServer.cpp
//

class CNewSubstationDetectionServerApp : public CWinApp
{
public:
	CNewSubstationDetectionServerApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CNewSubstationDetectionServerApp theApp;