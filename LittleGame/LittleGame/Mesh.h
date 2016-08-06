#pragma once

#include "Component.h"

#include <d3d9.h>
#include <d3dx9math.h>
#include <vector>

class Material;

///////////////
// DX9 vertex data

#define D3DFVF_SPRITE ( (D3DFVF_XYZ) | (D3DFVF_TEX1)  | (D3DFVF_TEX2))
#define D3DFVF_BASIC ( (D3DFVF_XYZ) | (D3DFVF_NORMAL) | (D3DFVF_DIFFUSE) | (D3DFVF_TEX0) )

struct VertexSprite
{
	D3DXVECTOR3 position;
	D3DXVECTOR2 uv;
	D3DXVECTOR2 uv2;

	VertexSprite(D3DXVECTOR3 p, D3DXVECTOR2 uv)
	{
		position = p;
		this->uv = uv;
		this->uv2 = uv;
	}
};

struct VertexBasic
{
	D3DXCOLOR color;
	D3DXVECTOR3 position;
	D3DXVECTOR3 normal;
	D3DXVECTOR2 uv;

	VertexBasic(D3DXVECTOR3 p, D3DXVECTOR3 n, D3DXCOLOR c, D3DXVECTOR2 uv)
	{
		position = p;
		normal = n;
		color = c;
		this->uv = uv;
	}
};

class VertexData
{
public:
	std::vector<VertexSprite> m_vertexArray;
	std::vector<uint16_t> m_indexArray;

	LPDIRECT3DVERTEXBUFFER9 m_vertexBuffer;
	LPDIRECT3DINDEXBUFFER9 m_indexBuffer;
};

///////////////

class Mesh : public Component
{
	friend class Material;
protected:

	VertexData m_vertexData;
	uint32_t m_fvf = 0x00000000;
	uint32_t m_vertexStructSize = 0;
	Material* m_material = nullptr;

	virtual void InitializeVertexData() = 0;
	virtual const inline void DrawArrays() const;
public:
	Mesh();
	~Mesh();

	virtual void Initialize(uint32_t uid, GameObject* obj, std::string* name = nullptr);
	virtual void Initialize(uint32_t uid, GameObject* obj, Material* mat, std::string* name = nullptr);
	virtual void Shutdown();
	virtual void Draw();
	virtual void Update();

#pragma region Accessors

	Material* const GetMaterial() { return m_material; }

	void SetMaterial(Material* const mat) { m_material = mat; }

#pragma endregion

};

