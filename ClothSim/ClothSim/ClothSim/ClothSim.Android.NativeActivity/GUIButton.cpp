#include "GUIButton.h"


GUIButton::GUIButton(const std::string* id) : GUIElement(id)
{
	m_color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	m_rotation = 0.0f;
	m_isBlockable = true;
}

GUIButton::GUIButton(const GUIButton* c) : GUIElement(c)
{
}

GUIButton::~GUIButton()
{
}



unsigned int GUIButton::Initialize()
{
	GUIElement::Initialize();

	m_mesh = new MeshGLRectButton(nullptr, this, &m_color);
	m_mesh->Initialize();

	return CS_ERR_NONE;
}

unsigned int GUIButton::Shutdown()
{
	m_mesh->Shutdown();
	delete m_mesh;

	return CS_ERR_NONE;
}



void GUIButton::GenerateTransformMatrix()
{
	glm::vec2 factors = glm::vec2(1.0f, 1.0f);
	if(m_isScaled) ComputeScaleFactors(&factors);

	m_transform = glm::translate(glm::vec3(m_position.x, m_position.y, 0.0f))
		* glm::scale(glm::vec3(m_scale.x * factors.x, m_scale.y * factors.y, 0.0f));

	m_rot = glm::rotate(2.0f * m_rotation, glm::vec3(0.0f, 0.0f, 1.0f));
}

void GUIButton::ComputeScaleFactors(glm::vec2 * factors)
{
	Engine* engine = System::GetInstance()->GetEngineData();
	float scrWidth = engine->width;
	float scrHeight = engine->height;
	float bBias = 0.6f;
	float factorX = (scrHeight / scrWidth);
	float factorY = (scrWidth / scrHeight);
	float hsFactor = 1.0f / Renderer::GetInstance()->GetScreenRatio();
	if (scrWidth > scrHeight)
	{
		factorY = 1.0f / factorY * hsFactor;
		factorX *= hsFactor;
	}

	factorX *= (1.0f / hsFactor);

	factors->x = factorX;
	factors->y = factorY;
}

void GUIButton::SetTextures(TextureID * texIdle, TextureID* texClicked)
{
	m_textureIdle = texIdle;
	m_textureClicked = texClicked;

	m_mesh->SetTextureID(m_textureIdle);
}

void GUIButton::RemoveTextures()
{
	m_textureClicked = ResourceManager::GetInstance()->GetTextureWhite();
	m_textureIdle = ResourceManager::GetInstance()->GetTextureWhite();

	m_mesh->SetTextureID(m_textureIdle);
}

void GUIButton::SetParamsClick(void * params)
{
	m_paramsClick.push_back(params);
}

void GUIButton::SetParamsHold(void * params)
{
	m_paramsHold.push_back(params);
}

std::vector<void*>* GUIButton::GetParamsClick()
{
	return &m_paramsClick;
}

std::vector<void*>* GUIButton::GetParamsHold()
{
	return &m_paramsHold;
}

unsigned int GUIButton::ExecuteClick(const glm::vec2* clickPos)
{
	unsigned int ctr = 0;

	ctr += GUIElement::ExecuteClick(clickPos);

	//LOGI("CLICK");

	if (m_isEnabled)
	{
		for (std::vector<std::function<void(std::vector<void*>* params, const glm::vec2* clickPos)>>::iterator it = EventClick.begin(); it != EventClick.end(); ++it)
		{
			(*it)(&m_paramsClick, clickPos);
		}
	}

	if (m_isBlockable)
		++ctr;

	return ctr;
}

unsigned int GUIButton::ExecuteHold(const glm::vec2* clickPos)
{
	unsigned int ctr = 0;

	ctr += GUIElement::ExecuteHold(clickPos);

	if (m_isEnabled)
	{
		if (!isClickInProgress)
		{
			isClickInProgress = true;
			m_mesh->SetTextureID(m_textureClicked);
		}

		for (std::vector<std::function<void(std::vector<void*>* params, const glm::vec2* clickPos)>>::iterator it = EventHold.begin(); it != EventHold.end(); ++it)
		{
			(*it)(&m_paramsHold, clickPos);
		}
	}

	if (m_isBlockable)
		++ctr;

	return ctr;
}