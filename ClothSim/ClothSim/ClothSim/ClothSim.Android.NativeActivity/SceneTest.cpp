#include "SceneTest.h"
#include "MeshGLRect.h"
#include "MeshGLBox.h"
#include "MeshGLPlane.h"
#include "MeshGLSphere.h"
#include "GUIElement.h"
#include "GUIText.h"
#include "BoxAACollider.h"
#include "SphereCollider.h"
#include "GUIButton.h"
#include "GUIAction.h"
#include "GUISettingsScreen.h"

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
	//////////// Ground Level

	m_groundLevel = 0.0f;

	////////////////////////
	/////////// Objects

	glm::vec3 tPos, tRot, tScl, tPosMin, tPosMax;
	glm::vec4 tCol;


	SimObject* ground = new SimObject();
	ground->Initialize("Ground");

	tPos = (glm::vec3(0.0f, m_groundLevel - 0.1f, 0.0f));
	tRot = (glm::vec3(-3.14f / 2.0f, 0.0f, 0.0f));
	tScl = (glm::vec3(100.0f, 100.0f, 100.0f));
	tCol = (glm::vec4(0.8f, 0.8f, 0.9f, 1.0f));

	Transform* groundTransform = new Transform(ground);
	groundTransform->Initialize();
	groundTransform->SetPosition(&tPos);
	groundTransform->SetRotation(&tRot);
	groundTransform->SetScale(&tScl);
	ground->SetTransform(groundTransform);
	MeshGLRect* triangle = new MeshGLRect(ground, &tCol);
	triangle->Initialize();
	triangle->SetGloss(100.0f);
	triangle->SetSpecular(0.8f);
	triangle->SetTextureID(ResourceManager::GetInstance()->GetTextureWhite());
	ground->AddMesh(triangle);

	tPosMin = glm::vec3(-0.5f, -100000.0f, -0.5f);
	tPosMax = glm::vec3(0.5f, 0.001f, 0.5f);
	BoxAACollider* gCollider = PhysicsManager::GetInstance()->CreateBoxAACollider(ground, &tPosMin, &tPosMax);
	ground->AddCollider(gCollider);

	AddObject(ground);


	SimObject* testObj = new SimObject();
	testObj->Initialize("testObj");

	tPos = (glm::vec3(0.0f, 2.5f, 0.0f));
	tScl = (glm::vec3(1.0f, 1.0f, 1.0f));
	tCol = (glm::vec4(0.4f, 0.7f, 0.9f, 1.0f));

	Transform* testObjTransform = new Transform(testObj);
	testObjTransform->Initialize();
	testObjTransform->SetPosition(&tPos);
	testObjTransform->SetScale(&tScl);
	testObj->SetTransform(testObjTransform);
	
	MeshGLSphere* sph = new MeshGLSphere(testObj, 2.0f, 16, 16, &tCol);
	sph->Initialize();
	sph->SetGloss(20.0f);
	sph->SetSpecular(0.6f);
	sph->SetTextureID(ResourceManager::GetInstance()->GetTextureWhite());
	testObj->AddMesh(sph);


	tPosMin = (glm::vec3(0.0f, 0.0f, 0.0f));
	SphereCollider* tObjCollider = PhysicsManager::GetInstance()->CreateSphereCollider(testObj, &tPosMin, 2.0f);
	testObj->AddCollider(tObjCollider);

	

	AddObject(testObj);

	// and here goes box
	SimObject* testObjBox = new SimObject();
	testObjBox->Initialize("testObjBox");

	tPos = (glm::vec3(0.0f, 999.0f, 0.0f));
	tScl = (glm::vec3(1.0f, 1.0f, 1.0f));
	tCol = (glm::vec4(0.4f, 0.7f, 0.9f, 1.0f));

	Transform* testObjBoxTransform = new Transform(testObjBox);
	testObjBoxTransform->Initialize();
	testObjBoxTransform->SetPosition(&tPos);
	testObjBoxTransform->SetScale(&tScl);
	testObjBox->SetTransform(testObjBoxTransform);

	tCol = glm::vec4(0.2f, 0.2f, 0.8f, 1.0f);
	MeshGLBox* box = new MeshGLBox(testObjBox, 4.0f, 3.0f, 3.0f, &tCol);
	box->Initialize();
	box->SetGloss(100.0f);
	box->SetSpecular(0.5f);
	box->SetTextureID(ResourceManager::GetInstance()->GetTextureWhite());
	testObjBox->AddMesh(box);

	tPosMin = glm::vec3(-2.0f, -1.5f, -1.5f);
	tPosMax = glm::vec3(2.0f, 1.5f, 1.5f);
	BoxAACollider* tObjBoxCollider = PhysicsManager::GetInstance()->CreateBoxAACollider(testObjBox, &tPosMin, &tPosMax);
	testObjBox->AddCollider(tObjBoxCollider);

	AddObject(testObjBox);
	testObjBoxTransform->SetPosition(&tPos);
	testObjBox->SetVisible(false);
	testObjBox->SetEnabled(false);

	m_currentObjectID = 1;

	/*
	SimObject* colObj = new SimObject();
	colObj->Initialize("colObj");

	tPos = (glm::vec3(3.0f, 2.5f, -8.0f));
	tScl = (glm::vec3(1.0f, 1.0f, 1.0f));
	tCol = (glm::vec4(0.8f, 0.2f, 0.2f, 1.0f));
	tPosMin = (glm::vec3(-0.5f, -0.5f, -0.5f));
	tPosMax = (glm::vec3(0.5f, 0.5f, 0.5f));

	Transform* colObjTransform = new Transform(colObj);
	colObjTransform->Initialize();
	colObjTransform->SetPosition(&tPos);
	colObjTransform->SetScale(&tScl);
	colObj->SetTransform(colObjTransform);
	MeshGLBox* colBox = new MeshGLBox(colObj, 1.0f, 1.0f, 1.0f, &tCol);
	colBox->Initialize();
	colBox->SetGloss(60.0f);
	colBox->SetSpecular(1.0f);
	colBox->SetTextureID(ResourceManager::GetInstance()->GetTextureWhite());
	colObj->AddMesh(colBox);
	BoxAACollider* cObjCollider = PhysicsManager::GetInstance()->CreateBoxAACollider(colObj, &tPosMin, &tPosMax);
	colObj->AddCollider(cObjCollider);

	RotateMe* rm = new RotateMe(colObj);
	rm->Initialize();
	tRot = glm::vec3(0.0f, 0.003f, 0.0f);
	rm->SetRotation(&tRot);
	colObj->AddComponent(rm);

	AddObject(colObj);
	*/

	SimObject* testCloth = new SimObject();
	testCloth->Initialize("testCloth");

	tPos = (glm::vec3(0.0f, 10.0f, 0.0f));
	tScl = (glm::vec3(1.0f, 1.0f, 1.0f));
	tCol = glm::vec4(1.0f, 0.5f, 0.7f, 1.0f);

	Transform* testClothTransform = new Transform(testCloth);
	testClothTransform->Initialize();
	testClothTransform->SetPosition(&tPos);
	testClothTransform->SetScale(&tScl);
	testClothTransform->Update();
	testCloth->SetTransform(testClothTransform);

	ClothSimulator* cSim = new ClothSimulator(testCloth);
	testCloth->AddComponent(cSim);
	cSim->Initialize(); // !!!!!!!!!!!!!!

	AddObject(testCloth);

	////////////////////////
	/////////// Camera

	Camera* testCam = new Camera(nullptr, CSSET_WINDOW_FOV, CSSET_WINDOW_NEAR, CSSET_WINDOW_FAR);
	testCam->Initialize();
	
	tPos = glm::vec3(-8.0f, 15.0f, 15.0f) * 2.0f;
	testCam->SetPosition(&tPos);

	AddCamera(testCam);


	m_currentCameraID = 0;
	

	////////////////////////
	/////////// Lights

	glm::vec3 aCol = (glm::vec3(0.1f, 0.05f, 0.1f));
	glm::vec3 ldCol = (glm::vec3(1.0f, 0.9f, 0.6f));
	glm::vec3 ldSpec = (glm::vec3(1.0f, 0.9f, 0.9f));
	glm::vec3 ldDir = (glm::vec3(-0.8f, -0.8f, -1.0f));
	LightAmbient* la = new LightAmbient(&aCol);
	SetAmbientLight(la);
	LightDirectional* dir1 = new LightDirectional(&ldCol, &ldSpec, &ldDir);
	AddDirectionalLight(dir1);

	////////////////////////
	/////////// GUI

	string gr1 = "GroupText";
	string gr2 = "GroupBtns";
	string gr3 = "GroupBtnsSteer";
	string gr4 = "GroupSettings";

	string t1n = "FPStitle";
	string t1v = "FPS: ";
	string t2n = "DeltaTimetitle";
	string t2v = "Delta time [ms]: ";
	string t3n = "SimTimetitle";
	string t3v = "Simulation time [ms]: ";
	string t4n = "SimModeTitle";
	string t4v = "Simulation mode: ";
	string t5n = "DTitle";
	string t5v = "Vibrations: ";
	string tb1 = "BtnExit";
	string tb2 = "BtnPreferences";
	string tb3 = "BtnWireframe";
	string tb4 = "BtnArrowFw";
	string tb5 = "BtnArrowBw";
	string tb6 = "BtnArrowLeft";
	string tb7 = "BtnArrowRight";
	string tb8 = "BtnArrowUp";
	string tb9 = "BtnArrowDown";
	string tb10 = "BtnMovementModeArrows";
	string tb11 = "BtnMovementModeFinger";
	string tval01 = "FPSvalue";
	string tval02 = "DTvalue";
	string tval03 = "STvalue";
	string tval04 = "SKvalue";
	string tval05 = "Dvalue";
	string slid0 = "Sld";
	string sllab0 = "Test";
	string dummy = "Dummy";
	string tex = "textures/ExportedFont.tga";
	string tBtnEx = "textures/btn_exit.png";
	string tBtnExA = "textures/btn_exit_a.png";
	string tBtnWf = "textures/btn_wireframe.png";
	string tBtnWfA = "textures/btn_wireframe_a.png";
	string tBtnSt = "textures/btn_settings.png";
	string tBtnStA = "textures/btn_settings_a.png";
	string tBtnMva = "textures/btn_movButtons.png";
	string tBtnMvaA = "textures/btn_movButtons_a.png";
	string tBtnMvf = "textures/btn_movTouch.png";
	string tBtnMvfA = "textures/btn_movTouch_a.png";
	string tBtnArr = "textures/btn_arrow_up.png";
	string tBtnArrA = "textures/btn_arrow_up_a.png";
	string tBtnFArr = "textures/btn_arrow_forward.png";
	string tBtnFArrA = "textures/btn_arrow_forward_a.png";
	string tSlHead = "textures/slider_head.png";
	string tSlBar = "textures/slider_bar.png";
	string tBgSettings = "textures/bg_settings.png";
	glm::vec2 scl = glm::vec2(0.025f, 0.025f);

	GUIElement* geGroupText = new GUIElement(&gr1);
	geGroupText->Initialize();
	geGroupText->SetScaled(false);
	GUIElement* geGroupBtns = new GUIElement(&gr2);
	geGroupBtns->Initialize();
	geGroupBtns->SetScaled(false);
	GUIElement* geGroupBtnsMove = new GUIElement(&gr3);
	geGroupBtnsMove->Initialize();
	geGroupBtnsMove->SetScaled(false);
	GUISettingsScreen* geGroupSettings = new GUISettingsScreen(&gr4);
	geGroupSettings->Initialize();
	geGroupSettings->SetPosition(glm::vec2(0.0f, 0.0f));
	geGroupSettings->SetScale(glm::vec2(1.0f, 1.0f));
	geGroupSettings->SetScaled(false);
	geGroupSettings->SetBlockable(true);
	geGroupSettings->SetVisible(true);
	geGroupSettings->SetEnabled(true);

	geGroupBtns->SetVisible(false);
	geGroupBtns->SetEnabled(false);

	geGroupText->SetVisible(false);
	geGroupText->SetEnabled(false);

	GUIText* gt = new GUIText(&t1n, &t1v, ResourceManager::GetInstance()->GetTexture(&tex));
	gt->Initialize();
	gt->SetPosition(glm::vec2(0.2f, 0.85f));
	gt->SetScale(scl);
	//0.71
	GUIText* gt2 = new GUIText(&t2n, &t2v, ResourceManager::GetInstance()->GetTexture(&tex));
	gt2->Initialize();
	gt2->SetPosition(glm::vec2(-0.95f, 0.85f));
	gt2->SetScale(scl);

	GUIText* gt3 = new GUIText(&t3n, &t3v, ResourceManager::GetInstance()->GetTexture(&tex));
	gt3->Initialize();
	gt3->SetPosition(glm::vec2(-0.95f, 0.78f));
	gt3->SetScale(scl);

	GUIText* gt4 = new GUIText(&tval01, &dummy, ResourceManager::GetInstance()->GetTexture(&tex));
	gt4->Initialize();
	gt4->SetPosition(glm::vec2(0.38f, 0.85f));
	gt4->SetScale(scl);

	GUIText* gt5 = new GUIText(&tval02, &dummy, ResourceManager::GetInstance()->GetTexture(&tex));
	gt5->Initialize();
	gt5->SetPosition(glm::vec2(-0.16f, 0.85f));
	gt5->SetScale(scl);

	GUIText* gt6 = new GUIText(&tval03, &dummy, ResourceManager::GetInstance()->GetTexture(&tex));
	gt6->Initialize();
	gt6->SetPosition(glm::vec2(-0.02f, 0.78f));
	gt6->SetScale(scl);

	GUIText* gt7 = new GUIText(&t4n, &t4v, ResourceManager::GetInstance()->GetTexture(&tex));
	gt7->Initialize();
	gt7->SetPosition(glm::vec2(-0.95f, 0.71f));
	gt7->SetScale(scl);

	GUIText* gt8 = new GUIText(&tval04, &dummy, ResourceManager::GetInstance()->GetTexture(&tex));
	gt8->Initialize();
	gt8->SetPosition(glm::vec2(-0.22f, 0.71f));
	gt8->SetScale(scl);

	GUIText* gt9 = new GUIText(&t5n, &t5v, ResourceManager::GetInstance()->GetTexture(&tex));
	gt9->Initialize();
	gt9->SetPosition(glm::vec2(-0.95f, 0.64f));
	gt9->SetScale(scl);

	GUIText* gt10 = new GUIText(&tval05, &dummy, ResourceManager::GetInstance()->GetTexture(&tex));
	gt10->Initialize();
	gt10->SetPosition(glm::vec2(-0.22f, 0.64f));
	gt10->SetScale(scl);

	geGroupText->AddChild(gt);
	geGroupText->AddChild(gt2);
	geGroupText->AddChild(gt3);
	geGroupText->AddChild(gt4);
	geGroupText->AddChild(gt5);
	geGroupText->AddChild(gt6);
	geGroupText->AddChild(gt7);
	geGroupText->AddChild(gt8);
	geGroupText->AddChild(gt9);
	geGroupText->AddChild(gt10);

	GUIButton* gb1 = new GUIButton(&tb1);
	gb1->Initialize();
	gb1->SetTextures(ResourceManager::GetInstance()->LoadTexture(&tBtnEx), ResourceManager::GetInstance()->LoadTexture(&tBtnExA));
	gb1->SetPosition(glm::vec2(0.7f, 0.8f));
	gb1->SetScale(glm::vec2(0.15f, 0.15f));

	GUIButton* gb2 = new GUIButton(&tb2);
	gb2->Initialize();
	gb2->SetTextures(ResourceManager::GetInstance()->LoadTexture(&tBtnSt), ResourceManager::GetInstance()->LoadTexture(&tBtnStA));
	gb2->SetPosition(glm::vec2(0.7f, -0.3f));
	gb2->SetScale(glm::vec2(0.2f, 0.2f));

	GUIButton* gb3 = new GUIButton(&tb3);
	gb3->Initialize();
	gb3->SetTextures(ResourceManager::GetInstance()->LoadTexture(&tBtnWf), ResourceManager::GetInstance()->LoadTexture(&tBtnWfA));
	gb3->SetPosition(glm::vec2(0.7f, -0.75f));
	gb3->SetScale(glm::vec2(0.2f, 0.2f));

	GUIButton* gb4 = new GUIButton(&tb4);
	gb4->Initialize();
	gb4->SetTextures(ResourceManager::GetInstance()->LoadTexture(&tBtnFArr), ResourceManager::GetInstance()->LoadTexture(&tBtnFArrA));
	gb4->SetPosition(glm::vec2(-0.4f, -0.3f));
	gb4->SetRotation(0.0f);
	gb4->SetScale(glm::vec2(0.15f, 0.15f));

	GUIButton* gb5 = new GUIButton(&tb5);
	gb5->Initialize();
	gb5->SetTextures(ResourceManager::GetInstance()->LoadTexture(&tBtnFArr), ResourceManager::GetInstance()->LoadTexture(&tBtnFArrA));
	gb5->SetPosition(glm::vec2(-0.4f, -0.8f));
	gb5->SetRotation(M_PI * 0.5f);
	gb5->SetScale(glm::vec2(0.15f, 0.15f));

	GUIButton* gb6 = new GUIButton(&tb6);
	gb6->Initialize();
	gb6->SetTextures(ResourceManager::GetInstance()->LoadTexture(&tBtnFArr), ResourceManager::GetInstance()->LoadTexture(&tBtnFArrA));
	gb6->SetPosition(glm::vec2(-0.6f, -0.55f));
	gb6->SetRotation(M_PI * 0.75f);
	gb6->SetScale(glm::vec2(0.15f, 0.15f));

	GUIButton* gb7 = new GUIButton(&tb7);
	gb7->Initialize();
	gb7->SetTextures(ResourceManager::GetInstance()->LoadTexture(&tBtnFArr), ResourceManager::GetInstance()->LoadTexture(&tBtnFArrA));
	gb7->SetPosition(glm::vec2(-0.2f, -0.55f));
	gb7->SetRotation(M_PI * 0.25f);
	gb7->SetScale(glm::vec2(0.15f, 0.15f));

	GUIButton* gb8 = new GUIButton(&tb8);
	gb8->Initialize();
	gb8->SetTextures(ResourceManager::GetInstance()->LoadTexture(&tBtnArr), ResourceManager::GetInstance()->LoadTexture(&tBtnArrA));
	gb8->SetPosition(glm::vec2(-0.8f, -0.3f));
	gb8->SetRotation(0.0f);
	gb8->SetScale(glm::vec2(0.15f, 0.15f));

	GUIButton* gb9 = new GUIButton(&tb9);
	gb9->Initialize();
	gb9->SetTextures(ResourceManager::GetInstance()->LoadTexture(&tBtnArr), ResourceManager::GetInstance()->LoadTexture(&tBtnArrA));
	gb9->SetPosition(glm::vec2(-0.8f, -0.8f));
	gb9->SetRotation(M_PI * 0.5f);
	gb9->SetScale(glm::vec2(0.15f, 0.15f));

	GUIButton* gb10 = new GUIButton(&tb10);
	gb10->Initialize();
	gb10->SetTextures(ResourceManager::GetInstance()->LoadTexture(&tBtnMva), ResourceManager::GetInstance()->LoadTexture(&tBtnMvaA));
	gb10->SetPosition(glm::vec2(0.7f, 0.15f));
	gb10->SetScale(glm::vec2(0.2f, 0.2f));

	GUIButton* gb11 = new GUIButton(&tb11);
	gb11->Initialize();
	gb11->SetTextures(ResourceManager::GetInstance()->LoadTexture(&tBtnMvf), ResourceManager::GetInstance()->LoadTexture(&tBtnMvfA));
	gb11->SetPosition(glm::vec2(0.7f, 0.15f));
	gb11->SetScale(glm::vec2(0.2f, 0.2f));

	geGroupBtnsMove->AddChild(gb4);
	geGroupBtnsMove->AddChild(gb5);
	geGroupBtnsMove->AddChild(gb6);
	geGroupBtnsMove->AddChild(gb7);
	geGroupBtnsMove->AddChild(gb8);
	geGroupBtnsMove->AddChild(gb9);

	geGroupBtns->AddChild(gb1);
	geGroupBtns->AddChild(gb2);
	geGroupBtns->AddChild(gb3);
	geGroupBtns->AddChild(geGroupBtnsMove);
	geGroupBtns->AddChild(gb10);
	geGroupBtns->AddChild(gb11);

	AddGUIElement(geGroupText);
	AddGUIElement(geGroupBtns);
	AddGUIElement(geGroupSettings);

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