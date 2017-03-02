#pragma once

#include "stdafx.h"
#include "Singleton.h"

#include <d3d11.h>
#include <DirectXMath.h>

class Renderer : public Singleton<Renderer>
{
protected:

#pragma region Protected

	ID3D11Device* _device = nullptr;
	ID3D11DeviceContext* _deviceContext = nullptr;

	IDXGISwapChain* _swapChain = nullptr;

	ID3D11Texture2D* _tMainRenderTarget = nullptr;
	ID3D11RenderTargetView* _vMainRenderTarget = nullptr;
	ID3D11Texture2D* _tDepthStencilBuffer = nullptr;
	ID3D11DepthStencilView* _vDepthStencilBuffer = nullptr;

#pragma endregion

#pragma region Functions Protected

#pragma endregion

public:

#pragma region Functions Public

	Renderer();
	~Renderer();

	void Initialize();
	void Run();
	void Shutdown();

#pragma endregion
};

