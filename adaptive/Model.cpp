#include "pch.h"
#define p2(x) ((x) * (x))
#define EXP_G(x, x0, sig) exp(-p2(x - x0) / p2(sig))
#include "Model.h"
using namespace std;

void blur::SetBlur(std::vector<std::vector<double>> orig, int g_row_col, double sigma)
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
	
	Convolution(orig, blur_pic, gauss, floor(g_row_col / 2), b_type);
}

void blur::SetType(borderType my_type)
{
	b_type = my_type;
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

double BlurScoreRQ(std::vector<std::vector<double>> pic)
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

void normirovka(std::vector<std::vector<double>>& pic, double& max, double& min)
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
	SetBlur(orig, g_row_col, sigma);
	score_orig_RQ = BlurScoreRQ(orig);
	score_blur_RQ = BlurScoreRQ(blur_pic);
	score_orig_C = BlurScoreC(orig, g_row_col, score_sig1, score_sig2, b_type);
	score_blur_C = BlurScoreC(blur_pic, g_row_col, score_sig1, score_sig2, b_type);
}

std::vector<std::vector<double>> TwoMatrSum(std::vector<std::vector<double>> orig, std::vector<std::vector<double>> my_h, int max_row, int max_col, int area_blur, borderType b_type)
{
	vector<vector<double>> res(max_row, vector<double>(max_col));
	switch (b_type)
	{
	case b_null:
		for (int i = 0; i < max_row; i++) //свертка ориг. с гауссом
		{
			for (int j = 0; j < max_col; j++)
			{
				for (int k = -area_blur; k < area_blur; k++)
				{
					for (int h = -area_blur; h < area_blur; h++)
					{
						if (i - k >= 0 && j - h >= 0 && i - k < max_row && j - h < max_col)
							res[i][j] += orig[i - k][j - h] * my_h[abs(k - area_blur)][abs(h - area_blur)];
					}
				}
			}
		} return res;
	case b_repeat:
		for (int i = 0; i < max_row; i++) //свертка ориг. с гауссом
		{
			for (int j = 0; j < max_col; j++)
			{
				for (int k = -area_blur; k < area_blur; k++)
				{
					for (int h = -area_blur; h < area_blur; h++)
					{
						if (i - k >= 0 && j - h >= 0 && i - k < max_row && j - h < max_col)
							res[i][j] += orig[i - k][j - h] * my_h[abs(k - area_blur)][abs(h - area_blur)];
						else
							if ((i - k < 0 || i - k >= max_row) && (j - h < 0 || j - h >= max_col))
								res[i][j] += orig[i][j] * my_h[abs(k - area_blur)][abs(h - area_blur)];
							else
								if (i - k < 0 || i - k >= max_row)
									res[i][j] += orig[i][j - h] * my_h[abs(k - area_blur)][abs(h - area_blur)];
								else
									res[i][j] += orig[i - k][j] * my_h[abs(k - area_blur)][abs(h - area_blur)];
					}
				}
			}
		} return res;
	case b_mirror:
		break;
	case b_loop:
		break;
	}
	return res;
}

void Convolution(std::vector<std::vector<double>> orig, std::vector<std::vector<double>>& res, std::vector<std::vector<double>> my_h, int area_blur, borderType b_type)
{
	int max_row = orig.size();
	int max_col = orig[0].size();
	res = TwoMatrSum(orig, my_h, max_row, max_col, area_blur, b_type);
}

void CreateGauss(std::vector<std::vector<double>>& pic, int r_matr, double sig)
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

double BlurScoreC(std::vector<std::vector<double>> pic, int r_matr, double sig1, double sig2, borderType b_type)
{
	if (sig1 > sig2)
		swap(sig1, sig2);

	vector<vector<double>> gauss1;
	vector<vector<double>> gauss2;
	vector<vector<double>> blur1;
	vector<vector<double>> blur2;

	CreateGauss(gauss1, r_matr, sig1);
	CreateGauss(gauss2, r_matr, sig2);
	Convolution(pic, blur1, gauss1, floor(r_matr / 2), b_type);
	Convolution(pic, blur2, gauss2, floor(r_matr / 2), b_type);

	double min1 = 0;
	double min2 = 0;
	double max1 = 0;
	double max2 = 0;

	int M = pic.size();
	int N = pic[0].size();

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

void averagePower::CalcAvePower(std::vector<std::vector<double>> blur_pic, double part_max, double part_min) //НАПИСАТЬ ГРАН УСЛОВИЯ ЧТОБЫ РАБОТАЛО
{
	int max_row = blur_pic.size();
	int max_col = blur_pic[0].size();
	maxPower = minPower = 0;
	int ind_col = 0;

	avePower.resize(max_row);
	switch (b_type)
	{
	case b_null:
		for (int i = 0; i < max_row; i++) //свертка ориг. с гауссом
		{
			avePower[i].resize(max_col);
			for (int j = 0; j < max_col; j++)
			{
				for (int k = 0; k <= 2; k++)
				{
					for (int h = 0; h <= 4; h += 2)
					{
						if (j - h >= 0 && j - h < max_col)
							if (i - k - 3 >= 0 && i - k - 3 < max_row)
								avePower[i][j] += abs(blur_pic[i - k - 1][j - h] - 2 * blur_pic[i - k - 2][j - h] + blur_pic[i - k - 3][j - h]);
							else
								if (i - k - 2 >= 0 && i - k - 2 < max_row)
									avePower[i][j] += abs(blur_pic[i - k - 1][j - h] - 2 * blur_pic[i - k - 2][j - h]);
								else
									if (i - k - 1 >= 0 && i - k - 1 < max_row)
										avePower[i][j] += abs(blur_pic[i - k - 1][j - h]);
					}
				}
				if (maxPower < avePower[i][j])
					maxPower = avePower[i][j];
				if (minPower > avePower[i][j])
					minPower = avePower[i][j];
			}
		} break;
	case b_repeat:
		for (int i = 0; i < max_row; i++) //свертка ориг. с гауссом
		{
			avePower[i].resize(max_col);
			for (int j = 0; j < max_col; j++)
			{
				for (int k = 0; k <= 2; k++)
				{
					for (int h = 0; h <= 4; h += 2)
					{
						if (j - h >= 0 && j - h < max_col)
							ind_col = j - h;
						else
							ind_col = j;

						if (i - k - 3 >= 0 && i - k - 3 < max_row)
							avePower[i][j] += abs(blur_pic[i - k - 1][ind_col] - 2 * blur_pic[i - k - 2][ind_col] + blur_pic[i - k - 3][ind_col]);
						else
							if (i - k - 2 >= 0 && i - k - 2 < max_row)
								avePower[i][j] += abs(blur_pic[i - k - 1][ind_col] - 2 * blur_pic[i - k - 2][ind_col] + blur_pic[i - k - 2][ind_col]);
							else
								if (i - k - 1 >= 0 && i - k - 1 < max_row)
									avePower[i][j] += abs(blur_pic[i - k - 1][ind_col]);
					}
				}
				if (maxPower < avePower[i][j])
					maxPower = avePower[i][j];
				if (minPower > avePower[i][j])
					minPower = avePower[i][j];
			}
		} break;
	case b_mirror:
		break;
	case b_loop:
		break;
	}

	teta1 = part_min * minPower;
	teta2 = part_max * maxPower;
}

void averagePower::SetType(borderType my_type)
{
	b_type = my_type;
}

void averagePower::SetTeta(double part_max, double part_min)
{
	teta1 = part_min * minPower;
	teta2 = part_max * maxPower;
}
