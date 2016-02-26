#pragma once

#include "GameResource.h"

#include <d3dx9math.h>

class Mesh;
class Effect;
class Texture;

class Material : public GameResource
{
protected:

#pragma region Constants

	const std::string DEFAULT_TECHNIQUE_NAME = "Main";
	const std::string DEFAULT_PASS_NAME = "Pass0";

#pragma endregion

#pragma region Components

	Effect* m_effect = nullptr;

#pragma endregion


#pragma region Parameters

	Texture* m_diffuseMap = nullptr;

	D3DXCOLOR m_diffuseColor = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	D3DXCOLOR m_specularColor = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	float m_specularCoeff = 1.0f;
	float m_glossCoeff = 50.0f;
	float m_coeffTransparency = 1.0f;

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

	Effect* const GetEffect() { return m_effect; }
	Texture* const GetTextureDiffuse() { return m_diffuseMap; }
	const D3DXCOLOR* GetColorDiffuse() { return &m_diffuseColor; }
	const D3DXCOLOR* GetColorSpecular() { return &m_specularColor; }
	float GetCoeffSpecular() { return m_specularCoeff; }
	float GetCoeffGloss() { return m_glossCoeff; }
	float GetCoeffTransparency() { return m_coeffTransparency; }

	void SetEffect(Effect* const effect) { m_effect = effect; }
	void SetTextureDiffuse(Texture* const texture) { m_diffuseMap = texture; }
	void SetColorDiffuse(const D3DXCOLOR* color) { m_diffuseColor = *color; }
	void SetColorSpecular(const D3DXCOLOR* color) { m_specularColor = *color; }
	void SetCoeffSpecular(float coeff) { m_specularCoeff = coeff; }
	void SetCoeffGloss(float coeff) { m_glossCoeff = coeff; }
	void SetCoeffTransparency(float coeff) { m_coeffTransparency = coeff; }

#pragma endregion

};

