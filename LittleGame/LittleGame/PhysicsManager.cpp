#include "stdafx.h"
#include "PhysicsManager.h"
#include "Collider.h"
#include "ColliderSphere2D.h"
#include "ColliderBoxAA2D.h"
#include "GameObject.h"
#include "Transform.h"


PhysicsManager::PhysicsManager()
{
}


PhysicsManager::~PhysicsManager()
{
}

void PhysicsManager::Initialize()
{
	// currently nth.
}

void PhysicsManager::Shutdown()
{
	m_collidersAll.clear();
	m_collidersToCheck.clear();
	m_collidersToSolve.clear();
}

void PhysicsManager::Run()
{
	for (std::vector<Collider*>::iterator it = m_collidersToCheck.begin(); it != m_collidersToCheck.end(); ++it)
	{
		CheckCollider((*it));
	}

	m_collidersToCheck.clear();

	for (std::vector<ColliderMovement>::iterator it = m_collidersToSolve.begin(); it != m_collidersToSolve.end(); ++it)
	{
		SolveCollider(*it);
	}

	m_collidersToSolve.clear();
}

bool PhysicsManager::RemoveColliderFromCheck(Collider * col)
{
	for (std::vector<Collider*>::iterator it = m_collidersToCheck.begin(); it != m_collidersToCheck.end(); ++it)
	{
		if ((*it) == col)
		{
			m_collidersToCheck.erase(it);
			return true;
		}
	}

	return false;
}

bool PhysicsManager::RemoveCollider(Collider * col)
{
	for (std::vector<Collider*>::iterator it = m_collidersAll.begin(); it != m_collidersAll.end(); ++it)
	{
		if ((*it) == col)
		{
			m_collidersAll.erase(it);
			return true;
		}
	}

	return false;
}

void PhysicsManager::CheckCollider(Collider * col)
{
	const std::vector<Collider*>* colColliders = col->GetMyGameObject()->GetColliderCollection();

	for (std::vector<Collider*>::iterator it = m_collidersAll.begin(); it != m_collidersAll.end(); ++it)
	{
		if ((*it) == col)
		{
			continue;
		}

		for (std::vector<Collider*>::const_iterator it2 = colColliders->begin(); it2 != colColliders->end(); ++it2)
		{
			if ((*it2) == (*it))
			{
				continue;
			}
		}

		Collider::ColliderType type = (*it)->GetType();

		if (type == Collider::ColliderType::BOXAA_2D)
		{
			if (col->CheckCollisionBoxAA2D((ColliderBoxAA2D*)(*it)))
			{
				D3DXVECTOR3 colVector;
				col->SolveCollisionBoxAA2D((ColliderBoxAA2D*)(*it), &colVector, false);
				ColliderMovement movement = ColliderMovement(col, colVector);
				m_collidersToSolve.push_back(movement);
			}
		}
		else if (type == Collider::ColliderType::SPHERE_2D)
		{
			if (col->CheckCollisionSphere2D((ColliderSphere2D*)(*it)))
			{
				D3DXVECTOR3 colVector;
				col->SolveCollisionSphere2D((ColliderSphere2D*)(*it), &colVector, false);
				ColliderMovement movement = ColliderMovement(col, colVector);
				m_collidersToSolve.push_back(movement);
			}
		}
	}
}

inline void PhysicsManager::SolveCollider(ColliderMovement mv)
{
	Transform* trans = mv.collider->GetMyGameObject()->GetTransform();
	D3DXVECTOR3 pos = *(trans->GetPosition());
	D3DXVECTOR3 nPos = pos + mv.movement;
	trans->SetPosition(&nPos, false);
}

const Collider * PhysicsManager::GetCollider(uint32_t id)
{
	for (std::vector<Collider*>::iterator it = m_collidersAll.begin(); it != m_collidersAll.end(); ++it)
	{
		if ((*it)->GetUID() == id)
		{
			return *it;
		}
	}

	return nullptr;
}

const Collider * PhysicsManager::GetCollider(const std::string * const name)
{
	for (std::vector<Collider*>::iterator it = m_collidersAll.begin(); it != m_collidersAll.end(); ++it)
	{
		if (*(*it)->GetName() == *name)
		{
			return *it;
		}
	}

	return nullptr;
}
