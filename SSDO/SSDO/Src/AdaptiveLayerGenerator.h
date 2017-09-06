#pragma once

#include "GlobalDefines.h"
#include "GBuffer.h"
#include "Singleton.h"

class Texture;
class RWTexture;
class ComputeShader;

typedef void GenerateTexParamsFunc(Texture*);

class AdaptiveLayerGenerator : public Singleton<AdaptiveLayerGenerator>
{
public:

#pragma region Struct Public

	struct AdaptiveLayerData
	{
		RWTexture* _layer1 = nullptr;
		RWTexture* _layer2 = nullptr;
		RWTexture* _layer10 = nullptr;
		RWTexture* _layer11 = nullptr;
		RWTexture* _layer20 = nullptr;
		RWTexture* _layer21 = nullptr;
		RWTexture* _satLayer1 = nullptr;
		// _satLayer2;	// is computed diferentially while sampling
		RWTexture* _satLayer10 = nullptr;
		// _satLayer11 is computed diferentially while sampling
		RWTexture* _satLayer20 = nullptr;
		// _satLayer21 is computed diferentially while sampling

		void Initialize(GenerateTexParamsFunc genFunc);
		void Shutdown();
	};

#pragma endregion

protected:

#pragma region Protected

	ComputeShader* _shaderCreateLayers = nullptr;
	AdaptiveLayerData _data;

#pragma endregion

#pragma region Functions Protected

	inline void GenerateInternal(ID3D11ShaderResourceView* baseInputA, ID3D11ShaderResourceView* baseInputB, RWTexture* baseSATA, RWTexture* baseSATB,
		RWTexture* bufferA, RWTexture* bufferB,
		AdaptiveLayerData* dataA, AdaptiveLayerData* dataB);

#pragma endregion

public:

#pragma region Functions Public

	AdaptiveLayerGenerator();
	~AdaptiveLayerGenerator();

	void Initialize(GenerateTexParamsFunc genFunc);
	void Shutdown();

	void Generate(GBuffer::RenderTarget* baseInputA, GBuffer::RenderTarget* baseInputB, RWTexture* baseSATA, RWTexture* baseSATB,
		RWTexture* bufferA, RWTexture* bufferB,
		AdaptiveLayerData* dataA, AdaptiveLayerData* dataB);
	void Generate(Texture* baseInputA, Texture* baseInputB, RWTexture* baseSATA, RWTexture* baseSATB,
		RWTexture* bufferA, RWTexture* bufferB,
		AdaptiveLayerData* dataA, AdaptiveLayerData* dataB);

#pragma endregion

};

