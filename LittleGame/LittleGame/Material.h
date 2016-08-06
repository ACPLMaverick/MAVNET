#pragma once

#include "GameResource.h"

#include <d3dx9math.h>

class Mesh;
class Texture;

class Material : public GameResource
{
protected:

#pragma region Constants

	const std::string DEFAULT_TECHNIQUE_NAME = "Main";
	const std::string DEFAULT_PASS_NAME = "Pass0";

#pragma endregion

#pragma region Components


#pragma endregion


#pragma region Parameters

	Texture* m_diffuseMap = nullptr;

	Texture* m_diffuseColorTexture;

	D3DXCOLOR m_diffuseColor = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

#pragma endregion

public:
	Material();
	~Material();

	// loading from file will have to be incorporated here
	virtual void Initialize(uint32_t manualHash = 0, const std::string* filePath = nullptr, const std::string* name = nullptr);
	virtual void Shutdown();
	virtual void Update();
	virtual inline void Draw(const Mesh* const mesh);

#pragma region Accessors

	Texture* const GetTextureDiffuse() { return m_diffuseMap; }
	const D3DXCOLOR* GetColorDiffuse() { return &m_diffuseColor; }

	void SetTextureDiffuse(Texture* const texture) { m_diffuseMap = texture; }
	void SetColorDiffuse(const D3DXCOLOR* color) { m_diffuseColor = *color; }

#pragma endregion

};

