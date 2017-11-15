#include "stdafx.h"
#include "Curve.h"


CCurve::CCurve()
{
}


CCurve::~CCurve()
{
}

CShape * CCurve::createNewShape(POINT pS, POINT pE)
{
	return nullptr;
}

void CCurve::Draw(HDC hdc)
{
	Graphics* graph = new Graphics(hdc);
	graph->DrawCurve(pen, &vPoint[0], vPoint.size());
	graph->Flush();
	delete graph;
}

void CCurve::Draw(HDC hdc, POINT pS, POINT pE)
{
	if (vPoint.empty())
		vPoint.push_back({ pS.x,pS.y });
	vPoint.push_back({ pE.x,pE.y });
	Graphics* graph = new Graphics(hdc);
	graph->DrawCurve(pen, &vPoint[0], vPoint.size());
	graph->Flush();
	delete graph;
}

CShape * CCurve::Clone() const
{
	return new CCurve(*this);
}

void CCurve::SaveObject(std::fstream & out)
{
	//int index = 3;
	//out.write((char*)&index, sizeof(int));
	////out.write((char*)&brush, sizeof(brush));
	//out.write((char*)&pen, sizeof(pen));
	//out.write((char*)&pStart, sizeof(pStart));
	//out.write((char*)&pEnd, sizeof(pEnd));
}

void CCurve::setPointStart(POINT pS)
{
	if (vPoint.empty())
		vPoint.push_back({pS.x,pS.y});
}

void CCurve::setPointEnd(POINT pE)
{
	vPoint.push_back({ pE.x,pE.y });
}
