#pragma once
#include "Shape.h"

class CLine : public CShape
{
private:
	POINT pStart, pEnd;
public:
	CLine(POINT pS, POINT pE);
	CShape* createNewShape(POINT pS, POINT pE);
	void Draw(HDC hdc);
	void Draw(HDC hdc, POINT pS, POINT pE);
	CShape* Clone() const;
	void SaveObject(std::fstream &out);
	void ReadObject(std::fstream &in);
	void clearObject();
	void setPointStart(POINT pS);
	void setPointEnd(POINT pE);
	CLine();
	~CLine();
};

