#include "stdafx.h"
#include "Ellipse.h"


CEllipse::CEllipse(POINT pS, POINT pE)
{
	pStart = pS;
	pEnd = pE;
}

CShape * CEllipse::createNewShape(POINT pS, POINT pE)
{
	CShape* shape = new CEllipse(pS, pE);
	return shape;
}

void CEllipse::fixCoor()
{
	if (abs(pStart.x - pEnd.x) > abs(pStart.y - pEnd.y))
	{
		if (pStart.x < pEnd.x)
			pEnd.x = pStart.x + abs(pStart.y - pEnd.y);
		else
			pEnd.x = pStart.x - abs(pStart.y - pEnd.y);
	}
	else
	{
		if (pStart.y < pEnd.y)
			pEnd.y = pStart.y + abs(pStart.x - pEnd.x);
		else
			pEnd.y = pStart.y - abs(pStart.x - pEnd.x);
	}
}

CEllipse::CEllipse()
{

}

void CEllipse::Draw(HDC hdc)
{
	int width = abs(pStart.x - pEnd.x);
	int height = abs(pStart.y - pEnd.y);
	if (pStart.x > pEnd.x)
	{
		if (pStart.y < pEnd.y)
			std::swap(pStart.x, pEnd.x);
		else
			std::swap(pStart, pEnd);
	}
	else
	{
		if (pStart.y > pEnd.y)
			std::swap(pStart.y, pEnd.y);
	}
	Graphics* graph = new Graphics(hdc);
	graph->DrawEllipse(pen, pStart.x, pStart.y, width, height);
	graph->Flush();
	delete graph;
	/*SelectObject(hdc, pen);
	SelectObject(hdc, brush);
	Ellipse(hdc, pStart.x, pStart.y, pEnd.x, pEnd.y);*/
}

void CEllipse::Draw(HDC hdc, POINT pS, POINT pE)
{
	pStart = pS;
	pEnd = pE;
	int width = abs(pStart.x - pEnd.x);
	int height = abs(pStart.y - pEnd.y);
	if (pStart.x > pEnd.x)
	{
		if (pStart.y < pEnd.y)
			std::swap(pStart.x, pEnd.x);
		else
			std::swap(pStart, pEnd);
	}
	else
	{
		if (pStart.y > pEnd.y)
			std::swap(pStart.y, pEnd.y);
	}
	Graphics* graph = new Graphics(hdc);
	graph->DrawEllipse(pen, pStart.x, pStart.y, width, height);
	graph->Flush();
	delete graph;
	/*SelectObject(hdc, pen);
	SelectObject(hdc, brush);
	Ellipse(hdc, pStart.x, pStart.y, pEnd.x, pEnd.y);*/
}

void CEllipse::SaveObject(std::fstream& out)
{
	int index = 3;
	out.write((char*)&index, sizeof(int));
	//out.write((char*)&brush, sizeof(brush));
	out.write((char*)&pen, sizeof(pen));
	out.write((char*)&pStart, sizeof(pStart));
	out.write((char*)&pEnd, sizeof(pEnd));
}

void CEllipse::ReadObject(std::fstream & in)
{
	in.read((char*)&pen, sizeof(pen));

	//fs.read((char*)&hbrush, sizeof(hbrush));
	in.read((char*)&pStart, sizeof(pStart));
	in.read((char*)&pEnd, sizeof(pEnd));
}

void CEllipse::clearObject()
{
}

void CEllipse::setPointStart(POINT pS)
{
	pStart = pS;

}

void CEllipse::setPointEnd(POINT pE)
{
	pEnd = pE;
}

CShape * CEllipse::Clone() const
{
	return new CEllipse(*this);
}


CEllipse::~CEllipse()
{
}


