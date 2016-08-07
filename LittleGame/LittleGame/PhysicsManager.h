#pragma once

/*
 This class is a management system for collision detecting and solving.
*/

#include "Singleton.h"

class Collider;
class ColliderSphere2D;
class ColliderBoxAA2D;

#include <vector>
#include <d3dx9math.h>

////

struct ColliderMovement
{
	Collider* collider;
	D3DXVECTOR3 movement;

	ColliderMovement()
	{
		collider = nullptr;
		movement = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}

	ColliderMovement(Collider* col, D3DXVECTOR3 mv)
	{
		collider = col;
		movement = mv;
	}
};

////

class PhysicsManager :
	public Singleton<PhysicsManager>
{
	friend class Singleton<PhysicsManager>;
	friend class Collider;
	friend class ColliderSphere2D;
	friend class ColliderBoxAA2D;

protected:
	std::vector<Collider*> m_collidersAll;
	std::vector<Collider*> m_collidersToCheck;
	std::vector<ColliderMovement> m_collidersToSolve;

	PhysicsManager();

	void AddCollider(Collider* col) { m_collidersAll.push_back(col); }
	bool RemoveCollider(Collider* col);

	inline void CheckCollider(Collider* col);
	inline void SolveCollider(ColliderMovement mv);
public:

	~PhysicsManager();

	void Initialize();
	void Shutdown();
	void Run();

	void AddColliderToCheck(Collider* col) { m_collidersToCheck.push_back(col); }

	bool RemoveColliderFromCheck(Collider* col);

	const Collider* GetCollider(uint32_t id);
	const Collider* GetCollider(const std::string* const name);
};

