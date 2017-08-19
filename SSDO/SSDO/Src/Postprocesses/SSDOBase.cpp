#include "stdafx.h"
#include "SSDOBase.h"
#include "System.h"
#include "Renderer.h"
#include "Random.h"
#include "Camera.h"
#include "Scenes\Scene.h"
#include "SimpleSSAO.h"
#include "Lights/LightDirectional.h"

namespace Postprocesses
{
	SSDOBase::SSDOBase() :
		_dataBuffer(nullptr),
		_randomVectorTexture(nullptr),
		_maxDistance(0.7f),
		_fadeStart(0.02f),
		_epsilon(0.01f),
		_powFactor(1.0f)
	{
		RandomVectorsGenerator::Generate(&_randomVectorTexture, 1024);

		_shaders.push_back(System::GetInstance()->GetScene()->LoadShader(std::wstring(L"SSDOBase_Base")));
		_shaders.push_back(System::GetInstance()->GetScene()->LoadShader(std::wstring(L"SSDOBase_BlurMerge")));

		ID3D11Device* device = Renderer::GetInstance()->GetDevice();

		D3D11_BUFFER_DESC bDesc;
		bDesc.Usage = D3D11_USAGE_DYNAMIC;
		bDesc.ByteWidth = sizeof(Shader::SSDOBasePS);
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

	SSDOBase::~SSDOBase()
	{
		delete _randomVectorTexture;
	}

	void SSDOBase::Update()
	{
	}

	void SSDOBase::SetPass(const Camera & camera, int32_t passIndex) const
	{
		Postprocess::SetPass(camera, passIndex);

		if (passIndex == 0)
		{
			Shader::SSDOBasePS* buffer = reinterpret_cast<Shader::SSDOBasePS*>(_shaders[0]->MapPsBuffer(1));
			XMMATRIX proj = XMLoadFloat4x4A(&camera.GetMatProj());
			XMStoreFloat4x4A(&buffer->Proj, XMMatrixTranspose(proj));
			memcpy(&buffer->Offsets, _offsets, SAMPLE_COUNT * sizeof(XMFLOAT4A));
			FillParams(&buffer->Params);
			reinterpret_cast<Lights::LightDirectional*>(&buffer->LightColor)->ApplyToShader(
				*System::GetInstance()->GetScene()->GetLightsDirectional()[0],
				camera
			);

			_shaders[0]->UnmapPsBuffer(1);

			_randomVectorTexture->Set(5);
		}
		else if (passIndex == 1)
		{
			Shader::SSDOBlurMergePS* buffer = reinterpret_cast<Shader::SSDOBlurMergePS*>(_shaders[1]->MapPsBuffer(1));
			buffer->TexelSize = XMFLOAT2A(1.0f / (float)System::GetInstance()->GetOptions()._windowWidth,
				1.0f / (float)System::GetInstance()->GetOptions()._windowHeight);
			_shaders[1]->UnmapPsBuffer(1);
		}
	}

	inline void SSDOBase::FillParams(XMFLOAT4A * paramBuffer) const
	{
		paramBuffer->x = _maxDistance;
		paramBuffer->y = _fadeStart;
		paramBuffer->z = _epsilon;
		paramBuffer->w = _powFactor;
	}

}
