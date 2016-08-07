#include "stdafx.h"
#include "ColliderBoxAA2D.h"
#include "ColliderSphere2D.h"
#include "GameObject.h"
#include "Transform.h"

ColliderBoxAA2D::ColliderBoxAA2D()
{
}


ColliderBoxAA2D::~ColliderBoxAA2D()
{
}

void ColliderBoxAA2D::Initialize(const D3DXVECTOR2* const min, const D3DXVECTOR2* const max, uint32_t uid, GameObject * obj, std::string * name)
{
	Collider::Initialize(uid, obj, name);
	m_type = Collider::ColliderType::BOXAA_2D;
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

void ColliderBoxAA2D::LateUpdate()
{
	D3DXVECTOR4 buffer;
	D3DXVec2Transform(&buffer, &m_startMin, m_obj->GetTransform()->GetWorld());
	m_min = D3DXVECTOR2(buffer);
	D3DXVec2Transform(&buffer, &m_startMax, m_obj->GetTransform()->GetWorld());
	m_max = D3DXVECTOR2(buffer);
}

void ColliderBoxAA2D::Draw()
{
	Collider::Draw();
}

const bool ColliderBoxAA2D::CheckCollisionBoxAA2D(ColliderBoxAA2D * const col)
{
	D3DXVECTOR2 cMin = *(col->GetMin());
	D3DXVECTOR2 cMax = *(col->GetMax());

	D3DXVECTOR2 min, max;
	min = m_min - cMax;
	max = (m_max - m_min) + (cMax - cMin);
	max = min + max;

	if (
			min.x <= 0.0f &&
			max.x >= 0.0f &&
			min.y <= 0.0f &&
			max.y >= 0.0f
		)
	{
		Collider::AssignTempColVec(D3DXVECTOR3(min.x, min.y, 0.0f), D3DXVECTOR3(max.x, max.y, 0.0f));
		return true;
	}
	else
	{
		return false;
	}
}

const bool ColliderBoxAA2D::CheckCollisionSphere2D(ColliderSphere2D * const col)
{
	D3DXVECTOR2 pos = *col->GetPosition();
	float rad = col->GetRadius();

	D3DXVECTOR2 cls, closest;
	
	D3DXVec2Maximize(&cls, &pos, &m_min);
	D3DXVec2Minimize(&closest, &cls, &m_max);

	cls = closest - pos;
	float distance = D3DXVec2LengthSq(&cls);

	if (distance < rad * rad)
	{
		Collider::AssignTempColVec(D3DXVECTOR3(cls), D3DXVECTOR3(distance, 0.0f, 0.0f));
		return true;
	}
	else
	{
		return false;
	}
}

const void ColliderBoxAA2D::SolveCollisionBoxAA2D(ColliderBoxAA2D * const col, D3DXVECTOR3 * movement, bool bias)
{
	*movement = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	if (!m_isTempColVecUsable)
	{
		CheckCollisionBoxAA2D(col);
		if (!m_isTempColVecUsable)
		{
			return;
		}
	}

	D3DXVECTOR3 negAbs, posAbs;
	negAbs.x = abs(m_tempColVecMin.x);
	negAbs.y = abs(m_tempColVecMin.y);
	//negAbs.z = abs(m_tempColVecMin.z);
	posAbs.x = abs(m_tempColVecMax.x);
	posAbs.y = abs(m_tempColVecMax.y);
	//posAbs.z = abs(m_tempColVecMax.z);

	if (
		negAbs.x <= posAbs.x &&
		negAbs.x <= negAbs.y &&
		negAbs.x <= posAbs.y/* &&
		negAbs.x <= negAbs.z &&
		negAbs.x <= posAbs.z*/
		)
	{
		*movement = D3DXVECTOR3(-m_tempColVecMin.x, 0.0f, 0.0f);
	}
	else if (
		posAbs.x <= negAbs.x &&
		posAbs.x <= negAbs.y &&
		posAbs.x <= posAbs.y /*&&
		posAbs.x <= negAbs.z &&
		posAbs.x <= posAbs.z*/
		)
	{
		*movement = D3DXVECTOR3(-m_tempColVecMax.x, 0.0f, 0.0f);
	}
	else if (
		negAbs.y <= negAbs.x &&
		negAbs.y <= posAbs.x &&
		negAbs.y <= posAbs.y /*&&
		negAbs.y <= negAbs.z &&
		negAbs.y <= posAbs.z*/
		)
	{
		*movement = D3DXVECTOR3(0.0f, -m_tempColVecMin.y, 0.0f);
	}
	else
	{
		*movement = D3DXVECTOR3(0.0f, -m_tempColVecMax.y, 0.0f);
	}
	/*
	else if (
		posAbs.y <= negAbs.x &&
		posAbs.y <= negAbs.y &&
		posAbs.y <= negAbs.y &&
		posAbs.y <= negAbs.z &&
		posAbs.y <= posAbs.z
		)
	{
		*movement = D3DXVECTOR3(0.0f, -m_tempColVecMax.y, 0.0f);
	}
	else if (
		negAbs.z <= negAbs.x &&
		negAbs.z <= posAbs.x &&
		negAbs.z <= posAbs.y &&
		negAbs.z <= negAbs.y &&
		negAbs.z <= posAbs.z
		)
	{
		*movement = D3DXVECTOR3(0.0f, 0.0f, -m_tempColVecMin.z);
	}
	else
	{
		*movement = D3DXVECTOR3(0.0f, 0.0f, -m_tempColVecMax.z);
	}
	*/

	m_isTempColVecUsable = false;
}

const void ColliderBoxAA2D::SolveCollisionSphere2D(ColliderSphere2D * const col, D3DXVECTOR3 * movement, bool bias)
{
	D3DXVECTOR2 norm;
	D3DXVec2Normalize(&norm, &D3DXVECTOR2(m_tempColVecMin));
	*movement = D3DXVECTOR3(norm * (col->GetRadius() - sqrt(m_tempColVecMax.x)));
	movement->z = 0.0f;
}
