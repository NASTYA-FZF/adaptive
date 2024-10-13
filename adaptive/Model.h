#pragma once
#include <vector>

//https://www.researchgate.net/publication/327727402_Mera_ocenki_rezkosti_cifrovogo_izobrazenia ������ �� �������� �������� (����������)

class blur
{
	//�������� �����������
	std::vector<std::vector<double>> blur_pic;
	//�����, � ������� �����������
	std::vector<std::vector<double>> gauss;
	//�������� ��� ����. �����������
	double score_orig_RQ;
	//�������� ��� ��������� �����������
	double score_blur_RQ;
	double score_orig_C;
	double score_blur_C;

public:
	//�������� ��������� �����������
	void SetBlur(std::vector<std::vector<double>> orig, int g_row_col, double sigma);

	//��������� ������
	std::vector<std::vector<double>> GetGauss();
	//��������� ��������� �����������
	std::vector<std::vector<double>> GetBlur();
	//��������� ������ �������� ��������� � ����. �����������
	double GetScoreRQBlur();
	double GetScoreRQOrig();
	double GetScoreCOrig();
	double GetScoreCBlur();

	//������� ������ ��������
	double BlurScoreRQ(std::vector<std::vector<double>> pic);
	//���������� (�� ������ ����� ��� ��������� (0, 255))
	void normirovka(std::vector<std::vector<double>>& pic, double& max, double& min);
	//������������������ ��������
	void Main(std::vector<std::vector<double>> orig, int g_row_col, double sigma, double score_sig1, double score_sig2);
	//������� ����������� � �����
	void Convolution(std::vector<std::vector<double>> orig, std::vector<std::vector<double>>& res, std::vector<std::vector<double>> my_h, int area_blur);
	//������� ���������
	void CreateGauss(std::vector<std::vector<double>>& pic, int r_matr, double sig);
	//������� ������ �������� �� 2 ���������
	double BlurScoreC(std::vector<std::vector<double>> pic, int r_matr, double sig1, double sig2);
};