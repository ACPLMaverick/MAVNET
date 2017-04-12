#pragma once

#include "GlobalDefines.h"
#include "Material.h"

class Camera;
class Mesh;

class Object
{
protected:

#pragma region Protected

	XMFLOAT4X4 _matWorld;
	XMFLOAT4X4 _matWorldInvTransp;

	XMFLOAT3 _rotation;
	XMFLOAT3 _scale;
	XMFLOAT3 _position;

	const Mesh& _mesh;
	const Material& _material;

	bool _bNeedCreateWorldMatrix = false;

#pragma endregion

#pragma region Functions Protected

	inline void CreateWorldMatrix();

#pragma endregion

public:

#pragma region Functions Public

	Object(const Mesh& mesh, const Material& material,
		const XMFLOAT3& pos = XMFLOAT3(0.0f, 0.0f, 0.0f),
		const XMFLOAT3& rot = XMFLOAT3(0.0f, 0.0f, 0.0f),
		const XMFLOAT3& scl = XMFLOAT3(1.0f, 1.0f, 1.0f));
	~Object();

	virtual void Update();
	virtual inline void Draw(const Camera& camera) const { _material.DrawMesh(*this, camera, _mesh); }

#pragma region GettersSetters

	inline const XMFLOAT3& GetPosition() const { return _position; }
	inline XMFLOAT3 GetRotation() const { return _rotation; }
	inline const XMFLOAT3& GetScale() const { return _scale; }
	inline const XMFLOAT4X4& GetWorldMatrix() const { return _matWorld; }
	inline const XMFLOAT4X4& GetWorldInvTransMatrix() const { return _matWorldInvTransp; }

	inline void SetPosition(const XMFLOAT3& pos) { _position = pos; _bNeedCreateWorldMatrix = true; }
	inline void SetRotation(const XMFLOAT3& rot) { _rotation = rot; _bNeedCreateWorldMatrix = true; }
	inline void SetScale(const XMFLOAT3& scl) { _scale = scl; _bNeedCreateWorldMatrix = true; }

#pragma endregion

#pragma endregion
};

