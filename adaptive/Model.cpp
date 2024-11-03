#include "pch.h"
#define p2(x) ((x) * (x))
#define EXP_G(x, x0, sig) exp(-p2(x - x0) / p2(sig))
#include "Model.h"
using namespace std;

void blur::SetBlur(int g_row_col, double sigma)
{
	if (g_row_col % 2 == 0) //ядро должно быть по размерам нечетное
	{
		MessageBox(NULL, L"Размер ядра должен быть нечетным! Автоматически размер увеличится на 1.", L"Предупреждение!", NULL);
		g_row_col++;
	}
	//очищаем матрицы
	gauss.clear();
	blur_pic.clear();

	CreateGauss(gauss, g_row_col, sigma);
	
	Convolution(orig_pic, blur_pic, gauss, floor(g_row_col / 2));
}

std::vector<std::vector<double>> blur::GetGauss()
{
	return gauss;
}

std::vector<std::vector<double>> blur::GetBlur()
{
	return blur_pic;
}

double blur::GetScoreRQBlur()
{
	return score_blur_RQ;
}

double blur::GetScoreRQOrig()
{
	return score_orig_RQ;
}

double blur::GetScoreCOrig()
{
	return score_orig_C;
}

double blur::GetScoreCBlur()
{
	return score_blur_C;
}

double blur::BlurScoreRQ(std::vector<std::vector<double>> pic)
{
	double max = 0;
	double min = 0;
	int M = pic.size();
	int N = pic[0].size();
	double sum = 0;

	normirovka(pic, max, min); //нормировать на (0, 255)

	for (int i = 1; i < M; i++) //вычисление суммы в критерии (ссылка)
	{
		for (int j = 1; j < N; j++)
		{
			sum += p2(abs(pic[i][j] - pic[i - 1][j]) + abs(pic[i][j] - pic[i][j - 1]));
		}
	}

	return sum / ((M - 1) * (N - 1) * 0.5 * max);
}

void blur::normirovka(std::vector<std::vector<double>>& pic, double& max, double& min)
{
	for (int i = 0; i < pic.size(); i++) //находим максимумы и минимумы
	{
		for (int j = 0; j < pic[0].size(); j++)
		{
			if (max < pic[i][j])
				max = pic[i][j];

			if (min > pic[i][j])
				min = pic[i][j];
		}
	}

	if (max == 255 && min == 0) return;
	for (int i = 0; i < pic.size(); i++)
	{
		for (int j = 0; j < pic[0].size(); j++)
		{
			pic[i][j] = (pic[i][j] - min) * 255 / (max - min);
		}
	}
}

void blur::Main(std::vector<std::vector<double>> orig, int g_row_col, double sigma, double score_sig1, double score_sig2)
{
	CreateFullOrig(orig, orig_pic, g_row_col);
	num_first_pic = (int)floor(g_row_col / 2);
	SetBlur(g_row_col, sigma);
	score_orig_RQ = BlurScoreRQ(orig);
	score_blur_RQ = BlurScoreRQ(blur_pic);
	score_orig_C = BlurScoreC(orig_pic, g_row_col, score_sig1, score_sig2);
	if (g_row_col < max_razm)
	{
		CreateFullOrig(blur_pic, blur_pic_cond, max_razm);
		num_first_pic = 3;
	}
	else
		CreateFullOrig(blur_pic, blur_pic_cond, g_row_col);
	score_blur_C = BlurScoreC(blur_pic_cond, g_row_col, score_sig1, score_sig2);
}

void blur::Convolution(std::vector<std::vector<double>> orig, std::vector<std::vector<double>>& res, std::vector<std::vector<double>> my_h, int area_blur)
{
	int max_row = orig.size() - 2 * num_first_pic;
	int max_col = orig[0].size() - 2 * num_first_pic;
	int max_h = 2 * area_blur + 1;
	res.resize(max_row);

	vector<double> first(max_h);
	vector<double> second(max_h);
	for (int i = 0; i < max_row; i++) //свертка ориг. с гауссом
	{
		res[i].resize(max_col);
		for (int j = 0; j < max_col; j++)
		{
			for (int k = -area_blur; k < area_blur; k++)
			{
				//res[i][j] = inner_product(orig[i + k].begin() + j, orig[i + k].begin() + j + max_h, my_h[k].begin(), 0.); //произведение векторов
				res[i][j] = inner_product(orig[i + num_first_pic + k].begin() + j + num_first_pic - area_blur, orig[i + num_first_pic + k].begin() + j + num_first_pic - area_blur + max_h, my_h[k + area_blur].begin(), 0.); //произведение векторов
			}
		}
	}
}

void blur::CreateGauss(std::vector<std::vector<double>>& pic, int r_matr, double sig)
{
	pic.resize(r_matr);
	int center_gauss = (int)floor(r_matr / 2);

	for (int i = 0; i < r_matr; i++) //создание гауссиана
	{
		pic[i].resize(r_matr);
		for (int j = 0; j < r_matr; j++)
		{
			pic[i][j] = EXP_G(j, r_matr, sig) * EXP_G(i, r_matr, sig);
		}
	}
}

