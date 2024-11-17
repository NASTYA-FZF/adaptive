#pragma once
#include <vector>
#include <algorithm>
#include <numeric>

//https://www.researchgate.net/publication/327727402_Mera_ocenki_rezkosti_cifrovogo_izobrazenia ссылка на критерии резкости (размытости)

//ПОДОБРАТЬ КАРТИНКУ С ЧЕТКИМИ ГРАНИЦАМИ (ДОМА КАКИЕ-НИБУДЬ)
//ПРОВЕРИТЬ МИНИМУМЫ И МАКСИМУМЫ ИЗОБРАЖЕНИЙ
//РЕШИТЬ ЧТО ДЕЛАТЬ, ЕСЛИ ВЫЛАЗИТ ЗА ПРЕДЕЛЫ 0 И 255 ВОССТАНОВЛЕННОЕ ИЗОБРАЖЕНИЕ
//ПОПРОБОВАТЬ ЕЩЕ ПАРОЧКУ ГРАНИЧЕК (ОТРАЖЕНИЕ И ЦИКЛИЧЕСКУЮ)

const int max_razm = 12;

class blur
{
	//исходное изображение с гу (повторение краевых пикселей)
	std::vector<std::vector<double>> orig_pic;
	//размытое изображение
	std::vector<std::vector<double>> blur_pic;
	//размытое изображение с гу
	std::vector<std::vector<double>> blur_pic_cond;
	//гаусс, с которым сворачиваем
	std::vector<std::vector<double>> gauss;
	//критерий для ориг. изображения
	double score_orig_RQ;
	//критерий для размытого изображения
	double score_blur_RQ;
	double score_orig_C;
	double score_blur_C;

public:
	int num_first_pic;
	//создание размытого изображения
	void SetBlur(int g_row_col, double sigma);

	//получение гаусса
	std::vector<std::vector<double>> GetGauss();
	//получение размытого изображения
	std::vector<std::vector<double>> GetBlur();
	//получение оценки резкости размытого и ориг. изображения
	double GetScoreRQBlur();
	double GetScoreRQOrig();
	double GetScoreCOrig();
	double GetScoreCBlur();

	//последовательность действий
	void Main(std::vector<std::vector<double>> orig, int g_row_col, double sigma, double score_sig1, double score_sig2);

	std::vector<std::vector<double>> GetBlurCond();
};

//рассчет оценки резкости
double BlurScoreRQ(std::vector<std::vector<double>> pic);
//нормировка (на случай когда вне интервала (0, 255))
void normirovka(std::vector<std::vector<double>>& pic, double& max, double& min);
//свертка изображения с ядром
void Convolution(std::vector<std::vector<double>> orig, std::vector<std::vector<double>>& res, std::vector<std::vector<double>> my_h, int area_blur, double num_first_pic);
//рассчет гауссиана
void CreateGauss(std::vector<std::vector<double>>& pic, int r_matr, double sig);
//рассчет оценки резкости по 2 размытиям
double BlurScoreC(std::vector<std::vector<double>> pic, int r_matr, double sig1, double sig2, double num_first_pic);
//создание изображения с гу
void CreateFullOrig(std::vector<std::vector<double>> orig, std::vector<std::vector<double>>& res, int g_row_col, int& num_first_pic);

class parametr
{
	std::vector<std::vector<double>> blur_pic;
	std::vector<std::vector<double>> param;
	double teta1;
	double teta2;
	double maxP;
	double minP;

public:
	int num_first_pic;

	void CalcParam(std::vector<std::vector<double>> blur_pic);

	void Main(std::vector<std::vector<double>> picture, double part_max, double part_min, int pixel_cond);

	void SetTeta(double part_max, double part_min);

	std::vector<std::vector<double>> GetParam();

	double GetMax();

	double GetMin();

	double GetTeta1();

	double GetTeta2();
};

class filter
{
	std::vector<std::vector<double>> res_pic;
	std::vector<std::vector<std::vector<double>>> h;
	int num_first_pic;
	double scoreRQfilter;
	double scoreCfilter;

public:
	void CalcResult(std::vector<std::vector<double>> blur_pic);

	void CalcH(double teta1, double teta2, std::vector<std::vector<double>> param);

	void Main(std::vector<std::vector<double>> blur_pic, double teta1, double teta2, std::vector<std::vector<double>> param, double first_pixel, int g_row_col, double s1, double s2);

	std::vector<std::vector<double>> GetResPic();

	double GetRQ();

	double GetC();
};