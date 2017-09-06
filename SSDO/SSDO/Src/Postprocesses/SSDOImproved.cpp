#include "stdafx.h"
#include "SSDOImproved.h"
#include "System.h"
#include "Renderer.h"
#include "Random.h"
#include "Camera.h"
#include "Scenes\Scene.h"
#include "SimpleSSAO.h"
#include "GBuffer.h"
#include "Lights/LightDirectional.h"
#include "RWTexture.h"
#include "SATGenerator.h"

namespace Postprocesses
{
	SSDOImproved::SSDOImproved() :
		_dataBuffer(nullptr),
		_sampleBoxHalfSize(0.3f),
		_occlusionPower(1.0f),
		_occlusionFaloff(1.0f),
		_powFactor(1.0f)
	{
		// TETIN

		const int32_t sizX(1280 / SAT_SIZE_DIVISOR);
		const int32_t sizY(720 / SAT_SIZE_DIVISOR);

		_testInput = new Texture();
		_testInput->SetWidth(sizX);
		_testInput->SetHeight(sizY);
		_testInput->SetFormat(DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT);
		_testInput->SetBPP(128);
		_testInput->SetMipmapped(false);
		_testInput->AllocateRawDataToTextureSize();

		XMFLOAT4* ptr = reinterpret_cast<XMFLOAT4*>(_testInput->GetRawData().GetDataPtr());
		for (size_t i = 0; i < sizX * sizY; ++i)
		{
			ptr[i] = XMFLOAT4(1.0f, 0.0f, 0.0f, 0.0f);
		}
		
		_testInput->InitResources(false, false);

		_testOutput = new RWTexture();
		_testOutput->SetWidth(sizX);
		_testOutput->SetHeight(sizY);
		_testOutput->SetFormat(DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT);
		_testOutput->SetBPP(128);
		_testOutput->SetMipmapped(false);
		_testOutput->AllocateRawDataToTextureSize();
		_testOutput->InitResources(false, false);

		_testBuf = new RWTexture();
		_testBuf->SetWidth(sizX);
		_testBuf->SetHeight(sizY);
		_testBuf->SetFormat(DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT);
		_testBuf->SetBPP(128);
		_testBuf->SetMipmapped(false);
		_testBuf->InitResources(true, false);

		// END TETIN

		_satColor = new RWTexture();
		_satNormalDepth = new RWTexture();
		_satBufferA = new RWTexture();
		_satBufferB = new RWTexture();
		
		AssignTextureParams(_satColor);
		AssignTextureParams(_satNormalDepth);
		AssignTextureParams(_satBufferA);
		AssignTextureParams(_satBufferB);

		_shaders.push_back(System::GetInstance()->GetScene()->LoadShader(std::wstring(L"SSDOImproved_Base")));

		ID3D11Device* device = Renderer::GetInstance()->GetDevice();

		D3D11_BUFFER_DESC bDesc;
		bDesc.Usage = D3D11_USAGE_DYNAMIC;
		bDesc.ByteWidth = sizeof(Shader::SSDOImprovedPS);
		bDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		bDesc.MiscFlags = 0;
		bDesc.StructureByteStride = 0;

		device->CreateBuffer(&bDesc, nullptr, &_dataBuffer);
		ASSERT(_dataBuffer != nullptr);

		AdaptiveLayerGenerator::GetInstance()->Initialize(&SSDOImproved::AssignTextureParams);
	}

	SSDOImproved::~SSDOImproved()
	{
		AdaptiveLayerGenerator::GetInstance()->Shutdown();

		delete _testInput;
		delete _testBuf;
		delete _testOutput;

		_dataBuffer->Release();
		delete _dataBuffer;

		delete _satColor;
		delete _satNormalDepth;
		delete _satBufferA;
		delete _satBufferB;
	}

	void SSDOImproved::Update()
	{
	}

	void SSDOImproved::SetPass(GBuffer& gBuffer, const Camera & camera, int32_t passIndex) const
	{
		Postprocess::SetPass(gBuffer, camera, passIndex);

		ID3D11DeviceContext* deviceContext = Renderer::GetInstance()->GetDeviceContext();
		ID3D11ShaderResourceView* nullik[2]{ nullptr, nullptr };
		deviceContext->PSSetShaderResources(4, 2, nullik);

		//_satGen.Generate(_testInput, _testBuf, _testOutput);
		SATGenerator::GetInstance()->Generate(gBuffer.GetNormalDepthBuffer(), gBuffer.GetColorBuffer(),
									_satBufferA, _satBufferB, _satNormalDepth, _satColor);

		// Split base buffers into two adaptive layers with index texture assignment
		// Generate SAT for first layer
		// Compute second layer's SAT using differential method

		// For each second layer, compute further two adaptive layers out of them with index texture assignment
		// For each child first layer compute SAT, second one will be computed in

		Shader::SSDOImprovedPS* buffer = reinterpret_cast<Shader::SSDOImprovedPS*>(_shaders[0]->MapPsBuffer(1));
		reinterpret_cast<Lights::LightDirectional*>(&buffer->LightColor)->ApplyToShader(
			*System::GetInstance()->GetScene()->GetLightsDirectional()[0],
			camera
		);
		buffer->SatDimensionsAndRecs.x = static_cast<float>(_satNormalDepth->GetWidth());
		buffer->SatDimensionsAndRecs.y = static_cast<float>(_satNormalDepth->GetHeight());
		buffer->SatDimensionsAndRecs.z = 1.0f / buffer->SatDimensionsAndRecs.x;
		buffer->SatDimensionsAndRecs.w = 1.0f / buffer->SatDimensionsAndRecs.y;
		buffer->SampleBoxHalfSize = _sampleBoxHalfSize;
		buffer->OcclusionPower = _occlusionPower;
		buffer->OcclusionFalloff = _occlusionFaloff;
		buffer->PowFactor = _powFactor;

		_shaders[0]->UnmapPsBuffer(1);

		// Set four adaptive layers for each data buffer (8 textures) and index texture (totally 9)
		_satNormalDepth->Set(4);
		_satColor->Set(5);

		const GBuffer::RenderTarget* in[1] = { gBuffer.PPGetOutputBBuffer() };
		const int32_t inSlots[1] = { 3 };
		gBuffer.PPSetBuffersAsInput(in, inSlots, 1);
		const GBuffer::RenderTarget* rts[1] = { gBuffer.PPGetOutputBuffer() };
		gBuffer.PPSetBuffersAsOutput(rts, 1, nullptr);
	}

	void SSDOImproved::AfterPass(GBuffer & gBuffer, const Camera & camera, int32_t passIndex) const
	{
		
	}

	void SSDOImproved::AssignTextureParams(Texture * tex)
	{
		tex->SetWidth(System::GetInstance()->GetOptions()._windowWidth / SAT_SIZE_DIVISOR);
		tex->SetHeight(System::GetInstance()->GetOptions()._windowHeight / SAT_SIZE_DIVISOR);
		tex->SetFormat(DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT);
		tex->SetBPP(128);
		tex->SetMipmapped(false);
		tex->InitResources(false, false);
	}
}
