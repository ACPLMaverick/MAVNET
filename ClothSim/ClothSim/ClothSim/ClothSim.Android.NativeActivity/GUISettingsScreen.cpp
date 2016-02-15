#include "GUISettingsScreen.h"
#include "GUIPicture.h"
#include "GUIButton.h"
#include "GUIValueSetter.h"
#include "GUIText.h"


GUISettingsScreen::GUISettingsScreen(const std::string* id) : GUIElement(id)
{
}

GUISettingsScreen::GUISettingsScreen(const GUISettingsScreen * c) : GUIElement(c)
{
}

GUISettingsScreen::~GUISettingsScreen()
{
}

unsigned int GUISettingsScreen::Initialize()
{
	unsigned int err = CS_ERR_NONE;

	err = GUIElement::Initialize();
	if (err != CS_ERR_NONE)
		return err;

	string vsSimTypeID = m_id + "_vsSimType";
	string vsObjTypeID = m_id + "_vsObjType";
	string vsGravityID = m_id + "_vsGravity";
	string vsWidthID = m_id + "_vsWidth";
	string vsLengthID = m_id + "_vsLength";
	string vsEdgesWidthID = m_id + "_vsEdgesWidth";
	string vsEdgesLengthID = m_id + "_vsEdgesLength";
	string vsElasticityID = m_id + "_vsElasticity";
	string vsMassID = m_id + "_vsMass";
	string vsAirDampID = m_id + "_vsAirDamp";
	string vsElDampID = m_id + "_vsElDamp";

	string btnApplyID = m_id + "_btnApply";
	string btnCancelID = m_id + "_btnCancel";
	string lblSettings = m_id + "_lblSettings";
	string bgID = m_id + "_bgID";

	string tPlus = "textures/btn_minus.png";
	string tPlusA = "textures/btn_minus_a.png";
	string tMinus = "textures/btn_plus.png";
	string tMinusA = "textures/btn_plus_a.png";
	string tText = "textures/ExportedFont.tga";
	string tBg = "textures/bg_settings.png";
	string tApply = "textures/btn_ok.png";
	string tApplyA = "textures/btn_ok_a.png";
	string tCancel = "textures/btn_exit.png";
	string tCancelA = "textures/btn_exit_a.png";

	string vsSimTypeLBL = "Simulation mode";
	string vsObjTypeLBL = "Object";
	string vsGravityLBL = "Gravity";
	string vsWidthLBL = "Width";
	string vsLengthLBL = "Length";
	string vsEdgesWidthLBL = "Edges width";
	string vsEdgesLengthLBL = "Edges length";
	string vsElasticityLBL = "Stiffness";
	string vsMassLBL = "Mass";
	string vsAirDampLBL = "Air damp";
	string vsElDampLBL ="Spring damp";

	string mainLBL = "Cloth parameters";

	glm::vec2 txtScale = glm::vec2(0.05f, 0.05f);
	glm::vec2 txtPos = glm::vec2(-0.7f, 0.73f);
	glm::vec2 btnScale = glm::vec2(0.2f, 0.2f);
	glm::vec2 btnApplyPos = glm::vec2(-0.0f, -0.8f);
	glm::vec2 btnCancelPos = glm::vec2(0.0f, -0.8f);
	glm::vec2 vsScale = glm::vec2(0.3f, 0.038f);
	glm::vec2 vsSScale = glm::vec2(0.75f, 0.038f);

	int vsCount = 9;
	int inOneCollumn = (int)glm::ceil((float)vsCount / 2.0f);
	float vsXOffset = -0.45f;
	float vsYOffset = 2.0f / (float)vsCount;
	float vsYStart = 0.6f;

	////////////////////////////////

	std::vector<std::string> vsSimTypeStateLabels;
	vsSimTypeStateLabels.push_back("MassSpring - GPU");
	vsSimTypeStateLabels.push_back("MassSpring - CPU");
	vsSimTypeStateLabels.push_back("MassSpring - CPUx4");
	vsSimTypeStateLabels.push_back("PosBased - GPU");
	vsSimTypeStateLabels.push_back("PosBased - CPU");
	vsSimTypeStateLabels.push_back("PosBased - CPUx4");

	m_vsSimType = new GUIValueSetter
		(
			&vsSimTypeID,
			&vsSimTypeLBL,
			ResourceManager::GetInstance()->LoadTexture(&tPlus),
			ResourceManager::GetInstance()->LoadTexture(&tPlusA),
			ResourceManager::GetInstance()->LoadTexture(&tMinus),
			ResourceManager::GetInstance()->LoadTexture(&tMinusA),
			ResourceManager::GetInstance()->LoadTexture(&tText),
			6,
			0,
			&vsSimTypeStateLabels,
			-0.3f,
			true,
			false
		);
	m_vsSimType->SetPosition(m_position + glm::vec2(0.0f, vsYStart));
	m_vsSimType->SetScale(vsSScale);
	m_vsSimType->Initialize();

	std::vector<std::string> vsObjTypeStateLabels;
	vsObjTypeStateLabels.push_back("Sphere");
	vsObjTypeStateLabels.push_back("Box");
	m_vsObjType = new GUIValueSetter
		(
			&vsObjTypeID,
			&vsObjTypeLBL,
			ResourceManager::GetInstance()->LoadTexture(&tPlus),
			ResourceManager::GetInstance()->LoadTexture(&tPlusA),
			ResourceManager::GetInstance()->LoadTexture(&tMinus),
			ResourceManager::GetInstance()->LoadTexture(&tMinusA),
			ResourceManager::GetInstance()->LoadTexture(&tText),
			(unsigned int)2,
			(unsigned int)0,
			&vsObjTypeStateLabels,
			-0.05f,
			true,
			false
			);

	m_vsGravity = new GUIValueSetter
		(
			&vsGravityID,
			&vsGravityLBL,
			ResourceManager::GetInstance()->LoadTexture(&tPlus),
			ResourceManager::GetInstance()->LoadTexture(&tPlusA),
			ResourceManager::GetInstance()->LoadTexture(&tMinus),
			ResourceManager::GetInstance()->LoadTexture(&tMinusA),
			ResourceManager::GetInstance()->LoadTexture(&tText),
			100,
			9,
			0.1f,
			0.1f,
			2,
			0.0f,
			false,
			false
			);

	m_vsWidth = new GUIValueSetter
		(
			&vsWidthID,
			&vsWidthLBL,
			ResourceManager::GetInstance()->LoadTexture(&tPlus),
			ResourceManager::GetInstance()->LoadTexture(&tPlusA),
			ResourceManager::GetInstance()->LoadTexture(&tMinus),
			ResourceManager::GetInstance()->LoadTexture(&tMinusA),
			ResourceManager::GetInstance()->LoadTexture(&tText),
			50,
			9,
			1.0f,
			1.0f,
			0,
			0.0f,
			false,
			false
			);

	m_vsLength = new GUIValueSetter
		(
			&vsLengthID,
			&vsLengthLBL,
			ResourceManager::GetInstance()->LoadTexture(&tPlus),
			ResourceManager::GetInstance()->LoadTexture(&tPlusA),
			ResourceManager::GetInstance()->LoadTexture(&tMinus),
			ResourceManager::GetInstance()->LoadTexture(&tMinusA),
			ResourceManager::GetInstance()->LoadTexture(&tText),
			50,
			9,
			1.0f,
			1.0f,
			0,
			0.0f,
			false,
			false
			);

	m_vsEdgesWidth = new GUIValueSetter
		(
			&vsEdgesWidthID,
			&vsEdgesWidthLBL,
			ResourceManager::GetInstance()->LoadTexture(&tPlus),
			ResourceManager::GetInstance()->LoadTexture(&tPlusA),
			ResourceManager::GetInstance()->LoadTexture(&tMinus),
			ResourceManager::GetInstance()->LoadTexture(&tMinusA),
			ResourceManager::GetInstance()->LoadTexture(&tText),
			117,
			21,
			1.0f,
			2.0f,
			0,
			0.0f,
			false,
			false
			);

	m_vsEdgesLength = new GUIValueSetter
		(
			&vsEdgesLengthID,
			&vsEdgesLengthLBL,
			ResourceManager::GetInstance()->LoadTexture(&tPlus),
			ResourceManager::GetInstance()->LoadTexture(&tPlusA),
			ResourceManager::GetInstance()->LoadTexture(&tMinus),
			ResourceManager::GetInstance()->LoadTexture(&tMinusA),
			ResourceManager::GetInstance()->LoadTexture(&tText),
			117,
			21,
			1.0f,
			2.0f,
			0,
			0.0f,
			false,
			false
			);

	m_vsElasticity = new GUIValueSetter
		(
			&vsElasticityID,
			&vsElasticityLBL,
			ResourceManager::GetInstance()->LoadTexture(&tPlus),
			ResourceManager::GetInstance()->LoadTexture(&tPlusA),
			ResourceManager::GetInstance()->LoadTexture(&tMinus),
			ResourceManager::GetInstance()->LoadTexture(&tMinusA),
			ResourceManager::GetInstance()->LoadTexture(&tText),
			1001,
			100,
			1.0f,
			0.0f,
			1,
			-0.01f,
			false,
			false
			);

	m_vsMass = new GUIValueSetter
		(
			&vsMassID,
			&vsMassLBL,
			ResourceManager::GetInstance()->LoadTexture(&tPlus),
			ResourceManager::GetInstance()->LoadTexture(&tPlusA),
			ResourceManager::GetInstance()->LoadTexture(&tMinus),
			ResourceManager::GetInstance()->LoadTexture(&tMinusA),
			ResourceManager::GetInstance()->LoadTexture(&tText),
			500,
			9,
			0.1f,
			0.1f,
			2,
			0.0f,
			false,
			false
			);

	m_vsAirDamp = new GUIValueSetter
		(
			&vsAirDampID,
			&vsAirDampLBL,
			ResourceManager::GetInstance()->LoadTexture(&tPlus),
			ResourceManager::GetInstance()->LoadTexture(&tPlusA),
			ResourceManager::GetInstance()->LoadTexture(&tMinus),
			ResourceManager::GetInstance()->LoadTexture(&tMinusA),
			ResourceManager::GetInstance()->LoadTexture(&tText),
			100,
			49,
			0.01f,
			0.0f,
			2,
			0.0f,
			false,
			false
			);

	m_vsElDamp = new GUIValueSetter
		(
			&vsElDampID,
			&vsElDampLBL,
			ResourceManager::GetInstance()->LoadTexture(&tPlus),
			ResourceManager::GetInstance()->LoadTexture(&tPlusA),
			ResourceManager::GetInstance()->LoadTexture(&tMinus),
			ResourceManager::GetInstance()->LoadTexture(&tMinusA),
			ResourceManager::GetInstance()->LoadTexture(&tText),
			201,
			150,
			-0.1f,
			0.0f,
			2,
			-0.08f,
			false,
			true
			);

	AddChild(m_vsSimType);
	AddChild(m_vsObjType);
	AddChild(m_vsGravity);
	AddChild(m_vsWidth);
	AddChild(m_vsLength);
	AddChild(m_vsEdgesLength);
	AddChild(m_vsEdgesWidth);
	AddChild(m_vsElasticity);
	AddChild(m_vsMass);
	AddChild(m_vsAirDamp);
	AddChild(m_vsElDamp);

	m_vsVector.push_back(m_vsObjType);
	m_vsVector.push_back(m_vsGravity);
	m_vsVector.push_back(m_vsWidth);
	m_vsVector.push_back(m_vsLength);
	m_vsVector.push_back(m_vsEdgesLength);
	m_vsVector.push_back(m_vsEdgesWidth);
	m_vsVector.push_back(m_vsElasticity);
	m_vsVector.push_back(m_vsMass);
	m_vsVector.push_back(m_vsAirDamp);
	m_vsVector.push_back(m_vsElDamp);

	int i = 0;
	for (std::vector<GUIValueSetter*>::iterator it = m_vsVector.begin(); it != m_vsVector.end(); ++it, ++i)
	{
		GUIValueSetter* vs = *it;
		vs->SetScale(vsScale);
		float xPos, yPos;
		xPos = vsXOffset;
		yPos = vsYStart - vsYOffset * (((i / 2) % inOneCollumn) + 1);
		if (i % 2)
			xPos *= -1.0f;
		vs->SetPosition(m_position + glm::vec2(xPos, yPos));
		vs->Initialize();
	}

	m_vsVector.clear();
	m_vsStatesVector.clear();
	m_vsVector.push_back(m_vsSimType);
	m_vsVector.push_back(m_vsObjType);
	m_vsVector.push_back(m_vsGravity);
	m_vsVector.push_back(m_vsWidth);
	m_vsVector.push_back(m_vsLength);
	m_vsVector.push_back(m_vsEdgesLength);
	m_vsVector.push_back(m_vsEdgesWidth);
	m_vsVector.push_back(m_vsElasticity);
	m_vsVector.push_back(m_vsMass);
	m_vsVector.push_back(m_vsAirDamp);
	m_vsVector.push_back(m_vsElDamp);
	for (std::vector<GUIValueSetter*>::iterator it = m_vsVector.begin(); it != m_vsVector.end(); ++it)
	{
		m_vsStatesVector.push_back((*it)->GetCurrentState());
	}

	m_bg = new GUIPicture(&bgID, ResourceManager::GetInstance()->LoadTexture(&tBg));
	m_bg->Initialize();
	m_bg->SetScaled(false);
	m_bg->SetPosition(m_position);
	AddChild(m_bg);

	m_lblSettings = new GUIText(&lblSettings, &mainLBL, ResourceManager::GetInstance()->LoadTexture(&tText));
	m_lblSettings->Initialize();
	m_lblSettings->SetScale(txtScale);
	m_lblSettings->SetPosition(txtPos);
	AddChild(m_lblSettings);

	m_btnApply = new GUIButton(&btnApplyID);
	m_btnApply->Initialize();
	m_btnApply->SetScale(btnScale);
	m_btnApply->SetPosition(btnApplyPos);
	m_btnApply->SetTextures(
		ResourceManager::GetInstance()->LoadTexture(&tApply),
		ResourceManager::GetInstance()->LoadTexture(&tApplyA)
		);
	m_btnApply->SetParamsClick(this);
	m_btnApply->EventClick.push_back(ApplyButtonClick);
	AddChild(m_btnApply);

	m_btnCancel = new GUIButton(&btnCancelID);
	m_btnCancel->Initialize();
	m_btnCancel->SetScale(btnScale);
	m_btnCancel->SetPosition(btnCancelPos);
	m_btnCancel->SetTextures(
		ResourceManager::GetInstance()->LoadTexture(&tCancel),
		ResourceManager::GetInstance()->LoadTexture(&tCancelA)
		);
	m_btnCancel->SetParamsClick(this);
	m_btnCancel->EventClick.push_back(CancelButtonClick);
	AddChild(m_btnCancel);

	SetCancelEnabled(false);

	return err;
}

