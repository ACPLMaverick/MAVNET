#pragma once

#include "stdafx.h"

class Scene;

class Camera
{
protected:

#pragma region Protected

	XMFLOAT4X4 _matViewProj;
	XMFLOAT4X4 _matView;
	XMFLOAT4X4 _matProj;

	XMFLOAT3 _position;
	XMFLOAT3 _target;
	XMFLOAT3 _up;

	XMFLOAT3 _direction;
	XMFLOAT3 _right;

	float _fov;
	float _near;
	float _far;

	bool _bNeedUpdateView = false;
	bool _bNeedUpdateProj = false;

#pragma endregion

#pragma region Functions Protected

	inline void UpdateView();
	inline void UpdateProjection();
	inline void MergeMatrices();

#pragma endregion

public:

#pragma region Functions Public

	Camera(	const XMFLOAT3& position = XMFLOAT3(5.0f, 5.0f, 5.0f),
			const XMFLOAT3& target = XMFLOAT3(0.0f, 0.0f, 0.0f),
			const XMFLOAT3& up = XMFLOAT3(0.0f, 1.0f, 0.0f),
			float fov = 0.7f,
			float near = 0.1f,
			float far = 1000.0f);
	~Camera();

	void Update();
	void Draw(const Scene& scene) const;

#pragma region GettersSetters

	inline void SetPosition(const XMFLOAT3& position) { _position = position; _bNeedUpdateView = true; }
	inline void SetTarget(const XMFLOAT3& target) { _target = target; _bNeedUpdateView = true; }
	inline void SetUp(const XMFLOAT3& up) { _up = up; _bNeedUpdateView = true; }
	inline void SetDirection(const XMFLOAT3& direction);
	inline void SetFOV(float fov) { _fov = fov; _bNeedUpdateProj = true; }
	void SetNear(float nearPlane) { _near = nearPlane; _bNeedUpdateProj = true; }
	void SetFar(float farPlane) { _far = farPlane; _bNeedUpdateProj = true; }

	inline const XMFLOAT4X4& GetMatViewProj() const { return _matViewProj; }
	inline const XMFLOAT4X4& GetMatView() const { return _matView; }
	inline const XMFLOAT4X4& GetMatProj() const { return _matProj; }
	inline const XMFLOAT3& GetPosition() const { return _position; }
	inline const XMFLOAT3& GetTarget() const { return _target; }
	inline const XMFLOAT3& GetUp() const { return _up; }
	inline const XMFLOAT3& GetDirection() const { return _direction; }
	inline const XMFLOAT3& GetRight() const { return _right; }
	inline const float& GetFOV() const { return _fov; }
	inline const float& GetNearPlane() const { return _near; }
	inline const float& GetFarPlane() const { return _far; }

#pragma endregion

#pragma endregion

};

