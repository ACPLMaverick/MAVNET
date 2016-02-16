#include "stdafx.h"
#include "GameObject.h"
#include "Component.h"
#include "Transform.h"

GameObject::GameObject()
{
}


GameObject::~GameObject()
{
}

void GameObject::Initialize(uint32_t uid, const std::string * name)
{
	m_uID = uid;
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

Transform * const GameObject::GetTransform()
{
	return m_transform;
}

void GameObject::AddComponent(Component * const component)
{
	if (typeid(component) == typeid(Transform))
	{
		m_transform = (Transform*)component;
	}

	m_componentsToAdd.push_back(component);
	m_flagComponentsToAdd = true;
}

void GameObject::AddChild(GameObject * const child)
{
	m_childrenToAdd.push_back(child);
	m_flagChildrenToAdd = true;
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
