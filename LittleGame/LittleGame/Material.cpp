#include "stdafx.h"
#include "Material.h"
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
}

void Material::Shutdown()
{
	// nothing
}

void Material::Update()
{
}

void Material::Draw(const Mesh * const mesh)
{
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
}
