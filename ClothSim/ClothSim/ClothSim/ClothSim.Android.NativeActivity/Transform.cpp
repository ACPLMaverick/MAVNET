#include "Transform.h"
#include "Collider.h"

Transform::Transform(SimObject* obj) : Component(obj)
{
}

Transform::Transform(const Transform* m) : Component(m)
{
}

Transform::~Transform()
{
}

unsigned int Transform::Initialize()
{
	m_worldMatrix = glm::mat4();
	m_worldInverseTransposeMatrix = glm::mat4();
	m_lastWorldMatrix = glm::mat4();

	m_position = glm::vec3();
	m_rotation = glm::vec3();
	m_scale = glm::vec3(1.0f, 1.0f, 1.0f);

	CalculateWorldMatrix();

	return CS_ERR_NONE;
}

unsigned int Transform::Shutdown()
{
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
	(m_lastWorldMatrix) = (m_worldMatrix);
	(m_worldMatrix) = glm::translate(m_position) *
						glm::rotate((m_rotation).x, glm::vec3(1.0f, 0.0f, 0.0f)) *
						glm::rotate((m_rotation).y, glm::vec3(0.0f, 1.0f, 0.0f)) *
						glm::rotate((m_rotation).z, glm::vec3(0.0f, 0.0f, 1.0f)) *
						glm::scale(m_scale);

	(m_worldInverseTransposeMatrix) = glm::transpose(glm::inverse(m_worldMatrix));

	m_obj->UpdateCollidersFast();
}

bool Transform::HasMovedLastFrame()
{
	if ((m_worldMatrix) == (m_lastWorldMatrix))
	{
		return false;
	}
	else return true;
}

bool Transform::HasMovedLastFrameFast()
{
	return m_isWorldMatrixDirty;
}

void Transform::CheckCollisions()
{
}


glm::mat4* Transform::GetWorldMatrix()
{
	return &m_worldMatrix;
}

glm::mat4* Transform::GetWorldInverseTransposeMatrix()
{
	return &m_worldInverseTransposeMatrix;
}



glm::vec3* Transform::GetPosition()
{
	return &m_position;
}

glm::vec3* Transform::GetRotation()
{
	return &m_rotation;
}

glm::vec3* Transform::GetScale()
{
	return &m_scale;
}


void Transform::SetPosition(glm::vec3* newPos)
{
	m_position = *newPos;

	CalculateWorldMatrix();

	m_obj->UpdateColliders();
	vector<Collider*>* cols = m_obj->GetColliders();
	CollisonTestResult tR;
	for (std::vector<Collider*>::iterator it = cols->begin(); it != cols->end(); ++it)
	{
		PhysicsManager::GetInstance()->CollisionCheck(*it, &tR);
	}

	if (tR.ifCollision)
	{
		m_position = m_position + tR.colVector;
		m_obj->UpdateColliders();
	}
}

void Transform::SetRotation(glm::vec3* newRot)
{
	m_rotation = *newRot;

	m_isWorldMatrixDirty = true;
}

void Transform::SetScale(glm::vec3* newScl)
{
	m_scale = *newScl;

	m_isWorldMatrixDirty = true;
}
