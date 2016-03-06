#include "stdafx.h"
#include "ColliderSphere2D.h"


ColliderSphere2D::ColliderSphere2D()
{
}


ColliderSphere2D::~ColliderSphere2D()
{
}

void ColliderSphere2D::Initialize(const D3DXVECTOR3* const pos, float radius, uint32_t uid, GameObject * obj, std::string * name)
{
	Collider::Initialize(uid, obj, name);
	m_type = Collider::ColliderType::SPHERE;

	m_startPosition = *pos;
	m_startRadius = radius;

	Update();
}

void ColliderSphere2D::Shutdown()
{
	Collider::Shutdown();
}

void ColliderSphere2D::Update()
{
	Collider::Update();
}

void ColliderSphere2D::Draw()
{
	Collider::Draw();
}

const bool ColliderSphere2D::CheckCollisionBoxAA2D(const ColliderBoxAA2D * const col)
{
	return false;
}

const bool ColliderSphere2D::CheckCollisionSphere2D(const ColliderSphere2D * const col)
{
	return false;
}

const void ColliderSphere2D::SolveCollisionBoxAA2D(const ColliderBoxAA2D * const col, D3DXVECTOR3 * movement, bool bias)
{
	return void();
}

const void ColliderSphere2D::SolveCollisionSphere2D(const ColliderSphere2D * const col, D3DXVECTOR3 * movement, bool bias)
{
	return void();
}
