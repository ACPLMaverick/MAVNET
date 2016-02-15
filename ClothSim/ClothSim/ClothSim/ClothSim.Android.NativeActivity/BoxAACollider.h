#pragma once

/*
Axis-aligned box collider.
*/

#include "Collider.h"

#include <glm\glm\glm.hpp>

class Collider;

class BoxAACollider :
	public Collider
{
	friend class SphereCollider;
	friend class PhysicsManager;
protected:
	glm::vec3 m_min;
	glm::vec3 m_max;

	glm::vec3 m_minEffective;
	glm::vec3 m_maxEffective;

	BoxAACollider(SimObject*, glm::vec3*, glm::vec3*, unsigned int cDataID);
public:
	~BoxAACollider();

	virtual unsigned int Initialize();
	virtual unsigned int Shutdown();

	virtual unsigned int Update();
	virtual unsigned int Draw();

	virtual CollisonTestResult TestWithBoxAA(BoxAACollider* other);
	virtual CollisonTestResult TestWithSphere(SphereCollider* other);
};

