#pragma once

#include "Collider.h"

class ColliderSphere2D :
	public Collider
{
protected:

	D3DXVECTOR2 m_position;
	float m_radius;

	D3DXVECTOR2 m_startPosition;
	float m_startRadius;

public:
	ColliderSphere2D();
	~ColliderSphere2D();

	virtual void Initialize(const D3DXVECTOR2* const pos, float radius, uint32_t uid, GameObject* obj, std::string* name = nullptr);
	virtual void Shutdown();
	virtual void Update();
	virtual void LateUpdate();
	virtual void Draw();

#pragma region Accessors

	const D3DXVECTOR2* const GetPosition() { return &m_position; }
	float GetRadius() { return m_radius; }
	const D3DXVECTOR2* const GetStartPosition() { return &m_startPosition; }
	float GetStartRadius() { return m_startRadius; }

	void SetStartPosition(const D3DXVECTOR2* const position) { m_startPosition = *position; Update(); }
	void SetStartRadius(float radius) { m_startRadius = radius; Update(); }

#pragma endregion

protected:

#pragma region Checkers

	const virtual bool CheckCollisionBoxAA2D(ColliderBoxAA2D* const col);
	const virtual bool CheckCollisionSphere2D(ColliderSphere2D* const col);

#pragma endregion

#pragma region Solvers

	const virtual void SolveCollisionBoxAA2D(ColliderBoxAA2D* const col, D3DXVECTOR3* movement, bool bias);
	const virtual void SolveCollisionSphere2D(ColliderSphere2D* const col, D3DXVECTOR3* movement, bool bias);

};

