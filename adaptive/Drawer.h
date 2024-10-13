#pragma once
#include <gdiplus.h>
#include <vector>
#include <math.h>

// Drawer

class Drawer : public CStatic
{
	DECLARE_DYNAMIC(Drawer)

public:
	Drawer();
	virtual ~Drawer();

protected:
	DECLARE_MESSAGE_MAP()
public:
	ULONG_PTR token;
	bool first_start, dec_log;
	double pWidth, pHeight;
	double lx, ly, width_granica;
	bool is_ampl;
	Gdiplus::Bitmap* picture;
	std::vector<std::vector<double>> matr, matr_log, res_image;
	virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
	void ConvertWB();
	double Inten(Gdiplus::Color my_col);
	void CreateImageFromArr();
	void LoadImage_(const wchar_t* path_file);
	void Norma(std::vector<std::vector<double>>& matrix);
	void normirovka(double min, double max, std::vector<std::vector<double>>& mat);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	void SetMatr(std::vector<std::vector<double>> get_matr, double x, double y, double wid, bool ampl);
	std::vector<std::vector<double>> GetMatr();
	void SetMatrLog();
};

void InterpolationMatr(std::vector<std::vector<double>>& result, std::vector<std::vector<double>> _image, double w, double h);