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

	Buffer<XMFLOAT3> _vPositions;
	Buffer<XMFLOAT3> _vNormals;
	Buffer<XMFLOAT2> _vUvs;
	Buffer<Triangle> _indices;

	ID3D11Buffer* _fPositions = nullptr;
	ID3D11Buffer* _fNormals = nullptr;
	ID3D11Buffer* _fUvs = nullptr;
	ID3D11Buffer* _fIndices = nullptr;

#pragma endregion

#pragma region Functions Protected

	inline bool Float3Equal(const XMFLOAT3& lhs, const XMFLOAT3& rhs) const;
	inline bool Float2Equal(const XMFLOAT2& lhs, const XMFLOAT2& rhs) const;

	inline void InitBuffers();

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
	Mesh();
	// Loads mesh from file
	Mesh(const std::wstring& filePath);
	~Mesh();

	static Mesh* CreateResource(const std::wstring& name) { return new Mesh(name); }

#pragma endregion
};

