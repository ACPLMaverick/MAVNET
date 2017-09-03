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

		const int32_t sizX(1280);
		const int32_t sizY(720);

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
		_bufColor = new RWTexture();
		_bufNormalDepth = new RWTexture();
		_satColor->SetWidth(System::GetInstance()->GetOptions()._windowWidth);
		_satColor->SetHeight(System::GetInstance()->GetOptions()._windowHeight);
		_satColor->SetFormat(DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT);
		_satColor->SetBPP(32);
		_satColor->SetMipmapped(false);
		_satColor->InitResources(false, false);
		_satNormalDepth->SetWidth(System::GetInstance()->GetOptions()._windowWidth);
		_satNormalDepth->SetHeight(System::GetInstance()->GetOptions()._windowHeight);
		_satNormalDepth->SetFormat(DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT);
		_satNormalDepth->SetBPP(128);
		_satNormalDepth->SetMipmapped(false);
		_satNormalDepth->InitResources(false, false);
		_bufColor->SetWidth(System::GetInstance()->GetOptions()._windowWidth);
		_bufColor->SetHeight(System::GetInstance()->GetOptions()._windowHeight);
		_bufColor->SetFormat(DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT);
		_bufColor->SetBPP(32);
		_bufColor->SetMipmapped(false);
		_bufColor->InitResources(true, false);
		_bufNormalDepth->SetWidth(System::GetInstance()->GetOptions()._windowWidth);
		_bufNormalDepth->SetHeight(System::GetInstance()->GetOptions()._windowHeight);
		_bufNormalDepth->SetFormat(DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT);
		_bufNormalDepth->SetBPP(128);
		_bufNormalDepth->SetMipmapped(false);
		_bufNormalDepth->InitResources(true, false);

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
	}

	SSDOImproved::~SSDOImproved()
	{
		delete _testInput;
		delete _testBuf;
		delete _testOutput;

		_dataBuffer->Release();
		delete _dataBuffer;

		delete _satColor;
		delete _satNormalDepth;
		delete _bufColor;
		delete _bufNormalDepth;
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
		_satGen.Generate(gBuffer.GetNormalDepthBuffer(), _bufNormalDepth, _satNormalDepth);
		_satGen.Generate(gBuffer.GetColorBuffer(), _bufColor, _satColor);

		Shader::SSDOImprovedPS* buffer = reinterpret_cast<Shader::SSDOImprovedPS*>(_shaders[0]->MapPsBuffer(1));
		reinterpret_cast<Lights::LightDirectional*>(&buffer->LightColor)->ApplyToShader(
			*System::GetInstance()->GetScene()->GetLightsDirectional()[0],
			camera
		);
		buffer->SatDimensions = XMFLOAT2A(
			static_cast<float>(_satNormalDepth->GetWidth()), 
			static_cast<float>(_satNormalDepth->GetHeight()));
		buffer->SampleBoxHalfSize = _sampleBoxHalfSize;
		buffer->OcclusionPower = _occlusionPower;
		buffer->OcclusionFalloff = _occlusionFaloff;
		buffer->PowFactor = _powFactor;

		_shaders[0]->UnmapPsBuffer(1);

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
}
