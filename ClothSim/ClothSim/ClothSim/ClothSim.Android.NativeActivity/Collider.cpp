#include "Collider.h"

Collider::Collider(SimObject* obj, unsigned int cDataID) : Component(obj)
{
	m_cDataID = cDataID;
}

Collider::Collider(const Collider* c) : Component(c)
{

}


Collider::~Collider()
{
}



unsigned int Collider::Initialize()
{

	return CS_ERR_NONE;
}

unsigned int Collider::Shutdown()
{
	m_collisionsSolvedWith.clear();

	return CS_ERR_NONE;
}



ColliderType Collider::GetType()
{
	return m_type;
}



bool Collider::HasAlreadyCollidedWith(Collider* col)
{
	for (std::vector<Collider*>::iterator it = m_collisionsSolvedWith.begin(); it != m_collisionsSolvedWith.end(); ++it)
	{
		if ((*it) == col)
		{
			return true;
		}	
	}

	return false;
}