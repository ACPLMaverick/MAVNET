#include "stdafx.h"
#include "SceneTest.h"
#include "Camera.h"
#include "Mesh.h"
#include "Material.h"
#include "Object.h"
#include "Controller.h"
#include "Text.h"
#include "Lights/LightAmbient.h"
#include "Lights/LightDirectional.h"
#include "Lights/LightPoint.h"
#include "Postprocesses/Sepia.h"
#include "Postprocesses/SimpleSSAO.h"
#include "Postprocesses/SSDOBase.h"
#include "Postprocesses/SSDOImproved.h"
#include "Postprocesses/SSDOImprovedB.h"

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
		_mainCamera = new Camera(XMFLOAT3A(8.0f, 4.0f, -8.0f));

		Shader* shdColor = LoadShader(std::wstring(L"ColorShader"));

		Material* matWhite = new Material(*shdColor, XMFLOAT4A(1.0f, 1.0f, 1.0f, 1.0f));
		_materials.emplace(L"MatWhite", matWhite);

		Material* matYellow = new Material(*shdColor, XMFLOAT4A(1.0f, 1.0f, 0.0f, 1.0f));
		_materials.emplace(L"MatYellow", matYellow);

		Material* matBlue = new Material(*shdColor, XMFLOAT4A(0.0f, 0.0f, 1.0f, 1.0f));
		_materials.emplace(L"MatBlue", matBlue);

		Mesh* box = new Mesh(L"box", true);
		_meshes.emplace(L"Box", box);

		Object* objFloor = new Object(*box, *matWhite);
		objFloor->SetScale(XMFLOAT3A(15.0f, 0.01f, 15.0f));
		objFloor->SetPosition(XMFLOAT3A(0.0f, 0.0f, 0.0f));
		_objects.push_back(objFloor);

		Object* objFloor02 = new Object(*box, *matWhite);
		objFloor02->SetScale(XMFLOAT3A(15.0f, 0.01f, 15.0f));
		objFloor02->SetPosition(XMFLOAT3A(0.0f, 0.0f, 15.0f));
		objFloor02->SetRotation(XMFLOAT3A(90.0f, 0.0f, 0.0f));
		_objects.push_back(objFloor02);

		Object* objBox01 = new Object(*box, *matWhite);
		objBox01->SetPosition(XMFLOAT3A(0.0f, 2.0f, 4.0f));
		objBox01->SetScale(XMFLOAT3A(2.0f, 2.0f, 2.0f));
		_objects.push_back(objBox01);

		Object* objBox02 = new Object(*box, *matYellow);
		objBox02->SetPosition(XMFLOAT3A(-2.0f, 1.0f, -2.0f));
		objBox02->SetRotation(XMFLOAT3A(0.0f, -35.0f, 0.0f));
		_objects.push_back(objBox02);

		Object* objBox03 = new Object(*box, *matBlue);
		objBox03->SetPosition(XMFLOAT3A(2.0f, 1.0f, -2.0f));
		objBox03->SetRotation(XMFLOAT3A(0.0f, 35.0f, 0.0f));
		_objects.push_back(objBox03);

		_lightAmbient = new LightAmbient(XMFLOAT4A(0.3f, 0.3f, 0.3f, 1.0f));

		const float dirBoost = 1.0f;
		_lightsDirectional.push_back(new LightDirectional(XMFLOAT4A(1.0f * dirBoost, 0.75f * dirBoost, 0.6f * dirBoost, 1.0f), XMFLOAT3A(1.0f, -0.3f, 1.0f)));

		//_lightsPoint.push_back(new LightPoint(XMFLOAT4A(0.6f, 1.0f, 0.9f, 1.0f), XMFLOAT3A(0.0f, 4.0f, -5.0f), 10.0f));

		_postprocesses.push_back(new SSDOImprovedB());
		_postprocesses.push_back(new SSDOImproved());
		_postprocesses.push_back(new SSDOBase());
		_postprocesses.push_back(new SimpleSSAO());

		Text* helpTxt = new Text("Press V to cycle through post processes. Use WSAD to move camera. Use mouse with RMB pressed to rotate camera.");
		helpTxt->SetScale(0.2f);
		helpTxt->SetPosition(XMFLOAT2A(0.0f, -0.9f));
		_texts.push_back(helpTxt);
	}
}
