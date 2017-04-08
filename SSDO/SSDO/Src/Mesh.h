#pragma once

#include "stdafx.h"
#include "Buffer.h"

#include <vector>

class Camera;
class Material;

class Mesh
{
	friend class Material;
protected:

#pragma region Structs Protected

	struct Triangle
	{
		union
		{
			struct
			{
				int16_t _x;
				int16_t _y;
				int16_t _z;
			};
			int16_t tab[3];
		};

		Triangle()
		{
			ZeroMemory(&tab, 3 * sizeof(int16_t));
		}

		Triangle(int16_t x, int16_t y, int16_t z) :
			_x(x),
			_y(y),
			_z(z)
		{
		}

		int16_t operator[](int64_t index) const
		{
			ASSERT(index >= 0 && index < 3);
			return tab[index];
		}

		int16_t& operator[](int64_t index)
		{
			ASSERT(index >= 0 && index < 3);
			return tab[index];
		}
	};

#pragma endregion

#pragma region Protected

	XMFLOAT4X4 _matWorld;
	XMFLOAT4X4 _matWorldInvTransp;

	XMFLOAT4 _rotation;
	XMFLOAT3 _scale;
	XMFLOAT3 _position;

	vector<XMFLOAT3> _vPositions;
	vector<XMFLOAT3> _vNormals;
	vector<XMFLOAT2> _vUvs;
	vector<Triangle> _indices;

	ID3D11Buffer* _fPositions = nullptr;
	ID3D11Buffer* _fNormals = nullptr;
	ID3D11Buffer* _fUvs = nullptr;
	ID3D11Buffer* _fIndices = nullptr;

	const Material& _myMaterial;

	bool _bNeedCreateWorldMatrix = false;

#pragma endregion

#pragma region Functions Protected

	inline bool Float3Equal(const XMFLOAT3& lhs, const XMFLOAT3& rhs) const;
	inline bool Float2Equal(const XMFLOAT2& lhs, const XMFLOAT2& rhs) const;

	inline void InitBuffers();
	inline XMFLOAT4 QuaterionFromEuler(const XMFLOAT3& euler) const;
	inline XMFLOAT3 EulerFromQuaternion(const XMFLOAT4& quat) const;
	inline void CreateWorldMatrix();

#pragma endregion

public:

#pragma region Public Struct

	struct VertexDescSimple
	{
		XMFLOAT3 Position;
		XMFLOAT3 Normal;
		XMFLOAT2 Uv;
	};

#pragma endregion

#pragma region Functions Public

	// Creates test box
	Mesh(const Material& mat,
		const XMFLOAT3& pos = XMFLOAT3(0.0f, 0.0f, 0.0f), 
		const XMFLOAT3& rot = XMFLOAT3(0.0f, 0.0f, 0.0f),
		const XMFLOAT3& scl = XMFLOAT3(1.0f, 1.0f, 1.0f));
	// Loads mesh from file
	Mesh(const std::wstring& filePath,
		const Material& mat,
		const XMFLOAT3& pos = XMFLOAT3(0.0f, 0.0f, 0.0f),
		const XMFLOAT3& rot = XMFLOAT3(0.0f, 0.0f, 0.0f),
		const XMFLOAT3& scl = XMFLOAT3(1.0f, 1.0f, 1.0f));
	~Mesh();

	void Update();
	void Draw(const Camera& camera) const;

#pragma region GettersSetters

	inline const XMFLOAT3& GetPosition() const { return _position; }
	inline XMFLOAT3 GetRotation() const { return EulerFromQuaternion(_rotation); }
	inline const XMFLOAT3& GetScale() const { return _scale; }
	
	inline void SetPosition(const XMFLOAT3& pos) { _position = pos; _bNeedCreateWorldMatrix = true; }
	inline void SetRotation(const XMFLOAT3& rot) { _rotation = QuaterionFromEuler(rot); _bNeedCreateWorldMatrix = true; }
	inline void SetScale(const XMFLOAT3& scl) { _scale = scl; _bNeedCreateWorldMatrix = true; }

#pragma endregion

#pragma endregion
};

