#include "Triangle.h"



Triangle::Triangle(Float3 x, Float3 y, Float3 z) :
	Primitive()
{
	m_x = x;
	m_y = y;
	m_z = z;
}


Triangle::~Triangle()
{
}

void Triangle::Update()
{
}

void Triangle::Draw(Buffer<int32_t>* const buf, Buffer<float>* const depth)
{
}
