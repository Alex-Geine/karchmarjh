
// karchmarjh.h: главный файл заголовка для приложения PROJECT_NAME
//

#pragma once

#ifndef __AFXWIN_H__
	#error "включить pch.h до включения этого файла в PCH"
#endif

#include "resource.h"		// основные символы


// CkarchmarjhApp:
// Сведения о реализации этого класса: karchmarjh.cpp
//

class CkarchmarjhApp : public CWinApp
{
public:
	CkarchmarjhApp();

// Переопределение
public:
	virtual BOOL InitInstance();

// Реализация

	DECLARE_MESSAGE_MAP()
};

extern CkarchmarjhApp theApp;
