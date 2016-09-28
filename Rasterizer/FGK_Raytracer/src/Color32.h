#pragma once

#include "stdafx.h"

struct Color32
{
	struct Color32Separate
	{
		uint8_t r, g, b, a;
	};
	union
	{
		uint32_t color;
		uint8_t colors[4];
		Color32Separate colors2;
	};
};

