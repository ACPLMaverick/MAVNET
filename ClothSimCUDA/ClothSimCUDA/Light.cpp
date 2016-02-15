#include "Light.h"


Light::Light()
{
	m_multiplier = 1.0f;
}

Light::Light(glm::vec3* diff, glm::vec3* spec) : Light()
{
	SetDiffuseColor(diff);
	SetSpecularColor(spec);
}

Light::Light(const Light*)
{
}

Light::~Light()
{
}



void Light::SetDiffuseColor(glm::vec3* diff)
{
	m_diffuse = (*diff);
	m_diffuseMul = (*diff) * m_multiplier;
}

void Light::SetSpecularColor(glm::vec3* spec)
{
	m_specular = (*spec);
	m_specularMul = (*spec) * m_multiplier;
}

void Light::SetMultiplier(float mul)
{
	m_multiplier = mul;
	m_diffuseMul = m_diffuse * m_multiplier;
	m_specularMul = m_diffuse * m_multiplier;
}


glm::vec3* Light::GetDiffuseColor()
{
	return &(m_diffuseMul);
}

glm::vec3* Light::GetSpecularColor()
{
	return &(m_specularMul);
}

float Light::GetMultiplier()
{
	return m_multiplier;
}