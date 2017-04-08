#include "stdafx.h"
#include "LightDirectional.h"

namespace Lights
{
	LightDirectional::LightDirectional(const XMFLOAT4 & color, const XMFLOAT3 & direction) :
		LightAmbient(color),
		_direction(direction)
	{
	}

	LightDirectional::~LightDirectional()
	{
	}

}