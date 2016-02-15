#pragma once

/*
A simple sphere collider.
*/

#include "Collider.h"

#include <glm\glm\glm.hpp>

class Collider;

class SphereCollider :
	public Collider
{
	friend class BoxAACollider;
	friend class PhysicsManager;
protected:
	glm::vec3 m_center;
	glm::vec3 m_effectiveCenter;
	float m_radius;
	float m_effectiveRadius;

	SphereCollider(SimObject* obj, glm::vec3* offset, float radius, unsigned int cDataID);
public:
	~SphereCollider();

	virtual unsigned int Initialize();
	virtual unsigned int Shutdown();

	virtual unsigned int Update();
	virtual unsigned int Draw();

	virtual CollisonTestResult TestWithBoxAA(BoxAACollider* other);
	virtual CollisonTestResult TestWithSphere(SphereCollider* other);
};

