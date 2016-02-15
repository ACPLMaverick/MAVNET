#pragma once
#include "Light.h"
class LightDirectional :
	public Light
{
private:
	glm::vec3 m_direction;
public:
	LightDirectional();
	LightDirectional(glm::vec3*, glm::vec3*, glm::vec3*);
	LightDirectional(const LightDirectional*);
	~LightDirectional();

	void SetDirection(glm::vec3*);

	glm::vec3* GetDirection();
};

