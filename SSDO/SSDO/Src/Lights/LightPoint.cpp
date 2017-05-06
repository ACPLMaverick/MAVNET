#include "stdafx.h"
#include "LightPoint.h"

namespace Lights
{
	LightPoint::LightPoint(const XMFLOAT4A & color, const XMFLOAT3A & position, float range) :
		_color(color),
		_position(position),
		_range(range)
	{
	}

	LightPoint::~LightPoint()
	{
	}
}