double blur::BlurScoreC(std::vector<std::vector<double>> pic, int r_matr, double sig1, double sig2)
{
	if (sig1 > sig2)
		swap(sig1, sig2);

	vector<vector<double>> gauss1;
	vector<vector<double>> gauss2;
	vector<vector<double>> blur1;
	vector<vector<double>> blur2;

	CreateGauss(gauss1, r_matr, sig1);
	CreateGauss(gauss2, r_matr, sig2);
	Convolution(pic, blur1, gauss1, floor(r_matr / 2));
	Convolution(pic, blur2, gauss2, floor(r_matr / 2));

	double min1 = 0;
	double min2 = 0;
	double max1 = 0;
	double max2 = 0;

	int M = pic.size() - (r_matr - 1);
	int N = pic[0].size() - (r_matr - 1);

	normirovka(blur1, max1, min1);
	normirovka(blur2, max2, min2);

	double sum = 0;
	for (int i = 0; i < M; i++)
	{
		for (int j = 0; j < N; j++)
		{
			sum += blur2[i][j] - blur1[i][j];
		}
	}

	return sum / (M * N);
}

void blur::CreateFullOrig(std::vector<std::vector<double>> orig, std::vector<std::vector<double>>& res, int g_row_col)
{
	int size_orig_row = orig.size();
	int size_orig_col = orig[0].size();
	int polovina = (int)floor(g_row_col / 2);
	int size_orig_pic_row = size_orig_row + 2 * polovina;
	int size_orig_pic_col = size_orig_col + 2 * polovina;
	res.resize(size_orig_pic_row);

	for (int i = 0; i < size_orig_pic_row; i++)
	{
		res[i].resize(size_orig_pic_col);

		if (i >= polovina && i < size_orig_pic_row - polovina)
		{
			for (int j = 0; j < size_orig_pic_col; j++)
			{
				if (j >= polovina && j < size_orig_pic_col - polovina)
				{
					copy(orig[i - polovina].begin(), orig[i - polovina].end(), res[i].begin() + polovina); //копируем исх. изображение
					j = size_orig_pic_col - polovina - 1;
				}
				else
					if (j < polovina)
						res[i][j] = orig[i - polovina][0]; //гу левая сторона
					else
						if (j >= size_orig_pic_col - polovina)
							res[i][j] = orig[i - polovina].back(); //гу правая сторона
			}
		}
		else
		{
			for (int j = 0; j < size_orig_pic_col; j++)
			{
				if (j >= polovina && j < size_orig_pic_col - polovina)
				{
					if (i < polovina)
						copy(orig[0].begin(), orig[0].end(), res[i].begin() + polovina); //гу верхние
					else
						copy(orig.back().begin(), orig.back().end(), res[i].begin() + polovina); //гу нижние
					j = size_orig_pic_col - polovina - 1;
				}
				else
					if (i < polovina && j < polovina)
					res[i][j] = orig[0][0]; //гу левый верхний угол
				else
					if (i < polovina && j >= size_orig_pic_col - polovina)
						res[i][j] = orig[0].back(); //гу правый верхний угол
					else
						if (i >= size_orig_pic_row - polovina && j < polovina)
							res[i][j] = orig.back()[0]; //гу нижний левый угол
						else
							if (i >= size_orig_pic_row - polovina && j >= size_orig_pic_col - polovina)
								res[i][j] = orig.back().back(); //гу нижний правый угол

			}
		}
	}
}

std::vector<std::vector<double>> blur::GetBlurCond()
{
	return blur_pic_cond;
}

void parametr::CalcParam(std::vector<std::vector<double>> blur_pic)
{
	int max_row = blur_pic.size() - 2 * num_first_pic;
	int max_col = blur_pic[0].size() - 2 * num_first_pic;
	double um1 = 0;
	double um2 = 0;
	double um3 = 0;
	maxP = 0;
	minP = 0;
	param.resize(max_row);

	for (int i = 0; i < max_row; i++)
	{
		param[i].resize(max_col);
		for (int j = 0; j < max_col; j++)
		{
			for (int k = 0; k < 3; k++)
			{
				for (int h = 0; h < 5; h += 2)
				{
					um1 = blur_pic[i + num_first_pic - k - 1][j + num_first_pic - h];
					um2 = blur_pic[i + num_first_pic - k - 2][j + num_first_pic - h];
					um3 = blur_pic[i + num_first_pic - k - 3][j + num_first_pic - h];
					param[i][j] += abs(um1 - 2. * um2 + um3);
				}
			}
			param[i][j] /= 9;

			if (maxP < param[i][j])
				maxP = param[i][j];
			else
				if (minP > param[i][j])
					minP = param[i][j];
		}
	}
}

void parametr::Main(std::vector<std::vector<double>> picture, double part_max, double part_min, int pixel_cond)
{
	if (!param.empty())
		param.clear();

	num_first_pic = pixel_cond;

	CalcParam(picture);
	SetTeta(part_max, part_min);
}

void parametr::SetTeta(double part_max, double part_min)
{
	teta1 = part_min * maxP;
	teta2 = part_max * maxP;
}

std::vector<std::vector<double>> parametr::GetParam()
{
	return param;
}

double parametr::GetMax()
{
	return maxP;
}

double parametr::GetMin()
{
	return minP;
}

double parametr::GetTeta1()
{
	return teta1;
}

double parametr::GetTeta2()
{
	return teta2;
}
