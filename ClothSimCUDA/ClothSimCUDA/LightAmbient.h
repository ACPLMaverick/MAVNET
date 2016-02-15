#pragma once
#include "Light.h"
class LightAmbient :
	public Light
{
public:
	LightAmbient();
	LightAmbient(glm::vec3*);
	LightAmbient(const LightAmbient*);
	~LightAmbient();
};

