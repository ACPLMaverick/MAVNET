#include "stdafx.h"
#include "Mesh.h"
#include "Material.h"
#include "Renderer.h"

Mesh::Mesh(const Material& mat, const XMFLOAT3 & pos, const XMFLOAT3 & rot, const XMFLOAT3 & scl) :
	_myMaterial(mat),
	_position(pos),
	_rotation(QuaterionFromEuler(rot)),
	_scale(scl)
{
	CreateWorldMatrix();

	_vPositions.Allocate(8);
	_vNormals.Allocate(8);
	_vUvs.Allocate(8);
	_indices.Allocate(12);

	float s = 0.5f;
	_vPositions[0] = XMFLOAT3(-s, -s, -s);
	_vPositions[1] = XMFLOAT3(s, -s, -s);
	_vPositions[2] = XMFLOAT3(-s, s, -s);
	_vPositions[3] = XMFLOAT3(s, s, -s);
	_vPositions[4] = XMFLOAT3(-s, -s, s);
	_vPositions[5] = XMFLOAT3(s, -s, s);
	_vPositions[6] = XMFLOAT3(-s, s, s);
	_vPositions[7] = XMFLOAT3(s, s, s);

	for (int i = 0; i < 8; ++i)
	{
		_vUvs[i] = XMFLOAT2(0.0f, 0.0f);
	}

	_indices[0] = Triangle(0, 1, 2);
	_indices[1] = Triangle(1, 3, 2);
	_indices[2] = Triangle(4, 5, 6);
	_indices[3] = Triangle(5, 7, 6);
	_indices[4] = Triangle(6, 2, 3);
	_indices[5] = Triangle(6, 3, 7);
	_indices[6] = Triangle(4, 0, 1);
	_indices[7] = Triangle(4, 1, 5);
	_indices[8] = Triangle(3, 1, 5);
	_indices[9] = Triangle(3, 5, 7);
	_indices[10] = Triangle(2, 0, 4);
	_indices[11] = Triangle(2, 4, 6);

	for (int i = 0; i < 12; ++i)
	{
		Triangle tr = _indices[i];
		for (int j = 0; j < 3; ++j)
		{
			XMFLOAT3 me = _vPositions[tr[j]];
			XMFLOAT3 neighbour1 = _vPositions[tr[(j + 1) % 3]];
			XMFLOAT3 neighbour2 = _vPositions[tr[(j + 2) % 3]];

			XMVECTOR nrm = XMLoadFloat3(&_vNormals[tr[j]]);
			XMVECTOR n1 = XMVector3Normalize(XMLoadFloat3(&neighbour1) - XMLoadFloat3(&me));
			XMVECTOR n2 = XMVector3Normalize(XMLoadFloat3(&neighbour2) - XMLoadFloat3(&me));
			XMVECTOR cr = XMVector3Cross(n1, n2) + nrm;
			XMStoreFloat3(&_vNormals[tr[j]], cr);
		}
	}

	for (int i = 0; i < 8; ++i)
	{
		XMVECTOR nrm = XMLoadFloat3(&_vNormals[i]);
		nrm = XMVector3Normalize(nrm);
		XMStoreFloat3(&_vNormals[i], nrm);
	}

	InitBuffers();
}

Mesh::Mesh(const std::wstring & filePath, const Material& mat, const XMFLOAT3 & pos, const XMFLOAT3 & rot, const XMFLOAT3 & scl) :
	_myMaterial(mat),
	_position(pos),
	_rotation(QuaterionFromEuler(rot)),
	_scale(scl)
{
}

Mesh::~Mesh()
{
	_fPositions->Release();
	_fPositions = nullptr;

	_fNormals->Release();
	_fNormals = nullptr;

	_fUvs->Release();
	_fUvs = nullptr;

	_fIndices->Release();
	_fIndices = nullptr;
}

