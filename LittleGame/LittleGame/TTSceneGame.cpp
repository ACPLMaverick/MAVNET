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

	std::string eName = "SpriteEffect";
	std::string tName = "brick";
	Effect* te = ResourceManager::GetInstance()->GetEffect(&eName);
	Texture* tn = ResourceManager::GetInstance()->GetTexture(&tName, ResourceManager::TextureExtension::DDS);

	std::string neMaterial = "TestMaterial";
	Material* mat = new Material();
	mat->SetTextureDiffuse(tn);
	mat->SetEffect(te);
	mat->Initialize(0, &neMaterial, &neMaterial);

	testMesh->SetMaterial(mat);

	m_gameObjects.push_back(testObject);

	////

	test = "Test2";
	tPos = D3DXVECTOR3(1.5f, -0.5f, 0.0f);

	GameObject* testObject2 = new GameObject();
	testObject2->Initialize(IdentificationManager::GetInstance()->GetUniqueID(), &test);

	Transform* testTransfrom2 = new Transform();
	testTransfrom2->Initialize(IdentificationManager::GetInstance()->GetUniqueID(), testObject2, &tPos, &tRot, &tScl);
	testObject2->AddTransform(testTransfrom2);

	MeshSprite* testMesh2 = new MeshSprite();
	testMesh2->Initialize(IdentificationManager::GetInstance()->GetUniqueID(), testObject2);
	testObject2->AddMesh(testMesh2);
	testMesh2->SetMaterial(mat);

	m_gameObjects.push_back(testObject2);

	/////////////////

	Camera* cam = new Camera();
	cam->Initialize(IdentificationManager::GetInstance()->GetUniqueID(), 800.0f, 600.0f, 3.0f, 1.0f, 100.0f, true);
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

