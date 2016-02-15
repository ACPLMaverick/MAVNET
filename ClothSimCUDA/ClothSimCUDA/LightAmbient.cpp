#include "LightAmbient.h"


LightAmbient::LightAmbient() : Light()
{
}

LightAmbient::LightAmbient(glm::vec3* diff) : Light(diff, &glm::vec3(0.0f, 0.0f, 0.0f))
{
}

LightAmbient::LightAmbient(const LightAmbient* c) : Light(c)
{
}

LightAmbient::~LightAmbient()
{
}

