#include "RotateMe.h"


RotateMe::RotateMe(SimObject* obj) : Component(obj)
{
}

RotateMe::RotateMe(const RotateMe* c) : Component(c)
{
}


RotateMe::~RotateMe()
{
}

unsigned int RotateMe::Initialize()
{
	m_rotation = glm::vec3(0.0f, 0.0f, 0.0f);
	return CS_ERR_NONE;
}

unsigned int RotateMe::Shutdown()
{
	return CS_ERR_NONE;
}



unsigned int RotateMe::Update()
{
	if (m_enabled)
	{
		if (m_obj->GetTransform() != nullptr)
		{
			m_obj->GetTransform()->SetRotation(&((*m_obj->GetTransform()->GetRotation()) + m_rotation));
		}
	}

	return CS_ERR_NONE;
}

unsigned int RotateMe::Draw()
{
	return CS_ERR_NONE;
}



void RotateMe::SetRotation(glm::vec3* rotation)
{
	m_rotation = *rotation;
}



glm::vec3* RotateMe::GetRotation()
{
	return &m_rotation;
}