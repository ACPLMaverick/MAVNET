#include "stdafx.h"
#include "SceneTest.h"
#include "GameObject.h"
#include "Transform.h"
#include "Camera.h"
#include "MeshSprite.h"
#include "Material.h"
#include "Texture.h"
#include "Effect.h"
#include "ResourceManager.h"
#include "TestInputController.h"

#include <d3dx9math.h>

SceneTest::SceneTest()
{
}


SceneTest::~SceneTest()
{
}

void SceneTest::InitializeScene()
{
	/////

	std::string test = "Test";

	/////
	
	GameObject* testObject = new GameObject();
	testObject->Initialize(1, &test);

	Transform* testTransfrom = new Transform();
	D3DXVECTOR3 tPos = D3DXVECTOR3(0.1f, 0.0f, 0.0f);
	D3DXVECTOR3 tRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 tScl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	testTransfrom->Initialize(2, testObject, &tPos, &tRot, &tScl);
	testObject->AddTransform(testTransfrom);

	MeshSprite* testMesh = new MeshSprite();
	testMesh->Initialize(3, testObject);
	testObject->AddMesh(testMesh);

	std::string neMaterial = "LE_EMPTY_WHITE";
	Material* mat = ResourceManager::GetInstance()->GetMaterial(&neMaterial);
	testMesh->SetMaterial(mat);

	TestInputController* tic = new TestInputController();
	tic->Initialize(4, testObject);
	testObject->AddComponent(tic);

	m_gameObjects.push_back(testObject);

	/////////////////

	Camera* cam = new Camera();
	cam->Initialize(4, 800.0f, 600.0f, 0.45f, 1.0f, 100.0f, true);
	tPos = D3DXVECTOR3(0.0f, 0.0f, 10.0f);
	cam->SetPosition(&tPos);
	m_cameras.push_back(cam);
}
