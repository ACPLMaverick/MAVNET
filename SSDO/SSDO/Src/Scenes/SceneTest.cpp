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
		_mainCamera = new Camera(XMFLOAT3(4.0f, 4.0f, -4.0f));
		
		_materials.push_back(new Material(L"TestShader"));

		_meshes.push_back(new Mesh(L"box", *_materials[0], XMFLOAT3(0.0f, 0.0f, 0.0f)));
	}
}
