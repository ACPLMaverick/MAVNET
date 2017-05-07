#include "stdafx.h"
#include "SimpleSSAO.h"
#include "Shader.h"
#include "System.h"
#include "Scenes\Scene.h"
#include "Renderer.h"
#include "System.h"
#include "Random.h"
#include "Camera.h"

namespace Postprocesses
{
	SimpleSSAO::SimpleSSAO() :
		_ssaoBuffer(nullptr),
		_maxDistance(0.5f)
	{
		_shaders.push_back(System::GetInstance()->GetScene().LoadShader(std::wstring(L"SimpleSSAO_Base")));
		_shaders.push_back(System::GetInstance()->GetScene().LoadShader(std::wstring(L"SimpleSSAO_BlurMerge")));

		ID3D11Device* device = Renderer::GetInstance()->GetDevice();

		D3D11_BUFFER_DESC bDesc;
		bDesc.Usage = D3D11_USAGE_DYNAMIC;
		bDesc.ByteWidth = sizeof(Shader::SSAOBasePS);
		bDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		bDesc.MiscFlags = 0;
		bDesc.StructureByteStride = 0;

		device->CreateBuffer(&bDesc, nullptr, &_ssaoBuffer);
		ASSERT(_ssaoBuffer != nullptr);

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

	SimpleSSAO::~SimpleSSAO()
	{
	}

	void SimpleSSAO::Update()
	{
	}

	void SimpleSSAO::SetPass(const Camera & camera, int32_t passIndex) const
	{
		Postprocess::SetPass(camera, passIndex);

		if (passIndex == 0)
		{
			Shader::SSAOBasePS* buffer = reinterpret_cast<Shader::SSAOBasePS*>(_shaders[0]->MapPsBuffer(1));
			XMMATRIX proj = XMLoadFloat4x4A(&camera.GetMatProj());
			XMStoreFloat4x4A(&buffer->Proj, XMMatrixTranspose(proj));
			memcpy(&buffer->Offsets, _offsets, SAMPLE_COUNT * sizeof(XMFLOAT4A));
			FillParams(&buffer->Params);

			_shaders[0]->UnmapPsBuffer(1);
		}
		else if (passIndex == 1)
		{

		}
	}

	inline void SimpleSSAO::FillParams(XMFLOAT4A * paramBuffer) const
	{
		paramBuffer->x = _maxDistance;
		paramBuffer->y = 0.0f;
		paramBuffer->z = 0.0f;
		paramBuffer->w = 0.0f;
	}
}