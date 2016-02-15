#include "GUIController.h"
#include "GUIText.h"
#include "GUIButton.h"
#include "ClothSimulator.h"
#include "GUISettingsScreen.h"

GUIController::GUIController(SimObject* obj) : Component(obj)
{
}

GUIController::GUIController(const GUIController* c) : Component(c)
{
}

GUIController::~GUIController()
{
}

unsigned int GUIController::Initialize()
{
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
	GUIElement* group = (GUIElement*)System::GetInstance()->GetCurrentScene()->GetGUIElement(&gr1);
	m_fpsText = (GUIText*)group->GetChild(&tval01);
	m_dtText = (GUIText*)group->GetChild(&tval02);
	m_ttText = (GUIText*)group->GetChild(&tval03);
	m_stText = (GUIText*)group->GetChild(&tval04);
	m_dText = (GUIText*)group->GetChild(&tval05);

	m_otherGroups.push_back(group);
	m_otherGroups.push_back((GUIElement*)System::GetInstance()->GetCurrentScene()->GetGUIElement(&gr2));

	SimObject* cObj = System::GetInstance()->GetCurrentScene()->GetObject(3);
	m_cSimulator = (ClothSimulator*)cObj->GetComponent(0);

	GUIButton* btn = (GUIButton*)((GUIElement*)System::GetInstance()->GetCurrentScene()->GetGUIElement(&gr2))->GetChild(&tb1);
	btn->EventClick.push_back(ActionExitProgram);
	btn = (GUIButton*)((GUIElement*)System::GetInstance()->GetCurrentScene()->GetGUIElement(&gr2))->GetChild(&tb2);
	btn->EventClick.push_back(ActionShowPreferences);
	btn->SetParamsClick(this);
	btn = (GUIButton*)((GUIElement*)System::GetInstance()->GetCurrentScene()->GetGUIElement(&gr2))->GetChild(&tb3);
	btn->EventClick.push_back(ActionSetDisplayMode);
	btn = (GUIButton*)((GUIElement*)((GUIElement*)System::GetInstance()->GetCurrentScene()->GetGUIElement(&gr2))->GetChild(&gr3))->GetChild(&tb4);
	btn->EventHold.push_back(ActionMoveActiveObject);
	btn->SetParamsHold((void*)1);
	btn = (GUIButton*)((GUIElement*)((GUIElement*)System::GetInstance()->GetCurrentScene()->GetGUIElement(&gr2))->GetChild(&gr3))->GetChild(&tb5);
	btn->EventHold.push_back(ActionMoveActiveObject);
	btn->SetParamsHold((void*)2);
	btn = (GUIButton*)((GUIElement*)((GUIElement*)System::GetInstance()->GetCurrentScene()->GetGUIElement(&gr2))->GetChild(&gr3))->GetChild(&tb6);
	btn->EventHold.push_back(ActionMoveActiveObject);
	btn->SetParamsHold((void*)3);
	btn = (GUIButton*)((GUIElement*)((GUIElement*)System::GetInstance()->GetCurrentScene()->GetGUIElement(&gr2))->GetChild(&gr3))->GetChild(&tb7);
	btn->EventHold.push_back(ActionMoveActiveObject);
	btn->SetParamsHold((void*)4);
	btn = (GUIButton*)((GUIElement*)((GUIElement*)System::GetInstance()->GetCurrentScene()->GetGUIElement(&gr2))->GetChild(&gr3))->GetChild(&tb8);
	btn->EventHold.push_back(ActionMoveActiveObject);
	btn->SetParamsHold((void*)5);
	btn = (GUIButton*)((GUIElement*)((GUIElement*)System::GetInstance()->GetCurrentScene()->GetGUIElement(&gr2))->GetChild(&gr3))->GetChild(&tb9);
	btn->EventHold.push_back(ActionMoveActiveObject);
	btn->SetParamsHold((void*)6);

	m_sScreen = (GUISettingsScreen*)System::GetInstance()->GetCurrentScene()->GetGUIElement(&gr4);
	m_sScreen->EventApply.push_back(ActionApplyPreferences);
	m_sScreen->EventCancel.push_back(ActionCancelPreferences);
	m_sScreen->SetParamsApply(this);
	m_sScreen->SetParamsCancel(this);

	m_btnArrowsGroup = System::GetInstance()->GetCurrentScene()->GetGUIElement(&gr2)->GetChild(&gr3);
	m_btnMvArrows = (GUIButton*)((GUIElement*)System::GetInstance()->GetCurrentScene()->GetGUIElement(&gr2))->GetChild(&tb10);
	m_btnMvTouch = (GUIButton*)((GUIElement*)System::GetInstance()->GetCurrentScene()->GetGUIElement(&gr2))->GetChild(&tb11);
	m_btnMvArrows->SetParamsClick(this);
	m_btnMvTouch->SetParamsClick(this);
	m_btnMvArrows->SetParamsClick((void*)0);
	m_btnMvTouch->SetParamsClick((void*)1);
	m_btnMvArrows->EventClick.push_back(ActionSwitchInputMode);
	m_btnMvTouch->EventClick.push_back(ActionSwitchInputMode);
	m_btnMvArrows->SetBlockable(false);
	m_btnMvArrows->SetVisible(false);
	m_btnMvArrows->SetEnabled(false);

	return CS_ERR_NONE;
}

