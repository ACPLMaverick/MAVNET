#include "stdafx.h"
#include "Mesh.h"
#include "Renderer.h"
#include "GameObject.h"
#include "Transform.h"
#include "System.h"
#include "Scene.h"
#include "Camera.h"

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
		m_vertexData.m_vertexArray.size() * sizeof(VertexSprite),
		NULL, D3DFVF_SPRITE, D3DPOOL_MANAGED, 
		&m_vertexData.m_vertexBuffer, NULL
		);
	void* accessPtr;
	m_vertexData.m_vertexBuffer->Lock(0, 0, &accessPtr, 0);
	memcpy(accessPtr, &m_vertexData.m_vertexArray[0],
		m_vertexData.m_vertexArray.size() * sizeof(VertexSprite));
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
	// material pre-draw operations

	// mesh draw operations

	LPDIRECT3DDEVICE9 dev = Renderer::GetInstance()->GetDirect3DDevice();

	D3DXMATRIX transform;
	D3DXMATRIX v;
	D3DXMATRIX p;
	D3DXMatrixIdentity(&transform);

	dev->SetFVF(D3DFVF_SPRITE);

	if (m_obj->GetTransform() != nullptr)
	{
		transform = *m_obj->GetTransform()->GetWorld();
	}
	//dev->SetTransform(D3DTS_WORLD, &transform);
	if (System::GetInstance()->GetCurrentScene()->GetCurrentCamera() != nullptr)
	{
		//transform = transform * *System::GetInstance()->GetCurrentScene()->GetCurrentCamera()->GetViewProj();
		v = *System::GetInstance()->GetCurrentScene()->GetCurrentCamera()->GetView();
		p = *System::GetInstance()->GetCurrentScene()->GetCurrentCamera()->GetProj();
		dev->SetTransform(D3DTS_VIEW, &v);
		dev->SetTransform(D3DTS_PROJECTION, &p);
	}

	dev->SetStreamSource(0, m_vertexData.m_vertexBuffer, 0, sizeof(VertexSprite));
	dev->SetIndices(m_vertexData.m_indexBuffer);
	dev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0,
		m_vertexData.m_vertexArray.size(), 0, m_vertexData.m_indexArray.size() / 3);

	// material post-draw operations
}

void Mesh::Update()
{
}
