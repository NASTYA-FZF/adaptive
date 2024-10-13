
// adaptiveDlg.cpp: файл реализации
//

#include "pch.h"
#include "framework.h"
#include "adaptive.h"
#include "adaptiveDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Диалоговое окно CadaptiveDlg



CadaptiveDlg::CadaptiveDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_ADAPTIVE_DIALOG, pParent)
	, r_gauss(11)
	, sigma(5)
	, sigma1(1)
	, sigma2(3)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CadaptiveDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ORIG, pic_original);
	DDX_Control(pDX, IDC_BAD, pic_blur);
	DDX_Control(pDX, IDC_ORIG2, pic_filter);
	DDX_Text(pDX, IDC_EDIT1, r_gauss);
	DDX_Text(pDX, IDC_EDIT2, sigma);
	DDX_Control(pDX, IDC_EDIT5, textRQOrig);
	DDX_Control(pDX, IDC_EDIT6, textRQBlur);
	DDX_Control(pDX, IDC_EDIT7, textCOrig);
	DDX_Control(pDX, IDC_EDIT8, textCBlur);
	DDX_Text(pDX, IDC_EDIT3, sigma1);
	DDX_Text(pDX, IDC_EDIT4, sigma2);
}

BEGIN_MESSAGE_MAP(CadaptiveDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_LOAD_PIC, &CadaptiveDlg::OnBnClickedLoadPic)
	ON_BN_CLICKED(IDOK, &CadaptiveDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// Обработчики сообщений CadaptiveDlg

BOOL CadaptiveDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Задает значок для этого диалогового окна.  Среда делает это автоматически,
	//  если главное окно приложения не является диалоговым
	SetIcon(m_hIcon, TRUE);			// Крупный значок
	SetIcon(m_hIcon, FALSE);		// Мелкий значок

	// TODO: добавьте дополнительную инициализацию

	return TRUE;  // возврат значения TRUE, если фокус не передан элементу управления
}

// При добавлении кнопки свертывания в диалоговое окно нужно воспользоваться приведенным ниже кодом,
//  чтобы нарисовать значок.  Для приложений MFC, использующих модель документов или представлений,
//  это автоматически выполняется рабочей областью.

void CadaptiveDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // контекст устройства для рисования

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Выравнивание значка по центру клиентского прямоугольника
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Нарисуйте значок
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// Система вызывает эту функцию для получения отображения курсора при перемещении
//  свернутого окна.
HCURSOR CadaptiveDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CadaptiveDlg::OnBnClickedLoadPic()
{
	// TODO: добавьте свой код обработчика уведомлений
	CFileDialog change_image(TRUE);
	change_image.DoModal();
	auto path = change_image.GetPathName();
	pic_original.LoadImage_(path);
	Invalidate(FALSE);
}


void CadaptiveDlg::OnBnClickedOk()
{
	// TODO: добавьте свой код обработчика уведомлений
	if (pic_original.GetMatr().empty())
	{
		MessageBox(L"Загрузите картинку!", L"Ошибка!");
		return;
	}

	UpdateData();
	matr_blur.Main(pic_original.GetMatr(), r_gauss, sigma, sigma1, sigma2);
	textRQBlur.SetWindowTextW(text_value(matr_blur.GetScoreRQBlur()));
	textRQOrig.SetWindowTextW(text_value(matr_blur.GetScoreRQOrig()));
	textCOrig.SetWindowTextW(text_value(matr_blur.GetScoreCOrig()));
	textCBlur.SetWindowTextW(text_value(matr_blur.GetScoreCBlur()));
	pic_blur.SetMatr(matr_blur.GetBlur(), 0, 0, 0, false);
	Invalidate(FALSE);
}

CString CadaptiveDlg::text_value(double val)
{
	CString str;
	str.Format(L"%.4f", val);
	return str;
}
