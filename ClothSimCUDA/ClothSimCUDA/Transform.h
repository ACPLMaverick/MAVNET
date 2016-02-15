#pragma once

/*
	This class encapsulates all data needed to generate world matrix for specified SimObject
*/

#include "SimObject.h"
#include "Component.h"

#include <glm\glm\glm.hpp>
#include <glm\glm\gtx\transform.hpp>

class SimObject;

class Transform : public Component
{
private:
	glm::mat4* m_worldMatrix;
	glm::mat4* m_worldInverseTransposeMatrix;
	glm::mat4* m_lastWorldMatrix;

	glm::vec3* m_position;
	glm::vec3* m_rotation;
	glm::vec3* m_scale;

	bool m_isWorldMatrixDirty;

	void CalculateWorldMatrix();
public:
	Transform(SimObject*);
	Transform(const Transform*);
	~Transform();

	virtual unsigned int Initialize();
	virtual unsigned int Shutdown();

	virtual unsigned int Update();
	virtual unsigned int Draw();

	bool HasMovedLastFrame();
	bool HasMovedLastFrameFast();

	glm::mat4* GetWorldMatrix();
	glm::mat4* GetWorldInverseTransposeMatrix();

	glm::vec3* GetPosition();
	glm::vec3* GetRotation();
	glm::vec3* GetScale();

	glm::vec3 GetPositionCopy();
	glm::vec3 GetRotationCopy();
	glm::vec3 GetScaleCopy();

	void SetPosition(glm::vec3*);
	void SetRotation(glm::vec3*);
	void SetScale(glm::vec3*);
};

