#include "LightAmbient.h"


LightAmbient::LightAmbient() : Light()
{
}

LightAmbient::LightAmbient(glm::vec3* diff) : Light()
{
	SetDiffuseColor(diff);
	glm::vec3 zero = glm::vec3();
	SetSpecularColor(&zero);
}

LightAmbient::LightAmbient(const LightAmbient* c) : Light(c)
{
}

LightAmbient::~LightAmbient()
{
}

