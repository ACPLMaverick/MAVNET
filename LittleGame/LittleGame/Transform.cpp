#include "stdafx.h"
#include "Transform.h"
#include "GameObject.h"

Transform::Transform()
{
}


Transform::~Transform()
{
}

void Transform::Initialize(uint32_t uid, GameObject* obj, D3DXVECTOR3* const position, D3DXVECTOR3* const rotation, D3DXVECTOR3* const scale,
	ParentMode mode, std::string* name)
{
	SetPosition(position);
	SetRotation(rotation);
	SetScale(scale);

	m_parentMode = mode;
	Component::Initialize(uid, obj, name);

	UpdateWorldMatrix();
}

void Transform::Shutdown()
{
}

void Transform::Update()
{
	// recalculate world matrix if necessary - once per frame
	if (m_flagUpdateMatrix)
	{
		m_flagUpdateMatrix = false;
		UpdateWorldMatrix();
	}
}

void Transform::Draw()
{
	// no drawing
}

inline void Transform::UpdateWorldMatrix()
{
	D3DXMATRIX mPos, mRot, mScl;
	D3DXMatrixTranslation(&mPos, m_position.x, m_position.y, m_position.z);
	D3DXMatrixRotationYawPitchRoll(&mRot, m_rotation.y, m_rotation.x, m_rotation.z);
	D3DXMatrixScaling(&mScl, m_scale.x, m_scale.y, m_scale.z);

	m_world = mPos * mRot * mScl;

	if (m_obj->GetParent() != nullptr && m_obj->GetParent()->GetTransform() != nullptr)
	{
		if (m_parentMode == ParentMode::CONSIDER_ALL)
		{
			m_world = *m_obj->GetParent()->GetTransform()->GetWorld() * m_world;
		}
		else if (m_parentMode == ParentMode::CONSIDER_NO_SCALE)
		{
			D3DXMATRIX pPos, pRot;
			const D3DXVECTOR3* pPosition = m_obj->GetParent()->GetTransform()->GetPosition();
			const D3DXVECTOR3* pRotation = m_obj->GetParent()->GetTransform()->GetRotation();
			D3DXMatrixTranslation(&pPos, pPosition->x, pPosition->y, pPosition->z);
			D3DXMatrixRotationYawPitchRoll(&pRot, pRotation->y, pRotation->x, pRotation->z);

			m_world = pPos * pRot * m_world;
		}
	}
}
