#include "SceneTest.h"
#include "MeshGLRect.h"
#include "MeshGLBox.h"
#include "MeshGLPlane.h"
#include "MeshGLSphere.h"
#include "GUIText.h"
#include "BoxAACollider.h"
#include "SphereCollider.h"
#include "ClothSimulator.h"

SceneTest::SceneTest(string n) : Scene(n)
{
}

SceneTest::SceneTest(const SceneTest* c) : Scene(c)
{
}


SceneTest::~SceneTest()
{
}



unsigned int SceneTest::Initialize()
{
	////////////////////////
	/////////// Objects

	SimObject* ground = new SimObject();
	ground->Initialize("Ground");
	Transform* groundTransform = new Transform(ground);
	groundTransform->Initialize();
	groundTransform->SetPosition(&(glm::vec3(0.0f, -10.0f, 0.0f)));
	groundTransform->SetRotation(&(glm::vec3(-3.14f / 2.0f, 0.0f, 0.0f)));
	groundTransform->SetScale(&(glm::vec3(100.0f, 100.0f, 100.0f)));
	ground->SetTransform(groundTransform);
	MeshGLRect* triangle = new MeshGLRect(ground, &(glm::vec4(0.6f, 0.6f, 0.6f, 1.0f)));
	triangle->Initialize();
	//triangle->SetGloss(100.0f);
	triangle->SetTextureID(ResourceManager::GetInstance()->GetTextureWhite());
	ground->AddMesh(triangle);

	AddObject(ground);

	SimObject* testObj = new SimObject();
	testObj->Initialize("testObj");
	Transform* testObjTransform = new Transform(testObj);
	testObjTransform->Initialize();
	testObjTransform->SetPosition(&(glm::vec3(0.0f, 2.5f, 0.0f)));
	testObjTransform->SetScale(&(glm::vec3(1.0f, 1.0f, 1.0f)));
	testObj->SetTransform(testObjTransform);
	/*
	MeshGLBox* box = new MeshGLBox(testObj, 4.0f, 3.0f, 3.0f, &(glm::vec4(0.2f, 0.2f, 0.8f, 1.0f)));
	box->Initialize();
	box->SetGloss(100.0f);
	box->SetTextureID(ResourceManager::GetInstance()->GetTextureWhite());
	testObj->AddMesh(box);
	*/
	
	MeshGLSphere* sph = new MeshGLSphere(testObj, 2.0f, 32, 32, &(glm::vec4(0.4f, 0.7f, 0.9f, 1.0f)));
	sph->Initialize();
	sph->SetGloss(20.0f);
	sph->SetTextureID(ResourceManager::GetInstance()->GetTextureWhite());
	testObj->AddMesh(sph);

	/*
	BoxAACollider* tObjCollider = PhysicsManager::GetInstance()->CreateBoxAACollider(testObj, &(glm::vec3(-2.0f, -1.5f, -1.5f)), &(glm::vec3(2.0f, 1.5f, 1.5f)));
	testObj->AddCollider(tObjCollider);
	*/

	SphereCollider* tObjCollider = PhysicsManager::GetInstance()->CreateSphereCollider(testObj, &(glm::vec3(0.0f, 0.0f, 0.0f)), 2.0f);
	testObj->AddCollider(tObjCollider);

	/*RotateMe* rm = new RotateMe(testObj);
	rm->Initialize();
	rm->SetRotation(&(glm::vec3(0.0f, 0.00003f, 0.0f)));
	testObj->AddComponent(rm);*/

	AddObject(testObj);

	m_currentObjectID = 1;


	SimObject* colObj = new SimObject();
	colObj->Initialize("colObj");
	Transform* colObjTransform = new Transform(colObj);
	colObjTransform->Initialize();
	colObjTransform->SetPosition(&(glm::vec3(5.0f, 2.5f, 0.0f)));
	colObjTransform->SetScale(&(glm::vec3(1.0f, 1.0f, 1.0f)));
	colObj->SetTransform(colObjTransform);
	MeshGLBox* colBox = new MeshGLBox(colObj, 1.0f, 1.0f, 1.0f, &(glm::vec4(0.8f, 0.2f, 0.2f, 1.0f)));
	colBox->Initialize();
	colBox->SetGloss(60.0f);
	colBox->SetTextureID(ResourceManager::GetInstance()->GetTextureWhite());
	colObj->AddMesh(colBox);
	BoxAACollider* cObjCollider = PhysicsManager::GetInstance()->CreateBoxAACollider(colObj, &(glm::vec3(-0.5f, -0.5f, -0.5f)), &(glm::vec3(0.5f, 0.5f, 0.5f)));
	colObj->AddCollider(cObjCollider);

	AddObject(colObj);


	SimObject* testCloth = new SimObject();
	testCloth->Initialize("testCloth");
	Transform* testClothTransform = new Transform(testCloth);
	testClothTransform->Initialize();
	testClothTransform->SetPosition(&(glm::vec3(0.0f, 7.5f, 0.0f)));
	testClothTransform->SetScale(&(glm::vec3(1.0f, 1.0f, 1.0f)));
	testClothTransform->Update();
	testCloth->SetTransform(testClothTransform);

	MeshGLPlane* clothMesh = new MeshGLPlane(testCloth, 10.0f, 10.0f, 20, 20);
	clothMesh->Initialize();
	clothMesh->SetTextureID(ResourceManager::GetInstance()->GetTextureWhite());
	testCloth->AddMesh(clothMesh);

	ClothSimulator* cSim = new ClothSimulator(testCloth, 1);
	testCloth->AddComponent(cSim);
	cSim->Initialize();

	AddObject(testCloth);

	////////////////////////
	/////////// Camera

	Camera* testCam = new Camera(nullptr, 0.6f, 0.01f, 1000.0f);
	testCam->Initialize();
	testCam->SetPosition(&glm::vec3(-8.0f, 15.0f, 15.0f));

	AddCamera(testCam);


	m_currentCameraID = 0;
	

	////////////////////////
	/////////// Lights

	SetAmbientLight(new LightAmbient(&(glm::vec3(0.1f, 0.05f, 0.1f))));
	LightDirectional* dir1 = new LightDirectional(&(glm::vec3(1.0f, 0.9f, 0.6f)), &(glm::vec3(1.0f, 0.9f, 0.9f)), &(glm::vec3(-0.8f, -0.8f, -1.0f)));
	AddDirectionalLight(dir1);

	////////////////////////
	/////////// GUI

	string t1n = "FPStitle";
	string t1v = "FPS: ";
	string t2n = "DeltaTimetitle";
	string t2v = "Delta time [ms]: ";
	string t3n = "TotalTimetitle";
	string t3v = "Total time [s]: ";
	string tval01 = "FPSvalue";
	string tval02 = "DTvalue";
	string tval03 = "TTvalue";
	string dummy = "Dummy";
	string tex = "..\\files\\ExportedFont.bmp";

	GUIText* gt = new GUIText(&t1n, &t1v, ResourceManager::GetInstance()->GetTexture(&tex));
	gt->Initialize();
	gt->SetPosition(glm::vec2(-0.9f, 0.8f));
	gt->SetScale(glm::vec2(0.02f, 0.02f));

	GUIText* gt2 = new GUIText(&t2n, &t2v, ResourceManager::GetInstance()->GetTexture(&tex));
	gt2->Initialize();
	gt2->SetPosition(glm::vec2(-0.9f, 0.7f));
	gt2->SetScale(glm::vec2(0.02f, 0.02f));

	GUIText* gt3 = new GUIText(&t3n, &t3v, ResourceManager::GetInstance()->GetTexture(&tex));
	gt3->Initialize();
	gt3->SetPosition(glm::vec2(-0.9f, 0.6f));
	gt3->SetScale(glm::vec2(0.02f, 0.02f));

	GUIText* gt4 = new GUIText(&tval01, &dummy, ResourceManager::GetInstance()->GetTexture(&tex));
	gt4->Initialize();
	gt4->SetPosition(glm::vec2(-0.79f, 0.8f));
	gt4->SetScale(glm::vec2(0.02f, 0.02f));

	GUIText* gt5 = new GUIText(&tval02, &dummy, ResourceManager::GetInstance()->GetTexture(&tex));
	gt5->Initialize();
	gt5->SetPosition(glm::vec2(-0.55f, 0.7f));
	gt5->SetScale(glm::vec2(0.02f, 0.02f));

	GUIText* gt6 = new GUIText(&tval03, &dummy, ResourceManager::GetInstance()->GetTexture(&tex));
	gt6->Initialize();
	gt6->SetPosition(glm::vec2(-0.55f, 0.6f));
	gt6->SetScale(glm::vec2(0.02f, 0.02f));

	AddGUIElement(gt);
	AddGUIElement(gt2);
	AddGUIElement(gt3);
	AddGUIElement(gt4);
	AddGUIElement(gt5);
	AddGUIElement(gt6);

	////////////////////////
	/////////// Controllers

	SimObject* controller = new SimObject();
	controller->Initialize("SimController");

	GUIController* sc = new GUIController(controller);
	sc->Initialize();
	controller->AddComponent(sc);

	AddObject(controller);

#ifdef _DEBUG
	printf("\SceneTest.Initialize has completed successfully.\n");
#endif

	return CS_ERR_NONE;
}