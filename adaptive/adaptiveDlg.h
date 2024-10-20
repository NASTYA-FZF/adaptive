
// adaptiveDlg.h: файл заголовка
//

#pragma once
#include "Drawer.h"
#include "Model.h"
#include <string>

// Диалоговое окно CadaptiveDlg
class CadaptiveDlg : public CDialogEx
{
// Создание
public:
	CadaptiveDlg(CWnd* pParent = nullptr);	// стандартный конструктор

// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ADAPTIVE_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// поддержка DDX/DDV


// Реализация
protected:
	HICON m_hIcon;

	// Созданные функции схемы сообщений
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	Drawer pic_original;
	Drawer pic_blur;
	Drawer pic_filter;
	blur matr_blur;
	afx_msg void OnBnClickedLoadPic();
	afx_msg void OnBnClickedOk();
	int r_gauss;
	double sigma;
	CEdit textRQOrig;
	CEdit textRQBlur;

	CString text_value(double val);
	CEdit textCOrig;
	CEdit textCBlur;
	double sigma1;
	double sigma2;
	CButton rbNull;
	CButton rbRepeat;
	CButton rbMirror;
	CButton rbLoop;
	afx_msg void OnBnClickedRbrepeat();
	afx_msg void OnBnClickedRbnull();
	afx_msg void OnBnClickedRbmirror();
	afx_msg void OnBnClickedRbloop();
};
