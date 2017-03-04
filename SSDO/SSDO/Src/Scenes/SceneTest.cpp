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

		_meshes.push_back(new Mesh(*_materials[0], XMFLOAT3(-1.0f, 0.0f, -1.0f), XMFLOAT3(0.1f, 0.2f, 0.3f)));
	}
}
