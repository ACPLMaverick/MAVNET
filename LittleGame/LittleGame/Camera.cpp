#include "stdafx.h"
#include "Camera.h"


Camera::Camera()
{
}


Camera::~Camera()
{
}

void Camera::Initialize(uint32_t uid, const std::string* name, float_t width, float_t height, float_t fov, float_t cn, float_t cf, bool ortho)
{
	m_uID = uid;
	m_name = *name;
	m_width = width;
	m_height = height;
	m_fov = fov;
	m_near = cn;
	m_far = cf;
	m_ortho = ortho;

	UpdateViewMatrixAndVectorData();
	UpdateProjMatrix();
	UpdateViewProjMatrix();
}

void Camera::Initialize(const D3DXVECTOR3 * pos, const D3DXVECTOR3 * tgt, const D3DXVECTOR3 * up, uint32_t uid,
	const std::string* name, float_t width, float_t height, float_t fov, float_t cn, float_t cf, bool ortho)
{
	m_position = *pos;
	m_target = *tgt;
	m_up = *up;

	Initialize(uid, name, width, height, fov, cn, cf, ortho);
}

void Camera::Shutdown()
{
	// nothing
}

void Camera::Update()
{
	// nothing here is done as camera is updated while changing its values
}

inline void Camera::UpdateViewProjMatrix()
{
	m_viewProj = m_view * m_proj;
}

inline void Camera::UpdateViewMatrixAndVectorData()
{
	D3DXMatrixLookAtLH(&m_view, &m_position, &m_target, &m_up);

	m_direction = m_target - m_position;
	D3DXVec3Normalize(&m_direction, &m_direction);
	D3DXVec3Cross(&m_right, &m_direction, &m_up);
	D3DXVec3Normalize(&m_right, &m_right);

	UpdateViewProjMatrix();
}

inline void Camera::UpdateProjMatrix()
{
	if (m_ortho)
	{
		D3DXMatrixOrthoLH(&m_proj, m_width, m_height, m_near, m_far);
	}
	else
	{
		D3DXMatrixPerspectiveFovLH(&m_proj, m_fov, (m_width / m_height), m_near, m_far);
	}
	UpdateViewProjMatrix();
}
