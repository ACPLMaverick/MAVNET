#include "stdafx.h"
#include "Scene.h"
#include "GameObject.h"
#include "Camera.h"

Scene::Scene()
{
}


Scene::~Scene()
{
}

void Scene::Initialize(uint32_t uID, std::string * name)
{
	m_name = *name;
	m_uID = uID;

	InitializeScene();
}

void Scene::Shutdown()
{
	for (std::vector<GameObject*>::iterator it = m_gameObjects.begin(); it != m_gameObjects.end(); ++it)
	{
		(*it)->Shutdown();
		delete *it;
	}

	for (std::vector<Camera*>::iterator it = m_cameras.begin(); it != m_cameras.end(); ++it)
	{
		(*it)->Shutdown();
		delete *it;
	}
}

void Scene::Update()
{
	if (m_flagToAddGameObject)
	{
		m_flagToAddGameObject = false;
		for (std::vector<GameObject*>::iterator it = m_gameObjectsToAdd.begin(); it != m_gameObjectsToAdd.end(); ++it)
		{
			m_gameObjects.push_back(*it);
		}
		m_gameObjectsToAdd.clear();
	}
	if (m_flagToRemoveGameObject)
	{
		m_flagToRemoveGameObject = false;
		for (std::vector<std::vector<GameObject*>::iterator>::iterator it = m_gameObjectsToRemove.begin(); it != m_gameObjectsToRemove.end(); ++it)
		{
			m_gameObjects.erase(*it);
		}
		m_gameObjectsToRemove.clear();
	}

	for (std::vector<GameObject*>::iterator it = m_gameObjects.begin(); it != m_gameObjects.end(); ++it)
	{
		(*it)->Update();
	}

	for (std::vector<Camera*>::iterator it = m_cameras.begin(); it != m_cameras.end(); ++it)
	{
		(*it)->Update();
	}
}

void Scene::Draw()
{
	for (std::vector<GameObject*>::iterator it = m_gameObjects.begin(); it != m_gameObjects.end(); ++it)
	{
		(*it)->Draw();
	}
}

GameObject * const Scene::GetGameObject(uint32_t uid)
{
	for (std::vector<GameObject*>::iterator it = m_gameObjects.begin(); it != m_gameObjects.end(); ++it)
	{
		if ((*it)->GetUID() == uid)
		{
			return *it;
		}
	}

	return nullptr;
}

GameObject * const Scene::GetGameObject(std::string * name)
{
	for (std::vector<GameObject*>::iterator it = m_gameObjects.begin(); it != m_gameObjects.end(); ++it)
	{
		if (*(*it)->GetName() == *name)
		{
			return *it;
		}
	}

	return nullptr;
}

Camera * const Scene::GetCamera(uint32_t uid)
{
	for (std::vector<Camera*>::iterator it = m_cameras.begin(); it != m_cameras.end(); ++it)
	{
		if ((*it)->GetUID() == uid)
		{
			return *it;
		}
	}

	return nullptr;
}

Camera * const Scene::GetCamera(std::string * name)
{
	for (std::vector<Camera*>::iterator it = m_cameras.begin(); it != m_cameras.end(); ++it)
	{
		if (*(*it)->GetName() == *name)
		{
			return *it;
		}
	}

	return nullptr;
}

void Scene::AddGameObject(GameObject * const gameObject)
{
	m_gameObjectsToAdd.push_back(gameObject);
	m_flagToAddGameObject = true;
}

void Scene::AddCamera(Camera * const camera)
{
	m_cameras.push_back(camera);
}

GameObject * const Scene::RemoveGameObject(uint32_t uid)
{
	for (std::vector<GameObject*>::iterator it = m_gameObjects.begin(); it != m_gameObjects.end(); ++it)
	{
		if ((*it)->GetUID() == uid)
		{
			m_gameObjectsToRemove.push_back(it);
			m_flagToRemoveGameObject = true;
			return *it;
		}
	}

	return nullptr;
}

GameObject * const Scene::RemoveGameObject(std::string * name)
{
	for (std::vector<GameObject*>::iterator it = m_gameObjects.begin(); it != m_gameObjects.end(); ++it)
	{
		if (*(*it)->GetName() == *name)
		{
			m_gameObjectsToRemove.push_back(it);
			m_flagToRemoveGameObject = true;
			return *it;
		}
	}

	return nullptr;
}

Camera * const Scene::RemoveCamera(uint32_t uid)
{
	for (std::vector<Camera*>::iterator it = m_cameras.begin(); it != m_cameras.end(); ++it)
	{
		if ((*it)->GetUID() == uid)
		{
			Camera* tmp = *it;
			m_cameras.erase(it);
			return *it;
		}
	}

	return nullptr;
}

Camera * const Scene::RemoveCamera(std::string * name)
{
	for (std::vector<Camera*>::iterator it = m_cameras.begin(); it != m_cameras.end(); ++it)
	{
		if (*(*it)->GetName() == *name)
		{
			Camera* tmp = *it;
			m_cameras.erase(it);
			return *it;
		}
	}

	return nullptr;
}
