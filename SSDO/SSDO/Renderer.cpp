#include "stdafx.h"
#include "Renderer.h"
#include "System.h"

Renderer::Renderer()
{
}


Renderer::~Renderer()
{
}

void Renderer::Initialize()
{
	// create device and device context
	D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0, nullptr, 0, D3D11_SDK_VERSION, &_device, nullptr, &_deviceContext);

	// create swap chain
	IDXGIDevice* dxgiDevice = nullptr;
	_device->QueryInterface(__uuidof(IDXGIDevice), (reinterpret_cast<void**>(&dxgiDevice)));

	ASSERT(dxgiDevice != nullptr);

	IDXGIAdapter* dxgiAdapter = nullptr;
	dxgiDevice->GetParent(__uuidof(IDXGIAdapter), reinterpret_cast<void**>(&dxgiAdapter));

	ASSERT(dxgiAdapter != nullptr);

	IDXGIFactory* dxgiFactory = nullptr;
	dxgiAdapter->GetParent(__uuidof(IDXGIFactory), reinterpret_cast<void**>(&dxgiFactory));

	ASSERT(dxgiFactory != nullptr);

	UINT sampleQuality = 0;
	_device->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 4, &sampleQuality);

	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	swapChainDesc.BufferDesc.Width = System::GetInstance()->GetOptions()._windowWidth;
	swapChainDesc.BufferDesc.Height = System::GetInstance()->GetOptions()._windowHeight;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = System::GetInstance()->GetOptions()._refreshRate;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	swapChainDesc.BufferCount = 1;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.SampleDesc.Count = 4;
	swapChainDesc.SampleDesc.Quality = sampleQuality - 1;
	swapChainDesc.OutputWindow = System::GetInstance()->GetHWND();
	swapChainDesc.Windowed = !System::GetInstance()->GetOptions()._bFullscreen;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.Flags = 0;

	dxgiFactory->CreateSwapChain(_device, &swapChainDesc, &_swapChain);

	ASSERT(_swapChain != nullptr);

	dxgiFactory->Release();
	dxgiAdapter->Release();
	dxgiDevice->Release();

	// create render target view

	_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&_tMainRenderTarget));
	ASSERT(_tMainRenderTarget != nullptr);
	_device->CreateRenderTargetView(_tMainRenderTarget, nullptr, &_vMainRenderTarget);
	ASSERT(_vMainRenderTarget != nullptr);

	// create depth and stencil buffer view

	D3D11_TEXTURE2D_DESC depthStencilDesc;
	depthStencilDesc.Width = System::GetInstance()->GetOptions()._windowWidth;
	depthStencilDesc.Height = System::GetInstance()->GetOptions()._windowHeight;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilDesc.SampleDesc.Count = 4;
	depthStencilDesc.SampleDesc.Quality = sampleQuality - 1;
	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags = 0;
	depthStencilDesc.MiscFlags = 0;

	_device->CreateTexture2D(&depthStencilDesc, nullptr, &_tDepthStencilBuffer);
	ASSERT(_tDepthStencilBuffer != nullptr);
	_device->CreateDepthStencilView(_tDepthStencilBuffer, nullptr, &_vDepthStencilBuffer);
	ASSERT(_vDepthStencilBuffer != nullptr);

	// set pipeline's output merger state
	
	_deviceContext->OMSetRenderTargets(1, &_vMainRenderTarget, _vDepthStencilBuffer);

	// set viewport (default 100%)

	D3D11_VIEWPORT viewport;
	viewport.Width = static_cast<float>(System::GetInstance()->GetOptions()._windowWidth);
	viewport.Height = static_cast<float>(System::GetInstance()->GetOptions()._windowHeight);
	viewport.TopLeftX = viewport.TopLeftY = viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;

	_deviceContext->RSSetViewports(1, &viewport);
}

void Renderer::Run()
{
	float black[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
	_deviceContext->ClearRenderTargetView(_vMainRenderTarget, black);
	_deviceContext->ClearDepthStencilView(_vDepthStencilBuffer, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 0.0f, 0);
	
	_swapChain->Present(0, 0);
}

void Renderer::Shutdown()
{
	_vMainRenderTarget->Release();
	_vMainRenderTarget = nullptr;

	_tMainRenderTarget->Release();
	_tMainRenderTarget = nullptr;

	_vDepthStencilBuffer->Release();
	_vDepthStencilBuffer = nullptr;

	_tDepthStencilBuffer->Release();
	_tDepthStencilBuffer = nullptr;

	_swapChain->Release();
	_swapChain = nullptr;

	_deviceContext->Release();
	_deviceContext = nullptr;

	_device->Release();
	_device = nullptr;
}
