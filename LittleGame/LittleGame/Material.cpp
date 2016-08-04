#include "stdafx.h"
#include "Material.h"

#include "Renderer.h"
#include "Texture.h"
#include "Effect.h"
#include "Texture.h"
#include "Mesh.h"
#include "GameObject.h"
#include "Transform.h"
#include "Camera.h"
#include "System.h"
#include "Scene.h"

Material::Material()
{
}


Material::~Material()
{
}

void Material::Initialize(uint32_t manualHash, const std::string * filePath, const std::string * name)
{
	std::string tmp = "NoNameMaterial";
	if (filePath != nullptr)
	{
		tmp = *filePath;

		// LOAD FROM FILE HERE
	}
		
	GameResource::Initialize(&tmp, name);

	if (manualHash != 0 && filePath == nullptr)
		m_hash = manualHash;

	m_diffuseColorTexture = new Texture();
	std::string texColorName = *name + "_ColorTexture";
	m_diffuseColorTexture->Initialize(&texColorName, 1, 1, &m_diffuseColor);
}

void Material::Shutdown()
{
	m_diffuseColorTexture->Shutdown();
	delete m_diffuseColorTexture;
}

void Material::Update()
{
}

void Material::Draw(const Mesh * const mesh)
{
	/*
	ID3DXEffect* ef = m_effect->GetDX9Effect();

	uint32_t pCount = 0;
	ef->Begin(&pCount, NULL);
	
	for (int i = 0; i < pCount; ++i)
	{
		ef->BeginPass(i);

		D3DXMATRIX transform = *mesh->m_obj->GetTransform()->GetWorld();
		transform *= *System::GetInstance()->GetCurrentScene()->GetCurrentCamera()->GetViewProj();

		D3DXCOLOR ambient = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);

		m_effect->SetMatTransform(&transform);
		m_effect->SetMapDiffuse(m_diffuseMap);
		m_effect->SetColorDiffuse(&m_diffuseColor);
		m_effect->SetColorAmbient(&ambient);
		m_effect->SetTransparency(m_coeffTransparency);

		ef->CommitChanges();

		mesh->DrawArrays();

		ef->EndPass();
	}

	ef->End();
	*/

	LPDIRECT3DDEVICE9 dev = Renderer::GetInstance()->GetDirect3DDevice();

	D3DXMATRIX world = *mesh->m_obj->GetTransform()->GetWorld();
	D3DXMATRIX view = *System::GetInstance()->GetCurrentScene()->GetCurrentCamera()->GetView();
	D3DXMATRIX proj = *System::GetInstance()->GetCurrentScene()->GetCurrentCamera()->GetProj();

	dev->SetTransform(D3DTS_WORLD, &world);
	dev->SetTransform(D3DTRANSFORMSTATETYPE::D3DTS_VIEW, &view);
	dev->SetTransform(D3DTRANSFORMSTATETYPE::D3DTS_PROJECTION, &proj);

	dev->SetTexture(0, m_diffuseMap->GetDX9Texture());
	dev->SetTexture(1, m_diffuseColorTexture->GetDX9Texture());

	mesh->DrawArrays();
}