void Mesh::Update()
{
	if (_bNeedCreateWorldMatrix)
	{
		CreateWorldMatrix();
		_bNeedCreateWorldMatrix = false;
	}
}

void Mesh::Draw(const Camera & camera) const
{
	_myMaterial.DrawMesh(camera, *this);
}

inline void Mesh::InitBuffers()
{
	D3D11_BUFFER_DESC desc[4];
	D3D11_SUBRESOURCE_DATA sData[4];
	ID3D11Buffer** bufferPtrs[4] = { &_fPositions, &_fNormals, &_fUvs, &_fIndices };

	desc[0].BindFlags = D3D11_BIND_VERTEX_BUFFER;
	desc[0].ByteWidth = static_cast<uint32_t>(sizeof(XMFLOAT3) * _vPositions.GetSize());
	desc[0].CPUAccessFlags = 0;
	desc[0].MiscFlags = 0;
	desc[0].StructureByteStride = 0;
	desc[0].Usage = D3D11_USAGE_DEFAULT;

	desc[1] = desc[2] = desc[3] = desc[0];
	desc[2].ByteWidth = static_cast<uint32_t>(sizeof(XMFLOAT2) * _vUvs.GetSize());
	desc[3].BindFlags = D3D11_BIND_INDEX_BUFFER;
	desc[3].ByteWidth = static_cast<uint32_t>(sizeof(int16_t) * _indices.GetSize());

	ZeroMemory(sData, sizeof(D3D11_SUBRESOURCE_DATA) * 4);
	sData[0].pSysMem = &_vPositions[0];
	sData[1].pSysMem = &_vNormals[0];
	sData[2].pSysMem = &_vUvs[0];
	sData[3].pSysMem = &_indices[0];

	ID3D11Device* device = Renderer::GetInstance()->GetDevice();

	for (int i = 0; i < 4; ++i)
	{
		device->CreateBuffer(desc + i, sData + i, bufferPtrs[i]);
		ASSERT(*bufferPtrs[i] != nullptr);
	}
}

inline XMFLOAT4 Mesh::QuaterionFromEuler(const XMFLOAT3 & euler) const
{
	XMFLOAT4 quaternion;
	XMVECTOR quat = XMQuaternionRotationRollPitchYaw(euler.x, euler.y, euler.z);
	XMStoreFloat4(&quaternion, quat);
	return quaternion;
}

inline XMFLOAT3 Mesh::EulerFromQuaternion(const XMFLOAT4 & quat) const
{
	XMFLOAT3 euler;
	XMFLOAT3 angleX(1.0f, 0.0f, 0.0f);
	XMFLOAT3 angleY(0.0f, 1.0f, 0.0f);
	XMFLOAT3 angleZ(0.0f, 0.0f, 1.0f);
	XMVECTOR x = XMLoadFloat3(&angleX);
	XMVECTOR y = XMLoadFloat3(&angleY);
	XMVECTOR z = XMLoadFloat3(&angleZ);
	XMVECTOR q = XMLoadFloat4(&quat);
	XMQuaternionToAxisAngle(&x, &euler.x, q);
	XMQuaternionToAxisAngle(&y, &euler.y, q);
	XMQuaternionToAxisAngle(&z, &euler.z, q);
	return euler;
}

inline void Mesh::CreateWorldMatrix()
{
	XMVECTOR pos = XMLoadFloat3(&_position);
	XMVECTOR quat = XMLoadFloat4(&_rotation);
	XMVECTOR scl = XMLoadFloat3(&_scale);
	XMMATRIX mat = XMMatrixTranslationFromVector(pos) * XMMatrixRotationQuaternion(quat) * XMMatrixScalingFromVector(scl);
	XMMATRIX matInvTransp = XMMatrixTranspose(XMMatrixInverse(nullptr, mat));
	XMStoreFloat4x4(&_matWorld, mat);
	XMStoreFloat4x4(&_matWorldInvTransp, matInvTransp);
}
