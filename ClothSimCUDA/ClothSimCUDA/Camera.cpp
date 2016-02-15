#include "Camera.h"


Camera::Camera(SimObject* obj, float fov, float near, float far) : Component(obj)
{
	m_fov = fov;
	m_near = near;
	m_far = far;

	m_viewMatrix = m_projMatrix = m_viewProjMatrix = nullptr;

	m_position = m_target = m_direction = m_up = m_right = nullptr;

	m_distanceFromTarget = 0;
}

Camera::Camera(const Camera* m) : Component(m)
{
}

Camera::~Camera()
{
}

unsigned int Camera::Initialize()
{
	m_viewMatrix = new glm::mat4();
	m_projMatrix = new glm::mat4();
	m_viewProjMatrix = new glm::mat4();
	
	m_position = new glm::vec3(0.0f, 0.0f, 10.0f);
	m_target = new glm::vec3();
	m_up = new glm::vec3(0.0f, 1.0f, 0.0f);
	
	m_direction = new glm::vec3();
	m_right = new glm::vec3();

	CalculateProjection();
	CalculateViewProjection();

	return CS_ERR_NONE;
}

unsigned int Camera::Shutdown()
{
	if (m_viewMatrix != nullptr)
		delete m_viewMatrix;
	if (m_projMatrix != nullptr)
		delete m_projMatrix;
	if (m_viewProjMatrix != nullptr)
		delete m_viewProjMatrix;
	if (m_position != nullptr)
		delete m_position;
	if (m_target != nullptr)
		delete m_target;
	if (m_up != nullptr)
		delete m_up;
	if (m_direction != nullptr)
		delete m_direction;
	if (m_right != nullptr)
		delete m_right;

	return CS_ERR_NONE;
}


unsigned int Camera::Update()
{
	if (m_projDirty)
	{
		CalculateProjection();
		m_projDirty = false;
	}
	if (m_viewDirty)
	{
		CalculateViewProjection();
		m_viewDirty = false;
	}

	return CS_ERR_NONE;
}

unsigned int Camera::Draw()
{
	return CS_ERR_NONE;
}


void Camera::CalculateProjection()
{
	(*m_projMatrix) = glm::perspectiveFov(m_fov, (float)CSSET_WINDOW_WIDTH, (float)CSSET_WINDOW_HEIGHT, m_near, m_far);
}

void Camera::CalculateViewProjection()
{
	(*m_viewMatrix) = glm::lookAt(*m_position, *m_target, *m_up);
	(*m_viewProjMatrix) = (*m_projMatrix) * (*m_viewMatrix);

	(*m_direction) = glm::normalize((*m_target) - (*m_position));
	(*m_right) = glm::normalize(glm::cross(*m_direction, *m_up));
}



float Camera::GetFov()
{
	return m_fov;
}

float Camera::GetNearPlane()
{
	return m_near;
}

float Camera::GetFarPlane()
{
	return m_far;
}



glm::mat4* Camera::GetViewMatrix()
{
	return m_viewMatrix;
}

glm::mat4* Camera::GetProjMatrix()
{
	return m_projMatrix;
}

glm::mat4* Camera::GetViewProjMatrix()
{
	return m_viewProjMatrix;
}



glm::vec3* Camera::GetPosition()
{
	return m_position;
}

glm::vec3* Camera::GetTarget()
{
	return m_target;
}

glm::vec3* Camera::GetDirection()
{
	return m_direction;
}

glm::vec3* Camera::GetUp()
{
	return m_up;
}

glm::vec3* Camera::GetRight()
{
	return m_right;
}



void Camera::SetFov(float fov)
{
	m_fov = fov;
	m_projDirty = true;
}

void Camera::SetNearPlane(float near)
{
	m_near = near;
	m_projDirty = true;
}

void Camera::SetFarPlane(float far)
{
	m_far = far;
	m_projDirty = true;
}



void Camera::SetPosition(glm::vec3* pos)
{
	(*m_position) = (*pos);
	m_viewDirty = true;
}

void Camera::SetTarget(glm::vec3* tgt)
{
	(*m_target) = (*tgt);
	m_viewDirty = true;
}

void Camera::SetUp(glm::vec3* up)
{
	(*m_up) = (*up);
	m_viewDirty = true;
}
