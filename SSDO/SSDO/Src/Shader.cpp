#include "stdafx.h"
#include "Shader.h"
#include "Renderer.h"
#include "Lights\LightAmbient.h"
#include "Lights\LightDirectional.h"
#include "Lights\LightPoint.h"
using namespace Lights;

#include <vector>
#include <fstream>

std::wstring PATH_PREFIX = L"./Resources/Shaders/_build/";
std::wstring PATH_SUFFIX_VS = L"_VS.cso";
std::wstring PATH_SUFFIX_HS = L"_HS.cso";
std::wstring PATH_SUFFIX_GS = L"_GS.cso";
std::wstring PATH_SUFFIX_DS = L"_DS.cso";
std::wstring PATH_SUFFIX_PS = L"_PS.cso";

Shader::Shader(const wstring& shaderFilename, size_t inputLayoutNumElements, 
	ConstantBufferDesc* cbVsDescs, size_t cbVsCount, ConstantBufferDesc* cbPsDescs, size_t cbPsCount)
{
	ID3D11Device* device = Renderer::GetInstance()->GetDevice();

	// loading compiled shaders from files
	uint8_t* vecVS, *vecPS;
	int64_t sizeVS, sizePS;

	wstring pathVS = PATH_PREFIX + shaderFilename + PATH_SUFFIX_VS;
	wstring pathPS = PATH_PREFIX + shaderFilename + PATH_SUFFIX_PS;

	ifstream streamVS(pathVS, ios::binary);
	ASSERT(streamVS.is_open());
	streamVS.seekg(0, streamVS.end);
	sizeVS = streamVS.tellg();
	streamVS.seekg(0, streamVS.beg);
	vecVS = new uint8_t[sizeVS];
	streamVS.read(reinterpret_cast<char*>(vecVS), sizeVS);
	streamVS.close();

	device->CreateVertexShader(vecVS, sizeVS, nullptr, &_vs);
	ASSERT(_vs != nullptr);

	ifstream streamPS(pathPS, ios::binary);
	if (streamPS.is_open())
	{
		streamPS.seekg(0, streamPS.end);
		sizePS = streamPS.tellg();
		streamPS.seekg(0, streamPS.beg);
		vecPS = new uint8_t[sizePS];
		streamPS.read(reinterpret_cast<char*>(vecPS), sizePS);
		streamPS.close();

		device->CreatePixelShader(vecPS, sizePS, nullptr, &_ps);
		ASSERT(_ps != nullptr);
		delete[] vecPS;
	}
	
	// create vertex layout description
	D3D11_INPUT_ELEMENT_DESC layout[3];
	layout[0].SemanticName = "POSITION";
	layout[0].SemanticIndex = 0;
	layout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	layout[0].InputSlot = 0;
	layout[0].AlignedByteOffset = 0;
	layout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	layout[0].InstanceDataStepRate = 0;

	layout[1].SemanticName = "TEXCOORD";
	layout[1].SemanticIndex = 0;
	layout[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	layout[1].InputSlot = 1;
	layout[1].AlignedByteOffset = 0;
	layout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	layout[1].InstanceDataStepRate = 0;

	layout[2].SemanticName = "NORMAL";
	layout[2].SemanticIndex = 0;
	layout[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	layout[2].InputSlot = 2;
	layout[2].AlignedByteOffset = 0;
	layout[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	layout[2].InstanceDataStepRate = 0;

	if (inputLayoutNumElements > 0)
	{
		device->CreateInputLayout(layout, (UINT)inputLayoutNumElements, vecVS, sizeVS, &_inputLayout);
		ASSERT(_inputLayout != nullptr);
	}

	delete[] vecVS;

	// creating GPU Constant Buffers for shaders

	D3D11_BUFFER_DESC bDesc;
	bDesc.Usage = D3D11_USAGE_DYNAMIC;
	bDesc.ByteWidth = 0;
	bDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bDesc.MiscFlags = 0;
	bDesc.StructureByteStride = 0;

	_constantVsBufferCount = cbVsCount;
	_constantVsBufferCount > 0 ? _constantVsBuffers = new ID3D11Buffer*[_constantVsBufferCount] : _constantPsBuffers = nullptr;

	for (int i = 0; i < _constantVsBufferCount; ++i)
	{
		bDesc.ByteWidth = (UINT)cbVsDescs[i].Size;
		ASSERT_X(device->CreateBuffer(&bDesc, nullptr, &_constantVsBuffers[i]));
	}

	if (_ps != nullptr)
	{
		_constantPsBufferCount = cbPsCount;
		_constantPsBufferCount > 0 ? _constantPsBuffers = new ID3D11Buffer*[_constantPsBufferCount] : _constantPsBuffers = nullptr;

		for (int i = 0; i < _constantPsBufferCount; ++i)
		{
			bDesc.ByteWidth = (UINT)cbPsDescs[i].Size;
			ASSERT_X(device->CreateBuffer(&bDesc, nullptr, &_constantPsBuffers[i]));
		}
	}
}


Shader::~Shader()
{
	_vs->Release();
	_vs = nullptr;

	if (_ps != nullptr)
	{
		_ps->Release();
		_ps = nullptr;
	}

	if (_inputLayout != nullptr)
	{
		_inputLayout->Release();
		_inputLayout = nullptr;
	}

	for (int i = 0; i < _constantVsBufferCount; ++i)
	{
		_constantVsBuffers[i]->Release();
	}
	if(_constantVsBuffers != nullptr) delete[] _constantVsBuffers;

	for (int i = 0; i < _constantPsBufferCount; ++i)
	{
		_constantPsBuffers[i]->Release();
	}
	if (_constantPsBuffers != nullptr) delete[] _constantPsBuffers;
}

void Shader::Set() const
{
	ID3D11DeviceContext* deviceContext = Renderer::GetInstance()->GetDeviceContext();

	deviceContext->IASetInputLayout(_inputLayout);
	deviceContext->VSSetShader(_vs, nullptr, 0);
	deviceContext->PSSetShader(_ps, nullptr, 0);

	if(_constantVsBufferCount > 0)
		deviceContext->VSSetConstantBuffers(0, static_cast<UINT>(_constantVsBufferCount), _constantVsBuffers);

	if (_constantPsBufferCount > 0)
		deviceContext->PSSetConstantBuffers(0, static_cast<UINT>(_constantPsBufferCount), _constantPsBuffers);
}

void * Shader::MapVsBuffer(size_t i) const
{
	ID3D11DeviceContext* deviceContext = Renderer::GetInstance()->GetDeviceContext();
	D3D11_MAPPED_SUBRESOURCE sr;
	deviceContext->Map(_constantVsBuffers[i], 0, D3D11_MAP_WRITE_DISCARD, 0, &sr);
	return sr.pData;
}

void Shader::UnmapVsBuffer(size_t i) const
{
	ID3D11DeviceContext* deviceContext = Renderer::GetInstance()->GetDeviceContext();
	deviceContext->Unmap(_constantVsBuffers[i], 0);
}

void * Shader::MapPsBuffer(size_t i) const
{
	ID3D11DeviceContext* deviceContext = Renderer::GetInstance()->GetDeviceContext();
	D3D11_MAPPED_SUBRESOURCE sr;
	deviceContext->Map(_constantPsBuffers[i], 0, D3D11_MAP_WRITE_DISCARD, 0, &sr);
	return sr.pData;
}

void Shader::UnmapPsBuffer(size_t i) const
{
	ID3D11DeviceContext* deviceContext = Renderer::GetInstance()->GetDeviceContext();
	deviceContext->Unmap(_constantPsBuffers[i], 0);
}

Shader * Shader::CreateResource(const std::wstring & name)
{
	if (name == L"ColorShader")
	{
		ConstantBufferDesc vsDesc(sizeof(ColorBufferVS));
		ConstantBufferDesc psDesc(sizeof(ColorBufferPS));
		return new Shader(name, 3, &vsDesc, 1, &psDesc, 1);
	}
	else if (name == L"TextShader")
	{
		ConstantBufferDesc vsDesc(sizeof(TextConstantBuffer));
		return new Shader(name, 2, &vsDesc, 1, nullptr, 0);
	}
	else if (name == L"DeferredDrawShader" || 
		name == L"DeferredLightMergeShader")
	{
		return new Shader(name, 2, nullptr, 0, nullptr, 0);
	}
	else if (name == L"DeferredLightAmbientShader")
	{
		ConstantBufferDesc psDesc(sizeof(LightAmbient));
		return new Shader(name, 0, nullptr, 0, &psDesc, 1);
	}
	else if (name == L"DeferredLightDirectionalShader")
	{
		ConstantBufferDesc descs[2] = { sizeof(LightCommonDataPS), sizeof(LightPoint) };
		return new Shader(name, 0, nullptr, 0, descs, 2);
	}
	else if (name == L"DeferredLightPointShader")
	{
		ConstantBufferDesc descs[2] = { sizeof(LightCommonDataPS), sizeof(LightPoint) };
		return new Shader(name, 0, nullptr, 0, descs, 2);
	}
	else if (name == L"PPSepia")
	{
		return new Shader(name, 0, nullptr, 0, nullptr, 0);
	}
	else if (name == L"SimpleSSAO_Base")
	{
		ConstantBufferDesc descs[2] = { sizeof(LightCommonDataPS), sizeof(SSAOBasePS) };
		return new Shader(name, 0, nullptr, 0, descs, 2);
	}
	else if (name == L"SimpleSSAO_BlurMerge")
	{
		ConstantBufferDesc descs[2] = { sizeof(LightCommonDataPS), sizeof(SSAOBlurMergePS) };
		return new Shader(name, 0, nullptr, 0, descs, 2);
	}
	else if (name == L"SSDOBase_Base")
	{
		ConstantBufferDesc descs[2] = { sizeof(LightCommonDataPS), sizeof(SSAOBasePS) };
		return new Shader(name, 0, nullptr, 0, descs, 2);
	}
	else if (name == L"SSDOBase_BlurMerge")
	{
		ConstantBufferDesc descs[2] = { sizeof(LightCommonDataPS), sizeof(SSAOBlurMergePS) };
		return new Shader(name, 0, nullptr, 0, descs, 2);
	}
	else
	{
		ASSERT(false);
		return nullptr;
	}
}
