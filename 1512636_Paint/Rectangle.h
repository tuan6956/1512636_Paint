#pragma once
#include "Shape.h"
class CRectangle :public CShape
{
private:
	POINT pStart, pEnd;
public:
	CRectangle(POINT pS, POINT pE);
	CShape* createNewShape(POINT pS, POINT pE);
	void fixCoor();
	CRectangle();
	void Draw(HDC hdc);
	void Draw(HDC hdc, POINT ps, POINT pE);
	void SaveObject(std::fstream& out);
	void ReadObject(std::fstream &in);
	void clearObject();
	void setPointStart(POINT pS);
	void setPointEnd(POINT pE);
	CShape* Clone() const;
	~CRectangle();
};

