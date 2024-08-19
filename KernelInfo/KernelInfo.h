
// KernelInfo.h: PROJECT_NAME 应用程序的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"		// 主符号
#include "DriverComm.h"


// CKernelInfoApp:
// 有关此类的实现，请参阅 KernelInfo.cpp
//

class CKernelInfoApp : public CWinApp
{
public:
	
	CKernelInfoApp();

// 重写
public:
	virtual BOOL InitInstance();
	DriverComm dc;

// 实现

	DECLARE_MESSAGE_MAP()
};

extern CKernelInfoApp theApp;
