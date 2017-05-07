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
#include "Postprocesses/SimpleSSAO.h"

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
		_mainCamera = new Camera(XMFLOAT3A(0.0f, 4.0f, -8.0f));

		Shader* shdColor = LoadShader(std::wstring(L"ColorShader"));

		Material* matTest = new Material(*shdColor, XMFLOAT4A(1.0f, 1.0f, 1.0f, 1.0f));
		_materials.emplace(L"TestMat", matTest);

		Mesh* box = new Mesh(L"box");
		_meshes.emplace(L"Box", box);

		Object* objFloor = new Object(*box, *matTest);
		objFloor->SetScale(XMFLOAT3A(15.0f, 0.01f, 15.0f));
		objFloor->SetPosition(XMFLOAT3A(0.0f, 0.0f, 0.0f));
		_objects.push_back(objFloor);

		Object* objFloor02 = new Object(*box, *matTest);
		objFloor02->SetScale(XMFLOAT3A(15.0f, 0.01f, 15.0f));
		objFloor02->SetPosition(XMFLOAT3A(0.0f, 0.0f, 15.0f));
		objFloor02->SetRotation(XMFLOAT3A(90.0f, 0.0f, 0.0f));
		_objects.push_back(objFloor02);

		Object* objBox01 = new Object(*box, *matTest);
		objBox01->SetPosition(XMFLOAT3A(0.0f, 2.0f, 4.0f));
		objBox01->SetScale(XMFLOAT3A(2.0f, 2.0f, 2.0f));
		_objects.push_back(objBox01);

		Object* objBox02 = new Object(*box, *matTest);
		objBox02->SetPosition(XMFLOAT3A(-2.0f, 1.0f, -2.0f));
		objBox02->SetRotation(XMFLOAT3A(0.0f, -35.0f, 0.0f));
		_objects.push_back(objBox02);

		Object* objBox03 = new Object(*box, *matTest);
		objBox03->SetPosition(XMFLOAT3A(2.0f, 1.0f, -2.0f));
		objBox03->SetRotation(XMFLOAT3A(0.0f, 35.0f, 0.0f));
		_objects.push_back(objBox03);

		_lightAmbient = new LightAmbient(XMFLOAT4A(0.05f, 0.05f, 0.1f, 1.0f));

		_lightsDirectional.push_back(new LightDirectional(XMFLOAT4A(0.7f, 0.5f, 0.5f, 1.0f), XMFLOAT3A(-1.0f, -0.3f, 1.0f)));

		_lightsPoint.push_back(new LightPoint(XMFLOAT4A(0.6f, 1.0f, 0.9f, 1.0f), XMFLOAT3A(-3.0f, 4.0f, -8.0f), 10.0f));

		//_postprocesses.push_back(new Sepia());
		_postprocesses.push_back(new SimpleSSAO());
	}
}
