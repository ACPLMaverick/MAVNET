#include "GUIElement.h"
#include "System.h"

GUIElement::GUIElement(const std::string* id)
{
	m_id = *id;

	m_rotation = 0.0f;
	m_position = glm::vec2(0.0f, 0.0f);
	m_scale = glm::vec2(1.0f, 1.0f);
	m_isEnabled = true;
	m_isVisible = true;
	m_isBlockable = false;
	m_isScaled = true;
	m_mesh = nullptr;

	GenerateTransformMatrix();
}

GUIElement::GUIElement(const GUIElement*)
{
}

GUIElement::~GUIElement()
{
}

unsigned int GUIElement::Initialize()
{
	return CS_ERR_NONE;
}

unsigned int GUIElement::Shutdown()
{
	return CS_ERR_NONE;
}



void GUIElement::GenerateTransformMatrix()
{
	float factorX = 1.0f;
	float factorY = 1.0f;
	float off = 0.0f;
	if (m_isScaled)
	{
		Engine* engine = System::GetInstance()->GetEngineData();
		float scrWidth = engine->width;
		float scrHeight = engine->height;
		factorX = (scrHeight / scrWidth);
		factorY = (scrWidth / scrHeight);

		float hsFactor = 1.0f / Renderer::GetInstance()->GetScreenRatio();
		if (scrWidth > scrHeight)
		{
			factorY = 1.0f / factorY * hsFactor;
			factorX *= hsFactor;
			off = 0.1f;
		}
	}


	m_transform = glm::translate(glm::vec3(m_position.x, m_position.y - off, 0.0f)) * glm::scale(glm::vec3(m_scale.x * factorX, m_scale.y * factorY, 0.0f));
	m_rot = glm::rotate(2.0f * m_rotation, glm::vec3(0.0f, 0.0f, 1.0f));
}


void GUIElement::CleanupAfterHold()
{
	for (std::map<std::string, GUIElement*>::iterator it = m_children.begin(); it != m_children.end(); ++it)
	{
		it->second->CleanupAfterHold();
	}
	if (isClickInProgress)
	{
		isClickInProgress = false;
		if(m_mesh != nullptr && m_textureIdle != nullptr)
			m_mesh->SetTextureID(m_textureIdle);
	}
}

unsigned int GUIElement::ExecuteClick(const glm::vec2* clickPos)
{
	unsigned int ctr = 0;

	if (m_isEnabled)
	{
		for (std::map<std::string, GUIElement*>::iterator it = m_children.begin(); it != m_children.end(); ++it)
		{
			if (InputManager::GetInstance()->GUIElementAreaInClick(it->second, clickPos))
			{
				ctr += (*it).second->ExecuteClick(clickPos);
			}
		}
	}

	if (m_isBlockable)
		++ctr;

	return ctr;
}

unsigned int GUIElement::ExecuteHold(const glm::vec2* clickPos)
{
	unsigned int ctr = 0;

	if (m_isEnabled)
	{
		for (std::map<std::string, GUIElement*>::iterator it = m_children.begin(); it != m_children.end(); ++it)
		{
			if (InputManager::GetInstance()->GUIElementAreaInClick(it->second, clickPos))
			{
				ctr += (*it).second->ExecuteHold(clickPos);
			}
		}
	}

	if (m_isBlockable)
		++ctr;

	return ctr;
}

unsigned int GUIElement::Update()
{
	unsigned int err = CS_ERR_NONE;

	if (m_isEnabled)
	{
		for (std::map<std::string, GUIElement*>::iterator it = m_children.begin(); it != m_children.end(); ++it)
		{
			err = (*it).second->Update();
			if (err != CS_ERR_NONE)
				return err;
		}
	}

	return err;
}

unsigned int GUIElement::Draw()
{
	unsigned int err = CS_ERR_NONE;

	if (m_isVisible)
	{
		for (std::map<std::string, GUIElement*>::iterator it = m_children.begin(); it != m_children.end(); ++it)
		{
			err = (*it).second->Draw();
			if (err != CS_ERR_NONE)
				return err;
		}

		if (m_mesh != nullptr)
			m_mesh->Draw();
	}

	return err;
}

void GUIElement::SetPosition(glm::vec2 pos)
{
	m_position = pos;
	GenerateTransformMatrix();
}

void GUIElement::SetScale(glm::vec2 scl)
{
	m_scale = scl;
	GenerateTransformMatrix();
}

void GUIElement::SetRotation(float r)
{
	m_rotation = r;
}

void GUIElement::SetEnabled(bool val)
{
	m_isEnabled = val;
}

void GUIElement::SetVisible(bool val)
{
	m_isVisible = val;
}

void GUIElement::SetBlockable(bool val)
{
	m_isBlockable = val;
}

void GUIElement::SetScaled(bool val)
{
	m_isScaled = val;
}

void GUIElement::AddChild(GUIElement * ge)
{
	m_children.emplace(*ge->GetID(), ge);
}

GUIElement * GUIElement::GetChild(const std::string * id)
{
	GUIElement* ret = nullptr;
	std::map<std::string, GUIElement*>::iterator it = m_children.find(*id);
	if (it != m_children.end())
		ret = it->second;
	return ret;
}

GUIElement * GUIElement::RemoveChild(const std::string * id)
{
	GUIElement* ret = nullptr;
	std::map<std::string, GUIElement*>::iterator it = m_children.find(*id);
	if (it != m_children.end())
	{
		ret = it->second;
		m_children.erase(*id);
	}
		
	return ret;
}



std::string* GUIElement::GetID()
{
	return &m_id;
}

glm::mat4* GUIElement::GetTransformMatrix()
{
	return &m_transform;
}

glm::mat4 * GUIElement::GetRotationMatrix()
{
	return &m_rot;
}

glm::vec2 GUIElement::GetPosition()
{
	return m_position;
}

glm::vec2 GUIElement::GetScale()
{
	return m_scale;
}

float GUIElement::GetRotation()
{
	return m_rotation;
}

bool GUIElement::GetHoldInProgress()
{
	return isClickInProgress;
}

bool GUIElement::GetEnabled()
{
	return m_isEnabled;
}

bool GUIElement::GetVisible()
{
	return m_isVisible;
}

bool GUIElement::GetBlockable()
{
	return m_isBlockable;
}

bool GUIElement::GetScaled()
{
	return m_isScaled;
}

void GUIElement::FlushDimensions()
{
	GenerateTransformMatrix();
	for (std::map<std::string, GUIElement*>::iterator it = m_children.begin(); it != m_children.end(); ++it)
	{
		(*it).second->FlushDimensions();
	}
}
