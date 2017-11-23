#include "stdafx.h"
#include "Line.h"


CLine::CLine(POINT pS, POINT pE)
{
	pStart = pS;
	pEnd = pE;
}

CShape * CLine::createNewShape(POINT pS, POINT pE)
{
	CShape* shape = new CLine(pS, pE);
	return shape;
}

void CLine::Draw(HDC hdc)
{
	Graphics* graph = new Graphics(hdc);
	graph->DrawLine(pen, pStart.x, pStart.y, pEnd.x, pEnd.y);
	graph->Flush();
	delete graph;
	/*SelectObject(hdc, pen);
	SelectObject(hdc, brush);
	MoveToEx(hdc, pStart.x, pStart.y, NULL);
	LineTo(hdc, pEnd.x, pEnd.y);*/
}

void CLine::Draw(HDC hdc, POINT pS, POINT pE)
{
	pStart = pS;
	pEnd = pE;

	Graphics* graph = new Graphics(hdc);
	graph->DrawLine(pen, pStart.x, pStart.y, pEnd.x, pEnd.y);
	graph->Flush();
	delete graph;
	//SelectObject(hdc, pen);
	//SelectObject(hdc, brush);
	//MoveToEx(hdc, pStart.x, pStart.y, NULL);
	//LineTo(hdc, pEnd.x, pEnd.y);
}

CShape * CLine::Clone() const
{
	return new CLine(*this);
}

void CLine::SaveObject(std::fstream &out)
{
	int index = 1;
	out.write((char*)&index, sizeof(int));
	//out.write((char*)&brush, sizeof(brush));
	out.write((char*)&pen, sizeof(pen));
	out.write((char*)&pStart, sizeof(pStart));
	out.write((char*)&pEnd, sizeof(pEnd));
}

void CLine::ReadObject(std::fstream & in)
{
	in.read((char*)&pen, sizeof(pen));

	//fs.read((char*)&hbrush, sizeof(hbrush));
	in.read((char*)&pStart, sizeof(pStart));
	in.read((char*)&pEnd, sizeof(pEnd));

}

void CLine::clearObject()
{
}



void CLine::setPointStart(POINT pS)
{
	pStart = pS;
}

void CLine::setPointEnd(POINT pE)
{
	pEnd = pE;
}

CLine::CLine()
{
}


CLine::~CLine()
{
}
