#include "stdafx.h"
#include "SceneTest.h"
#include "Camera.h"
#include "Mesh.h"
#include "Material.h"

namespace Scenes
{
	SceneTest::SceneTest()
	{
	}


	SceneTest::~SceneTest()
	{
	}

	void SceneTest::SetupScene()
	{
		_mainCamera = new Camera();
		
		_materials.push_back(new Material(L"TestShader"));

		_meshes.push_back(new Mesh(*_materials[0]));
	}
}
