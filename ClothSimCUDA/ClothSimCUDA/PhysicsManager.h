#pragma once

/*
This class manages all collisions that take place in the scene.
It is also responsible for solving forces and object movements.
*/

#include "Singleton.h"
#include "Collider.h"

#include <vector>

class Collider;
class BoxAACollider;
class SphereCollider;

struct BoxAAData
{
	glm::vec3 min;
	glm::vec3 max;

	BoxAAData()
	{
		min = glm::vec3(0.0f, 0.0f, 0.0f);
		max = glm::vec3(0.0f, 0.0f, 0.0f);
	}

	BoxAAData(glm::vec3* min, glm::vec3* max)
	{
		this->min = *min;
		this->max = *max;
	}
};

struct SphereData
{
	glm::vec3 center;
	float radius;

	SphereData()
	{
		this->center = glm::vec3(0.0f, 0.0f, 0.0f);
		this->radius = 0.0f;
	}

	SphereData(glm::vec3* center, float radius)
	{
		this->center = *center;
		this->radius = radius;
	}
};

class PhysicsManager :
	public Singleton<PhysicsManager>
{
	friend class Singleton<PhysicsManager>;
private:
	std::vector<Collider*> m_colliders;
	std::vector<BoxAAData> m_boxCollidersData;
	std::vector<SphereData> m_sphereCollidersData;

	float m_gravity;
	bool m_ifDrawColliders;

	PhysicsManager();
public:
	PhysicsManager(const PhysicsManager*);
	~PhysicsManager();

	unsigned int Initialize();
	unsigned int Shutdown();
	unsigned int Run();

	BoxAACollider* CreateBoxAACollider(SimObject* obj, glm::vec3* min, glm::vec3* max);
	SphereCollider* CreateSphereCollider(SimObject* obj, glm::vec3* offset, float radius);
	bool RemoveCollider(Collider*);

	float GetGravity();
	bool GetIfDrawColliders();
	std::vector<Collider*>* GetColliders();
	std::vector<BoxAAData>* GetBoxCollidersData();
	std::vector<SphereData>* GetSphereCollidersData();
	int GetCollidersCount();
};

