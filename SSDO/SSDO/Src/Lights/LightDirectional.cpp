#include "stdafx.h"
#include "LightDirectional.h"

namespace Lights
{
	LightDirectional::LightDirectional(const XMFLOAT4A & color, const XMFLOAT3A & direction) :
		_color(color),
		_direction(direction)
	{
		SetDirection(direction);
	}

	LightDirectional::~LightDirectional()
	{
	}

}