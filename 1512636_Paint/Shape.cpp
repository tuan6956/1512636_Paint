#include "stdafx.h"
#include "Shape.h"




void CShape::setPen(Pen* p)
{
	
	pen =  p->Clone();
}

//void CShape::setBrush(HBRUSH brush)
//{
//	this->brush = brush;
//}

CShape::CShape()
{
}


CShape::~CShape()
{
}
