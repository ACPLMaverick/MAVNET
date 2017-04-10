#include "stdafx.h"
#include "LightPoint.h"

namespace Lights
{
	LightPoint::LightPoint(const XMFLOAT4 & color, const XMFLOAT3 & position, float range) :
		_color(color),
		_position(position),
		_range(range)
	{
	}

	LightPoint::~LightPoint()
	{
	}
}
