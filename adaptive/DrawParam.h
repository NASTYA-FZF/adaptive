#pragma once
#include <gdiplus.h>
#include <vector>

// DrawParam

class DrawParam : public CStatic
{
	DECLARE_DYNAMIC(DrawParam)

public:
	DrawParam();
	virtual ~DrawParam();

protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	ULONG_PTR token;
	double max, min;
	std::vector<double> param;
	double heightY;
	double teta1, teta2;
	void SetParam(double maxP, double minP, double teta11, double teta22, std::vector<std::vector<double>> matr_par);
};


