// Drawer.cpp: файл реализации
//

#include "pch.h"
#define my_rect lpDrawItemStruct->rcItem
#include "adaptive.h"
#include "Drawer.h"
using namespace std;
using namespace Gdiplus;

// Drawer

IMPLEMENT_DYNAMIC(Drawer, CStatic)

Drawer::Drawer()
{
	GdiplusStartupInput input;
	Status s;
	s = GdiplusStartup(&token, &input, NULL);
	if (s != Ok) MessageBox(L"s != Ok", L"Error!");
	first_start = true;
	dec_log = true;
	is_ampl = false;
}

Drawer::~Drawer()
{
	GdiplusShutdown(token);
}


BEGIN_MESSAGE_MAP(Drawer, CStatic)
//	ON_WM_DRAWITEM()
ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()



// Обработчики сообщений Drawer




//void Drawer::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
//{
	// TODO: добавьте свой код обработчика сообщений или вызов стандартного
	
//}


void Drawer::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	// TODO:  Добавьте исходный код для отображения указанного элемента
	if (matr.empty())
	{
		//MessageBox(L"Нечего выводить! Матрица пуста!", L"Ошибка");
		return;
	}

	if (dec_log)
	{
		Norma(matr);
		InterpolationMatr(res_image, matr, lpDrawItemStruct->rcItem.right, lpDrawItemStruct->rcItem.bottom);
	}
	else
	{
		Norma(matr_log);
		InterpolationMatr(res_image, matr_log, lpDrawItemStruct->rcItem.right, lpDrawItemStruct->rcItem.bottom);
	}
	//Norma(res_image);
	

	Graphics wnd(lpDrawItemStruct->hDC);
	Bitmap buffer(lpDrawItemStruct->rcItem.right, lpDrawItemStruct->rcItem.bottom, &wnd);
	Graphics draw_in_buffer(&buffer);

	pWidth = res_image[0].size();
	pHeight = res_image.size();
	Bitmap pic(pWidth, pHeight);
	picture = &pic;

	CreateImageFromArr();
	double scaleX = (double)(my_rect.right - my_rect.left) / matr[0].size();
	double scaleY = (double)(my_rect.bottom - my_rect.top) / matr.size();

	draw_in_buffer.DrawImage(picture, Rect(0, 0, picture->GetWidth(), picture->GetHeight()));

	Pen p_granica(Color::Green, 5);
	if (is_ampl) draw_in_buffer.DrawRectangle(&p_granica, (REAL)(lx * scaleX), (REAL)(ly * scaleY), (REAL)(width_granica * scaleX), (REAL)(width_granica * scaleY));

	wnd.DrawImage(&buffer, 0, 0, 0, 0, lpDrawItemStruct->rcItem.right, lpDrawItemStruct->rcItem.bottom, UnitPixel);
}

void Drawer::ConvertWB()
{
	pHeight = picture->GetHeight();
	pWidth = picture->GetWidth();
	matr = vector<vector<double>>(pHeight, vector<double>(pWidth));

	if (pHeight == 0 || pWidth == 0)
	{
		MessageBox(L"Изображение некорректно загружено!", L"Ошибка");
		return;
	}

	Status ctrl;
	Color my_color;
	for (int i = 0; i < pWidth; i++)
	{
		for (int j = 0; j < pHeight; j++)
		{
			ctrl = picture->GetPixel(i, j, &my_color);
			matr[j][i] = Inten(my_color);
			if (ctrl != Ok)
			{
				MessageBox(L"Что-то пошло не так при получении пикселя!", L"Ошибка");
				break;
			}
		}
	}
}

double Drawer::Inten(Color my_col)
{
	return 0.299 * my_col.GetRed() + 0.587 * my_col.GetGreen() + 0.114 * my_col.GetBlue();
}

void Drawer::CreateImageFromArr()
{
	Status ctrl;

	for (int i = 0; i < pWidth; i++)
	{
		for (int j = 0; j < pHeight; j++)
		{
			ctrl = picture->SetPixel(i, j, Color(res_image[j][i], res_image[j][i], res_image[j][i]));
			if (ctrl != Ok)
			{
				MessageBox(L"Что-то пошло не так при установлении цвета пикселя!", L"Ошибка");
				break;
			}
		}
	}
}

