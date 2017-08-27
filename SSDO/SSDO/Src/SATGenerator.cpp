#include "stdafx.h"
#include "SATGenerator.h"
#include "Texture.h"
#include "RWTexture.h"
#include "ComputeShader.h"
#include "System.h"
#include "Scenes\Scene.h"
#include "Renderer.h"


SATGenerator::SATGenerator()
{
	_shader = System::GetInstance()->GetScene()->LoadComputeShader(L"SAT");
}

SATGenerator::~SATGenerator()
{
}

void SATGenerator::Generate(const Texture* inputA, const Texture* inputB, 
	RWTexture* bufferA, RWTexture* bufferB,
	RWTexture* outputA, RWTexture* outputB) const
{
	//ASSERT(inputA->GetWidth() == inputB->GetWidth());
	//ASSERT(outputA->GetWidth() == outputB->GetWidth());
	uint32_t inputWidth(inputA->GetWidth());
	uint32_t inputHeight(inputA->GetHeight());
	uint32_t inputLevel(inputWidth / outputA->GetWidth() - 1);
	GenerateInternal(inputA->GetSRV(), inputB->GetSRV(), 
		bufferA->GetSRV(), bufferB->GetSRV(),
		bufferA->GetUAV(), bufferB->GetUAV(),
		outputA->GetUAV(), outputB->GetUAV(), inputWidth, inputHeight, inputLevel);

	// TETIN!
	outputA->AcquireDeviceData();
	PrintRawData(inputA);
	PrintRawData(outputA);

	int i = 5;
	i++;
}

void SATGenerator::Generate(const GBuffer::RenderTarget* inputA, const GBuffer::RenderTarget* inputB,
	RWTexture* bufferA, RWTexture* bufferB,
	RWTexture* outputA, RWTexture* outputB) const
{
	ASSERT(outputA->GetWidth() == outputB->GetWidth());
	uint32_t inputWidth(System::GetInstance()->GetOptions()._windowWidth / GBuffer::PP_BUFFER_SIZE_DIVISOR);
	uint32_t inputHeight(System::GetInstance()->GetOptions()._windowHeight / GBuffer::PP_BUFFER_SIZE_DIVISOR);
	uint32_t inputLevel(inputWidth / outputA->GetWidth() - 1);
	GenerateInternal(inputA->SRV, inputB->SRV,
		bufferA->GetSRV(), bufferB->GetSRV(),
		bufferA->GetUAV(), bufferB->GetUAV(),
		outputA->GetUAV(), outputB->GetUAV(), inputWidth, inputHeight, inputLevel);
}

inline void SATGenerator::GenerateInternal(
	ID3D11ShaderResourceView* const inputSRVA, ID3D11ShaderResourceView* const inputSRVB,
	ID3D11ShaderResourceView* const bufferSRVA, ID3D11ShaderResourceView* const bufferSRVB,
	ID3D11UnorderedAccessView* bufferUAVA, ID3D11UnorderedAccessView* bufferUAVB,
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

	deviceContext->CSSetUnorderedAccessViews(0, 1, &bufferUAVA, nullptr);
	//deviceContext->CSSetUnorderedAccessViews(1, 1, &bufferUAVB, nullptr);

	uint32_t threadGroupCountX, threadGroupCountY;

	threadGroupCountX = inputWidth / _shader->GetThreadCountInGroupX() + 1;
	threadGroupCountY = inputHeight / _shader->GetThreadCountInGroupY();

	_shader->Dispatch(threadGroupCountX, threadGroupCountY, 1);

	ID3D11UnorderedAccessView* nullik[2]{ nullptr, nullptr };
	deviceContext->CSSetUnorderedAccessViews(0, 2, nullik, nullptr);

	buf = _shader->MapConstantBuffer<ComputeShader::SATBuffer>();
	buf->WidthHeightLevel[0] = inputWidth;
	buf->WidthHeightLevel[1] = inputHeight;
	buf->WidthHeightLevel[2] = 0;
	buf->Vertical = true;
	_shader->UnmapConstantBuffer();

	deviceContext->CSSetShaderResources(4, 1, &bufferSRVA);
	//deviceContext->CSSetShaderResources(5, 1, &bufferSRVB);

	deviceContext->CSSetUnorderedAccessViews(0, 1, &outputUAVA, nullptr);
	//deviceContext->CSSetUnorderedAccessViews(1, 1, &outputUAVB, nullptr);

	threadGroupCountX = inputHeight / _shader->GetThreadCountInGroupX() + 1;
	threadGroupCountY = inputWidth / _shader->GetThreadCountInGroupY();

	_shader->Dispatch(threadGroupCountX, threadGroupCountY, 1);
}

inline void SATGenerator::PrintRawData(const Texture * tex) const
{
	PackedVector::XMHALF4* data(reinterpret_cast<PackedVector::XMHALF4*>(const_cast<Texture*>(tex)->GetRawData().GetDataPtr()));
	std::wstring dbg = L"\n";
	for (size_t i = 0; i < tex->GetHeight(); ++i)
	{
		for (size_t j = 0; j < tex->GetWidth(); ++j)
		{
			XMFLOAT4 field;
			field.x = PackedVector::XMConvertHalfToFloat(data[tex->GetWidth() * i + j].x);
			field.y = PackedVector::XMConvertHalfToFloat(data[tex->GetWidth() * i + j].y);
			field.z = PackedVector::XMConvertHalfToFloat(data[tex->GetWidth() * i + j].z);
			field.w = PackedVector::XMConvertHalfToFloat(data[tex->GetWidth() * i + j].w);
			dbg += L" [ " + to_wstring(field.x) + L" , " + to_wstring(field.y) + L" , " + to_wstring(field.z) + L" , " + to_wstring(field.w) + L" ] ";
		}
		dbg += L"\n";
	}

	OutputDebugString(dbg.c_str());
}
