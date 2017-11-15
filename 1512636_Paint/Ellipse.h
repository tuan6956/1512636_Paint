#pragma once
#include "Shape.h"

class CEllipse :public CShape
{
 private:
	POINT pStart, pEnd;
public:
	CEllipse(POINT pS, POINT pE);
	CShape* createNewShape(POINT pS, POINT pE);
	void fixCoor();
	CEllipse();
	void Draw(HDC hdc);
	void Draw(HDC hdc, POINT pS, POINT pE);
	void SaveObject(std::fstream& out);
	void setPointStart(POINT pS);
	void setPointEnd(POINT pE);
	CShape* Clone() const;
	~CEllipse();
};



