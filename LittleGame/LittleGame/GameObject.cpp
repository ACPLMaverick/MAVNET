#include "stdafx.h"
#include "GameObject.h"
#include "Component.h"
#include "Transform.h"
#include "Mesh.h"
#include "Collider.h"

GameObject::GameObject()
{
}


GameObject::~GameObject()
{
}

void GameObject::Initialize(uint32_t uid, const std::string * name)
{
	m_uID = uid;
	if(name != nullptr)
		m_name = *name;
}

void GameObject::Shutdown()
{
	for (std::vector<GameObject*>::iterator it = m_children.begin(); it != m_children.end(); ++it)
	{
		(*it)->Shutdown();
		delete (*it);
	}

	for (std::vector<Component*>::iterator it = m_components.begin(); it != m_components.end(); ++it)
	{
		(*it)->Shutdown();
		delete (*it);
	}
}

void GameObject::Update()
{
	if (m_flagChildrenToAdd)
	{
		m_flagChildrenToAdd = false;
		for (std::vector<GameObject*>::iterator it = m_childrenToAdd.begin(); it != m_childrenToAdd.end(); ++it)
		{
			m_children.push_back(*it);
		}
		m_childrenToAdd.clear();
	}
	if (m_flagChildrenToRemove)
	{
		m_flagChildrenToRemove = false;
		for (std::vector<std::vector<GameObject*>::iterator>::iterator it = m_childrenToRemove.begin(); it != m_childrenToRemove.end(); ++it)
		{
			m_children.erase(*it);
		}
		m_childrenToRemove.clear();
	}
	if (m_flagComponentsToAdd)
	{
		m_flagComponentsToAdd = false;
		for (std::vector<Component*>::iterator it = m_componentsToAdd.begin(); it != m_componentsToAdd.end(); ++it)
		{
			m_components.push_back(*it);
		}
		m_componentsToAdd.clear();
	}
	if (m_flagComponentsToRemove)
	{
		m_flagComponentsToRemove = false;
		for (std::vector<std::vector<Component*>::iterator>::iterator it = m_componentsToRemove.begin(); it != m_componentsToRemove.end(); ++it)
		{
			m_components.erase(*it);
		}
		m_componentsToRemove.clear();
	}

	for (std::vector<GameObject*>::iterator it = m_children.begin(); it != m_children.end(); ++it)
	{
		(*it)->Update();
	}

	for (std::vector<Component*>::iterator it = m_components.begin(); it != m_components.end(); ++it)
	{
		(*it)->Update();
	}

	for (std::vector<Mesh*>::iterator it = m_meshes.begin(); it != m_meshes.end(); ++it)
	{
		(*it)->Update();
	}
}

void GameObject::LateUpdate()
{
	for (std::vector<GameObject*>::iterator it = m_children.begin(); it != m_children.end(); ++it)
	{
		(*it)->LateUpdate();
	}

	for (std::vector<Component*>::iterator it = m_components.begin(); it != m_components.end(); ++it)
	{
		(*it)->LateUpdate();
	}

	for (std::vector<Mesh*>::iterator it = m_meshes.begin(); it != m_meshes.end(); ++it)
	{
		(*it)->LateUpdate();
	}
}

void GameObject::Draw()
{
	for (std::vector<GameObject*>::iterator it = m_children.begin(); it != m_children.end(); ++it)
	{
		(*it)->Draw();
	}

	for (std::vector<Component*>::iterator it = m_components.begin(); it != m_components.end(); ++it)
	{
		(*it)->Draw();
	}

	for (std::vector<Mesh*>::iterator it = m_meshes.begin(); it != m_meshes.end(); ++it)
	{
		(*it)->Draw();
	}
}

Component * const GameObject::GetComponent(uint32_t id)
{
	for (std::vector<Component*>::iterator it = m_components.begin(); it != m_components.end(); ++it)
	{
		if ((*it)->GetUID() == id)
		{
			return (*it);
		}
	}

	return nullptr;
}

Component * const GameObject::GetComponent(const std::string * name)
{
	for (std::vector<Component*>::iterator it = m_components.begin(); it != m_components.end(); ++it)
	{
		if (*(*it)->GetName() == *name)
		{
			return (*it);
		}
	}

	return nullptr;
}

GameObject * const GameObject::GetChild(uint32_t id)
{
	for (std::vector<GameObject*>::iterator it = m_children.begin(); it != m_children.end(); ++it)
	{
		if ((*it)->GetUID() == id)
		{
			return (*it);
		}
	}

	return nullptr;
}

GameObject * const GameObject::GetChild(const std::string * name)
{
	for (std::vector<GameObject*>::iterator it = m_children.begin(); it != m_children.end(); ++it)
	{
		if (*(*it)->GetName() == *name)
		{
			return (*it);
		}
	}

	return nullptr;
}

GameObject* const GameObject::GetParent()
{
	return m_parent;
}

Mesh * const GameObject::GetMesh(uint32_t id)
{
	for (std::vector<Mesh*>::iterator it = m_meshes.begin(); it != m_meshes.end(); ++it)
	{
		if ((*it)->GetUID() == id)
		{
			return (*it);
		}
	}

	return nullptr;
}

Mesh * const GameObject::GetMesh(const std::string * name)
{
	for (std::vector<Mesh*>::iterator it = m_meshes.begin(); it != m_meshes.end(); ++it)
	{
		if (*(*it)->GetName() == *name)
		{
			return (*it);
		}
	}

	return nullptr;
}

