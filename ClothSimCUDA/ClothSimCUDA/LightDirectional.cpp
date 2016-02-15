#include "LightDirectional.h"


LightDirectional::LightDirectional() : Light()
{
}

LightDirectional::LightDirectional(glm::vec3* diff, glm::vec3* spec, glm::vec3* dir) : Light(diff, spec)
{
	SetDirection(dir);
}

LightDirectional::LightDirectional(const LightDirectional* c) : Light(c)
{
}


LightDirectional::~LightDirectional()
{
}



void LightDirectional::SetDirection(glm::vec3* dir)
{
	m_direction = glm::normalize(*dir);
}




glm::vec3* LightDirectional::GetDirection()
{
	return &m_direction;
}