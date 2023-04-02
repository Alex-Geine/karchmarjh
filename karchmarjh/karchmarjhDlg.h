
// karchmarjhDlg.h: файл заголовка
//

#pragma once
#include "Drawer1.h"
#include "Signl.h"


// Диалоговое окно CkarchmarjhDlg
class CkarchmarjhDlg : public CDialogEx
{
// Создание
public:
	CkarchmarjhDlg(CWnd* pParent = nullptr);	// стандартный конструктор

// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_KARCHMARJH_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// поддержка DDX/DDV


// Реализация
protected:
	HICON m_hIcon;

	// Созданные функции схемы сообщений
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	double A1;
	double A2;
	double A3;
	double D1;
	double D2;
	double D3;
	double t1;
	double t2;
	double t3;
	double fd;
	double N;
	
	Drawer
		GR1,
		GR2,
		GR3,
		GR4;

	MSG msg;
	Signal curSignal;

	afx_msg void OnBnClickedOk();
	double alfa;
	double max_noise;
};