const std::vector<Mesh*>* GameObject::GetMeshCollection()
{
	return &m_meshes;
}

Transform * const GameObject::GetTransform()
{
	return m_transform;
}

const std::vector<Collider*>* GameObject::GetColliderCollection()
{
	return &m_colliders;
}

const std::vector<GameObject*>* GameObject::GetChildrenCollection()
{
	return &m_children;
}

void GameObject::AddTransform(Transform * const transform)
{
	m_transform = transform;
	AddComponent((Component*)transform);
}

void GameObject::AddComponent(Component * const component)
{
	m_componentsToAdd.push_back(component);
	m_flagComponentsToAdd = true;
}

void GameObject::AddCollider(Collider * const component)
{
	m_colliders.push_back(component);
	AddComponent((Component*)component);
}

void GameObject::AddChild(GameObject * const child)
{
	m_childrenToAdd.push_back(child);
	m_flagChildrenToAdd = true;
}

void GameObject::AddMesh(Mesh * const mesh)
{
	m_meshes.push_back(mesh);
}

void GameObject::SetParent(GameObject * const parent)
{
	m_parent = parent;
}

Component * const GameObject::RemoveComponent(uint32_t id)
{
	for (std::vector<Component*>::iterator it = m_components.begin(); it != m_components.end(); ++it)
	{
		if ((*it)->GetUID() == id)
		{
			m_componentsToRemove.push_back(it);
			m_flagComponentsToRemove = true;
			return *it;
		}
	}

	return nullptr;
}

Component * const GameObject::RemoveComponent(const std::string * name)
{
	for (std::vector<Component*>::iterator it = m_components.begin(); it != m_components.end(); ++it)
	{
		if (*(*it)->GetName() == *name)
		{
			m_componentsToRemove.push_back(it);
			m_flagComponentsToRemove = true;
			return *it;
		}
	}

	return nullptr;
}

Component * const GameObject::RemoveComponent(const Component * ptr)
{
	for (std::vector<Component*>::iterator it = m_components.begin(); it != m_components.end(); ++it)
	{
		if ((*it) == ptr)
		{
			m_componentsToRemove.push_back(it);
			m_flagComponentsToRemove = true;
			return *it;
		}
	}

	return nullptr;
}

GameObject * const GameObject::RemoveChild(uint32_t id)
{
	for (std::vector<GameObject*>::iterator it = m_children.begin(); it != m_children.end(); ++it)
	{
		if ((*it)->GetUID() == id)
		{
			m_childrenToRemove.push_back(it);
			m_flagChildrenToRemove = true;
			return *it;
		}
	}

	return nullptr;
}

GameObject * const GameObject::RemoveChild(const std::string * name)
{
	for (std::vector<GameObject*>::iterator it = m_children.begin(); it != m_children.end(); ++it)
	{
		if (*(*it)->GetName() == *name)
		{
			m_childrenToRemove.push_back(it);
			m_flagChildrenToRemove = true;
			return *it;
		}
	}

	return nullptr;
}

GameObject * const GameObject::RemoveChild(const GameObject * ptr)
{
	for (std::vector<GameObject*>::iterator it = m_children.begin(); it != m_children.end(); ++it)
	{
		if ((*it) == ptr)
		{
			m_childrenToRemove.push_back(it);
			m_flagChildrenToRemove = true;
			return *it;
		}
	}

	return nullptr;
}

Mesh * const GameObject::RemoveMesh(uint32_t id)
{
	for (std::vector<Mesh*>::iterator it = m_meshes.begin(); it != m_meshes.end(); ++it)
	{
		if ((*it)->GetUID() == id)
		{
			Mesh* tmp = (*it);
			m_meshes.erase(it);
			return tmp;
		}
	}

	return nullptr;
}

Mesh * const GameObject::RemoveMesh(const std::string * name)
{
	for (std::vector<Mesh*>::iterator it = m_meshes.begin(); it != m_meshes.end(); ++it)
	{
		if (*(*it)->GetName() == *name)
		{
			Mesh* tmp = (*it);
			m_meshes.erase(it);
			return tmp;
		}
	}

	return nullptr;
}

Mesh * const GameObject::RemoveMesh(const Mesh * ptr)
{
	for (std::vector<Mesh*>::iterator it = m_meshes.begin(); it != m_meshes.end(); ++it)
	{
		if ((*it) == ptr)
		{
			Mesh* tmp = (*it);
			m_meshes.erase(it);
			return tmp;
		}
	}

	return nullptr;
}

Collider * const GameObject::RemoveCollider(uint32_t id)
{
	for (std::vector<Collider*>::iterator it = m_colliders.begin(); it != m_colliders.end(); ++it)
	{
		if ((*it)->GetUID() == id)
		{
			Collider* tmp = (*it);
			m_colliders.erase(it);
			return tmp;
		}
	}

	return nullptr;
}

Collider * const GameObject::RemoveCollider(const std::string * name)
{
	for (std::vector<Collider*>::iterator it = m_colliders.begin(); it != m_colliders.end(); ++it)
	{
		if (*(*it)->GetName() == *name)
		{
			Collider* tmp = (*it);
			m_colliders.erase(it);
			return tmp;
		}
	}

	return nullptr;
}

Collider * const GameObject::RemoveCollider(const Collider * ptr)
{
	for (std::vector<Collider*>::iterator it = m_colliders.begin(); it != m_colliders.end(); ++it)
	{
		if ((*it) == ptr)
		{
			Collider* tmp = (*it);
			m_colliders.erase(it);
			return tmp;
		}
	}

	return nullptr;
}
