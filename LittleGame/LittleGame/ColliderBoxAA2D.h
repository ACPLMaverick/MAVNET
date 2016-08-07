#pragma once

#include "Collider.h"

class ColliderBoxAA2D :
	public Collider
{
protected:

	D3DXVECTOR2 m_min;
	D3DXVECTOR2 m_max;

	D3DXVECTOR2 m_startMin;
	D3DXVECTOR2 m_startMax;

public:
	ColliderBoxAA2D();
	~ColliderBoxAA2D();

	virtual void Initialize(const D3DXVECTOR2* const min, const D3DXVECTOR2* const max, uint32_t uid, GameObject* obj, std::string* name = nullptr);
	virtual void Shutdown();
	virtual void Update();
	virtual void LateUpdate();
	virtual void Draw();

#pragma region MyRegion

	const D3DXVECTOR2* const GetMin() { return &m_min; }
	const D3DXVECTOR2* const GetMax() { return &m_max; }
	const D3DXVECTOR2* const GetStartMin() { return &m_startMin; }
	const D3DXVECTOR2* const GetStartMax() { return &m_startMax; }

	void SetStartMin(const D3DXVECTOR2* const min) { m_startMin = *min; Update(); }
	void SetStartMax(const D3DXVECTOR2* const max) { m_startMax = *max; Update(); }

#pragma endregion

protected:

#pragma region Checkers

	const virtual bool CheckCollisionBoxAA2D(ColliderBoxAA2D* const col);
	const virtual bool CheckCollisionSphere2D(ColliderSphere2D* const col);

#pragma endregion

#pragma region Solvers

	const virtual void SolveCollisionBoxAA2D(ColliderBoxAA2D* const col, D3DXVECTOR3* movement, bool bias);
	const virtual void SolveCollisionSphere2D(ColliderSphere2D* const col, D3DXVECTOR3* movement, bool bias);

#pragma endregion
};

