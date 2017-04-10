#pragma once

#include "GlobalDefines.h"

class Shader
{
public:

	struct ConstantBufferDesc
	{
		size_t Size;

		ConstantBufferDesc(size_t size) :
			Size(size)
		{

		}
	};


	// shader-specific

#pragma region Structs

	__declspec(align(16))
		struct ColorBufferVS
	{
		XMFLOAT4X4 gMatWVP;
		XMFLOAT4X4 gMatW;
		XMFLOAT4X4 gMatWInvTransp;
	};

	__declspec(align(16))
		struct ColorBufferPS
	{
		XMFLOAT4 gColBase;
		XMFLOAT4 gColSpecular;
		float gGloss;
	};

#pragma endregion

protected:

#pragma region Protected

	ID3D11VertexShader* _vs = nullptr;
	ID3D11PixelShader* _ps = nullptr;

	ID3D11InputLayout* _inputLayout = nullptr;

	ID3D11Buffer** _constantVsBuffers = nullptr;
	size_t _constantVsBufferCount = 0;

	ID3D11Buffer** _constantPsBuffers = nullptr;
	size_t _constantPsBufferCount = 0;

#pragma endregion

#pragma region Functions Protected


#pragma endregion

public:

	Shader(const wstring& shaderFilename, size_t inputLayoutNumElements, 
		ConstantBufferDesc* cbVsDescs, size_t cbVsCount,
		ConstantBufferDesc* cbPsDescs, size_t cbPsCount);
	~Shader();

	void Set() const;
	void* MapVsBuffer(size_t i) const;
	void UnmapVsBuffer(size_t i) const;
	void* MapPsBuffer(size_t i) const;
	void UnmapPsBuffer(size_t i) const;

	static Shader* CreateResource(const std::wstring& name);
};
