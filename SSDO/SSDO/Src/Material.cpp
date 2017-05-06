#include "stdafx.h"
#include "Material.h"
#include "Renderer.h"
#include "Camera.h"
#include "Mesh.h"
#include "Shader.h"
#include "Object.h"
#include "System.h"
#include "Scenes\Scene.h"

Material::Material(const Shader& shader, const XMFLOAT4A & colorBase, const XMFLOAT4A & colorSpecular, float gloss) :
	_shader(shader),
	_colorBase(colorBase),
	_colorSpecular(colorSpecular),
	_gloss(gloss)
{
	
}

Material::~Material()
{
}

void Material::Update()
{
}

void Material::DrawMesh(const Object& object, const Camera & camera, const Mesh & mesh) const
{
	// set shader parameters
	Shader::ColorBufferVS* bufferVs;
	Shader::ColorBufferPS* bufferPs;
	ID3D11DeviceContext* deviceContext = Renderer::GetInstance()->GetDeviceContext();

	XMFLOAT4X4A wvp;
	XMMATRIX w = XMLoadFloat4x4(&object.GetWorldMatrix());
	XMMATRIX vp = XMLoadFloat4x4(&camera.GetMatViewProj());
	XMMATRIX transposedWVP = XMMatrixTranspose(w * vp);
	XMStoreFloat4x4(&wvp, transposedWVP);

	XMMATRIX transposedW = XMMatrixTranspose(w);
	XMMATRIX transposedWInvTransp = XMMatrixTranspose(XMLoadFloat4x4(&object.GetWorldInvTransMatrix()));

	// set vertex and index buffers

	bufferVs = reinterpret_cast<Shader::ColorBufferVS*>(_shader.MapVsBuffer(0));

	XMStoreFloat4x4(&bufferVs->gMatWVP, transposedWVP);
	XMStoreFloat4x4(&bufferVs->gMatW, transposedW);
	XMStoreFloat4x4(&bufferVs->gMatWInvTransp, transposedWInvTransp);

	_shader.UnmapVsBuffer(0);

	bufferPs = reinterpret_cast<Shader::ColorBufferPS*>(_shader.MapPsBuffer(0));
	memcpy(&bufferPs->gColBase, &_colorBase, sizeof(XMFLOAT4A));
	memcpy(&bufferPs->gColSpecular, &_colorSpecular, sizeof(XMFLOAT4A));
	bufferPs->gGloss = _gloss;
	_shader.UnmapPsBuffer(0);

	_shader.Set();

	// DRAW!

	uint32_t stridePos = sizeof(mesh._vPositions[0]);
	uint32_t strideUvs = sizeof(mesh._vUvs[0]);
	uint32_t strideNrms = sizeof(mesh._vNormals[0]);
	uint32_t offset = 0;
	deviceContext->IASetVertexBuffers(0, 1, &mesh._fPositions, &stridePos, &offset);	// this can be simplified
	deviceContext->IASetVertexBuffers(1, 1, &mesh._fUvs, &strideUvs, &offset);
	deviceContext->IASetVertexBuffers(2, 1, &mesh._fNormals, &strideNrms, &offset);
	deviceContext->IASetIndexBuffer(mesh._fIndices, DXGI_FORMAT_R16_UINT, 0);

	deviceContext->DrawIndexed(static_cast<uint32_t>(mesh._indices.GetSize() * 3), 0, 0);
}

Material * Material::CreateResource(const std::wstring & name)
{
	return new Material(*System::GetInstance()->GetScene().LoadShader(name));
}
