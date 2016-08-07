#include "stdafx.h"
#include "ColliderSphere2D.h"
#include "GameObject.h"
#include "Transform.h"

ColliderSphere2D::ColliderSphere2D()
{
}


ColliderSphere2D::~ColliderSphere2D()
{
}

void ColliderSphere2D::Initialize(const D3DXVECTOR2* const pos, float radius, uint32_t uid, GameObject * obj, std::string * name)
{
	Collider::Initialize(uid, obj, name);
	m_type = Collider::ColliderType::SPHERE_2D;

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

void ColliderSphere2D::LateUpdate()
{
	D3DXVECTOR4 buffer;
	D3DXVec2Transform(&buffer, &m_startPosition, m_obj->GetTransform()->GetWorld());
	m_position = D3DXVECTOR2(buffer);
	float scaleLength = D3DXVec3Length(m_obj->GetTransform()->GetScale());
	m_radius = m_startRadius * (scaleLength / 1.73205f);
}

void ColliderSphere2D::Draw()
{
	Collider::Draw();
}

const bool ColliderSphere2D::CheckCollisionBoxAA2D(ColliderBoxAA2D * const col)
{
	return false;
}

const bool ColliderSphere2D::CheckCollisionSphere2D(ColliderSphere2D * const col)
{
	return false;
}

const void ColliderSphere2D::SolveCollisionBoxAA2D(ColliderBoxAA2D * const col, D3DXVECTOR3 * movement, bool bias)
{
	return void();
}

const void ColliderSphere2D::SolveCollisionSphere2D(ColliderSphere2D * const col, D3DXVECTOR3 * movement, bool bias)
{
	return void();
}