unsigned int GUISettingsScreen::Shutdown()
{
	unsigned int err = CS_ERR_NONE;

	for (std::map<string, GUIElement*>::iterator it = m_children.begin(); it != m_children.end(); ++it)
	{
		it->second->Shutdown();
		delete it->second;
	}
	m_children.clear();

	err = GUIElement::Shutdown();
	if (err != CS_ERR_NONE)
		return err;

	return err;
}

SimParams * GUISettingsScreen::GetSimParams()
{
	return &m_params;
}

void GUISettingsScreen::SetParamsApply(void * params)
{
	m_paramsApply.push_back(params);
}

void GUISettingsScreen::SetParamsCancel(void * params)
{
	m_paramsCancel.push_back(params);
}

std::vector<void*>* GUISettingsScreen::GetParamsApply()
{
	return &m_paramsApply;
}

std::vector<void*>* GUISettingsScreen::GetParamsCancel()
{
	return &m_paramsCancel;
}

void GUISettingsScreen::SetCancelEnabled(bool enabled)
{
	float yA = m_btnApply->GetPosition().y;
	float yC = m_btnCancel->GetPosition().y;
	if (enabled)
	{
		
		m_btnApply->SetPosition(glm::vec2(-M_BTN_X_OFFSET, yA));
		m_btnCancel->SetPosition(glm::vec2(M_BTN_X_OFFSET, yC));
	}
	else
	{
		m_btnApply->SetPosition(glm::vec2(0.0f, yA));
		m_btnCancel->SetPosition(glm::vec2(0.0f, yC));
	}
	m_btnCancel->SetEnabled(enabled);
	m_btnCancel->SetVisible(enabled);
}

