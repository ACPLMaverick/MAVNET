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
	if (m_collidersAll.size() > 0)
	{
		for (std::vector<Collider*>::iterator it = m_collidersAll.begin(); it != m_collidersAll.end(); ++it)
		{
			(*it)->Shutdown();
			delete (*it);
		}
	}

	if (m_collidersToCheck.size() > 0)
	{
		for (std::vector<Collider*>::iterator it = m_collidersToCheck.begin(); it != m_collidersToCheck.end(); ++it)
		{
			(*it)->Shutdown();
			delete (*it);
		}
	}

	m_collidersAll.clear();
	m_collidersToCheck.clear();
	m_collidersToSolve.clear();
}

void PhysicsManager::Run()
{
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
