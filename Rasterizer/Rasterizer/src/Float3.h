#pragma once

#include "stdafx.h"

struct Float3
{
	//union
	//{
	//	float tab[3];
		float x, y, z;
	//};

	Float3()
	{
		x = 0.0f;
		y = 0.0f;
		z = 0.0f;
	}

	Float3(const Float3& c)
	{
		this->x = c.x;
		this->y = c.y;
		this->z = c.z;
	}

	Float3(float x, float y, float z)
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}
};

