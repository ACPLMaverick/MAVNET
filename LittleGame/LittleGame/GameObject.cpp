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

const Component * GameObject::GetComponent(uint32_t id)
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

const Component * GameObject::GetComponent(const std::string * name)
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

const GameObject * GameObject::GetChild(uint32_t id)
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

const GameObject * GameObject::GetChild(const std::string * name)
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

const GameObject * GameObject::GetParent()
{
	return m_parent;
}

const Transform * GameObject::GetTransform()
{
	return m_transform;
}

void GameObject::AddComponent(Component * const component)
{
	if (typeid(component) == typeid(Transform))
	{
		m_transform = (Transform*)component;
	}

	m_components.push_back(component);
}

void GameObject::AddChild(GameObject * const child)
{
	m_children.push_back(child);
}

void GameObject::SetParent(GameObject * const parent)
{
	m_parent = parent;
}

const Component * GameObject::RemoveComponent(uint32_t id)
{
	for (std::vector<Component*>::iterator it = m_components.begin(); it != m_components.end(); ++it)
	{
		if ((*it)->GetUID() == id)
		{
			Component* tmp = (*it);
			m_components.erase(it);
			return tmp;
		}
	}

	return nullptr;
}

const Component * GameObject::RemoveComponent(const std::string * name)
{
	for (std::vector<Component*>::iterator it = m_components.begin(); it != m_components.end(); ++it)
	{
		if (*(*it)->GetName() == *name)
		{
			Component* tmp = (*it);
			m_components.erase(it);
			return tmp;
		}
	}

	return nullptr;
}

const Component * GameObject::RemoveComponent(const Component * ptr)
{
	for (std::vector<Component*>::iterator it = m_components.begin(); it != m_components.end(); ++it)
	{
		if ((*it) == ptr)
		{
			Component* tmp = (*it);
			m_components.erase(it);
			return tmp;
		}
	}

	return nullptr;
}

const GameObject * GameObject::RemoveChild(uint32_t id)
{
	for (std::vector<GameObject*>::iterator it = m_children.begin(); it != m_children.end(); ++it)
	{
		if ((*it)->GetUID() == id)
		{
			GameObject* tmp = (*it);
			m_children.erase(it);
			return tmp;
		}
	}

	return nullptr;
}

const GameObject * GameObject::RemoveChild(const std::string * name)
{
	for (std::vector<GameObject*>::iterator it = m_children.begin(); it != m_children.end(); ++it)
	{
		if (*(*it)->GetName() == *name)
		{
			GameObject* tmp = (*it);
			m_children.erase(it);
			return tmp;
		}
	}

	return nullptr;
}

const GameObject * GameObject::RemoveChild(const GameObject * ptr)
{
	for (std::vector<GameObject*>::iterator it = m_children.begin(); it != m_children.end(); ++it)
	{
		if ((*it) == ptr)
		{
			GameObject* tmp = (*it);
			m_children.erase(it);
			return tmp;
		}
	}

	return nullptr;
}
