#pragma once

/*
	Represents an abstract Light object, which is a container for light color (and thus, intensity), light specular color and other basic data.
*/

#include "Common.h"

#include <glm\glm\glm.hpp>

class Light
{
protected:
	glm::vec3 m_diffuse;
	glm::vec3 m_specular;

	glm::vec3 m_diffuseMul;
	glm::vec3 m_specularMul;

	float m_multiplier;
public:
	Light();
	Light(glm::vec3*, glm::vec3*);
	Light(const Light*);
	~Light();

	virtual void SetDiffuseColor(glm::vec3*);
	virtual void SetSpecularColor(glm::vec3*);
	virtual void SetMultiplier(float);

	virtual glm::vec3* GetDiffuseColor();
	virtual glm::vec3* GetSpecularColor();
	virtual float GetMultiplier();
};

