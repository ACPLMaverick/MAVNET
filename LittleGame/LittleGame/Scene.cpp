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
