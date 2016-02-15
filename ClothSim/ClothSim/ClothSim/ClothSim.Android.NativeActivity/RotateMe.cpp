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
			glm::vec3 tempRot = ((*m_obj->GetTransform()->GetRotation()) + m_rotation * (float)Timer::GetInstance()->GetDeltaTime());
			m_obj->GetTransform()->SetRotation(&tempRot);
			//LOGI("RotateMe: %f, %f, %f", m_obj->GetTransform()->GetRotation()->x, m_obj->GetTransform()->GetRotation()->y, m_obj->GetTransform()->GetRotation()->z);
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