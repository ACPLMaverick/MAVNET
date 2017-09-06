#include "stdafx.h"
#include "AdaptiveLayerGenerator.h"
#include "Texture.h"
#include "RWTexture.h"
#include "ComputeShader.h"
#include "System.h"
#include "Scenes\Scene.h"


AdaptiveLayerGenerator::AdaptiveLayerGenerator()
{
}


AdaptiveLayerGenerator::~AdaptiveLayerGenerator()
{
}

void AdaptiveLayerGenerator::Initialize(GenerateTexParamsFunc genFunc)
{
	_shaderCreateLayers = System::GetInstance()->GetScene()->LoadComputeShader(L"AdaptiveLayers");
	_data.Initialize(genFunc);
}

void AdaptiveLayerGenerator::Shutdown()
{
	_data.Shutdown();
}

void AdaptiveLayerGenerator::Generate(GBuffer::RenderTarget * baseInputA, GBuffer::RenderTarget * baseInputB, 
	RWTexture * baseSATA, RWTexture * baseSATB, RWTexture* bufferA, RWTexture* bufferB,
	AdaptiveLayerData * dataA, AdaptiveLayerData * dataB)
{
	GenerateInternal(baseInputA->SRV, baseInputB->SRV, baseSATA, baseSATB, bufferA, bufferB, dataA, dataB);
}

void AdaptiveLayerGenerator::Generate(Texture * baseInputA, Texture * baseInputB, 
	RWTexture * baseSATA, RWTexture * baseSATB, RWTexture* bufferA, RWTexture* bufferB,
	AdaptiveLayerData * dataA, AdaptiveLayerData * dataB)
{
	GenerateInternal(baseInputA->GetSRV(), baseInputB->GetSRV(), baseSATA, baseSATB, bufferA, bufferB, dataA, dataB);
}

inline void AdaptiveLayerGenerator::GenerateInternal(ID3D11ShaderResourceView * baseInputA, ID3D11ShaderResourceView * baseInputB, 
	RWTexture * baseSATA, RWTexture * baseSATB, RWTexture* bufferA, RWTexture* bufferB,
	AdaptiveLayerData * dataA, AdaptiveLayerData * dataB)
{
}

void AdaptiveLayerGenerator::AdaptiveLayerData::Initialize(GenerateTexParamsFunc genFunc)
{
	ASSERT(_layer1 == nullptr);
	_layer1 = new RWTexture();
	_layer2 = new RWTexture();
	_layer10 = new RWTexture();
	_layer11 = new RWTexture();
	_layer20 = new RWTexture();
	_layer21 = new RWTexture();
	_satLayer1 = new RWTexture();
	_satLayer10 = new RWTexture();
	_satLayer20 = new RWTexture();
	genFunc(_layer1);
	genFunc(_layer2);
	genFunc(_layer10);
	genFunc(_layer11);
	genFunc(_layer20);
	genFunc(_layer21);
	genFunc(_satLayer1);
	genFunc(_satLayer10);
	genFunc(_satLayer20);
}

void AdaptiveLayerGenerator::AdaptiveLayerData::Shutdown()
{
	ASSERT(_layer1 != nullptr);
	delete _layer1;
	delete _layer2;
	delete _layer10;
	delete _layer11;
	delete _layer20;
	delete _layer21;
	delete _satLayer1;
	delete _satLayer10;
	delete _satLayer20;
}
