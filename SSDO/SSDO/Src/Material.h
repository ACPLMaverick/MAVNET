#pragma once

#include "stdafx.h"

class Mesh;
class Camera;
class Shader;

class Material
{
protected:

#pragma region Protected

	const Shader& _shader;

	XMFLOAT4 _colorBase;
	XMFLOAT4 _colorSpecular;
	float _gloss;

#pragma endregion

#pragma region Functions Protected

	inline void LoadShader(const wstring& shaderFilename);

#pragma endregion

public:

#pragma region Functions Public

	Material(const Shader& shader, 
		const XMFLOAT4& colorBase = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), 
		const XMFLOAT4& colorSpecular = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
		float gloss = 50.0f);
	~Material();

	// update per frame and/or infrequent shader parameters
	void Update();
	void DrawMesh(const Camera& camera, const Mesh& mesh) const;

#pragma endregion
};

