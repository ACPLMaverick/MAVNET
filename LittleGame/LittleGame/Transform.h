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
		const D3DXVECTOR3* position, const D3DXVECTOR3* rotation, const D3DXVECTOR3* scale,
		ParentMode mode = ParentMode::CONSIDER_NO_SCALE, std::string* name = nullptr);
	virtual void Shutdown();
	virtual void Update();
	virtual void Draw();

#pragma region Accessors

	const D3DXMATRIX* GetWorld() { return &m_world; }
	const D3DXVECTOR3* GetPosition() { return &m_position; }
	const D3DXVECTOR3* GetRotation() { return &m_rotation; }
	const D3DXVECTOR3* GetScale() { return &m_scale; }
	ParentMode GetParentMode() { return m_parentMode; }

	void SetPosition(D3DXVECTOR3* const position) { m_position = *position; m_flagUpdateMatrix = true; }
	void SetRotation(D3DXVECTOR3* const rotation) { m_rotation = *rotation; m_flagUpdateMatrix = true; }
	void SetScale(D3DXVECTOR3* const scale) { m_scale = *scale; m_flagUpdateMatrix = true; }
	void SetParentMode(ParentMode mode) { m_parentMode = mode; m_flagUpdateMatrix = true; }

#pragma endregion

};

