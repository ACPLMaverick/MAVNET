#include "Primitive.h"




Primitive::Primitive()
{
}


Primitive::~Primitive()
{
}

inline uint16_t Primitive::ConvertFromScreenToBuffer(float point, uint16_t maxValue)
{
	return (uint16_t)(point * (float)maxValue * 0.5f + ((float)maxValue * 0.5f));
}