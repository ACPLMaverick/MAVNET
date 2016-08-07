#include "stdafx.h"
#include "Collider.h"

Collider::Collider()
{
}


Collider::~Collider()
{
}

void Collider::Initialize(uint32_t uid, GameObject * obj, std::string * name)
{
	Component::Initialize(uid, obj, name);

	PhysicsManager::GetInstance()->AddCollider(this);
}

void Collider::Shutdown()
{
	PhysicsManager::GetInstance()->RemoveCollider(this);
	Component::Shutdown();
}

void Collider::Update()
{
	if (m_isTempColVecUsable)
	{
		m_isTempColVecUsable = false;
	}
}

void Collider::LateUpdate()
{
}

void Collider::Draw()
{
	// do nth
}

void Collider::AssignTempColVec(D3DXVECTOR3 vecMin, D3DXVECTOR3 vecMax)
{
	m_tempColVecMin = vecMin;
	m_tempColVecMax = vecMax;
	m_isTempColVecUsable = true;
}
