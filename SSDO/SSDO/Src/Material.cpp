#include "stdafx.h"
#include "Material.h"
#include "Renderer.h"
#include "Camera.h"
#include "Mesh.h"

#include <vector>
#include <fstream>

std::wstring PATH_PREFIX = L"./Resources/Shaders/";
std::wstring PATH_SUFFIX_VS = L"_VS.cso";
std::wstring PATH_SUFFIX_HS = L"_HS.cso";
std::wstring PATH_SUFFIX_GS = L"_GS.cso";
std::wstring PATH_SUFFIX_DS = L"_DS.cso";
std::wstring PATH_SUFFIX_PS = L"_PS.cso";

Material::Material(const wstring & shaderFilename, const XMFLOAT4 & colorBase, const XMFLOAT4 & colorSpecular, float gloss) :
	_colorBase(colorBase),
	_colorSpecular(colorSpecular),
	_gloss(gloss)
{
	LoadShader(shaderFilename);
}

Material::~Material()
{
	_vs->Release();
	_vs = nullptr;

	_ps->Release();
	_ps = nullptr;

	_inputLayout->Release();
	_inputLayout = nullptr;

	_bufferPerObject->Release();
	_bufferPerObject = nullptr;

	_bufferPerFrame->Release();
	_bufferPerFrame = nullptr;
}

void Material::Update()
{
}

void Material::DrawMesh(const Camera & camera, const Mesh & mesh) const
{
	// set shader parameters
	BufferPerObject* buffer;
	D3D11_MAPPED_SUBRESOURCE mappedRes;
	ID3D11DeviceContext* deviceContext = Renderer::GetInstance()->GetDeviceContext();


	deviceContext->IASetInputLayout(_inputLayout);
	deviceContext->VSSetShader(_vs, nullptr, 0);
	deviceContext->PSSetShader(_ps, nullptr, 0);

	XMFLOAT4X4 wvp;
	XMMATRIX w = XMLoadFloat4x4(&mesh._matWorld);
	XMMATRIX vp = XMLoadFloat4x4(&camera.GetMatViewProj());
	XMMATRIX transposedWVP = XMMatrixTranspose(w * vp);
	XMStoreFloat4x4(&wvp, transposedWVP);

	XMMATRIX transposedW = XMMatrixTranspose(w);
	XMMATRIX transposedWInvTransp = XMMatrixTranspose(XMLoadFloat4x4(&mesh._matWorldInvTransp));

	// set vertex and index buffers

	deviceContext->Map(_bufferPerObject, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedRes);

	buffer = reinterpret_cast<BufferPerObject*>(mappedRes.pData);

	XMStoreFloat4x4(&buffer->gMatWVP, transposedWVP);
	XMStoreFloat4x4(&buffer->gMatW, transposedW);
	XMStoreFloat4x4(&buffer->gMatWInvTransp, transposedWInvTransp);
	memcpy(&buffer->gColBase, &_colorBase, sizeof(XMFLOAT4));
	memcpy(&buffer->gColSpecular, &_colorSpecular, sizeof(XMFLOAT4));
	buffer->gGloss = _gloss;

	deviceContext->Unmap(_bufferPerObject, 0);

	deviceContext->VSSetConstantBuffers(0, 1, &_bufferPerObject);
	deviceContext->PSSetConstantBuffers(0, 1, &_bufferPerObject);

	// DRAW!

	uint32_t stridePos = sizeof(mesh._vPositions[0]);
	uint32_t strideNrms = sizeof(mesh._vNormals[0]);
	uint32_t strideUvs = sizeof(mesh._vUvs[0]);
	uint32_t offset = 0;
	deviceContext->IASetVertexBuffers(0, 1, &mesh._fPositions, &stridePos, &offset);	// this can be simplified
	deviceContext->IASetVertexBuffers(1, 1, &mesh._fNormals, &strideNrms, &offset);
	deviceContext->IASetVertexBuffers(2, 1, &mesh._fUvs, &strideUvs, &offset);
	deviceContext->IASetIndexBuffer(mesh._fIndices, DXGI_FORMAT_R16_UINT, 0);

	deviceContext->DrawIndexed(static_cast<uint32_t>(mesh._indices.GetSize() * 3), 0, 0);
}

inline void Material::LoadShader(const wstring & shaderFilename)
{
	ID3D11Device* device = Renderer::GetInstance()->GetDevice();

	// loading compiled shaders from files
	uint8_t* vecVS, *vecPS;
	int64_t sizeVS, sizePS;

	wstring pathVS = PATH_PREFIX + shaderFilename + L"/" + shaderFilename + PATH_SUFFIX_VS;
	wstring pathPS = PATH_PREFIX + shaderFilename + L"/" + shaderFilename + PATH_SUFFIX_PS;

	ifstream streamVS(pathVS, ios::binary);
	ASSERT(streamVS.is_open());
	streamVS.seekg(0, streamVS.end);
	sizeVS = streamVS.tellg();
	streamVS.seekg(0, streamVS.beg);
	vecVS = new uint8_t[sizeVS];
	streamVS.read(reinterpret_cast<char*>(vecVS), sizeVS);
	streamVS.close();

	ifstream streamPS(pathPS, ios::binary);
	ASSERT(streamPS.is_open());
	streamPS.seekg(0, streamPS.end);
	sizePS = streamPS.tellg();
	streamPS.seekg(0, streamPS.beg);
	vecPS = new uint8_t[sizePS];
	streamPS.read(reinterpret_cast<char*>(vecPS), sizePS);
	streamPS.close();

	// creating shader resources
	HRESULT hr = device->CreateVertexShader(vecVS, sizeVS, nullptr, &_vs);
	hr = device->CreatePixelShader(vecPS, sizePS, nullptr, &_ps);
	ASSERT(_vs != nullptr && _ps != nullptr);

	// create vertex layout description
	D3D11_INPUT_ELEMENT_DESC layout[3];
	layout[0].SemanticName = "POSITION";
	layout[0].SemanticIndex = 0;
	layout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	layout[0].InputSlot = 0;
	layout[0].AlignedByteOffset = 0;
	layout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	layout[0].InstanceDataStepRate = 0;

	layout[1].SemanticName = "NORMAL";
	layout[1].SemanticIndex = 0;
	layout[1].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	layout[1].InputSlot = 1;
	layout[1].AlignedByteOffset = 0;
	layout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	layout[1].InstanceDataStepRate = 0;

	layout[2].SemanticName = "TEXCOORD";
	layout[2].SemanticIndex = 0;
	layout[2].Format = DXGI_FORMAT_R32G32_FLOAT;
	layout[2].InputSlot = 2;
	layout[2].AlignedByteOffset = 0;
	layout[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	layout[2].InstanceDataStepRate = 0;

	device->CreateInputLayout(layout, 3, vecVS, sizeVS, &_inputLayout);
	ASSERT(_inputLayout != nullptr);

	delete[] vecVS;
	delete[] vecPS;

	// creating GPU Constant Buffers for shaders

	D3D11_BUFFER_DESC bDesc;
	bDesc.Usage = D3D11_USAGE_DYNAMIC;
	bDesc.ByteWidth = sizeof(BufferPerObject);
	bDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bDesc.MiscFlags = 0;
	bDesc.StructureByteStride = 0;

	hr = device->CreateBuffer(&bDesc, nullptr, &_bufferPerObject);
	
	bDesc.ByteWidth = sizeof(BufferPerFrame);

	device->CreateBuffer(&bDesc, nullptr, &_bufferPerFrame);

	ASSERT(_bufferPerObject != nullptr && _bufferPerFrame != nullptr);
}