#pragma once

#include "stdafx.h"

struct Float3
{
	struct Float3Internal
	{
		float x, y, z;
	};
	union
	{
		Float3Internal inter;
		float tab[3];
	};
};

