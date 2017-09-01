#pragma once

#include "GlobalDefines.h"
#include "RWStructuredBuffer.h"
#include "GBuffer.h"

class Texture;
class RWTexture;
class ComputeShader;

class SATGenerator
{
protected:

#pragma region Structs Protected

	struct TempBufferData
	{
		XMFLOAT4 NormalDepth;
		XMFLOAT4 Color;
	};

#pragma endregion

#pragma region Protected

	ComputeShader* _shader = nullptr;

	inline void GenerateInternal(
		ID3D11ShaderResourceView* const inputSRV,
		ID3D11ShaderResourceView* const bufferSRV,
		ID3D11UnorderedAccessView* bufferUAV,
		ID3D11UnorderedAccessView* outputUAV,
		uint32_t inputWidth, uint32_t inputHeight, uint32_t inputLevel) const;

	inline void PrintRawData(const Texture* tex) const;
	inline uint32_t GetPowerOfTwoHigherThan(uint32_t val) const;

#pragma endregion

public:

#pragma region Public

	SATGenerator();
	~SATGenerator();

	void Generate(const Texture* input,
		RWTexture* buffer,
		RWTexture* output) const;
	void Generate(const GBuffer::RenderTarget* input, 
		RWTexture* buffer,
		RWTexture* output) const;

#pragma endregion

};