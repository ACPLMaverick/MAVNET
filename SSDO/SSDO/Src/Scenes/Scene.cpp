#include "stdafx.h"
#include "Scene.h"
#include "Camera.h"
#include "Mesh.h"
#include "Material.h"
#include "../Lights/LightAmbient.h"
#include "../Lights/LightDirectional.h"
#include "../Lights/LightPoint.h"
using namespace Lights;

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

		for (auto it = _lightsDirectional.begin(); it != _lightsDirectional.end(); ++it)
		{
			delete *it;
		}

		for (auto it = _lightsPoint.begin(); it != _lightsPoint.end(); ++it)
		{
			delete *it;
		}

		_materials.clear();
		_meshes.clear();
		_lightsPoint.clear();
		_lightsDirectional.clear();

		if (_lightAmbient != nullptr) delete _lightAmbient;

		for (auto it = _shaders.begin(); it != _shaders.end(); ++it)
		{
			delete (*it).second;
		}
		_shaders.clear();
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
		_mainCamera->Draw(*this);
	}

}