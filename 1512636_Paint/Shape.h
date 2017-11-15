#pragma once
#include <fstream>
#include <objidl.h>
#include <gdiplus.h>
#pragma comment (lib,"Gdiplus.lib")
using namespace Gdiplus;
class CShape
{
protected:
	Pen* pen;
public:
	virtual void Draw(HDC hdc) = 0;
	virtual void Draw(HDC hdc, POINT, POINT) = 0;
	virtual CShape* Clone() const = 0;
	virtual CShape* createNewShape(POINT, POINT) = 0;
	virtual void SaveObject(std::fstream& out) = 0;
	virtual void setPointStart(POINT pS) = 0;
	virtual void setPointEnd(POINT pE) = 0;
	void setPen(Pen* p);
	//void setBrush(HBRUSH brush);
	CShape();
	~CShape();
};

