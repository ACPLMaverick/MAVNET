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
	Component::Shutdown();

	PhysicsManager::GetInstance()->RemoveCollider(this);
}

void Collider::Update()
{
	// do nth
}

void Collider::Draw()
{
	// do nth
}
