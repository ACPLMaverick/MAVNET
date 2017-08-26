#include "stdafx.h"
#include "SATGenerator.h"
#include "Texture.h"
#include "RWTexture.h"
#include "ComputeShader.h"
#include "System.h"
#include "Scenes\Scene.h"
#include "Renderer.h"


SATGenerator::SATGenerator() : 
	_globalTempBuffer(16, sizeof(TempBufferData))
{
	_shader = System::GetInstance()->GetScene()->LoadComputeShader(L"SAT");
}

SATGenerator::~SATGenerator()
{
}

void SATGenerator::Generate(const Texture* inputA, const Texture* inputB, RWTexture* outputA, RWTexture* outputB) const
{
	//ASSERT(inputA->GetWidth() == inputB->GetWidth());
	//ASSERT(outputA->GetWidth() == outputB->GetWidth());
	uint32_t inputWidth(inputA->GetWidth());
	uint32_t inputHeight(inputA->GetHeight());
	uint32_t inputLevel(inputWidth / outputA->GetWidth() - 1);
	GenerateInternal(inputA->GetSampler(), inputA->GetSRV(), inputB->GetSampler(), inputB->GetSRV(), 
		outputA->GetUAV(), outputB->GetUAV(), inputWidth, inputHeight, inputLevel);

	// TETIN!
	outputA->AcquireDeviceData();
	PackedVector::XMHALF4* data(reinterpret_cast<PackedVector::XMHALF4*>(outputA->GetRawData().GetDataPtr()));
	std::wstring dbg = L"\n";
	for (size_t i = 0; i < outputA->GetHeight(); ++i)
	{
		for (size_t j = 0; j < outputA->GetWidth(); ++j)
		{
			XMFLOAT4 field;
			field.x = PackedVector::XMConvertHalfToFloat(data[outputA->GetWidth() * i + j].x);
			field.y = PackedVector::XMConvertHalfToFloat(data[outputA->GetWidth() * i + j].y);
			field.z = PackedVector::XMConvertHalfToFloat(data[outputA->GetWidth() * i + j].z);
			field.w = PackedVector::XMConvertHalfToFloat(data[outputA->GetWidth() * i + j].w);
			dbg += L" [ " + to_wstring(field.x) + L" , " + to_wstring(field.y) + L" , " + to_wstring(field.z) + L" , " + to_wstring(field.w) + L" ] ";
		}
		dbg += L"\n";
	}

	OutputDebugString(dbg.c_str());

	dbg += L"c";
}

void SATGenerator::Generate(const GBuffer::RenderTarget* inputA, const GBuffer::RenderTarget* inputB,
	RWTexture* outputA, RWTexture* outputB) const
{
	ASSERT(outputA->GetWidth() == outputB->GetWidth());
	uint32_t inputWidth(System::GetInstance()->GetOptions()._windowWidth / GBuffer::PP_BUFFER_SIZE_DIVISOR);
	uint32_t inputHeight(System::GetInstance()->GetOptions()._windowHeight / GBuffer::PP_BUFFER_SIZE_DIVISOR);
	uint32_t inputLevel(inputWidth / outputA->GetWidth() - 1);
	GenerateInternal(inputA->Sampler, inputA->SRV, inputB->Sampler, inputB->SRV,
		outputA->GetUAV(), outputB->GetUAV(), inputWidth, inputHeight, inputLevel);
}

inline void SATGenerator::GenerateInternal(
	ID3D11SamplerState* const inputSamplerA, ID3D11ShaderResourceView* const inputSRVA,
	ID3D11SamplerState* const inputSamplerB, ID3D11ShaderResourceView* const inputSRVB,
	ID3D11UnorderedAccessView* outputUAVA, ID3D11UnorderedAccessView* outputUAVB,
	uint32_t inputWidth, uint32_t inputHeight, uint32_t inputLevel) const
{
	ID3D11DeviceContext* deviceContext(Renderer::GetInstance()->GetDeviceContext());
	
	ComputeShader::SATBuffer* buf = _shader->MapConstantBuffer<ComputeShader::SATBuffer>();
	buf->WidthHeightLevel[0] = inputWidth;
	buf->WidthHeightLevel[1] = inputHeight;
	buf->WidthHeightLevel[2] = inputLevel;
	buf->Vertical = false;
	_shader->UnmapConstantBuffer();
	_shader->Set(1);

	deviceContext->CSSetShaderResources(4, 1, &inputSRVA);
	//deviceContext->CSSetShaderResources(5, 1, &inputSRVB);

	deviceContext->CSSetUnorderedAccessViews(0, 1, &outputUAVA, nullptr);
	//deviceContext->CSSetUnorderedAccessViews(1, 1, &outputUAVB, nullptr);
	_globalTempBuffer.Set(2);

	uint32_t threadGroupCountX(inputWidth / _shader->GetThreadCountInGroupX() + 1);
	uint32_t threadGroupCountY(inputHeight / _shader->GetThreadCountInGroupY());

	_shader->Dispatch(threadGroupCountX, threadGroupCountY, 1);
}
