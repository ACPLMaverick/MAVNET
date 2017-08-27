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
		ID3D11ShaderResourceView* const inputSRVA, ID3D11ShaderResourceView* const inputSRVB,
		ID3D11ShaderResourceView* const bufferSRVA, ID3D11ShaderResourceView* const bufferSRVB,
		ID3D11UnorderedAccessView* bufferUAVA, ID3D11UnorderedAccessView* bufferUAVB,
		ID3D11UnorderedAccessView* outputUAVA, ID3D11UnorderedAccessView* outputUAVB,
		uint32_t inputWidth, uint32_t inputHeight, uint32_t inputLevel) const;

	inline void PrintRawData(const Texture* tex) const;

#pragma endregion

public:

#pragma region Public

	SATGenerator();
	~SATGenerator();

	void Generate(const Texture* inputA, const Texture* inputB,
		RWTexture* bufferA, RWTexture* bufferB,
		RWTexture* outputA, RWTexture* outputB) const;
	void Generate(const GBuffer::RenderTarget* inputA, const GBuffer::RenderTarget* inputB, 
		RWTexture* bufferA, RWTexture* bufferB,
		RWTexture* outputA, RWTexture* outputB) const;

#pragma endregion

};