unsigned int GUIController::Shutdown()
{
	return CS_ERR_NONE;
}



unsigned int GUIController::Update()
{
	// UPDATING UI INFORMATION

	if (Timer::GetInstance()->GetTotalTime() - infoTimeDisplayHelper >= INFO_UPDATE_RATE)
	{
		double fps, dt, tt;
		string fpsTxt, dtTxt, ttTxt, dTxt;
		fps = Timer::GetInstance()->GetFps();
		dt = Timer::GetInstance()->GetDeltaTime();
		tt = m_cSimulator->GetSimTimeMS();
		infoTimeDisplayHelper = Timer::GetInstance()->GetTotalTime();

		DoubleToStringPrecision(fps, 2, &fpsTxt);
		DoubleToStringPrecision(dt, 4, &dtTxt);
		DoubleToStringPrecision(tt, 4, &ttTxt);
		DoubleToStringPrecision(m_cSimulator->GetD(), 8, &dTxt);

		m_fpsText->SetText(&fpsTxt);
		m_dtText->SetText(&dtTxt);
		m_ttText->SetText(&ttTxt);
		m_dText->SetText(&dTxt);
	}

	///////////////////////////
	// MODE: ARROWS MOVEMENT
	
	if (m_inputMode == InputMode::ARROWS)
	{
		// ROTATING CAMERA

		if (InputHandler::GetInstance()->GetMove() && InputHandler::GetInstance()->GetHold())
		{
			glm::vec2 camVec;
			InputHandler::GetInstance()->GetCameraRotationVector(&camVec);
			glm::vec4 camCurrentPos = glm::vec4(*System::GetInstance()->GetCurrentScene()->GetCamera()->GetPosition(), 1.0f);
			glm::vec3 camRight = *System::GetInstance()->GetCurrentScene()->GetCamera()->GetRight();

			glm::mat4 horRotation = glm::rotate(camVec.x * CSSET_CAMERA_ROTATE_SPEED, glm::vec3(0.0f, 1.0f, 0.0f));
			glm::mat4 vertRotation = glm::rotate(camVec.y * CSSET_CAMERA_ROTATE_SPEED, camRight);

			camCurrentPos = camCurrentPos * horRotation * vertRotation;
			glm::vec3 newPos = glm::vec3(camCurrentPos.x, camCurrentPos.y, camCurrentPos.z);

			glm::vec3 dir = glm::normalize(*System::GetInstance()->GetCurrentScene()->GetCamera()->GetTarget() - newPos);
			glm::vec3 dirYZero = glm::vec3(0.0f, -1.0f, 0.0f);
			float dot = glm::dot(dir, dirYZero);

			if (dot < CSSET_CAMERA_ROTATE_BARRIER && newPos.y > CSSET_CAMERA_ROTATE_MIN_Y)
			{
				System::GetInstance()->GetCurrentScene()->GetCamera()->SetPosition(&newPos);
			}
		}

		//////////////////////////

		// ZOOMING CAMERA

		float relScroll = InputHandler::GetInstance()->GetZoomValue();
		if (relScroll != 0.0f && InputHandler::GetInstance()->GetZoom() && InputHandler::GetInstance()->GetMove())
		{
			glm::vec3 cPos = *System::GetInstance()->GetCurrentScene()->GetCamera()->GetPosition();
			float length = glm::length<float>(cPos);
			float scrollValue = relScroll * CSSET_CAMERA_ZOOM_SPEED;
			scrollValue = length - scrollValue;

			if (scrollValue >= CSSET_CAMERA_ZOOM_BARRIER_MIN && scrollValue <= CSSET_CAMERA_ZOOM_BARRIER_MAX)
			{
				glm::vec3 finalPos = glm::normalize(cPos) * scrollValue;
				System::GetInstance()->GetCurrentScene()->GetCamera()->SetPosition(&finalPos);
			}
		}

		//////////////////////////

		// MOVING CAMERA ON XZ PLANE
		if (InputManager::GetInstance()->GetDoubleTouch() && InputHandler::GetInstance()->GetMove())
		{
			glm::vec2 mVec;
			InputHandler::GetInstance()->GetCameraMovementVector(&mVec);

			if (mVec.x != 0.0f || mVec.y != 0.0f)
			{
				glm::vec4 cTarget = glm::vec4(*System::GetInstance()->GetCurrentScene()->GetCamera()->GetTarget(), 1.0f);
				glm::vec4 cPos = glm::vec4(*System::GetInstance()->GetCurrentScene()->GetCamera()->GetPosition(), 1.0f);
				glm::mat4 viewMatrix = *System::GetInstance()->GetCurrentScene()->GetCamera()->GetViewMatrix();
				glm::mat4 projMatrix = *System::GetInstance()->GetCurrentScene()->GetCamera()->GetProjMatrix();
				cTarget = projMatrix * (viewMatrix * cTarget);
				cPos = projMatrix * (viewMatrix * cPos);
				cTarget.x += -mVec.x * CSSET_CAMERA_MOVE_SPEED;
				cTarget.y += mVec.y * CSSET_CAMERA_MOVE_SPEED;
				cPos.x += -mVec.x * CSSET_CAMERA_MOVE_SPEED;
				cPos.y += mVec.y * CSSET_CAMERA_MOVE_SPEED;
				cTarget = glm::inverse(viewMatrix) * (glm::inverse(projMatrix) * cTarget);
				cPos = glm::inverse(viewMatrix) * (glm::inverse(projMatrix) * cPos);

				if (glm::abs(cPos.x) <= CSSET_CAMERA_POSITION_MAX && 
					glm::abs(cPos.y) <= CSSET_CAMERA_POSITION_MAX &&
					glm::abs(cPos.z) <= CSSET_CAMERA_POSITION_MAX)
				{
					glm::vec3 fTarget = glm::vec3(cTarget);
					glm::vec3 fPos = glm::vec3(cPos);

					System::GetInstance()->GetCurrentScene()->GetCamera()->SetPosition(&fPos);
					System::GetInstance()->GetCurrentScene()->GetCamera()->SetTarget(&fTarget);
				}

				/*
				glm::vec3 cTarget = *System::GetInstance()->GetCurrentScene()->GetCamera()->GetTarget();
				glm::vec3 cPos = *System::GetInstance()->GetCurrentScene()->GetCamera()->GetPosition();

				glm::vec3 cDir = *System::GetInstance()->GetCurrentScene()->GetCamera()->GetDirection();
				glm::vec3 cRght = *System::GetInstance()->GetCurrentScene()->GetCamera()->GetRight();
				cDir.y = 0.0f;
				cRght.y = 0.0f;

				cTarget = cTarget + cRght * -mVec.x * CSSET_CAMERA_MOVE_SPEED;
				cTarget = cTarget + cDir * mVec.y * CSSET_CAMERA_MOVE_SPEED;

				if (glm::abs(cTarget.x) <= CSSET_CAMERA_POSITION_MAX && glm::abs(cTarget.z) <= CSSET_CAMERA_POSITION_MAX)
				{
					cPos = cPos + cRght * -mVec.x * CSSET_CAMERA_MOVE_SPEED;
					cPos = cPos + cDir * mVec.y * CSSET_CAMERA_MOVE_SPEED;
					System::GetInstance()->GetCurrentScene()->GetCamera()->SetPosition(&cPos);
					System::GetInstance()->GetCurrentScene()->GetCamera()->SetTarget(&cTarget);
				}
				*/
			}
		}
	}
	else if (m_inputMode == InputMode::TOUCH)
	{
		// update touch position and direction in cloth simulator
		glm::vec2 tPos = glm::vec2(0.0f);
		glm::vec2 tDir = glm::vec2(0.0f);
		if (InputManager::GetInstance()->GetMove() && InputManager::GetInstance()->GetTouch())
		{
			InputManager::GetInstance()->GetTouchPosition(&tPos);
			InputManager::GetInstance()->GetTouchDirection(&tDir);
			Engine* engine = System::GetInstance()->GetEngineData();
			tPos.x = tPos.x / (float)engine->width * 2.0f - 1.0f;
			tPos.y = tPos.y / (float)engine->height * 2.0f - 1.0f;
			tPos.y = -tPos.y;
			tDir.y = -tDir.y;
			tDir *= 0.001f;
		}
		m_cSimulator->UpdateTouchVector(&tPos, &tDir);
		//LOGI("%f %f %f %f", tPos.x, tPos.y, tDir.x, tDir.y);
		

		// update camera rotation and gravity according to accelerometer input
		glm::vec3 camVec;
		InputManager::GetInstance()->GetAccelerationDelta(&camVec);
		camVec *= 0.0f;	// !!!
		glm::vec4 camCurrentPos = glm::vec4(*System::GetInstance()->GetCurrentScene()->GetCamera()->GetPosition(), 1.0f);
		glm::vec3 camRight = *System::GetInstance()->GetCurrentScene()->GetCamera()->GetRight();

		glm::mat4 horRotation = glm::rotate(camVec.x * CSSET_CAMERA_ROTATE_SPEED, glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4 vertRotation = glm::rotate(camVec.y * CSSET_CAMERA_ROTATE_SPEED, camRight);

		camCurrentPos = camCurrentPos * horRotation * vertRotation;
		glm::vec3 newPos = glm::vec3(camCurrentPos.x, camCurrentPos.y, camCurrentPos.z);

		glm::vec3 dir = *System::GetInstance()->GetCurrentScene()->GetCamera()->GetTarget() - newPos;
		glm::vec3 dirYZero = glm::vec3(dir.x, 0.0f, dir.z);
		float dot = glm::dot(dir, dirYZero);

		if (dot > CSSET_CAMERA_ROTATE_BARRIER)
		{
			System::GetInstance()->GetCurrentScene()->GetCamera()->SetPosition(&newPos);
		}
	}

	// update mode change helper
	if (m_modeChangeHelper != -1)
		m_modeChangeHelper = -1;

	//////////////////////////
	return CS_ERR_NONE;
}

unsigned int GUIController::Draw()
{
	return CS_ERR_NONE;
}

void GUIController::ActionExitProgram(std::vector<void*>* params, const glm::vec2* clickPos)
{
	LOGI("Shutting down!");

	System::GetInstance()->Stop();
}

void GUIController::ActionMoveActiveObject(std::vector<void*>* params, const glm::vec2* clickPos)
{
	if (params->size() != 1)
		return;

	SimObject* cObj = System::GetInstance()->GetCurrentScene()->GetObject();

	glm::vec3 mVector = glm::vec3();
	MovementDirection dir = (MovementDirection)(int)(params->at(0));
	float scl = cObj->GetTransform()->GetScale()->y;
	float pos = cObj->GetTransform()->GetPosition()->y;
	switch (dir)
	{
	case MovementDirection::FORWARD:
		mVector = glm::vec3(0.0f, 0.0f, -1.0f);
		break;
	case MovementDirection::BACKWARD:
		mVector = glm::vec3(0.0f, 0.0f, 1.0f);
		break;
	case MovementDirection::LEFT:
		mVector = glm::vec3(-1.0f, 0.0f, 0.0f);
		break;
	case MovementDirection::RIGHT:
		mVector = glm::vec3(1.0f, 0.0f, 0.0f);
		break;
	case MovementDirection::UP:
		mVector = glm::vec3(0.0f, 1.0f, 0.0f);
		break;
	case MovementDirection::DOWN:
		if ((pos - scl) > System::GetInstance()->GetCurrentScene()->GetGroundLevel())
			mVector = glm::vec3(0.0f, -1.0f, 0.0f);
		break;
	default:
		mVector = glm::vec3();
		break;
	}

	glm::vec3 cPosVector = *cObj->GetTransform()->GetPosition();

	mVector = mVector * BOX_SPEED * (float)Timer::GetInstance()->GetDeltaTime();

	glm::vec3 addedVector = cPosVector + mVector;
	cObj->GetTransform()->SetPosition(&addedVector);
}

void GUIController::ActionSetDisplayMode(std::vector<void*>* params, const glm::vec2* clickPos)
{
	LOGI("Changing diplay mode!");
	DrawMode m = Renderer::GetInstance()->GetDrawMode();
	int newMode = (((int)m + 1) % 3);
	Renderer::GetInstance()->SetDrawMode((DrawMode)newMode);
}

void GUIController::ActionShowPreferences(std::vector<void*>* params, const glm::vec2* clickPos)
{
	if (params->size() < 1)
		return;

	GUIController* inst = (GUIController*)params->at(0);
	inst->m_sScreen->SetBlockable(true);
	inst->m_sScreen->SetVisible(true);
	inst->m_sScreen->SetEnabled(true);

	for (std::vector<GUIElement*>::iterator it = inst->m_otherGroups.begin(); it != inst->m_otherGroups.end(); ++it)
	{
		(*it)->SetVisible(false);
		(*it)->SetEnabled(false);
	}

	/*
	ClothSimulator* cSim = (ClothSimulator*)(params->at(0));
	cSim->SwitchMode();
	const string MS_VALUE = "Mass-spring";
	const string PB_VALUE = "Position based";
	const string UN_VALUE = "Unknown";

	ClothSimulationMode cMode = cSim->GetMode();
	GUIText* txt = (GUIText*)(params->at(1));
	switch (cMode)
	{
	case ClothSimulationMode::MASS_SPRING:
		txt->SetText(&MS_VALUE);
		break;

	case ClothSimulationMode::POSITION_BASED:
		txt->SetText(&PB_VALUE);
		break;

	default:
		txt->SetText(&UN_VALUE);
		break;
	}
	*/
}

void GUIController::ActionApplyPreferences(std::vector<void*>* params, const glm::vec2* clickPos)
{
	if (params->size() < 1)
		return;

	GUIController* inst = (GUIController*)params->at(0);
	inst->m_sScreen->SetBlockable(false);
	inst->m_sScreen->SetVisible(false);
	inst->m_sScreen->SetEnabled(false);

	PhysicsManager::GetInstance()->SetGravity(inst->m_sScreen->GetSimParams()->gravity);

	ClothSimulationVersusObject v = inst->m_sScreen->GetSimParams()->vsObj;
	glm::vec3 tPos = (glm::vec3(0.0f, 2.5f, 0.0f));
	glm::vec3 outPos = (glm::vec3(0.0f, 999.0f, 0.0f));
	int me, other;
	me = other = 0;
	switch (v)
	{
	case ClothSimulationVersusObject::OBJ_SPHERE:
		me = 1;
		other = 2;
		break;
	case ClothSimulationVersusObject::OBJ_BOX:
		me = 2;
		other = 1;
		break;
	}
	System::GetInstance()->GetCurrentScene()->SetCurrentObject(me);

	System::GetInstance()->GetCurrentScene()->GetObject(other)->GetTransform()->SetPosition(&outPos);
	System::GetInstance()->GetCurrentScene()->GetObject(other)->SetVisible(false);
	System::GetInstance()->GetCurrentScene()->GetObject(other)->SetEnabled(false);

	System::GetInstance()->GetCurrentScene()->GetObject(me)->GetTransform()->SetPosition(&tPos);
	System::GetInstance()->GetCurrentScene()->GetObject(me)->SetVisible(true);
	System::GetInstance()->GetCurrentScene()->GetObject(me)->SetEnabled(true);

	inst->m_cSimulator->Restart(inst->m_sScreen->GetSimParams());

	GUIText* st = inst->m_stText;
	ClothSimulationMode cMode = inst->m_sScreen->GetSimParams()->mode;
	const string MSG_VALUE = "Mass-spring - GPU";
	const string PBG_VALUE = "Position based - GPU";
	const string MSC_VALUE = "Mass-spring - CPU";
	const string PBC_VALUE = "Position based - CPU";
	const string MSC4_VALUE = "Mass-spring - CPUx4";
	const string PBC4_VALUE = "Position based - CPUx4";
	const string UN_VALUE = "Unknown";

	switch (cMode)
	{
	case ClothSimulationMode::MASS_SPRING_GPU:
		st->SetText(&MSG_VALUE);
		break;

	case ClothSimulationMode::POSITION_BASED_GPU:
		st->SetText(&PBG_VALUE);
		break;

	case ClothSimulationMode::MASS_SPRING_CPU:
		st->SetText(&MSC_VALUE);
		break;

	case ClothSimulationMode::POSITION_BASED_CPU:
		st->SetText(&PBC_VALUE);
		break;

	case ClothSimulationMode::MASS_SPRING_CPUx4:
		st->SetText(&MSC4_VALUE);
		break;

	case ClothSimulationMode::POSITION_BASED_CPUx4:
		st->SetText(&PBC4_VALUE);
		break;

	default:
		st->SetText(&UN_VALUE);
		break;
	}

	if (inst->m_firstRun)
	{
		inst->m_firstRun = false;
		inst->m_sScreen->SetCancelEnabled(true);
	}

	for (std::vector<GUIElement*>::iterator it = inst->m_otherGroups.begin(); it != inst->m_otherGroups.end(); ++it)
	{
		(*it)->SetVisible(true);
		(*it)->SetEnabled(true);
	}
}

void GUIController::ActionCancelPreferences(std::vector<void*>* params, const glm::vec2* clickPos)
{
	if (params->size() < 1)
		return;

	GUIController* inst = (GUIController*)params->at(0);
	inst->m_sScreen->SetBlockable(false);
	inst->m_sScreen->SetVisible(false);
	inst->m_sScreen->SetEnabled(false);

	for (std::vector<GUIElement*>::iterator it = inst->m_otherGroups.begin(); it != inst->m_otherGroups.end(); ++it)
	{
		(*it)->SetVisible(true);
		(*it)->SetEnabled(true);
	}
}

void GUIController::ActionSwitchInputMode(std::vector<void*>* params, const glm::vec2 * clickPos)
{
	if (params->size() < 2)
		return;

	GUIController* inst = (GUIController*)params->at(0);
	int mode = (int)params->at(1);
	if (inst->m_modeChangeHelper == -1)
	{
		inst->m_modeChangeHelper = mode;
		bool bl = true;
		if (mode == 0)
		{
			inst->m_inputMode = InputMode::ARROWS;
		}
		else if (mode == 1)
		{
			bl = false;
			inst->m_inputMode = InputMode::TOUCH;
		}

		inst->m_btnMvTouch->SetEnabled(bl);
		inst->m_btnMvTouch->SetVisible(bl);
		inst->m_btnMvTouch->SetBlockable(bl);
		inst->m_btnMvArrows->SetEnabled(!bl);
		inst->m_btnMvArrows->SetVisible(!bl);
		inst->m_btnMvArrows->SetBlockable(!bl);
		inst->m_btnArrowsGroup->SetVisible(bl);
		inst->m_btnArrowsGroup->SetEnabled(bl);

		glm::vec2 tPos = glm::vec2(0.0f);
		glm::vec2 tDir = glm::vec2(0.0f);
		inst->m_cSimulator->UpdateTouchVector(&tPos, &tDir);
	}
}
