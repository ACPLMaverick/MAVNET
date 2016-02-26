#include "stdafx.h"
#include "Effect.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "Texture.h"

Effect::Effect()
{
}


Effect::~Effect()
{
}

void Effect::Initialize(const std::string * filePath, const std::string * name)
{
	GameResource::Initialize(filePath, name);

	//ResourceManager::GetInstance()->LoadWideTextFileFromAssets(&data, filePath);

	std::wstring filePathW = std::wstring(filePath->begin(), filePath->end());

	D3DXCreateEffectFromFile(Renderer::GetInstance()->GetDirect3DDevice(),
		filePathW.c_str(),
		NULL,
		NULL,
		D3DXSHADER_OPTIMIZATION_LEVEL3 | D3DXSHADER_ENABLE_BACKWARDS_COMPATIBILITY,
		NULL,
		&m_effect,
		NULL
		);

	h_matTransform = m_effect->GetParameterByName(NULL, "MatWorldViewProj");
	h_matWorld = m_effect->GetParameterByName(NULL, "MatWorld");
	h_matView = m_effect->GetParameterByName(NULL, "MatView");
	h_matProj = m_effect->GetParameterByName(NULL, "MatProj");
	h_matWorldInvTrans = m_effect->GetParameterByName(NULL, "MatWorldInvTrans");
	h_diffMap = m_effect->GetParameterByName(NULL, "DiffuseMap");
	h_diffCol = m_effect->GetParameterByName(NULL, "DiffuseColor");
	h_specCol = m_effect->GetParameterByName(NULL, "SpecularColor");
	h_ambCol = m_effect->GetParameterByName(NULL, "AmbientColor");
	h_transp = m_effect->GetParameterByName(NULL, "Transparency");
}

void Effect::Shutdown()
{
	m_effect->Release();
}

const void Effect::SetMatTransform(const D3DXMATRIX * mat) const
{
	m_effect->SetMatrix(h_matTransform, mat);
}

const void Effect::SetMatWorld(const D3DXMATRIX * mat) const
{
	m_effect->SetMatrix(h_matWorld, mat);
}

const void Effect::SetMatView(const D3DXMATRIX * mat) const
{
	m_effect->SetMatrix(h_matView, mat);
}

const void Effect::SetMatProj(const D3DXMATRIX * mat) const
{
	m_effect->SetMatrix(h_matProj, mat);
}

const void Effect::SetMatWorldInvTrans(const D3DXMATRIX * mat) const
{
	m_effect->SetMatrix(h_matWorldInvTrans, mat);
}

const void Effect::SetMapDiffuse(const Texture * map) const
{
	m_effect->SetTexture(h_diffMap, map->GetDX9Texture());
}

const void Effect::SetColorDiffuse(const D3DXCOLOR * col) const
{
	m_effect->SetFloatArray(h_diffCol, (float*)col, 4);
}

const void Effect::SetColorSpecular(const D3DXCOLOR * col) const
{
	m_effect->SetFloatArray(h_specCol, (float*)col, 4);
}

const void Effect::SetColorAmbient(const D3DXCOLOR * col) const
{
	m_effect->SetFloatArray(h_ambCol, (float*)col, 4);
}

const void Effect::SetTransparency(const float transp) const
{
	m_effect->SetFloat(h_transp, transp);
}
