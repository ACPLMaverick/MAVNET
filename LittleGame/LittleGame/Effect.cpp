#include "stdafx.h"
#include "Effect.h"
#include "ResourceManager.h"
#include "Renderer.h"

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

	//ID3DXBuffer* errors;
	//HRESULT h = D3DXCreateBuffer(1024, &errors);
	/*h = */D3DXCreateEffectFromFile(Renderer::GetInstance()->GetDirect3DDevice(),
		filePathW.c_str(),
		NULL,
		NULL,
		D3DXSHADER_OPTIMIZATION_LEVEL3 | D3DXSHADER_ENABLE_BACKWARDS_COMPATIBILITY,
		NULL,
		&m_effect,
		//&errors
		NULL
		);

	//errors->Release();
}

void Effect::Shutdown()
{
	m_effect->Release();
}
