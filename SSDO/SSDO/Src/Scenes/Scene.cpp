#include "stdafx.h"
#include "Scene.h"
#include "Camera.h"
#include "Mesh.h"
#include "Material.h"

namespace Scenes
{
	Scene::Scene()
	{

	}


	Scene::~Scene()
	{
	}

	void Scene::Initialize()
	{
		SetupScene();
	}

	void Scene::Shutdown()
	{
		if (_mainCamera != nullptr)
		{
			delete _mainCamera;
		}

		for (auto it = _materials.begin(); it != _materials.end(); ++it)
		{
			delete *it;
		}

		for (auto it = _meshes.begin(); it != _meshes.end(); ++it)
		{
			delete *it;
		}

		_materials.clear();
		_meshes.clear();
	}

	void Scene::Update()
	{
		_mainCamera->Update();

		for (auto it = _materials.begin(); it != _materials.end(); ++it)
		{
			(*it)->Update();
		}

		for (auto it = _meshes.begin(); it != _meshes.end(); ++it)
		{
			(*it)->Update();
		}
	}

	void Scene::Draw() const
	{
		for (auto it = _meshes.begin(); it != _meshes.end(); ++it)
		{
			(*it)->Draw(*_mainCamera);
		}
	}

}