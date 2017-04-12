#include "stdafx.h"
#include "SceneTest.h"
#include "Camera.h"
#include "Mesh.h"
#include "Material.h"
#include "Object.h"
#include "Lights/LightAmbient.h"
#include "Lights/LightDirectional.h"
#include "Lights/LightPoint.h"
#include "Postprocesses/Sepia.h"
using namespace Lights;
using namespace Postprocesses;

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

		Material* matTest = new Material(*shdColor, XMFLOAT4(0.7f, 1.0f, 0.7f, 1.0f));
		_materials.emplace(L"TestMat", matTest);

		Mesh* meshBox = new Mesh(L"box");
		_meshes.emplace(L"Box", meshBox);

		_objects.push_back(new Object(*meshBox, *matTest));

		_lightAmbient = new LightAmbient(XMFLOAT4(0.05f, 0.05f, 0.1f, 1.0f));

		_lightsDirectional.push_back(new LightDirectional(XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f), XMFLOAT3(-1.0f, -0.3f, 1.0f)));

		_lightsPoint.push_back(new LightPoint(XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), XMFLOAT3(-2.0f, 2.0f, -2.0f), 25.0f));

		_postprocesses.push_back(new Sepia());
	}
}