void GUISettingsScreen::ApplyButtonClick(std::vector<void*>* params, const glm::vec2 * clickPos)
{
	if (params->size() < 1)
		return;

	GUISettingsScreen* in = (GUISettingsScreen*)params->at(0);
	in->UpdateParams();

	for (std::vector<std::function<void(std::vector<void*>*, const glm::vec2*)>>::iterator it = in->EventApply.begin(); it != in->EventApply.end(); ++it)
	{
		(*it)(&in->m_paramsApply, clickPos);
	}
}

void GUISettingsScreen::CancelButtonClick(std::vector<void*>* params, const glm::vec2 * clickPos)
{
	if (params->size() < 1)
		return;

	GUISettingsScreen* in = (GUISettingsScreen*)params->at(0);

	for (std::vector<std::function<void(std::vector<void*>*, const glm::vec2*)>>::iterator it = in->EventCancel.begin(); it != in->EventCancel.end(); ++it)
	{
		(*it)(&in->m_paramsCancel, clickPos);
	}
}

void GUISettingsScreen::UpdateParams()
{
	unsigned int state;
	float stateVal, stateMultiplier, stateOffset;

	state = m_vsSimType->GetCurrentState();
	stateMultiplier = m_vsSimType->GetLabelMultiplier();
	stateOffset = m_vsSimType->GetLabelOffset();
	m_params.mode = (ClothSimulationMode)state;

	state = m_vsObjType->GetCurrentState();
	stateMultiplier = m_vsObjType->GetLabelMultiplier();
	stateOffset = m_vsObjType->GetLabelOffset();
	m_params.vsObj = (ClothSimulationVersusObject)state;

	state = m_vsGravity->GetCurrentState();
	stateMultiplier = m_vsGravity->GetLabelMultiplier();
	stateOffset = m_vsGravity->GetLabelOffset();
	stateVal = (float)state * stateMultiplier + stateOffset;
	m_params.gravity = stateVal;
	
	state = m_vsMass->GetCurrentState();
	stateMultiplier = m_vsMass->GetLabelMultiplier();
	stateOffset = m_vsMass->GetLabelOffset();
	stateVal = (float)state * stateMultiplier + stateOffset;
	m_params.vertexMass = stateVal;

	state = m_vsAirDamp->GetCurrentState();
	stateMultiplier = m_vsAirDamp->GetLabelMultiplier();
	stateOffset = m_vsAirDamp->GetLabelOffset();
	stateVal = (float)state * stateMultiplier + stateOffset;
	m_params.vertexAirDamp = stateVal;

	state = m_vsElasticity->GetCurrentState();
	stateMultiplier = m_vsElasticity->GetLabelMultiplier();
	stateOffset = m_vsElasticity->GetLabelOffset();
	stateVal = (float)state * stateMultiplier + stateOffset;
	m_params.elasticity = stateVal;

	state = m_vsElDamp->GetCurrentState();
	stateMultiplier = m_vsElDamp->GetLabelMultiplier();
	stateOffset = m_vsElDamp->GetLabelOffset();
	stateVal = (float)state * stateMultiplier + stateOffset;
	m_params.elasticityDamp = stateVal;

	state = m_vsWidth->GetCurrentState();
	stateMultiplier = m_vsWidth->GetLabelMultiplier();
	stateOffset = m_vsWidth->GetLabelOffset();
	stateVal = (float)state * stateMultiplier + stateOffset;
	m_params.width = stateVal;

	state = m_vsLength->GetCurrentState();
	stateMultiplier = m_vsLength->GetLabelMultiplier();
	stateOffset = m_vsLength->GetLabelOffset();
	stateVal = (float)state * stateMultiplier + stateOffset;
	m_params.length = stateVal;

	state = m_vsEdgesWidth->GetCurrentState();
	stateMultiplier = m_vsEdgesWidth->GetLabelMultiplier();
	stateOffset = m_vsEdgesWidth->GetLabelOffset();
	unsigned int uival = state * stateMultiplier + stateOffset;
	m_params.edgesWidth = uival;

	state = m_vsEdgesLength->GetCurrentState();
	stateMultiplier = m_vsEdgesLength->GetLabelMultiplier();
	stateOffset = m_vsEdgesLength->GetLabelOffset();
	uival = state * stateMultiplier + stateOffset;
	m_params.edgesLength = uival;

	m_vsStatesVector.clear();
	for (std::vector<GUIValueSetter*>::iterator it = m_vsVector.begin(); it != m_vsVector.end(); ++it)
	{
		m_vsStatesVector.push_back((*it)->GetCurrentState());
	}
}

inline void GUISettingsScreen::ResetStates()
{
	std::vector<unsigned int>::iterator its = m_vsStatesVector.begin();
	for (std::vector<GUIValueSetter*>::iterator it = m_vsVector.begin(); it != m_vsVector.end(); ++it, ++its)
	{
		(*it)->SetCurrentState(*its);
	}
}
