#include "Transform.h"


Transform::Transform(SimObject* obj) : Component(obj)
{
	m_worldMatrix = nullptr;
	m_worldInverseTransposeMatrix = nullptr;
	m_lastWorldMatrix = nullptr;

	m_position = nullptr;
	m_rotation = nullptr;
	m_scale = nullptr;
}

Transform::Transform(const Transform* m) : Component(m)
{
}

Transform::~Transform()
{
}

unsigned int Transform::Initialize()
{
	m_worldMatrix = new glm::mat4;
	m_worldInverseTransposeMatrix = new glm::mat4;
	m_lastWorldMatrix = new glm::mat4;

	m_position = new glm::vec3;
	m_rotation = new glm::vec3;
	m_scale = new glm::vec3;

	for (int i = 0; i < 3; ++i)
	{
		(*m_position)[i] = 0.0f;
		(*m_rotation)[i] = 0.0f;
		(*m_scale)[i] = 1.0f;
	}

	CalculateWorldMatrix();

	return CS_ERR_NONE;
}

unsigned int Transform::Shutdown()
{
	if (m_worldMatrix != nullptr)
		delete m_worldMatrix;
	if (m_worldInverseTransposeMatrix != nullptr)
		delete m_worldInverseTransposeMatrix;
	if (m_position != nullptr)
		delete m_position;
	if (m_rotation != nullptr)
		delete m_rotation;
	if (m_scale != nullptr)
		delete m_scale;

	return CS_ERR_NONE;
}


unsigned int Transform::Update()
{
	if (m_isWorldMatrixDirty)
	{
		CalculateWorldMatrix();
		m_isWorldMatrixDirty = false;
	}

	return CS_ERR_NONE;
}

unsigned int Transform::Draw()
{
	return CS_ERR_NONE;
}



void Transform::CalculateWorldMatrix()
{
	(*m_lastWorldMatrix) = (*m_worldMatrix);
	(*m_worldMatrix) = glm::translate(*m_position) *
						glm::rotate((*m_rotation).x, glm::vec3(1.0f, 0.0f, 0.0f)) *
						glm::rotate((*m_rotation).y, glm::vec3(0.0f, 1.0f, 0.0f)) *
						glm::rotate((*m_rotation).z, glm::vec3(0.0f, 0.0f, 1.0f)) *
						glm::scale(*m_scale);

	(*m_worldInverseTransposeMatrix) = glm::transpose(glm::inverse(*m_worldMatrix));

	m_obj->UpdateCollidersFast();
}

bool Transform::HasMovedLastFrame()
{
	if ((*m_worldMatrix) == (*m_lastWorldMatrix))
	{
		return false;
	}
	else return true;
}

bool Transform::HasMovedLastFrameFast()
{
	return m_isWorldMatrixDirty;
}


glm::mat4* Transform::GetWorldMatrix()
{
	return m_worldMatrix;
}

glm::mat4* Transform::GetWorldInverseTransposeMatrix()
{
	return m_worldInverseTransposeMatrix;
}



glm::vec3* Transform::GetPosition()
{
	return m_position;
}

glm::vec3* Transform::GetRotation()
{
	return m_rotation;
}

glm::vec3* Transform::GetScale()
{
	return m_scale;
}



glm::vec3 Transform::GetPositionCopy()
{
	return (*m_position);
}

glm::vec3 Transform::GetRotationCopy()
{
	return (*m_rotation);
}

glm::vec3 Transform::GetScaleCopy()
{
	return (*m_scale);
}



void Transform::SetPosition(glm::vec3* newPos)
{
	for (int i = 0; i < 3; ++i)
	{
		(*m_position)[i] = (*newPos)[i];
	}

	m_isWorldMatrixDirty = true;
}

void Transform::SetRotation(glm::vec3* newRot)
{
	for (int i = 0; i < 3; ++i)
	{
		(*m_rotation)[i] = (*newRot)[i];
	}

	m_isWorldMatrixDirty = true;
}

void Transform::SetScale(glm::vec3* newScl)
{
	for (int i = 0; i < 3; ++i)
	{
		(*m_scale)[i] = (*newScl)[i];
	}

	m_isWorldMatrixDirty = true;
}
