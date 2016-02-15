#include "GUIElement.h"


GUIElement::GUIElement(const std::string* id)
{
	m_id = *id;

	m_position = glm::vec2(0.0f, 0.0f);
	m_scale = glm::vec2(1.0f, 1.0f);

	GenerateTransformMatrix();
}

GUIElement::GUIElement(const GUIElement*)
{
}

GUIElement::~GUIElement()
{
}



void GUIElement::GenerateTransformMatrix()
{
	m_transform = glm::translate(glm::vec3(m_position.x, m_position.y, 0.0f)) * glm::scale(glm::vec3(m_scale.x, m_scale.y, 0.0f));
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



std::string* GUIElement::GetID()
{
	return &m_id;
}

glm::mat4* GUIElement::GetTransformMatrix()
{
	return &m_transform;
}

glm::vec2 GUIElement::GetPosition()
{
	return m_position;
}

glm::vec2 GUIElement::GetScale()
{
	return m_scale;
}