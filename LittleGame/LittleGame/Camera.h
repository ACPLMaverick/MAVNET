 #pragma once

// a virtual camera

#include "GameObject.h"

#include <vector>
#include <d3dx9math.h>

class Camera : public GameObject
{
protected:

#pragma region Params

	float_t m_width;
	float_t m_height;
	float_t m_fov;
	float_t m_near;
	float_t m_far;
	bool m_ortho = false;

#pragma endregion

#pragma region Data

	D3DXMATRIX m_viewProj;
	D3DXMATRIX m_view;
	D3DXMATRIX m_proj;

	D3DXVECTOR3 m_position = D3DXVECTOR3(0.0f, 0.0f, 10.0f);
	D3DXVECTOR3 m_target = D3DXVECTOR3(0.0f, 0.0f, 0.0f);;
	D3DXVECTOR3 m_up = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	D3DXVECTOR3 m_direction = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	D3DXVECTOR3 m_right = D3DXVECTOR3(1.0f, 0.0f, 0.0f);

#pragma endregion

#pragma region FunctionsInternal

	inline void UpdateViewProjMatrix();
	inline void UpdateViewMatrixAndVectorData();
	inline void UpdateProjMatrix();

#pragma endregion

public:
	Camera();
	~Camera();

	void Initialize(
		uint32_t uid,
		float_t width,
		float_t height,
		float_t fov,
		float_t cn,
		float_t cf,
		bool ortho = false,
		const std::string* name = nullptr
		);
	void Initialize(
		const D3DXVECTOR3* pos,
		const D3DXVECTOR3* tgt,
		const D3DXVECTOR3* up,
		uint32_t uid,
		float_t width,
		float_t height,
		float_t fov,
		float_t cn,
		float_t cf,
		bool ortho = false,
		const std::string* name = nullptr
		);
	void Shutdown();
	virtual void Update();

#pragma region Accessors

	float_t GetWidth() { return m_width; }
	float_t GetHeight() { return m_height; }
	float_t GetFov() { return m_fov; }
	float_t GetNear() { return m_near; }
	float_t GetFar() { return m_far; }
	bool GetOrtho() { return m_ortho; }

	const D3DXVECTOR3* GetPosition() { return &m_position; }
	const D3DXVECTOR3* GetTarget() { return &m_target; }
	const D3DXVECTOR3* GetUp() { return &m_up; }
	const D3DXVECTOR3* GetDirection() { return &m_direction; }
	const D3DXVECTOR3* GetRight() { return &m_right; }
	const D3DXMATRIX* GetViewProj() { return &m_viewProj; }
	const D3DXMATRIX* GetView() { return &m_view; }
	const D3DXMATRIX* GetProj() { return &m_proj; }

	void SetWidth(float_t width) { m_width = width; UpdateProjMatrix(); }
	void SetHeight(float_t height) { m_height = height; UpdateProjMatrix(); }
	void SetFov(float_t fov) { m_fov = fov; UpdateProjMatrix(); }
	void SetNear(float_t cn) { m_near = cn; UpdateProjMatrix(); }
	void SetFar(float_t cf) { m_far = cf; UpdateProjMatrix(); }
	void SetOrtho(bool ortho) { m_ortho = ortho; UpdateProjMatrix(); }

	void SetPosition(const D3DXVECTOR3* pos) { m_position = *pos; UpdateViewMatrixAndVectorData(); }
	void SetTarget(const D3DXVECTOR3* tgt) { m_target = *tgt; UpdateViewMatrixAndVectorData(); }
	void SetUp(const D3DXVECTOR3* up) { m_up = *up; D3DXVec3Normalize(&m_up, &m_up);
										UpdateViewMatrixAndVectorData(); }

#pragma endregion

};

