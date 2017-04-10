#include "stdafx.h"
#include "LightDirectional.h"

namespace Lights
{
	LightDirectional::LightDirectional(const XMFLOAT4 & color, const XMFLOAT3 & direction) :
		_color(color),
		_direction(direction)
	{
		SetDirection(direction);
	}

	LightDirectional::~LightDirectional()
	{
	}

}