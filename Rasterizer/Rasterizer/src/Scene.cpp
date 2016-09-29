#include "stdafx.h"
#include "Scene.h"
#include "Primitive.h"
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
	for (std::vector<Primitive*>::iterator it = m_primitives.begin(); it != m_primitives.end(); ++it)
	{
		delete *it;
	}

	for (std::vector<Camera*>::iterator it = m_cameras.begin(); it != m_cameras.end(); ++it)
	{
		delete *it;
	}

	m_primitives.clear();
	m_cameras.clear();
}

void Scene::Update()
{
	if (m_flagToAddPrimitive)
	{
		m_flagToAddPrimitive = false;
		for (std::vector<Primitive*>::iterator it = m_primitivesToAdd.begin(); it != m_primitivesToAdd.end(); ++it)
		{
			m_primitives.push_back(*it);
		}
		m_primitivesToAdd.clear();
	}
	if (m_flagToRemovePrimitive)
	{
		m_flagToRemovePrimitive = false;
		for (std::vector<std::vector<Primitive*>::iterator>::iterator it = m_primitivesToRemove.begin(); it != m_primitivesToRemove.end(); ++it)
		{
			m_primitives.erase(*it);
		}
		m_primitivesToRemove.clear();
	}

	for (std::vector<Primitive*>::iterator it = m_primitives.begin(); it != m_primitives.end(); ++it)
	{
		(*it)->Update();
	}

	for (std::vector<Camera*>::iterator it = m_cameras.begin(); it != m_cameras.end(); ++it)
	{
		(*it)->Update();
	}
}

void Scene::Draw(Buffer<int32_t> * const buf, Buffer<float>* const depth)
{
	for (std::vector<Primitive*>::iterator it = m_primitives.begin(); it != m_primitives.end(); ++it)
	{
		(*it)->Draw(buf, depth);
	}
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

void Scene::AddPrimitive(Primitive * const Primitive)
{
	m_primitivesToAdd.push_back(Primitive);
	m_flagToAddPrimitive = true;
}

void Scene::AddCamera(Camera * const camera)
{
	m_cameras.push_back(camera);
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
