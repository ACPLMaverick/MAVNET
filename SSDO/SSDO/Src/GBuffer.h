#pragma once

#include "GlobalDefines.h"
#include "Buffer.h"

class Shader;
class Camera;

namespace Postprocesses
{
	class Postprocess;
}

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

		virtual RenderTarget& operator=(const RenderTarget& other)
		{
			Texture = other.Texture;
			View = other.View;
			Sampler = other.Sampler;
			SRV = other.SRV;
			return *this;
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

		virtual RenderTargetDepthBuffer& operator=(const RenderTargetDepthBuffer& other)
		{
			RenderTarget::operator=(other);
			DepthStencilView = other.DepthStencilView;
			return *this;
		}
	};

#pragma endregion

#pragma region Protected

	const Camera& _camera;

	RenderTarget _color;
	RenderTarget _normal;
	RenderTarget _worldPos;
	RenderTargetDepthBuffer _depth;
				
	RenderTarget _outputA;
	RenderTarget _outputB;

	ID3D11BlendState* _additiveBlendState;

	Shader* _shaderDraw;
	Shader* _shaderMerge;
	Shader* _shaderLightAmbient;
	Shader* _shaderLightDirectional;
	Shader* _shaderLightPoint;

#pragma endregion

#pragma region Functions Protected

	inline void FlipOutputs();
	inline void SetMapData();
	inline void UnsetMapData();
	inline void DrawFullscreenPlane();

#pragma endregion

public:

#pragma region Functions Public

	GBuffer(const Camera& camera);
	~GBuffer();

	void SetDrawMeshes();

	void SetDrawLights();

	void SetDrawLightAmbient();
	void SetDrawLightDirectional();
	void SetDrawLightPoint();

	// Before calling this function, SetDrawLightAmbient must be called.
	void DrawLightAmbient(const Lights::LightAmbient& lightAmbient);
	// Before calling this function, SetDrawLightDirectional must be called.
	void DrawLightDirectional(const Lights::LightDirectional& lightAmbient);
	// Before calling this function, SetDrawLightPoint must be called.
	void DrawLightPoint(const Lights::LightPoint& lightAmbient);

	void SetDrawPostproecesses();

	void DrawPostprocess(const Postprocesses::Postprocess& pp);

	void EndFrame();

	void Draw() const;

	void Merge(const GBuffer& other);

#pragma endregion

};

