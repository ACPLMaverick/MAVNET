#pragma once

#include "GlobalDefines.h"
#include "Buffer.h"

class Shader;

namespace Lights
{
	class LightAmbient;
	class LightDirectional;
	class LightPoint;
}

class GBuffer
{
protected:

#pragma region Structs

	struct RenderTarget
	{
		ID3D11Texture2D* Texture = nullptr;
		ID3D11RenderTargetView* View = nullptr;

		ID3D11SamplerState* Sampler = nullptr;
		ID3D11ShaderResourceView* SRV = nullptr;

		inline virtual void Shutdown()
		{
			if (Texture != nullptr)
			{
				Texture->Release();
				Texture = nullptr;
			}
			if (View != nullptr)
			{
				View->Release();
				View = nullptr;
			}
			if (Sampler != nullptr)
			{
				Sampler->Release();
				Sampler = nullptr;
			}
			if (SRV != nullptr)
			{
				SRV->Release();
				SRV = nullptr;
			}
		}
	};

	struct RenderTargetDepthBuffer : RenderTarget
	{
		ID3D11DepthStencilView* DepthStencilView = nullptr;

		inline virtual void Shutdown() override
		{
			RenderTarget::Shutdown();
			if (DepthStencilView != nullptr)
			{
				DepthStencilView->Release();
				DepthStencilView = nullptr;
			}
		}
	};

#pragma endregion

#pragma region Protected

	RenderTarget _color;
	RenderTarget _normal;
	RenderTargetDepthBuffer _depth;
				
	RenderTarget _outputA;
	RenderTarget _outputB;

	ID3D11Buffer* _fullscreenPlaneVertexBuffer;
	ID3D11Buffer* _fullscreenPlaneUvBuffer;
	ID3D11Buffer* _fullscreenPlaneIndexBuffer;

	Shader* _shaderDraw;
	Shader* _shaderMerge;
	Shader* _shaderLightAmbient;
	Shader* _shaderLightDirectional;
	Shader* _shaderLightPoint;

#pragma endregion

#pragma region Functions Protected

	inline void FlipOutputs();

#pragma endregion

public:

#pragma region Functions Public

	GBuffer();
	~GBuffer();

	void SetDrawMeshes();

	void SetDrawLightAmbient();
	void SetDrawLightDirectional();
	void SetDrawLightPoint();

	void SetDrawPostprocess();

	void Draw() const;

	void Merge(const GBuffer& other);

#pragma endregion

};

