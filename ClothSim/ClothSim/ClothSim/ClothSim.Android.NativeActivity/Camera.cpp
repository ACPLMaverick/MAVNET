#include "Camera.h"
#include "System.h"

Camera::Camera(SimObject* obj, float fov, float near, float far) : Component(obj)
{
	m_fov = fov;
	m_near = near;
	m_far = far;

	m_viewMatrix = m_projMatrix = m_viewProjMatrix = glm::mat4();

	m_position = m_target = m_direction = m_up = m_right = glm::vec3();

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
	m_viewMatrix = glm::mat4();
	m_projMatrix = glm::mat4();
	m_viewProjMatrix = glm::mat4();
	
	m_position = glm::vec3(0.0f, 0.0f, 10.0f);
	m_target = glm::vec3();
	m_up = glm::vec3(0.0f, 1.0f, 0.0f);
	
	m_direction = glm::vec3();
	m_right = glm::vec3();

	m_windowWidth = CSSET_WINDOW_WIDTH_DEFAULT;
	m_windowHeight = CSSET_WINDOW_HEIGHT_DEFAULT;

	// now try to read real values
	Engine* engine = System::GetInstance()->GetEngineData();
	m_windowWidth = (float)engine->width;
	m_windowHeight = (float)engine->height;

	CalculateProjection();
	CalculateViewProjection();

	return CS_ERR_NONE;
}

unsigned int Camera::Shutdown()
{
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
	m_projMatrix = glm::perspectiveFov(m_fov, m_windowWidth, m_windowHeight, m_near, m_far);
}

void Camera::CalculateViewProjection()
{
	m_viewMatrix = glm::lookAt(m_position, m_target, m_up);
	m_viewProjMatrix = m_projMatrix * m_viewMatrix;

	m_direction = glm::normalize(m_target - m_position);
	m_right = glm::normalize(glm::cross(m_direction, m_up));
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
	return &m_viewMatrix;
}

glm::mat4* Camera::GetProjMatrix()
{
	return &m_projMatrix;
}

glm::mat4* Camera::GetViewProjMatrix()
{
	return &m_viewProjMatrix;
}



glm::vec3* Camera::GetPosition()
{
	return &m_position;
}

glm::vec3* Camera::GetTarget()
{
	return &m_target;
}

glm::vec3* Camera::GetDirection()
{
	return &m_direction;
}

glm::vec3* Camera::GetUp()
{
	return &m_up;
}

glm::vec3* Camera::GetRight()
{
	return &m_right;
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
	m_position = *pos;
	m_viewDirty = true;
}

void Camera::SetTarget(glm::vec3* tgt)
{
	m_target = *tgt;
	m_viewDirty = true;
}

void Camera::SetUp(glm::vec3* up)
{
	m_up = *up;
	m_viewDirty = true;
}

void Camera::FlushDimensions()
{
	Engine* engine = System::GetInstance()->GetEngineData();
	m_windowWidth = (float)engine->width;
	m_windowHeight = (float)engine->height;
	CalculateProjection();
	CalculateViewProjection();
}
