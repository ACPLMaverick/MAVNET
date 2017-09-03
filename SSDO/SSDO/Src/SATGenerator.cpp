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

void SATGenerator::Generate(const Texture* input,
	RWTexture* buffer,
	RWTexture* output) const
{
	uint32_t inputWidth(input->GetWidth());
	uint32_t inputHeight(input->GetHeight());
	uint32_t inputLevel(GetTextureLevel(inputWidth, output->GetWidth()));
	GenerateInternal(input->GetSRV(), buffer->GetSRV(), buffer->GetUAV(), 
		 output->GetUAV(), inputWidth, inputHeight, inputLevel);

	// TETIN!
	/*output->AcquireDeviceData();
	PrintRawData(input);
	PrintRawData(output);

	int i = 5;
	i++;*/
}

void SATGenerator::Generate(const GBuffer::RenderTarget* input,
	RWTexture* buffer,
	RWTexture* output) const
{
	uint32_t inputWidth(System::GetInstance()->GetOptions()._windowWidth / GBuffer::PP_BUFFER_SIZE_DIVISOR);
	uint32_t inputHeight(System::GetInstance()->GetOptions()._windowHeight / GBuffer::PP_BUFFER_SIZE_DIVISOR);
	uint32_t inputLevel(GetTextureLevel(inputWidth, output->GetWidth()));
	GenerateInternal(input->SRV, buffer->GetSRV(), buffer->GetUAV(),
		output->GetUAV(), inputWidth, inputHeight, inputLevel);
}

inline void SATGenerator::GenerateInternal(
	ID3D11ShaderResourceView* const inputSRV,
	ID3D11ShaderResourceView* const bufferSRV,
	ID3D11UnorderedAccessView* bufferUAV,
	ID3D11UnorderedAccessView* outputUAV,
	uint32_t inputWidth, uint32_t inputHeight, uint32_t inputLevel) const
{
	ID3D11DeviceContext* deviceContext(Renderer::GetInstance()->GetDeviceContext());
	
	inputWidth >>= inputLevel;
	inputHeight >>= inputLevel;

	ComputeShader::SATBuffer* buf = _shader->MapConstantBuffer<ComputeShader::SATBuffer>();
	buf->WidthWidthpowLevel[0] = inputWidth;
	buf->WidthWidthpowLevel[1] = GetPowerOfTwoHigherThan(inputWidth);
	buf->WidthWidthpowLevel[2] = inputLevel;
	buf->Vertical = false;
	_shader->UnmapConstantBuffer();
	_shader->Set(1);

	deviceContext->CSSetShaderResources(4, 1, &inputSRV);

	deviceContext->CSSetUnorderedAccessViews(0, 1, &bufferUAV, nullptr);

	uint32_t threadGroupCountX, threadGroupCountY;

	threadGroupCountX = inputWidth / (_shader->GetThreadCountInGroupX() * 2) + 1;
	threadGroupCountY = inputHeight / _shader->GetThreadCountInGroupY();

	_shader->Dispatch(threadGroupCountX, threadGroupCountY, 1);

	ID3D11UnorderedAccessView* nullik[2]{ nullptr, nullptr };
	deviceContext->CSSetUnorderedAccessViews(0, 2, nullik, nullptr);

	buf = _shader->MapConstantBuffer<ComputeShader::SATBuffer>();
	buf->WidthWidthpowLevel[0] = inputHeight;
	buf->WidthWidthpowLevel[1] = GetPowerOfTwoHigherThan(inputHeight);
	buf->WidthWidthpowLevel[2] = 0;
	buf->Vertical = true;
	_shader->UnmapConstantBuffer();

	deviceContext->CSSetShaderResources(4, 1, &bufferSRV);

	deviceContext->CSSetUnorderedAccessViews(0, 1, &outputUAV, nullptr);

	threadGroupCountX = inputHeight / (_shader->GetThreadCountInGroupX() * 2) + 1;
	threadGroupCountY = inputWidth / _shader->GetThreadCountInGroupY();

	_shader->Dispatch(threadGroupCountX, threadGroupCountY, 1);

	deviceContext->CSSetShaderResources(4, 1, reinterpret_cast<ID3D11ShaderResourceView**>(nullik));
	deviceContext->CSSetUnorderedAccessViews(0, 2, nullik, nullptr);
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

inline uint32_t SATGenerator::GetPowerOfTwoHigherThan(uint32_t val) const
{
	--val;
	val |= val >> 1;
	val |= val >> 2;
	val |= val >> 4;
	val |= val >> 8;
	val |= val >> 16;
	return val + 1;
}

inline uint32_t SATGenerator::GetTextureLevel(uint32_t inputWidth, uint32_t outputWidth) const
{
	uint32_t level = 0;
	while (inputWidth > outputWidth)
	{
		inputWidth >>= 1;
		++level;
	}
	return level;
}
