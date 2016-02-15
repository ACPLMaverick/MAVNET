#include "SphereCollider.h"
#include "BoxAACollider.h"


SphereCollider::SphereCollider(SimObject* obj, glm::vec3* offset, float radius, unsigned int cDataID) : Collider(obj, cDataID)
{
	m_effectiveCenter = glm::vec3(0.0f, 0.0f, 0.0f);
	m_center = *offset;
	m_radius = radius;
}

SphereCollider::~SphereCollider()
{
}



unsigned int SphereCollider::Initialize()
{
	unsigned int err = Collider::Initialize();
	if (err != CS_ERR_NONE)
		return err;

	m_type = SPHERE;

	return err;
}

unsigned int SphereCollider::Shutdown()
{
	unsigned int err = Collider::Shutdown();
	if (err != CS_ERR_NONE)
		return err;

	return err;
}

unsigned int SphereCollider::Update()
{
	if (m_obj->GetTransform() != nullptr)
	{
		glm::vec4 tempCenter = glm::vec4(m_center.x, m_center.y, m_center.z, 1.0f);

		tempCenter = (*(m_obj->GetTransform()->GetWorldMatrix())) * tempCenter;

		m_effectiveCenter.x = tempCenter.x;
		m_effectiveCenter.y = tempCenter.y;
		m_effectiveCenter.z = tempCenter.z;

		glm::vec3* scale = m_obj->GetTransform()->GetScale();
		float sclModifier = (scale->x + scale->y + scale->z) / 3.0f;

		m_effectiveRadius = m_radius * sclModifier;

		PhysicsManager::GetInstance()->GetSphereCollidersData()->at(m_cDataID).center = m_effectiveCenter;
		PhysicsManager::GetInstance()->GetSphereCollidersData()->at(m_cDataID).radius = m_effectiveRadius;
	}
	else
	{
		m_effectiveCenter = m_center;
	}
	return CS_ERR_NONE;
}

unsigned int SphereCollider::Draw()
{
	return CS_ERR_NONE;
}



CollisonTestResult SphereCollider::TestWithBoxAA(BoxAACollider* other)
{
	CollisonTestResult res;

	glm::vec3 cls, closest;

	Vec3Max(&m_effectiveCenter, &other->m_minEffective, &cls);
	Vec3Min(&cls, &other->m_maxEffective, &closest);

	float distance = Vec3LengthSquared(&(closest - m_effectiveCenter));

	if (distance < (m_effectiveRadius * m_effectiveRadius))
	{
		closest = m_effectiveCenter - closest;
		res.ifCollision = true;
		res.colVector = glm::normalize(closest) * (m_effectiveRadius - glm::sqrt(distance));
		//printf("Collision! %f %f %f\n", closest.x, closest.y, closest.z);
	}

	return res;
}

CollisonTestResult SphereCollider::TestWithSphere(SphereCollider* other)
{
	CollisonTestResult res;

	if (m_effectiveCenter != other->m_effectiveCenter)
	{
		glm::vec3 diff = m_effectiveCenter - other->m_effectiveCenter;
		float diffLength = Vec3LengthSquared(&diff);
		
		if (diffLength < (m_effectiveRadius + other->m_effectiveRadius) * (m_effectiveRadius + other->m_effectiveRadius))
		{
			//printf("Collision! %f %f %f | %f %f %f\n", m_effectiveCenter.x, m_effectiveCenter.y, m_effectiveCenter.z, 
			//	other->m_effectiveCenter.x, other->m_effectiveCenter.y, other->m_effectiveCenter.z);

			res.ifCollision = true;

			diff = glm::normalize(diff);
			diff = diff * ((m_effectiveRadius + other->m_effectiveRadius) - glm::sqrt(diffLength));

			glm::vec3 cPos = (*m_obj->GetTransform()->GetPosition());
			cPos = cPos + diff;
			m_obj->GetTransform()->SetPosition(&cPos);
		}
	}
	else
	{
		res.ifCollision = true;
		res.colVector = glm::vec3(0.0f, m_effectiveRadius + other->m_effectiveRadius, 0.0f);
	}

	return res;
}