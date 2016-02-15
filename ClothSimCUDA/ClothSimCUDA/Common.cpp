#include "Common.h"

void Vec3Min(const glm::vec3* vec1, const glm::vec3* vec2, glm::vec3* ret)
{
	ret->x = glm::min(vec1->x, vec2->x);
	ret->y = glm::min(vec1->y, vec2->y);
	ret->z = glm::min(vec1->z, vec2->z);
}

void Vec3Max(const glm::vec3* vec1, const glm::vec3* vec2, glm::vec3* ret)
{
	ret->x = glm::max(vec1->x, vec2->x);
	ret->y = glm::max(vec1->y, vec2->y);
	ret->z = glm::max(vec1->z, vec2->z);
}

float Vec3LengthSquared(const glm::vec3* vec)
{
	return vec->x * vec->x + vec->y * vec->y + vec->z * vec->z;
}