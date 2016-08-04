#include "stdafx.h"
#include "Mesh.h"
#include "Renderer.h"
#include "GameObject.h"
#include "Transform.h"
#include "System.h"
#include "Scene.h"
#include "Camera.h"
#include "Material.h"
#include "Effect.h"
#include "Texture.h"

#include <DxErr.h>
#pragma comment(lib, "dxerr.lib")
#pragma comment(lib, "legacy_stdio_definitions.lib")

Mesh::Mesh()
{
}


Mesh::~Mesh()
{
}

void Mesh::Initialize(uint32_t uid, GameObject* obj, std::string* name)
{
	Component::Initialize(uid, obj, name);

	InitializeVertexData();

	LPDIRECT3DDEVICE9 device = Renderer::GetInstance()->GetDirect3DDevice();
	HRESULT res = device->CreateVertexBuffer(
		m_vertexData.m_vertexArray.size() * m_vertexStructSize,
		NULL, m_fvf, D3DPOOL_MANAGED, 
		&m_vertexData.m_vertexBuffer, NULL
		);
	void* accessPtr;
	m_vertexData.m_vertexBuffer->Lock(0, 0, &accessPtr, 0);
	memcpy(accessPtr, &m_vertexData.m_vertexArray[0],
		m_vertexData.m_vertexArray.size() * m_vertexStructSize);
	m_vertexData.m_vertexBuffer->Unlock();

	res = device->CreateIndexBuffer(
		m_vertexData.m_indexArray.size() * sizeof(uint16_t),
		NULL, D3DFORMAT::D3DFMT_INDEX16, D3DPOOL_MANAGED,
		&m_vertexData.m_indexBuffer, NULL
		);

	m_vertexData.m_indexBuffer->Lock(0, 0, &accessPtr, 0);
	memcpy(accessPtr, &m_vertexData.m_indexArray[0],
		m_vertexData.m_indexArray.size() * sizeof(uint16_t));
	m_vertexData.m_indexBuffer->Unlock();
}

void Mesh::Initialize(uint32_t uid, GameObject* obj, Material* mat, std::string* name)
{
	m_material = mat;
	Initialize(uid, obj, name);
}

void Mesh::Shutdown()
{
	m_vertexData.m_vertexBuffer->Release();
	m_vertexData.m_indexBuffer->Release();
}

void Mesh::Draw()
{
	// material controls all drawing
	// IFS WILL BE REMOVED!!!
	if (m_material != nullptr)
	{
		m_material->Draw(this);
	}
}

const inline void Mesh::DrawArrays() const
{
	LPDIRECT3DDEVICE9 dev = Renderer::GetInstance()->GetDirect3DDevice();
	HRESULT r = dev->SetFVF(m_fvf);
	r = dev->SetStreamSource(0, m_vertexData.m_vertexBuffer, 0, m_vertexStructSize);
	r = dev->SetIndices(m_vertexData.m_indexBuffer);
	r = dev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0,
		m_vertexData.m_vertexArray.size(), 0, m_vertexData.m_indexArray.size() / 3);
}

void Mesh::Update()
{
	// IFS WILL BE REMOVED!!!
	if(m_material != nullptr)
		m_material->Update();
}
