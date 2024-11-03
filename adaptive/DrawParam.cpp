// DrawParam.cpp: файл реализации
//

#include "pch.h"
#define my_rect lpDrawItemStruct->rcItem
#include "adaptive.h"
#include "DrawParam.h"
using namespace Gdiplus;

// DrawParam

IMPLEMENT_DYNAMIC(DrawParam, CStatic)

DrawParam::DrawParam()
{
	GdiplusStartupInput input;
	Status s;
	s = GdiplusStartup(&token, &input, NULL);
	if (s != Ok) MessageBox(L"s != Ok", L"Error!");
	heightY = 10;
}

DrawParam::~DrawParam()
{
	GdiplusShutdown(token);
}


BEGIN_MESSAGE_MAP(DrawParam, CStatic)
END_MESSAGE_MAP()



// Обработчики сообщений DrawParam




void DrawParam::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	Graphics wnd(lpDrawItemStruct->hDC);
	Bitmap buffer(lpDrawItemStruct->rcItem.right, lpDrawItemStruct->rcItem.bottom, &wnd);
	Graphics draw_in_buffer(&buffer);

	draw_in_buffer.Clear(Color::White);

	double scaleX = (double)(my_rect.right - my_rect.left) / (max - min);
	double scaleY = (double)(my_rect.bottom - my_rect.top) / heightY;
	Matrix matr;
	matr.Scale(scaleX, scaleY);
	//draw_in_buffer.ScaleTransform(scaleX, scaleY);
	
	Pen p_black(Color::Black);
	Pen p_blue(Color::Blue, 2);
	Pen p_red(Color::Red, 2);

	Point pt[2];
	for (int i = 0; i < param.size(); i++)
	{
		pt[0] = Point(param[i], 0);
		pt[1] = Point(param[i], heightY);
		matr.TransformPoints(pt, 2);

		draw_in_buffer.DrawLine(&p_black, pt[0], pt[1]);
	}

	pt[0] = Point(teta1, 0);
	pt[1] = Point(teta1, heightY);
	matr.TransformPoints(pt, 2);

	draw_in_buffer.DrawLine(&p_blue, pt[0], pt[1]);

	pt[0] = Point(teta2, 0);
	pt[1] = Point(teta2, heightY);
	matr.TransformPoints(pt, 2);

	draw_in_buffer.DrawLine(&p_red, pt[0], pt[1]);

	wnd.DrawImage(&buffer, 0, 0, 0, 0, lpDrawItemStruct->rcItem.right, lpDrawItemStruct->rcItem.bottom, UnitPixel);
}

void DrawParam::SetParam(double maxP, double minP, double teta11, double teta22, std::vector<std::vector<double>> matr_par)
{
	max = maxP; min = minP; teta1 = teta11; teta2 = teta22;
	int rows = matr_par.size();
	int cols = matr_par[0].size();
	param.resize(rows * cols);

	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			param[i * cols + j] = matr_par[i][j];
		}
	}
}
