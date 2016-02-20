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

void Material::PreDraw(Mesh* msh)
{
	ID3DXEffect* ef = m_effect->GetDX9Effect();

	ef->Begin(NULL, NULL);
	ef->BeginPass(0);

	D3DXHANDLE hMat = ef->GetParameterByName(NULL, "MatWorldViewProj");
	D3DXHANDLE hDiffMap = ef->GetParameterByName(NULL, "DiffuseMap");
	D3DXHANDLE hDiffCol = ef->GetParameterByName(NULL, "DiffuseColor");
	D3DXHANDLE hAmbCol = ef->GetParameterByName(NULL, "AmbientColor");

	D3DXMATRIX transform = *msh->GetMyGameObject()->GetTransform()->GetWorld();
	transform *= *System::GetInstance()->GetCurrentScene()->GetCurrentCamera()->GetViewProj();

	D3DXCOLOR ambient = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);

	ef->SetMatrix(hMat, &transform);
	ef->SetTexture(hDiffMap, m_diffuseMap->GetDX9Texture());
	ef->SetFloatArray(hDiffCol, (float*)&m_diffuseColor, 4);
	ef->SetFloatArray(hAmbCol, (float*)&ambient, 4);

	ef->CommitChanges();
}

void Material::PostDraw(Mesh* msh)
{
	ID3DXEffect* ef = m_effect->GetDX9Effect();
	ef->EndPass();
	ef->End();
}

void Material::Update()
{
}
