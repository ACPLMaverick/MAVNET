#pragma once

#include "stdafx.h"

class Mesh;
class Camera;

class Material
{
protected:

#pragma region Structs

	__declspec(align(16))
	struct BufferPerObject
	{
		XMFLOAT4X4 gMatWVP;
		XMFLOAT4X4 gMatW;
		XMFLOAT4X4 gMatWInvTransp;
		XMFLOAT4 gColBase;
		XMFLOAT4 gColSpecular;
		float gGloss;
	};

	__declspec(align(16))
	struct BufferPerFrame
	{
		XMFLOAT3 gLightDir;
	};

#pragma endregion

#pragma region Protected

	ID3D11VertexShader* _vs = nullptr;
	ID3D11PixelShader* _ps = nullptr;

	ID3D11InputLayout* _inputLayout = nullptr;

	ID3D11Buffer* _bufferPerObject = nullptr;
	ID3D11Buffer* _bufferPerFrame = nullptr;

	XMFLOAT4 _colorBase;
	XMFLOAT4 _colorSpecular;
	float _gloss;

#pragma endregion

#pragma region Functions Protected

	inline void LoadShader(const wstring& shaderFilename);

#pragma endregion

public:

#pragma region Functions Public

	Material(const wstring& shaderFilename, 
		const XMFLOAT4& colorBase = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f), 
		const XMFLOAT4& colorSpecular = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
		float gloss = 50.0f);
	~Material();

	// update per frame and/or infrequent shader parameters
	void Update();
	void DrawMesh(const Camera& camera, const Mesh& mesh) const;

#pragma endregion
};

