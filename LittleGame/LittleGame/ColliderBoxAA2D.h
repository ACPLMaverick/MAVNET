#pragma once

#include "Collider.h"

class ColliderBoxAA2D :
	public Collider
{
protected:

	D3DXVECTOR3 m_min;
	D3DXVECTOR3 m_max;

	D3DXVECTOR3 m_startMin;
	D3DXVECTOR3 m_startMax;

public:
	ColliderBoxAA2D();
	~ColliderBoxAA2D();

	virtual void Initialize(const D3DXVECTOR3* const min, const D3DXVECTOR3* const max, uint32_t uid, GameObject* obj, std::string* name = nullptr);
	virtual void Shutdown();
	virtual void Update();
	virtual void Draw();

#pragma region MyRegion

	const D3DXVECTOR3* const GetMin() { return &m_min; }
	const D3DXVECTOR3* const GetMax() { return &m_max; }
	const D3DXVECTOR3* const GetStartMin() { return &m_startMin; }
	const D3DXVECTOR3* const GetStartMax() { return &m_startMax; }

	void SetStartMin(const D3DXVECTOR3* const min) { m_startMin = *min; Update(); }
	void SetStartMax(const D3DXVECTOR3* const max) { m_startMax = *max; Update(); }

#pragma endregion

#pragma region Checkers

	const virtual bool CheckCollisionBoxAA2D(const ColliderBoxAA2D* const col);
	const virtual bool CheckCollisionSphere2D(const ColliderSphere2D* const col);

#pragma endregion

#pragma region Solvers

	const virtual void SolveCollisionBoxAA2D(const ColliderBoxAA2D* const col, D3DXVECTOR3* movement, bool bias);
	const virtual void SolveCollisionSphere2D(const ColliderSphere2D* const col, D3DXVECTOR3* movement, bool bias);

#pragma endregion
};

