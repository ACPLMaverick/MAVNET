#include "Triangle.h"



Triangle::Triangle(Float3 x, Float3 y, Float3 z, Color32 col) :
	Primitive()
{
	v1 = x;
	v2 = y;
	v3 = z;
	m_col = col;
}


Triangle::~Triangle()
{
}

void Triangle::Update()
{
}

void Triangle::Draw(Buffer<int32_t>* const buf, Buffer<float>* const depth)
{
	int32_t v1x, v1y, v2x, v2y, v3x, v3y;
	v1x = (int32_t)ConvertFromScreenToBuffer(v1.x, buf->GetWidth());
	v1y = (int32_t)ConvertFromScreenToBuffer(v1.y, buf->GetHeight());
	v2x = (int32_t)ConvertFromScreenToBuffer(v2.x, buf->GetWidth());
	v2y = (int32_t)ConvertFromScreenToBuffer(v2.y, buf->GetHeight());
	v3x = (int32_t)ConvertFromScreenToBuffer(v3.x, buf->GetWidth());
	v3y = (int32_t)ConvertFromScreenToBuffer(v3.y, buf->GetHeight());

	int32_t minX = (min(min(v1x, v2x), v3x));
	int32_t minY = (min(min(v1y, v2y), v3y));
	int32_t maxX = (max(max(v1x, v2x), v3x));
	int32_t maxY = (max(max(v1y, v2y), v3y));

	for (int32_t i = minY; i <= maxY; ++i)
	{
		for (int32_t j = minX; j <= maxX; ++j)
		{
			if (
				((v2x - v1x) * (i - v1y) - (v2y - v1y) * (j - v1x)) <= 0 &&
				((v3x - v2x) * (i - v2y) - (v3y - v2y) * (j - v2x)) <= 0 &&
				((v1x - v3x) * (i - v3y) - (v1y - v3y) * (j - v3x)) <= 0
				)
			{
				buf->SetPixel(j, i, m_col.color);
			}
		}
	}
}