void Drawer::LoadImage_(const wchar_t* path_file)
{
	Bitmap pic(path_file);
	picture = &pic;
	ConvertWB();
	matr_log = matr;
	SetMatrLog();
}

void Drawer::Norma(std::vector<std::vector<double>>& matrix)
{
	int h = matrix.size();
	int w = matrix[0].size();
	double min = 0, max = 0;
	for (int i = 0; i < h; i++)
	{
		for (int j = 0; j < w; j++)
		{
			if (max < matrix[i][j])
				max = matrix[i][j];
			if (min > matrix[i][j])
				min = matrix[i][j];
		}
	}
	normirovka(min, max, matrix);
}

void Drawer::normirovka(double min, double max, vector<vector<double>>& mat)
{
	for (int i = 0; i < mat.size(); i++)
	{
		for (int j = 0; j < mat[0].size(); j++)
			mat[i][j] = (mat[i][j] - min) * 255 / (max - min);
	}
}


void Drawer::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: добавьте свой код обработчика сообщений или вызов стандартного
	dec_log = !dec_log;
	Invalidate(FALSE);
	CStatic::OnLButtonDown(nFlags, point);
}

void Drawer::SetMatr(std::vector<std::vector<double>> get_matr, double x, double y, double wid, bool ampl)
{
	lx = x; ly = y; width_granica = wid; is_ampl = ampl;
	matr_log = matr = get_matr;

	SetMatrLog();
}

std::vector<std::vector<double>> Drawer::GetMatr()
{
	/*int w = matr[0].size(), h = matr.size(), iter = 2;
	bool step_w = false, step_h = false;
	do
	{
		if (w <= iter && !step_w)
		{
			step_w = true;
			w = iter;
		}
		if (h <= iter && !step_h)
		{
			step_h = true;
			h = iter;
		}
		iter *= 2;
	} while (!step_w || !step_h);

	vector<vector<double>> res;*/
	//AddNull(res, w, h);
	//InterpolationMatr(res, matr, w, h);
	return matr;
}

void Drawer::SetMatrLog()
{
	int h = matr_log.size();

	if (h <= 0)
		return;

	int w = matr_log[0].size();
	double max = 0;

	for (int i = 0; i < h; i++)
	{
		for (int j = 0; j < w; j++)
		{
			if (max < matr_log[i][j])
				max = matr_log[i][j];
		}
	}

	for (int i = 0; i < h; i++)
	{
		for (int j = 0; j < w; j++)
		{
			if (matr_log[i][j] != 0)
				matr_log[i][j] = -log10(matr_log[i][j] / max);
		}
	}
}

//void Drawer::AddNull(std::vector<std::vector<double>>& result, int w_new, int h_new)
//{
//	w_start = matr[0].size();
//	h_start = matr.size();
//
//	result = matr;
//	result.resize(h_new);
//	for (int i = 0; i < h_new; i++)
//	{
//		result[i].resize(w_new, 0);
//	}
//}
//
//void Drawer::DeleteNull(std::vector<std::vector<double>>& result)
//{
//	result.resize(h_start);
//
//	for (int i = 0; i < h_start; i++)
//	{
//		result[i].resize(w_start);
//	}
//}

void InterpolationMatr(std::vector<std::vector<double>>& result, std::vector<std::vector<double>> _image, double w, double h)
{
	result = vector<vector<double>>(h, vector<double>(w));

	double d_x, d_y, im_w = _image[0].size(), im_h = _image.size();
	int i_x, i_y;
	double fR1, fR2;

	double otn_w = (im_w - 1) / (w - 1), otn_h = (im_h - 1) / (h - 1);

	for (int i = 0; i < h; i++)
	{
		d_y = i * otn_h; i_y = floor(d_y);
		for (int j = 0; j < w; j++)
		{
			d_x = j * otn_w; i_x = floor(d_x);

			if (i_x == _image[0].size() - 1)
				i_x--;
			if (i_y == _image.size() - 1)
				i_y--;

			fR1 = (i_x + 1. - d_x) * _image[i_y + 1][i_x] + (d_x - i_x) * _image[i_y + 1][i_x + 1];
			fR2 = (i_x + 1. - d_x) * _image[i_y][i_x] + (d_x - i_x) * _image[i_y][i_x + 1];

			result[i][j] = abs(i_y - d_y) * fR1 + abs(d_y - (i_y + 1)) * fR2;
		}
	}
}
