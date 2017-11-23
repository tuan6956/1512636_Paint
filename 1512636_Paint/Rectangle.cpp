#include "stdafx.h"
#include "Rectangle.h"


CRectangle::CRectangle(POINT pS, POINT pE)
{
	pStart = pS;
	pEnd = pE;
}

CShape * CRectangle::createNewShape(POINT pS, POINT pE)
{
	CShape* shape = new CRectangle(pS, pE);
	return shape;
}

void CRectangle::fixCoor()
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

CRectangle::CRectangle()
{
}

void CRectangle::Draw(HDC hdc)
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
		{
			std::swap(pStart.y, pEnd.y);
		}
	}
	Graphics* graph = new Graphics(hdc);
	graph->DrawRectangle(pen, pStart.x, pStart.y, width, height);
	graph->Flush();
	delete graph;
	//SelectObject(hdc, pen);
	//SelectObject(hdc, brush);
	//Rectangle(hdc, pStart.x, pStart.y, pEnd.x, pEnd.y);
}

void CRectangle::Draw(HDC hdc, POINT ps, POINT pE)
{
	pStart = ps;
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
		{
			std::swap(pStart.y, pEnd.y);
		}
	}
	
	Graphics* graph = new Graphics(hdc);
	graph->DrawRectangle(pen, pStart.x, pStart.y, width, height);
	graph->Flush();
	delete graph;
	//SelectObject(hdc, pen);
	//SelectObject(hdc, brush);
	//Rectangle(hdc, pStart.x, pStart.y, pEnd.x, pEnd.y);
}

void CRectangle::SaveObject(std::fstream& out)
{
	int index = 2;
	out.write((char*)&index, sizeof(int));
	//out.write((char*)&brush, sizeof(brush));
	out.write((char*)&pen, sizeof(pen));
	out.write((char*)&pStart, sizeof(pStart));
	out.write((char*)&pEnd, sizeof(pEnd));

}

void CRectangle::ReadObject(std::fstream & in)
{
	in.read((char*)&pen, sizeof(pen));

	//fs.read((char*)&hbrush, sizeof(hbrush));
	in.read((char*)&pStart, sizeof(pStart));
	in.read((char*)&pEnd, sizeof(pEnd));
}

void CRectangle::clearObject()
{
}

void CRectangle::setPointStart(POINT pS)
{
	pStart = pS;
}

void CRectangle::setPointEnd(POINT pE)
{
	pEnd = pE;
}

CShape * CRectangle::Clone() const
{
	return new CRectangle(*this);
}


CRectangle::~CRectangle()
{
}
