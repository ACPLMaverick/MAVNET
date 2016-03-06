#include "stdafx.h"
#include "ColliderBoxAA2D.h"


ColliderBoxAA2D::ColliderBoxAA2D()
{
}


ColliderBoxAA2D::~ColliderBoxAA2D()
{
}

void ColliderBoxAA2D::Initialize(const D3DXVECTOR3* const min, const D3DXVECTOR3* const max, uint32_t uid, GameObject * obj, std::string * name)
{
	Collider::Initialize(uid, obj, name);
	m_type = Collider::ColliderType::BOXAA;
	m_startMin = *min;
	m_startMax = *max;

	Update();
}

void ColliderBoxAA2D::Shutdown()
{
	Collider::Shutdown();
}

void ColliderBoxAA2D::Update()
{
	Collider::Update();
}

void ColliderBoxAA2D::Draw()
{
	Collider::Draw();
}

const bool ColliderBoxAA2D::CheckCollisionBoxAA2D(const ColliderBoxAA2D * const col)
{
	return false;
}

const bool ColliderBoxAA2D::CheckCollisionSphere2D(const ColliderSphere2D * const col)
{
	return false;
}

const void ColliderBoxAA2D::SolveCollisionBoxAA2D(const ColliderBoxAA2D * const col, D3DXVECTOR3 * movement, bool bias)
{
	return void();
}

const void ColliderBoxAA2D::SolveCollisionSphere2D(const ColliderSphere2D * const col, D3DXVECTOR3 * movement, bool bias)
{
	return void();
}
