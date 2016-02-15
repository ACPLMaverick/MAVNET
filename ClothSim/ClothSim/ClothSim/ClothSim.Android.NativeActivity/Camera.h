#pragma once

/*
	Camera class encapsulates all data necessary to compute view and projection matrices.
	It is a component itself, but it is not a part of an SimObject, it is assigned directly to the Scene and its m_obj pointer is by default null.
	If a SimObject pointer is given to the camera, the camera should follow that object, based on its Transform.
*/

#include "Common.h"
#include "Settings.h"
#include "Component.h"

#include <glm\glm\glm.hpp>
#include <glm\glm\gtx\transform.hpp>

#define CAMERA_MODE_ORTHO 0
#define CAMERA_MODE_PERSP 1

class Camera :
	public Component
{
protected:
	glm::mat4 m_viewMatrix;
	glm::mat4 m_projMatrix;
	glm::mat4 m_viewProjMatrix;

	glm::vec3 m_position; 
	glm::vec3 m_target;
	glm::vec3 m_direction;
	glm::vec3 m_up;
	glm::vec3 m_right;

	float m_fov, m_near, m_far;

	float m_distanceFromTarget;

	float m_windowWidth, m_windowHeight;

	bool m_viewDirty;
	bool m_projDirty;


	void CalculateProjection();
	void CalculateViewProjection();
public:
	Camera(SimObject*, float, float, float);
	Camera(const Camera*);
	~Camera();


	virtual unsigned int Initialize();
	virtual unsigned int Shutdown();

	virtual unsigned int Update();
	virtual unsigned int Draw();


	float GetFov();
	float GetNearPlane();
	float GetFarPlane();

	glm::mat4* GetViewMatrix();
	glm::mat4* GetProjMatrix();
	glm::mat4* GetViewProjMatrix();

	glm::vec3* GetPosition();
	glm::vec3* GetTarget();
	glm::vec3* GetDirection();
	glm::vec3* GetUp();
	glm::vec3* GetRight();


	void SetFov(float);
	void SetNearPlane(float);
	void SetFarPlane(float);

	void SetPosition(glm::vec3*);
	void SetTarget(glm::vec3*);
	void SetUp(glm::vec3*);

	void FlushDimensions();
};

