#pragma once

#include "Component.h"
#include "PhysicsManager.h"

class ColliderBoxAA2D;
class ColliderSphere2D;

class Collider :
	public Component
{
protected:

	/*
		Here will be collider data in deriving classes.
	*/

public:

	/* enums */

	enum ColliderType
	{
		UNKNOWN,

		BOXAA_2D,
		SPHERE_2D,
		BOXO_2D,
		CAPSULE_2D,
		TERRAIN_2D,

		BOXAA,
		SPHERE,
		BOXO,
		CYLINDER,
		CAPSULE,
		MESH,
		TERRAIN
	};

	////

	Collider();
	~Collider();

	virtual void Initialize(uint32_t uid, GameObject* obj, std::string* name = nullptr);
	virtual void Shutdown();
	virtual void Update();
	virtual void Draw();

#pragma region Accessors

	const ColliderType GetType() { return m_type; }

#pragma endregion

#pragma region Checkers

	const virtual bool CheckCollisionBoxAA2D(const ColliderBoxAA2D* const col) = 0;
	const virtual bool CheckCollisionSphere2D(const ColliderSphere2D* const col) = 0;

#pragma endregion

#pragma region Solvers

	const virtual void SolveCollisionBoxAA2D(const ColliderBoxAA2D* const col, D3DXVECTOR3* movement, bool bias) = 0;
	const virtual void SolveCollisionSphere2D(const ColliderSphere2D* const col, D3DXVECTOR3* movement, bool bias) = 0;

#pragma endregion

protected:

	ColliderType m_type = UNKNOWN;
};

