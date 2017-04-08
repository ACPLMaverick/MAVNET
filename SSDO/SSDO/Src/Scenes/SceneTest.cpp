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
		_mainCamera = new Camera(XMFLOAT3(0.0f, 4.0f, -5.0f));

		Shader* shdColor = LoadShader(std::wstring(L"ColorShader"));

		_materials.push_back(new Material(*shdColor));

		_meshes.push_back(new Mesh(L"box", *_materials[0], XMFLOAT3(0.7f, 0.0f, 0.0f)));
	}
}
