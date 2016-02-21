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
	// IFS WILL BE REMOVED!!!
	if (m_material != nullptr)
	{
		ID3DXEffect* ef = m_material->GetEffect()->GetDX9Effect();

		ef->Begin(NULL, NULL);
		ef->BeginPass(0);

		D3DXHANDLE hMat = ef->GetParameterByName(NULL, "MatWorldViewProj");
		D3DXHANDLE hDiffMap = ef->GetParameterByName(NULL, "DiffuseMap");
		D3DXHANDLE hDiffCol = ef->GetParameterByName(NULL, "DiffuseColor");
		D3DXHANDLE hAmbCol = ef->GetParameterByName(NULL, "AmbientColor");

		D3DXMATRIX transform = *m_obj->GetTransform()->GetWorld();
		transform *= *System::GetInstance()->GetCurrentScene()->GetCurrentCamera()->GetViewProj();

		D3DXCOLOR ambient = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);

		ef->SetMatrix(hMat, &transform);
		ef->SetTexture(hDiffMap, m_material->GetTextureDiffuse()->GetDX9Texture());
		ef->SetFloatArray(hDiffCol, (float*)m_material->GetColorDiffuse(), 4);
		ef->SetFloatArray(hAmbCol, (float*)&ambient, 4);

		ef->CommitChanges();
	}

	// mesh draw operations

	LPDIRECT3DDEVICE9 dev = Renderer::GetInstance()->GetDirect3DDevice();
	/*
	D3DXMATRIX transform;
	D3DXMATRIX v;
	D3DXMATRIX p;
	D3DXMatrixIdentity(&transform);

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
	*/
	HRESULT r = dev->SetFVF(D3DFVF_SPRITE);
	r = dev->SetStreamSource(0, m_vertexData.m_vertexBuffer, 0, sizeof(VertexSprite));
	r = dev->SetIndices(m_vertexData.m_indexBuffer);
	r = dev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0,
		m_vertexData.m_vertexArray.size(), 0, m_vertexData.m_indexArray.size() / 3);
	//std::wstring err = DXGetErrorDescription(r);

	// material post-draw operations
	// IFS WILL BE REMOVED!!!
	if (m_material != nullptr)
	{
		/*ID3DXEffect* ef = m_material->GetEffect()->GetDX9Effect();
		ef->EndPass();
		ef->End();*/
	}
}

void Mesh::Update()
{
	// IFS WILL BE REMOVED!!!
	if(m_material != nullptr)
		m_material->Update();
}
