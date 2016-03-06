#pragma once

// this component's main purpose is to generate world matrix of an object
// each gameobject should have only one transform

#include "Component.h"

#include <d3dx9math.h>

class Transform :
	public Component
{
public:
#pragma region Enums

	enum ParentMode
	{
		DONT_CONSIDER,
		CONSIDER_NO_SCALE,
		CONSIDER_ALL
	};

#pragma endregion

private:

#pragma region Data

	D3DXMATRIX m_world;

	D3DXVECTOR3 m_position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 m_rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 m_scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	D3DXVECTOR2 m_position2D = D3DXVECTOR2(0.0f, 0.0f);
	float m_rotation2D = 0.0f;
	D3DXVECTOR2 m_scale2D = D3DXVECTOR2(0.0f, 0.0f);

#pragma endregion

#pragma region Variables

	ParentMode m_parentMode = ParentMode::CONSIDER_NO_SCALE;
	bool m_flagUpdateMatrix = false;

#pragma endregion

///////////////////////////

#pragma region Internal Functions

	inline void UpdateWorldMatrix();

#pragma endregion

public:

	Transform();
	~Transform();

	virtual void Initialize(
		uint32_t uid, GameObject* obj,
		D3DXVECTOR3* const position, D3DXVECTOR3* const rotation, D3DXVECTOR3* const scale,
		ParentMode mode = ParentMode::CONSIDER_NO_SCALE, std::string* name = nullptr);
	virtual void Shutdown();
	virtual void Update();
	virtual void Draw();

#pragma region Accessors

	const D3DXMATRIX* GetWorld() { return &m_world; }
	const D3DXVECTOR3* GetPosition() { return &m_position; }
	const D3DXVECTOR3* GetRotation() { return &m_rotation; }
	const D3DXVECTOR3* GetScale() { return &m_scale; }
	const D3DXVECTOR2* GetPosition2D() { return &m_position2D; }
	const float GetRotation2D() { return m_rotation2D; }
	const D3DXVECTOR2* GetScale2D() { return &m_scale2D; }
	ParentMode GetParentMode() { return m_parentMode; }

	void SetPosition(D3DXVECTOR3* const position) { m_position = *position; m_position2D.x = position->x; m_position2D.y = position->y; m_flagUpdateMatrix = true; }
	void SetRotation(D3DXVECTOR3* const rotation) { m_rotation = *rotation; m_rotation2D = rotation->z; m_flagUpdateMatrix = true; }
	void SetScale(D3DXVECTOR3* const scale) { m_scale = *scale; m_scale2D.x = scale->x; m_scale2D.y = scale->y; m_flagUpdateMatrix = true; }
	void SetPosition2D(D3DXVECTOR2* const position) { m_position = D3DXVECTOR3(position->x, position->y, 0.0f); m_position2D = *position; m_flagUpdateMatrix = true; }
	void SetRotation2D(float rotation) { m_rotation = D3DXVECTOR3(0.0f, 0.0f, rotation); m_rotation2D = rotation; m_flagUpdateMatrix = true; }
	void SetScale2D(D3DXVECTOR2* const scale) { m_scale = D3DXVECTOR3(scale->x, scale->y, 1.0f); m_scale2D = *scale, m_flagUpdateMatrix = true; }
	void SetParentMode(ParentMode mode) { m_parentMode = mode; m_flagUpdateMatrix = true; }

#pragma endregion

};

