#pragma once
#include "Shape.h"
#include <vector>

class CCurve : public CShape
{
private:
	std::vector<Point> vPoint;
public:
	//CCurve(POINT pS, POINT pE);
	CShape* createNewShape(POINT pS, POINT pE);
	void Draw(HDC hdc);
	void Draw(HDC hdc, POINT pS, POINT pE);
	CShape* Clone() const;
	void SaveObject(std::fstream &out);
	void ReadObject(std::fstream &in);
	void clearObject();
	void setPointStart(POINT pS);
	void setPointEnd(POINT pE);
	CCurve();
	~CCurve();
};

