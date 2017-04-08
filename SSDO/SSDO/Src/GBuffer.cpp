#include "stdafx.h"
#include "GBuffer.h"
#include "System.h"
#include "Renderer.h"
#include "Scenes\Scene.h"
#include "Lights\LightAmbient.h"
#include "Lights\LightDirectional.h"
#include "Lights\LightPoint.h"

#define SHADER_DRAW L"DeferredDrawShader"
#define SHADER_MERGE L"DeferredLightMergeShader"
#define SHADER_L_AMBIENT L"DeferredLightAmbientShader"
#define SHADER_L_DIRECTIONAL L"DeferredLightDirectionalShader"
#define SHADER_L_POINT L"DeferredLightPointShader"

GBuffer::GBuffer()
{
	const uint32_t w = System::GetInstance()->GetOptions()._windowWidth;
	const uint32_t h = System::GetInstance()->GetOptions()._windowHeight;
	const DXGI_FORMAT format = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	const DXGI_FORMAT formatDepthTexture = DXGI_FORMAT::DXGI_FORMAT_R32_TYPELESS;
	const DXGI_FORMAT formatDepthDsv = DXGI_FORMAT::DXGI_FORMAT_D32_FLOAT;
	const DXGI_FORMAT formatDepthSrv = DXGI_FORMAT::DXGI_FORMAT_R32_FLOAT;
	ID3D11Device* device = Renderer::GetInstance()->GetDevice();

	D3D11_TEXTURE2D_DESC descTexture;
	descTexture.ArraySize = 1;
	descTexture.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	descTexture.CPUAccessFlags = 0;
	descTexture.Format = format;
	descTexture.MipLevels = 1;
	descTexture.MiscFlags = 0;
	descTexture.Usage = D3D11_USAGE_DEFAULT;
	descTexture.Width = w;
	descTexture.Height = h;
	descTexture.SampleDesc.Count = 1;
	descTexture.SampleDesc.Quality = 0;

	D3D11_SAMPLER_DESC descSampler;
	descSampler.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	descSampler.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	descSampler.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	descSampler.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	descSampler.MipLODBias = 0.0f;
	descSampler.MaxAnisotropy = 1;
	descSampler.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	descSampler.BorderColor[0] = 0;
	descSampler.BorderColor[1] = 0;
	descSampler.BorderColor[2] = 0;
	descSampler.BorderColor[3] = 0;
	descSampler.MinLOD = 0;
	descSampler.MaxLOD = D3D11_FLOAT32_MAX;

	D3D11_SHADER_RESOURCE_VIEW_DESC descSrv;
	ZeroMemory(&descSrv, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
	descSrv.Format = format;
	descSrv.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	descSrv.Texture2D.MipLevels = 1;
	descSrv.Texture2D.MostDetailedMip = 0;

	D3D11_TEXTURE2D_DESC descDs;
	descDs.Width = w;
	descDs.Height = h;
	descDs.MipLevels = 1;
	descDs.ArraySize = 1;
	descDs.Format = formatDepthTexture;
	descDs.SampleDesc.Count = 1;
	descDs.SampleDesc.Quality = 0;
	descDs.Usage = D3D11_USAGE_DEFAULT;
	descDs.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	descDs.CPUAccessFlags = 0;
	descDs.MiscFlags = 0;
	

	ASSERT(device->CreateTexture2D(&descTexture, nullptr, &_color.Texture) == S_OK);
	ASSERT(device->CreateTexture2D(&descTexture, nullptr, &_normal.Texture) == S_OK);
	ASSERT(device->CreateTexture2D(&descTexture, nullptr, &_outputA.Texture) == S_OK);
	ASSERT(device->CreateTexture2D(&descTexture, nullptr, &_outputB.Texture) == S_OK);
	ASSERT(device->CreateTexture2D(&descDs, nullptr, &_depth.Texture) == S_OK);

	ASSERT(device->CreateRenderTargetView(_color.Texture, nullptr, &_color.View) == S_OK);
	ASSERT(device->CreateRenderTargetView(_normal.Texture, nullptr, &_normal.View) == S_OK);
	ASSERT(device->CreateRenderTargetView(_outputA.Texture, nullptr, &_outputA.View) == S_OK);
	ASSERT(device->CreateRenderTargetView(_outputB.Texture, nullptr, &_outputB.View) == S_OK);

	ASSERT(device->CreateSamplerState(&descSampler, &_color.Sampler) == S_OK);
	ASSERT(device->CreateSamplerState(&descSampler, &_normal.Sampler) == S_OK);
	ASSERT(device->CreateSamplerState(&descSampler, &_outputA.Sampler) == S_OK);
	ASSERT(device->CreateSamplerState(&descSampler, &_outputB.Sampler) == S_OK);
	ASSERT(device->CreateSamplerState(&descSampler, &_depth.Sampler) == S_OK);

	ASSERT(device->CreateShaderResourceView(_color.Texture, &descSrv, &_color.SRV) == S_OK);
	ASSERT(device->CreateShaderResourceView(_normal.Texture, &descSrv, &_normal.SRV) == S_OK);
	ASSERT(device->CreateShaderResourceView(_outputA.Texture, &descSrv, &_outputA.SRV) == S_OK);
	ASSERT(device->CreateShaderResourceView(_outputB.Texture, &descSrv, &_outputB.SRV) == S_OK);
	descSrv.Format = formatDepthSrv;
	ASSERT(device->CreateShaderResourceView(_depth.Texture, &descSrv, &_depth.SRV) == S_OK);

	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
	ZeroMemory(&dsvDesc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
	dsvDesc.Format = formatDepthDsv;
	dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	dsvDesc.Texture2D.MipSlice = 0;
	ASSERT(device->CreateDepthStencilView(_depth.Texture, &dsvDesc, &_depth.DepthStencilView) == S_OK);


	_shaderDraw = System::GetInstance()->GetScene().LoadShader(std::wstring(SHADER_DRAW));
	_shaderMerge = System::GetInstance()->GetScene().LoadShader(std::wstring(SHADER_MERGE));
	_shaderLightAmbient = System::GetInstance()->GetScene().LoadShader(std::wstring(SHADER_L_AMBIENT));
	_shaderLightDirectional = System::GetInstance()->GetScene().LoadShader(std::wstring(SHADER_L_DIRECTIONAL));
	_shaderLightPoint = System::GetInstance()->GetScene().LoadShader(std::wstring(SHADER_L_POINT));


	XMFLOAT3 fpVertices[4];
	XMFLOAT2 fpUvs[4];
	uint16_t fpIndices[6];
	fpVertices[0] = (XMFLOAT3(-1.0f, -1.0f, 0.0f));
	fpVertices[1] = (XMFLOAT3(1.0f, -1.0f, 0.0f));
	fpVertices[2] = (XMFLOAT3(1.0f, 1.0f, 0.0f));
	fpVertices[3] = (XMFLOAT3(-1.0f, 1.0f, 0.0f));
	fpUvs[0] = (XMFLOAT2(0.0f, 1.0f));
	fpUvs[1] = (XMFLOAT2(1.0f, 1.0f));
	fpUvs[2] = (XMFLOAT2(1.0f, 0.0f));
	fpUvs[3] = (XMFLOAT2(0.0f, 0.0f));
	fpIndices[0] = 0;
	fpIndices[1] = 1;
	fpIndices[2] = 2;
	fpIndices[3] = 2;
	fpIndices[4] = 3;
	fpIndices[5] = 0;
	
	D3D11_BUFFER_DESC desc;
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	desc.ByteWidth = static_cast<uint32_t>(sizeof(XMFLOAT3) * 4);
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;
	desc.StructureByteStride = 0;
	desc.Usage = D3D11_USAGE_DEFAULT;
	D3D11_SUBRESOURCE_DATA data;
	data.pSysMem = &fpVertices;
	data.SysMemPitch = 0;
	data.SysMemSlicePitch = 0;

	device->CreateBuffer(&desc, &data, &_fullscreenPlaneVertexBuffer);

	data.pSysMem = &fpUvs;
	desc.ByteWidth = static_cast<uint32_t>(sizeof(XMFLOAT2) * 4);
	device->CreateBuffer(&desc, &data, &_fullscreenPlaneUvBuffer);

	data.pSysMem = &fpIndices;
	desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	desc.ByteWidth = static_cast<uint32_t>(sizeof(int16_t) * 6);
	device->CreateBuffer(&desc, &data, &_fullscreenPlaneIndexBuffer);
}


GBuffer::~GBuffer()
{
	_color.Shutdown();
	_normal.Shutdown();
	_depth.Shutdown();
	_outputA.Shutdown();
	_outputB.Shutdown();
}

void GBuffer::SetDrawMeshes()
{
	ID3D11DeviceContext* context = Renderer::GetInstance()->GetDeviceContext();
	
	ID3D11RenderTargetView* rtPtrs[2] = { _color.View, _normal.View };
	context->OMSetRenderTargets(2, rtPtrs, _depth.DepthStencilView);

	const float black[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	context->ClearRenderTargetView(_color.View, black);
	context->ClearRenderTargetView(_normal.View, black);
	context->ClearRenderTargetView(_outputA.View, black);
	context->ClearRenderTargetView(_outputB.View, black);
	context->ClearDepthStencilView(_depth.DepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 255);
}

void GBuffer::SetDrawLightAmbient()
{
	ID3D11DeviceContext* context = Renderer::GetInstance()->GetDeviceContext();
}

void GBuffer::SetDrawLightDirectional()
{
	ID3D11DeviceContext* context = Renderer::GetInstance()->GetDeviceContext();
}

void GBuffer::SetDrawLightPoint()
{
	ID3D11DeviceContext* context = Renderer::GetInstance()->GetDeviceContext();
}

void GBuffer::SetDrawPostprocess()
{
	ID3D11DeviceContext* context = Renderer::GetInstance()->GetDeviceContext();
	FlipOutputs();
}

void GBuffer::Draw() const
{
	ID3D11DeviceContext* context = Renderer::GetInstance()->GetDeviceContext();

	_shaderDraw->Set();
	Renderer::GetInstance()->SetMainRenderTarget();
	ID3D11ShaderResourceView* srv[3] = { _color.SRV, _normal.SRV, _depth.SRV };
	ID3D11SamplerState* ss[3] = { _color.Sampler, _normal.Sampler, _depth.Sampler };
	context->PSSetShaderResources(0, 3, srv);
	context->PSSetSamplers(0, 3, ss);


	uint32_t stride = sizeof(XMFLOAT3);
	uint32_t offset = 0;
	context->IASetVertexBuffers(0, 1, &_fullscreenPlaneVertexBuffer, &stride, &offset);
	stride = sizeof(XMFLOAT2);
	context->IASetVertexBuffers(1, 1, &_fullscreenPlaneUvBuffer, &stride, &offset);
	context->IASetIndexBuffer(_fullscreenPlaneIndexBuffer, DXGI_FORMAT::DXGI_FORMAT_R16_UINT, 0);

	context->DrawIndexed(6, 0, 0);

	ID3D11ShaderResourceView* const nullSrv[1] = { nullptr };
	ID3D11SamplerState* const nullSmp[1] = { nullptr };

	context->PSSetShaderResources(0, 1, nullSrv);
	context->PSSetShaderResources(1, 1, nullSrv);
	context->PSSetShaderResources(2, 1, nullSrv);
	context->PSSetSamplers(0, 1, nullSmp);
	context->PSSetSamplers(1, 1, nullSmp);
	context->PSSetSamplers(2, 1, nullSmp);
}

void GBuffer::Merge(const GBuffer & other)
{
	FlipOutputs();
}

inline void GBuffer::FlipOutputs()
{
	RenderTarget vmp = _outputA;
	_outputA = _outputB;
	_outputB = vmp;
}
