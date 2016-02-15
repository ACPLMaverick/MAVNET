#pragma once

/*
	Rotates SimObject about given rotation every frame.
*/

#include "Common.h"
#include "Component.h"
#include "SimObject.h"

#include <glm\glm\glm.hpp>

class RotateMe :
	public Component
{
private:
	glm::vec3 m_rotation;
public:
	RotateMe(SimObject* obj);
	RotateMe(const RotateMe*);
	~RotateMe();

	virtual unsigned int Initialize();
	virtual unsigned int Shutdown();

	virtual unsigned int Update();
	virtual unsigned int Draw();

	void SetRotation(glm::vec3*);

	glm::vec3* GetRotation();
};

