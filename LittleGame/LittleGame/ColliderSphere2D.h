#pragma once

#include "Collider.h"

class ColliderSphere2D :
	public Collider
{
protected:

	D3DXVECTOR3 m_position;
	float m_radius;

	D3DXVECTOR3 m_startPosition;
	float m_startRadius;

public:
	ColliderSphere2D();
	~ColliderSphere2D();

	virtual void Initialize(const D3DXVECTOR3* const pos, float radius, uint32_t uid, GameObject* obj, std::string* name = nullptr);
	virtual void Shutdown();
	virtual void Update();
	virtual void Draw();

#pragma region Accessors

	const D3DXVECTOR3* const GetPosition() { return &m_position; }
	float GetRadius() { return m_radius; }
	const D3DXVECTOR3* const GetStartPosition() { return &m_startPosition; }
	float GetStartRadius() { return m_startRadius; }

	void SetStartPosition(const D3DXVECTOR3* const position) { m_startPosition = *position; Update(); }
	void SetStartRadius(float radius) { m_startRadius = radius; Update(); }

#pragma endregion

#pragma region Checkers

	const virtual bool CheckCollisionBoxAA2D(const ColliderBoxAA2D* const col);
	const virtual bool CheckCollisionSphere2D(const ColliderSphere2D* const col);

#pragma endregion

#pragma region Solvers

	const virtual void SolveCollisionBoxAA2D(const ColliderBoxAA2D* const col, D3DXVECTOR3* movement, bool bias);
	const virtual void SolveCollisionSphere2D(const ColliderSphere2D* const col, D3DXVECTOR3* movement, bool bias);

};

