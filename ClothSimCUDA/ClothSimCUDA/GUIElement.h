#pragma once

/*
	An abstract representation of an element of a GUI.
*/

#include "Common.h"

#include <string>
#include <glm\glm\glm.hpp>
#include <glm\glm\gtx\transform.hpp>

class GUIElement
{
protected:
	std::string m_id;
	glm::mat4 m_transform;

	glm::vec2 m_position;
	glm::vec2 m_scale;

	void GenerateTransformMatrix();
public:
	GUIElement(const std::string*);
	GUIElement(const GUIElement*);
	~GUIElement();

	virtual unsigned int Initialize() = 0;
	virtual unsigned int Shutdown() = 0;

	virtual unsigned int Update() = 0;
	virtual unsigned int Draw() = 0;

	void SetPosition(glm::vec2);
	void SetScale(glm::vec2);

	std::string* GetID();
	glm::mat4* GetTransformMatrix();
	glm::vec2 GetPosition();
	glm::vec2 GetScale();
};

