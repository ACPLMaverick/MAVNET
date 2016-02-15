#include "SimObject.h"


SimObject::SimObject()
{
	m_name = "";
	m_id = -1;
	m_visible = true;
	m_enabled = true;
	m_transform = nullptr;
}

SimObject::SimObject(const SimObject*)
{
}


SimObject::~SimObject()
{
}

unsigned int SimObject::Initialize(string name)
{
	m_name = name;
	hash<string> h = hash<string>();
	m_id = h(m_name);
	m_collidersDirty = false;

	return CS_ERR_NONE;
}

unsigned int SimObject::Shutdown()
{
	unsigned int err;

	for (vector<Component*>::iterator it = m_components.begin(); it != m_components.end(); ++it)
	{
		err = (*it)->Shutdown();
		if (err != CS_ERR_NONE) return err;
		delete (*it);
	}
	m_components.clear();

	for (vector<Mesh*>::iterator it = m_meshes.begin(); it != m_meshes.end(); ++it)
	{
		err = (*it)->Shutdown();
		if (err != CS_ERR_NONE) return err;
		delete (*it);
	}
	m_components.clear();

	for (vector<Collider*>::iterator it = m_colliders.begin(); it != m_colliders.end(); ++it)
	{
		err = (*it)->Shutdown();
		if (err != CS_ERR_NONE) return err;
	}
	m_colliders.clear();

	if (m_transform != nullptr)
		err = m_transform->Shutdown();

	if (err != CS_ERR_NONE) return err;
	delete m_transform;

	return CS_ERR_NONE;
}

unsigned int SimObject::Update()
{
	unsigned int err = CS_ERR_NONE; 

	if (m_enabled)
	{
		for (vector<Component*>::iterator it = m_components.begin(); it != m_components.end(); ++it)
		{
			err = (*it)->Update();
			if (err != CS_ERR_NONE) return err;
		}

		for (vector<Mesh*>::iterator it = m_meshes.begin(); it != m_meshes.end(); ++it)
		{
			err = (*it)->Update();
			if (err != CS_ERR_NONE) return err;
		}

		if (m_transform != nullptr)
			err = m_transform->Update();

		if (m_collidersDirty)
		{
			for (vector<Collider*>::iterator it = m_colliders.begin(); it != m_colliders.end(); ++it)
			{
				err = (*it)->Update();
				if (err != CS_ERR_NONE) return err;
			}
			m_collidersDirty = false;
		}
	}

	if (err != CS_ERR_NONE) return err;

	return CS_ERR_NONE;
}

unsigned int SimObject::Draw()
{
	unsigned int err = CS_ERR_NONE;

	if (m_visible)
	{
		for (vector<Component*>::iterator it = m_components.begin(); it != m_components.end(); ++it)
		{
			err = (*it)->Draw();
			if (err != CS_ERR_NONE) return err;
		}

		for (vector<Mesh*>::iterator it = m_meshes.begin(); it != m_meshes.end(); ++it)
		{
			err = (*it)->Draw();
			if (err != CS_ERR_NONE) return err;
		}

		if (PhysicsManager::GetInstance()->GetIfDrawColliders())
		{
			for (vector<Collider*>::iterator it = m_colliders.begin(); it != m_colliders.end(); ++it)
			{
				err = (*it)->Draw();
				if (err != CS_ERR_NONE) return err;
			}
		}
	}

	return CS_ERR_NONE;
}



void SimObject::UpdateColliders()
{
	for (vector<Collider*>::iterator it = m_colliders.begin(); it != m_colliders.end(); ++it)
	{
		(*it)->Update();
	}
}

void SimObject::UpdateCollidersFast()
{
	m_collidersDirty = true;
}



void SimObject::AddComponent(Component* ptr)
{
	m_components.push_back(ptr);
}

void SimObject::AddMesh(Mesh* ptr)
{
	m_meshes.push_back(ptr);
}

void SimObject::AddCollider(Collider* ptr)
{
	m_colliders.push_back(ptr);
}

void SimObject::SetTransform(Transform* ptr)
{
	m_transform = ptr;
}

void SimObject::SetVisible(bool vis)
{
	m_visible = vis;
}

void SimObject::SetEnabled(bool t)
{
	m_enabled = t;
}



void SimObject::RemoveComponent(Component* ptr)
{
	for (vector<Component*>::iterator it = m_components.begin(); it != m_components.end(); ++it)
	{
		if (ptr == (*it))
		{
			m_components.erase(it);
			break;
		}
	}
}

void SimObject::RemoveMesh(Mesh* ptr)
{
	for (vector<Mesh*>::iterator it = m_meshes.begin(); it != m_meshes.end(); ++it)
	{
		if (ptr == (*it))
		{
			m_meshes.erase(it);
			break;
		}
	}
}

void SimObject::RemoveCollider(Collider* ptr)
{
	for (vector<Collider*>::iterator it = m_colliders.begin(); it != m_colliders.end(); ++it)
	{
		if (ptr == (*it))
		{
			m_colliders.erase(it);
			break;
		}
	}
}

void SimObject::RemoveComponent(int which)
{
	int ctr = 0;
	for (vector<Component*>::iterator it = m_components.begin(); it != m_components.end(); ++it, ++ctr)
	{
		if (ctr == which)
		{
			m_components.erase(it);
			break;
		}
	}
}

void SimObject::RemoveMesh(int which)
{
	int ctr = 0;
	for (vector<Mesh*>::iterator it = m_meshes.begin(); it != m_meshes.end(); ++it)
	{
		if (ctr == which)
		{
			m_meshes.erase(it);
			break;
		}
	}
}

void SimObject::RemoveTransform()
{
	m_transform->Shutdown();
	delete m_transform;
}

Collider* SimObject::GetCollider(unsigned int w)
{
	if (w < m_colliders.size())
	{
		return m_colliders.at(w);
	}
	else return nullptr;
}

std::vector<Collider*>* SimObject::GetColliders()
{
	return &m_colliders;
}



Component* SimObject::GetComponent(unsigned int w)
{
	if (m_components.size() < w)
		return nullptr;
	return m_components.at(w);
}

Mesh* SimObject::GetMesh(unsigned int w)
{
	if (m_meshes.size() < w)
		return nullptr;
	return m_meshes.at(w);
}

Transform* SimObject::GetTransform()
{
	return m_transform;
}

bool SimObject::GetVisible()
{
	return m_visible;
}

bool SimObject::GetEnabled()
{
	return m_enabled;
}

string* SimObject::GetName()
{
	return &m_name;
}

unsigned int SimObject::GetId()
{
	return m_id;
}
