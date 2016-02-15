#include "PhysicsManager.h"
#include "BoxAACollider.h"
#include "SphereCollider.h"
#include "System.h"

PhysicsManager::PhysicsManager()
{
}


PhysicsManager::PhysicsManager(const PhysicsManager * c)
{
}

PhysicsManager::~PhysicsManager()
{
}



unsigned int PhysicsManager::Initialize()
{
	// HARD CODED VALUES
	m_gravity = glm::vec3(0.0f, -10.0f, 0.0f);
	m_ifDrawColliders = true;
	///////////////////////////

	return CS_ERR_NONE;
}

unsigned int PhysicsManager::Shutdown()
{
	for (std::vector<Collider*>::iterator it = m_colliders.begin(); it != m_colliders.end(); ++it)
	{
		(*it)->Shutdown();
	}
	m_colliders.clear();
	m_boxCollidersData.clear();
	m_sphereCollidersData.clear();

	return CS_ERR_NONE;
}

unsigned int PhysicsManager::Run()
{
	/*
	// clear checked collider data
	for (std::vector<Collider*>::iterator it = m_colliders.begin(); it != m_colliders.end(); ++it)
	{
		(*it)->m_collisionsSolvedWith.clear();
	}

	// solve collisions (every collider with every collider, using m_collisionsSolvedWith
	for (std::vector<Collider*>::iterator it = m_colliders.begin(); it != m_colliders.end(); ++it)
	{
		for (std::vector<Collider*>::iterator it2 = m_colliders.begin(); it2 != m_colliders.end(); ++it2)
		{
			// check conditions under we do not have to check for collisions.
			if (
				*it == *it2 || 
				(*it)->HasAlreadyCollidedWith(*it2)
				)
			{
				continue;
			}

			// we can check for collision now.
			CollisonTestResult res;

			if ((*it2)->m_type == BOX_AA)
			{
				res = (*it)->TestWithBoxAA((BoxAACollider*)(*it2));
			}
			else if ((*it2)->m_type == SPHERE)
			{
				res = (*it)->TestWithSphere((SphereCollider*)(*it2));
			}

			// transform collided objects accordingly, if necessary
			if (res.ifCollision)
			{
				// only it object has moved
				if ((*it)->GetMySimObject()->GetTransform()->HasMovedLastFrame() && !(*it2)->GetMySimObject()->GetTransform()->HasMovedLastFrameFast())
				{
					glm::vec3 nPos = ((*(*it)->GetMySimObject()->GetTransform()->GetPosition()) + res.colVector);
					(*it)->GetMySimObject()->GetTransform()->SetPosition(&nPos);
				}
				// only it2 object has moved
				else if (!(*it)->GetMySimObject()->GetTransform()->HasMovedLastFrame() && (*it2)->GetMySimObject()->GetTransform()->HasMovedLastFrameFast())
				{
					glm::vec3 nPos = ((*(*it2)->GetMySimObject()->GetTransform()->GetPosition()) - res.colVector);
					(*it2)->GetMySimObject()->GetTransform()->SetPosition(&nPos);
				}
				// both objects have moved or none had
				else
				{
					res.colVector = res.colVector / 2.0f;
					glm::vec3 second = -res.colVector;

					glm::vec3 nPos = ((*(*it)->GetMySimObject()->GetTransform()->GetPosition()) + res.colVector);
					glm::vec3 nPos2 = ((*(*it2)->GetMySimObject()->GetTransform()->GetPosition()) + second);

					(*it)->GetMySimObject()->GetTransform()->SetPosition(&nPos);
					(*it2)->GetMySimObject()->GetTransform()->SetPosition(&nPos2);
				}
			}

			// update m_collisionsSolvedWith
			(*it)->m_collisionsSolvedWith.push_back(*it2);
			(*it2)->m_collisionsSolvedWith.push_back(*it);
		}
	}
	*/
	return CS_ERR_NONE;
}

void PhysicsManager::CollisionCheck(Collider * col, CollisonTestResult* result)
{
	// solve collisions (this collider with every collider)
	for (std::vector<Collider*>::iterator it = m_colliders.begin(); it != m_colliders.end(); ++it)
	{
		// check conditions under we do not have to check for collisions.
		if (
			*it == col ||
			!(*it)->m_obj->GetEnabled()
			)
		{
			continue;
		}

		CollisonTestResult res;
		// we can check for collision now.
		if (col->m_type == BOX_AA)
		{
			res = (*it)->TestWithBoxAA((BoxAACollider*)col);
		}
		else if (col->m_type == SPHERE)
		{
			res = (*it)->TestWithSphere((SphereCollider*)col);
		}

		if(res.ifCollision) result->ifCollision = true;
		result->colVector += res.colVector;

		// ground level
		float gLevel = System::GetInstance()->GetCurrentScene()->GetGroundLevel();
		if (col->m_type == BOX_AA)
		{
			BoxAACollider* bc = (BoxAACollider*)col;
			float glAddition = glm::max(gLevel - bc->m_minEffective.y, 0.0f);
			if (glAddition != 0.0f)
			{
				result->ifCollision = true;
				result->colVector.y += glAddition;
			}
				
		}
		else if (col->m_type == SPHERE)
		{
			SphereCollider* s = (SphereCollider*)col;
			float glAddition = glm::max(gLevel - (s->m_effectiveCenter.y - s->m_effectiveRadius), 0.0f);
			if (glAddition != 0.0f)
			{
				result->ifCollision = true;
				result->colVector.y += glAddition;
			}
		}
	}
}


/*
void PhysicsManager::AddCollider(Collider* col)
{
	m_colliders.push_back(col);
}
*/

BoxAACollider* PhysicsManager::CreateBoxAACollider(SimObject* obj, glm::vec3* min, glm::vec3* max)
{
	m_boxCollidersData.push_back(BoxAAData(min, max));
	BoxAACollider* colPtr = new BoxAACollider(obj, min, max, m_boxCollidersData.size() - 1);
	m_colliders.push_back(colPtr);
	colPtr->Initialize();
	return colPtr;
}

SphereCollider* PhysicsManager::CreateSphereCollider(SimObject* obj, glm::vec3* offset, float radius)
{
	m_sphereCollidersData.push_back(SphereData(offset, radius));
	SphereCollider* colPtr = new SphereCollider(obj, offset, radius, m_sphereCollidersData.size() - 1);
	m_colliders.push_back(colPtr);
	colPtr->Initialize();
	return colPtr;
}

bool PhysicsManager::RemoveCollider(Collider* col)
{
	if (m_colliders.size() > 0)
	{
		for (std::vector<Collider*>::iterator it = m_colliders.begin(); it != m_colliders.end(); ++it)
		{
			if (*it == col)
			{
				m_colliders.erase(it);
				return true;
			}
		}
	}
	

	return false;
}



void PhysicsManager::SetGravity(float newGravValue)
{
	m_gravity = glm::vec3(0.0f, -newGravValue, 0.0f);
}

glm::vec3* PhysicsManager::GetGravity()
{
	return &m_gravity;
}

bool PhysicsManager::GetIfDrawColliders()
{
	return m_ifDrawColliders;
}

std::vector<Collider*>* PhysicsManager::GetColliders()
{
	return &m_colliders;
}

std::vector<BoxAAData>* PhysicsManager::GetBoxCollidersData()
{
	return &m_boxCollidersData;
}

std::vector<SphereData>* PhysicsManager::GetSphereCollidersData()
{
	return &m_sphereCollidersData;
}

int PhysicsManager::GetCollidersCount()
{
	return m_colliders.size();
}