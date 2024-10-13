#pragma once
#include <vector>

//https://www.researchgate.net/publication/327727402_Mera_ocenki_rezkosti_cifrovogo_izobrazenia ссылка на критерии резкости (размытости)

class blur
{
	//размытое изображение
	std::vector<std::vector<double>> blur_pic;
	//гаусс, с которым сворачиваем
	std::vector<std::vector<double>> gauss;
	//критерий для ориг. изображения
	double score_orig_RQ;
	//критерий для размытого изображения
	double score_blur_RQ;
	double score_orig_C;
	double score_blur_C;

public:
	//создание размытого изображения
	void SetBlur(std::vector<std::vector<double>> orig, int g_row_col, double sigma);

	//получение гаусса
	std::vector<std::vector<double>> GetGauss();
	//получение размытого изображения
	std::vector<std::vector<double>> GetBlur();
	//получение оценки резкости размытого и ориг. изображения
	double GetScoreRQBlur();
	double GetScoreRQOrig();
	double GetScoreCOrig();
	double GetScoreCBlur();

	//рассчет оценки резкости
	double BlurScoreRQ(std::vector<std::vector<double>> pic);
	//нормировка (на случай когда вне интервала (0, 255))
	void normirovka(std::vector<std::vector<double>>& pic, double& max, double& min);
	//последовательность действий
	void Main(std::vector<std::vector<double>> orig, int g_row_col, double sigma, double score_sig1, double score_sig2);
	//свертка изображения с ядром
	void Convolution(std::vector<std::vector<double>> orig, std::vector<std::vector<double>>& res, std::vector<std::vector<double>> my_h, int area_blur);
	//рассчет гауссиана
	void CreateGauss(std::vector<std::vector<double>>& pic, int r_matr, double sig);
	//рассчет оценки резкости по 2 размытиям
	double BlurScoreC(std::vector<std::vector<double>> pic, int r_matr, double sig1, double sig2);
};