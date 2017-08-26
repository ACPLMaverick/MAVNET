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
		_maxDistance(1.2f),
		_fadeStart(0.02f),
		_epsilon(0.01f),
		_powFactor(1.0f)
	{
		// TETIN

		const int32_t siz(4);

		_testInput = new Texture();
		_testInput->SetWidth(siz);
		_testInput->SetHeight(siz);
		_testInput->SetFormat(DXGI_FORMAT::DXGI_FORMAT_R16G16B16A16_FLOAT);
		_testInput->SetBPP(64);
		_testInput->SetMipmapped(false);
		_testInput->AllocateRawDataToTextureSize();

		PackedVector::XMHALF4* ptr = reinterpret_cast<PackedVector::XMHALF4*>(_testInput->GetRawData().GetDataPtr());
		ptr[0 + 0] = PackedVector::XMHALF4(1.0f, 0.0f, 0.0f, 0.0f);
		ptr[0 + 1] = PackedVector::XMHALF4(3.0f, 0.0f, 0.0f, 0.0f);
		ptr[0 + 2] = PackedVector::XMHALF4(0.0f, 0.0f, 0.0f, 0.0f);
		ptr[0 + 3] = PackedVector::XMHALF4(2.0f, 0.0f, 0.0f, 0.0f);
		ptr[4 + 0] = PackedVector::XMHALF4(3.0f, 0.0f, 0.0f, 0.0f);
		ptr[4 + 1] = PackedVector::XMHALF4(2.0f, 0.0f, 0.0f, 0.0f);
		ptr[4 + 2] = PackedVector::XMHALF4(4.0f, 0.0f, 0.0f, 0.0f);
		ptr[4 + 3] = PackedVector::XMHALF4(3.0f, 0.0f, 0.0f, 0.0f);
		ptr[8 + 0] = PackedVector::XMHALF4(0.0f, 0.0f, 0.0f, 0.0f);
		ptr[8 + 1] = PackedVector::XMHALF4(5.0f, 0.0f, 0.0f, 0.0f);
		ptr[8 + 2] = PackedVector::XMHALF4(1.0f, 0.0f, 0.0f, 0.0f);
		ptr[8 + 3] = PackedVector::XMHALF4(1.0f, 0.0f, 0.0f, 0.0f);
		ptr[12 + 0] = PackedVector::XMHALF4(2.0f, 0.0f, 0.0f, 0.0f);
		ptr[12 + 1] = PackedVector::XMHALF4(2.0f, 0.0f, 0.0f, 0.0f);
		ptr[12 + 2] = PackedVector::XMHALF4(3.0f, 0.0f, 0.0f, 0.0f);
		ptr[12 + 3] = PackedVector::XMHALF4(3.0f, 0.0f, 0.0f, 0.0f);
		
		_testInput->InitResources(false, false);

		_testOutput = new RWTexture();
		_testOutput->SetWidth(siz);
		_testOutput->SetHeight(siz);
		_testOutput->SetFormat(DXGI_FORMAT::DXGI_FORMAT_R16G16B16A16_FLOAT);
		_testOutput->SetBPP(64);
		_testOutput->SetMipmapped(false);
		_testOutput->AllocateRawDataToTextureSize();
		_testOutput->InitResources(false, false);

		// END TETIN

		_satColor = new RWTexture();
		_satNormalDepth = new RWTexture();
		_satColor->SetWidth(System::GetInstance()->GetOptions()._windowWidth);
		_satColor->SetHeight(System::GetInstance()->GetOptions()._windowHeight);
		_satColor->SetFormat(DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM);
		_satColor->SetBPP(32);
		_satColor->SetMipmapped(false);
		_satColor->InitResources(false, false);
		_satNormalDepth->SetWidth(System::GetInstance()->GetOptions()._windowWidth);
		_satNormalDepth->SetHeight(System::GetInstance()->GetOptions()._windowHeight);
		_satNormalDepth->SetFormat(DXGI_FORMAT::DXGI_FORMAT_R16G16B16A16_FLOAT);
		_satNormalDepth->SetBPP(64);
		_satNormalDepth->SetMipmapped(false);
		_satNormalDepth->InitResources(false, false);

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

		// offsets

		_offsets[0] = XMFLOAT4A(1.0f, 1.0f, 1.0f, 0.0f);
		_offsets[1] = XMFLOAT4A(-1.0f, -1.0f, -1.0f, 0.0f);
		_offsets[2] = XMFLOAT4A(-1.0f, 1.0f, 1.0f, 0.0f);
		_offsets[3] = XMFLOAT4A(1.0f, -1.0f, -1.0f, 0.0f);
		_offsets[4] = XMFLOAT4A(1.0f, 1.0f, -1.0f, 0.0f);
		_offsets[5] = XMFLOAT4A(-1.0f, -1.0f, 1.0f, 0.0f);
		_offsets[6] = XMFLOAT4A(-1.0f, 1.0f, -1.0f, 0.0f);
		_offsets[7] = XMFLOAT4A(1.0f, -1.0f, 1.0f, 0.0f);
		_offsets[8] = XMFLOAT4A(-1.0f, 0.0f, 0.0f, 0.0f);
		_offsets[9] = XMFLOAT4A(1.0f, 0.0f, 0.0f, 0.0f);
		_offsets[10] = XMFLOAT4A(0.0f, -1.0f, 0.0f, 0.0f);
		_offsets[11] = XMFLOAT4A(0.0f, 1.0f, 0.0f, 0.0f);
		_offsets[12] = XMFLOAT4A(0.0f, 0.0f, -1.0f, 0.0f);
		_offsets[13] = XMFLOAT4A(0.0f, 0.0f, 1.0f, 0.0f);

		for (int32_t i = 0; i < SAMPLE_COUNT; ++i)
		{
			float rand = System::GetInstance()->GetRandom()->GetFloat(0.25f, 1.0f);
			XMVECTOR vc = XMLoadFloat4A(&_offsets[i]);
			vc = rand * XMVector4Normalize(vc);
			XMStoreFloat4(&_offsets[i], vc);
		}
	}

	SSDOImproved::~SSDOImproved()
	{
		delete _testInput;
		delete _testOutput;

		_dataBuffer->Release();
		delete _dataBuffer;

		delete _satColor;
		delete _satNormalDepth;
	}

	void SSDOImproved::Update()
	{
	}

	void SSDOImproved::SetPass(GBuffer& gBuffer, const Camera & camera, int32_t passIndex) const
	{
		Postprocess::SetPass(gBuffer, camera, passIndex);

		// GENERATE SATS HERE
		_satGen.Generate(_testInput, _testInput, _testOutput, _testOutput);

		Shader::SSDOImprovedPS* buffer = reinterpret_cast<Shader::SSDOImprovedPS*>(_shaders[0]->MapPsBuffer(1));
		XMMATRIX proj = XMLoadFloat4x4A(&camera.GetMatProj());
		XMStoreFloat4x4A(&buffer->Proj, XMMatrixTranspose(proj));
		memcpy(&buffer->Offsets, _offsets, SAMPLE_COUNT * sizeof(XMFLOAT4A));
		FillParams(&buffer->Params);
		reinterpret_cast<Lights::LightDirectional*>(&buffer->LightColor)->ApplyToShader(
			*System::GetInstance()->GetScene()->GetLightsDirectional()[0],
			camera
		);

		_shaders[0]->UnmapPsBuffer(1);

		// SET SATS HERE

		const GBuffer::RenderTarget* in[1] = { gBuffer.PPGetOutputBBuffer() };
		const int32_t inSlots[1] = { 3 };
		gBuffer.PPSetBuffersAsInput(in, inSlots, 1);
		const GBuffer::RenderTarget* rts[1] = { gBuffer.PPGetOutputBuffer() };
		gBuffer.PPSetBuffersAsOutput(rts, 1, nullptr);
	}

	void SSDOImproved::AfterPass(GBuffer & gBuffer, const Camera & camera, int32_t passIndex) const
	{
		//gBuffer.FlipOutputs();
	}

	inline void SSDOImproved::FillParams(XMFLOAT4A * paramBuffer) const
	{
		paramBuffer->x = _maxDistance;
		paramBuffer->y = _fadeStart;
		paramBuffer->z = _epsilon;
		paramBuffer->w = _powFactor;
	}

}
