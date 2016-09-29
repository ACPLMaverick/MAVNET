#pragma once

#include "stdafx.h"

struct Color32
{

	union
	{
		uint32_t color;
		uint8_t colors[4];
		uint8_t r, g, b, a;
	};

	Color32()
	{
		color = 0x00000000;
	}

	Color32(uint32_t color)
	{
		this->color = color;
	}

	Color32(uint8_t a, uint8_t r, uint8_t g, uint8_t b)
	{
		colors[0] = a;
		colors[1] = r;
		colors[2] = g;
		colors[3] = b;
	}
};

