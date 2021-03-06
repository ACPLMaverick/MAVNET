#include "stdafx.h"
#include "TTSceneGame.h"
#include "GameObject.h"
#include "Transform.h"
#include "Camera.h"
#include "MeshSprite.h"
#include "Material.h"
#include "Texture.h"
#include "Effect.h"
#include "ResourceManager.h"
#include "IdentificationManager.h"
#include "System.h"
#include "ColliderSphere2D.h"
#include "ColliderBoxAA2D.h"

#include "TTGameController.h"

TTSceneGame::TTSceneGame()
{
}


TTSceneGame::~TTSceneGame()
{
}

void TTSceneGame::InitializeScene()
{
	/////

	std::string test = "Test";

	GameObject* testObject = new GameObject();
	testObject->Initialize(IdentificationManager::GetInstance()->GetUniqueID(), &test);

	Transform* testTransfrom = new Transform();
	D3DXVECTOR3 tPos = D3DXVECTOR3(0.1f, 0.0f, 0.0f);
	D3DXVECTOR3 tRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 tScl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	testTransfrom->Initialize(IdentificationManager::GetInstance()->GetUniqueID(), testObject, &tPos, &tRot, &tScl);
	testObject->AddTransform(testTransfrom);

	MeshSprite* testMesh = new MeshSprite();
	testMesh->Initialize(IdentificationManager::GetInstance()->GetUniqueID(), testObject);
	testObject->AddMesh(testMesh);

	std::string tName = "brick";
	Texture* tn = ResourceManager::GetInstance()->GetTexture(&tName, ResourceManager::TextureExtension::DDS);

	std::string neMaterial = "TestMaterial";
	D3DXCOLOR color = D3DXCOLOR(0.7f, 0.7f, 0.9f, 1.0f);
	Material* mat = new Material();
	mat->SetTextureDiffuse(tn);
	mat->SetColorDiffuse(&color);
	mat->Initialize(0, &neMaterial, &neMaterial);

	testMesh->SetMaterial(mat);

	ColliderBoxAA2D* colliderOne = new ColliderBoxAA2D();
	D3DXVECTOR2 min = D3DXVECTOR2(-0.55f, -0.55f);
	D3DXVECTOR2 max = D3DXVECTOR2(0.55f, 0.55f);
	colliderOne->Initialize(&min, &max, IdentificationManager::GetInstance()->GetUniqueID(), testObject);
	testObject->AddCollider(colliderOne);

	m_gameObjects.push_back(testObject);

	////

	test = "Test2";
	tPos = D3DXVECTOR3(2.0f, 1.0f, 0.0f);

	GameObject* testObject2 = new GameObject();
	testObject2->Initialize(IdentificationManager::GetInstance()->GetUniqueID(), &test);

	Transform* testTransfrom2 = new Transform();
	testTransfrom2->Initialize(IdentificationManager::GetInstance()->GetUniqueID(), testObject2, &tPos, &tRot, &tScl);
	testObject2->AddTransform(testTransfrom2);

	MeshSprite* testMesh2 = new MeshSprite();
	testMesh2->Initialize(IdentificationManager::GetInstance()->GetUniqueID(), testObject2);
	testObject2->AddMesh(testMesh2);
	testMesh2->SetMaterial(mat);

	ColliderBoxAA2D* colliderTwo = new ColliderBoxAA2D();
	colliderTwo->Initialize(&min, &max, IdentificationManager::GetInstance()->GetUniqueID(), testObject2);
	testObject2->AddCollider(colliderTwo);

	m_gameObjects.push_back(testObject2);

	/////////////////

	Camera* cam = new Camera();
	cam->Initialize(IdentificationManager::GetInstance()->GetUniqueID(), 
		System::GetInstance()->GetSystemSettings()->GetWindowWidth(), 
		System::GetInstance()->GetSystemSettings()->GetWindowHeight(), 
		3.0f, 1.0f, 100.0f, true);
	tPos = D3DXVECTOR3(0.0f, 0.0f, -10.0f);
	cam->SetPosition(&tPos);
	m_cameras.push_back(cam);

	////////////////

	GameObject* controller = new GameObject();
	std::string contr = "GameController";
	controller->Initialize(IdentificationManager::GetInstance()->GetUniqueID(), &contr);

	Transform* cTransform = new Transform();
	tPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	tRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	tScl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	cTransform->Initialize(IdentificationManager::GetInstance()->GetUniqueID(), controller, &tPos, &tRot, &tScl);
	controller->AddTransform(cTransform);

	TTGameController* gameContrComp = new TTGameController();
	gameContrComp->Controllable = testObject;
	gameContrComp->Controllable2 = testObject2;
	gameContrComp->Initialize(IdentificationManager::GetInstance()->GetUniqueID(), controller);
	controller->AddComponent(gameContrComp);

	m_gameObjects.push_back(controller);

	/////
}

