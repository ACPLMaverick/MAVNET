#include "BoxAACollider.h"
#include "SphereCollider.h"
#include "Timer.h"

BoxAACollider::BoxAACollider(SimObject* obj, glm::vec3* min, glm::vec3* max, unsigned int cDataID) : Collider(obj, cDataID)
{
	m_min = *min;
	m_max = *max;
}


BoxAACollider::~BoxAACollider()
{
}



unsigned int BoxAACollider::Initialize()
{
	unsigned int err = Collider::Initialize();
	if (err != CS_ERR_NONE)
		return err;

	m_type = BOX_AA;

	return err;
}

unsigned int BoxAACollider::Shutdown()
{
	unsigned int err = Collider::Shutdown();
	if (err != CS_ERR_NONE)
		return err;

	return err;
}

unsigned int BoxAACollider::Update()
{
	if (m_obj->GetTransform() != nullptr)
	{
		glm::vec4 min = glm::vec4(m_min, 1.0f);
		glm::vec4 max = glm::vec4(m_max, 1.0f);

		glm::mat4 wm = glm::translate(*m_obj->GetTransform()->GetPosition()) *
			glm::scale(*m_obj->GetTransform()->GetScale());

		min = wm * min;
		max = wm * max;

		m_minEffective.x = min.x;
		m_minEffective.y = min.y;
		m_minEffective.z = min.z;

		m_maxEffective.x = max.x;
		m_maxEffective.y = max.y;
		m_maxEffective.z = max.z;

		PhysicsManager::GetInstance()->GetBoxCollidersData()->at(m_cDataID).min = m_minEffective;
		PhysicsManager::GetInstance()->GetBoxCollidersData()->at(m_cDataID).max = m_maxEffective;
	}
	else
	{
		m_minEffective = m_min;
		m_maxEffective = m_max;
	}
	

	return CS_ERR_NONE;
}

unsigned int BoxAACollider::Draw()
{
	return CS_ERR_NONE;
}



CollisonTestResult BoxAACollider::TestWithBoxAA(BoxAACollider* other)
{
	CollisonTestResult res;

	glm::vec3 min, max;
	min = m_minEffective - other->m_maxEffective;
	max = (m_maxEffective - m_minEffective) + (other->m_maxEffective - other->m_minEffective);
	max = min + max;

	if (
		min.x <= 0.0f &&
		max.x >= 0.0f &&
		min.y <= 0.0f &&
		max.y >= 0.0f &&
		min.z <= 0.0f &&
		max.z >= 0.0f
		)
	{
		res.ifCollision = true;

		glm::vec3 negAbs, posAbs;
		negAbs.x = glm::abs(min.x);
		negAbs.y = glm::abs(min.y);
		negAbs.z = glm::abs(min.z);
		posAbs.x = glm::abs(max.x);
		posAbs.y = glm::abs(max.y);
		posAbs.z = glm::abs(max.z);

		if (
			negAbs.x <= posAbs.x &&
			negAbs.x <= negAbs.y &&
			negAbs.x <= posAbs.y &&
			negAbs.x <= negAbs.z &&
			negAbs.x <= posAbs.z
			)
		{
			res.colVector = glm::vec3(-min.x, 0.0f, 0.0f);
		}
		else if (
			posAbs.x <= negAbs.x &&
			posAbs.x <= negAbs.y &&
			posAbs.x <= posAbs.y &&
			posAbs.x <= negAbs.z &&
			posAbs.x <= posAbs.z
			)
		{
			res.colVector = glm::vec3(-max.x, 0.0f, 0.0f);
		}
		else if (
			negAbs.y <= negAbs.x &&
			negAbs.y <= posAbs.x &&
			negAbs.y <= posAbs.y &&
			negAbs.y <= negAbs.z &&
			negAbs.y <= posAbs.z
			)
		{
			res.colVector = glm::vec3(0.0f, -min.y, 0.0f);
		}
		else if (
			posAbs.y <= negAbs.x &&
			posAbs.y <= negAbs.y &&
			posAbs.y <= negAbs.y &&
			posAbs.y <= negAbs.z &&
			posAbs.y <= posAbs.z
			)
		{
			res.colVector = glm::vec3(0.0f, -max.y, 0.0f);
		}
		else if (
			negAbs.z <= negAbs.x &&
			negAbs.z <= posAbs.x &&
			negAbs.z <= posAbs.y &&
			negAbs.z <= negAbs.y &&
			negAbs.z <= posAbs.z
			)
		{
			res.colVector = glm::vec3(0.0f, 0.0f, -min.z);
		}
		else
		{
			res.colVector = glm::vec3(0.0f, 0.0f, -max.z);
		}

//#ifdef _DEBUG
//		printf("Collision! %f %f %f | %f %f %f\n", min.x, min.y, min.z, max.x, max.y, max.z);
//#endif
	}
	else
	{
		res.ifCollision = false;
		res.colVector = glm::vec3(0.0f, 0.0f, 0.0f);
	}

	return res;
}

CollisonTestResult BoxAACollider::TestWithSphere(SphereCollider* other)
{
	CollisonTestResult res;

	glm::vec3 cls, closest;

	Vec3Max(&other->m_effectiveCenter, &m_minEffective, &cls);
	Vec3Min(&cls, &m_maxEffective, &closest);

	glm::vec3 distVec = closest - other->m_effectiveCenter;
	float distance = Vec3LengthSquared(&distVec);

	if (distance < (other->m_effectiveRadius * other->m_effectiveRadius))
	{
		closest = other->m_effectiveCenter - closest;
		res.ifCollision = true;
		res.colVector = glm::normalize(closest) * (other->m_effectiveRadius - glm::sqrt(distance));
		//printf("Collision! %f %f %f\n", closest.x, closest.y, closest.z);
	}

	return res;
}