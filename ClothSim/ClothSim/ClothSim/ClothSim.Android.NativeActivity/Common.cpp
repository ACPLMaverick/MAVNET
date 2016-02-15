#include "Common.h"
#include <sstream>
#include <iomanip>

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

void DoubleToStringPrecision(double value, int decimals, std::string* str)
{
	std::ostringstream ss;
	ss << std::fixed << std::setprecision(decimals) << value;
	*str = ss.str();
	if (decimals > 0 && (*str)[str->find_last_not_of('0')] == '.') {
		str->erase(str->size() - decimals + 1);
	}
}