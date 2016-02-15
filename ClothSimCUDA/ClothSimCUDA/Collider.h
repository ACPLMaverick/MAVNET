#pragma once

/*
This is an abstract representation of a Collider.
*/

#include "Component.h"
#include "SimObject.h"
#include "PhysicsManager.h"

#include <vector>
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <glm\glm\glm.hpp>

class SimObject;
class SphereCollider;
class BoxAACollider;

struct CollisonTestResult
{
	glm::vec3 colVector;
	bool ifCollision;

	CollisonTestResult()
	{
		colVector = glm::vec3(0.0f, 0.0f, 0.0f);
		ifCollision = false;
	}
};

enum ColliderType
{
	BOX_AA,
	BOX_OO,
	SPHERE,
	CYLINDER,
	CLOTH
};

class Collider :
	public Component
{
	friend class PhysicsManager;
protected:
	std::vector<Collider*> m_collisionsSolvedWith;
	ColliderType m_type;
	unsigned int m_cDataID;

	Collider(SimObject* obj, unsigned int cDataID);
	bool HasAlreadyCollidedWith(Collider*);
public:
	
	Collider(const Collider* c);
	~Collider();

	virtual unsigned int Initialize();
	virtual unsigned int Shutdown();

	virtual unsigned int Update() = 0;
	virtual unsigned int Draw() = 0;

	virtual CollisonTestResult TestWithBoxAA(BoxAACollider* other) = 0;
	virtual CollisonTestResult TestWithSphere(SphereCollider* other) = 0;

	ColliderType GetType();
};

