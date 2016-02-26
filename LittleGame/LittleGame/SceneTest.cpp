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

	/////////////////

	Camera* cam = new Camera();
	cam->Initialize(4, 800.0f, 600.0f, 0.45f, 1.0f, 100.0f, true);
	tPos = D3DXVECTOR3(0.0f, 0.0f, 10.0f);
	cam->SetPosition(&tPos);
	m_cameras.push_back(cam);
}
