#include "stdafx.h"
#include "Camera.h"
#include "System.h"
#include "Scenes\Scene.h"
#include "Mesh.h"
#include "Renderer.h"
#include "Object.h"

using namespace Scenes;

Camera::Camera(const XMFLOAT3 & position, const XMFLOAT3 & target, const XMFLOAT3 & up, float fov, float nearPlane, float farPlane) :
	_position(position),
	_target(target),
	_up(up),
	_fov(fov),
	_near(nearPlane),
	_far(farPlane),
	_gBuffer(*this)
{
	UpdateView();
	UpdateProjection();
	MergeMatrices();
}

Camera::~Camera()
{
}

void Camera::Update()
{
	bool mergeMatrices = false;

	if (_bNeedUpdateView)
	{
		UpdateView();
		_bNeedUpdateView = false;
		mergeMatrices = true;
	}

	if (_bNeedUpdateProj)
	{
		UpdateProjection();
		_bNeedUpdateProj = false;
		mergeMatrices = true;
	}

	if (mergeMatrices)
	{
		MergeMatrices();
	}
}

void Camera::Draw(const Scene& scene) const
{
	if (Renderer::GetInstance()->GetRenderMode() == Renderer::RenderMode::FORWARD)
	{
		for (auto it = scene._objects.begin(); it != scene._objects.end(); ++it)
		{
			(*it)->Draw(*this);
		}
	}
	else
	{
		GBuffer& gBuffer = const_cast<GBuffer&>(_gBuffer);
		// draw objects
		gBuffer.SetDrawMeshes();
		for (auto it = scene._objects.begin(); it != scene._objects.end(); ++it)
		{
			(*it)->Draw(*this);
		}

		// enlighten objects
		gBuffer.SetDrawLights();

		if (scene._lightAmbient != nullptr)
		{
			gBuffer.SetDrawLightAmbient();
			gBuffer.DrawLightAmbient(*scene._lightAmbient);
		}

		if (scene._lightsDirectional.size() > 0)
		{
			gBuffer.SetDrawLightDirectional();

			for (auto it = scene._lightsDirectional.begin(); it != scene._lightsDirectional.end(); ++it)
			{
				gBuffer.DrawLightDirectional(**it);
			}
		}

		if (scene._lightsPoint.size() > 0)
		{
			gBuffer.SetDrawLightPoint();

			for (auto it = scene._lightsPoint.begin(); it != scene._lightsPoint.end(); ++it)
			{
				gBuffer.DrawLightPoint(**it);
			}
		}

		// apply postprocesses
		gBuffer.SetDrawPostproecesses();
		for (auto it = scene._postprocesses.begin(); it != scene._postprocesses.end(); ++it)
		{
			gBuffer.DrawPostprocess(**it);
		}

		gBuffer.EndFrame();
	}
}

void Camera::SetDirection(const XMFLOAT3 & direction)
{
	XMVECTOR pos = XMLoadFloat3(&_position);
	XMVECTOR dir = XMLoadFloat3(&_target) - pos;

	XMVECTOR len = XMVector3Length(dir);
	XMVECTOR nTgt = pos + XMVector3Normalize(XMLoadFloat3(&direction)) * len;

	XMStoreFloat3(&_target, nTgt);

	_bNeedUpdateView = true;
}

inline void Camera::UpdateView()
{
	XMVECTOR pos = XMLoadFloat3(&_position);
	XMVECTOR tgt = XMLoadFloat3(&_target);
	XMVECTOR up = XMVector3Normalize(XMLoadFloat3(&_up));
	XMVECTOR dir = XMVector3Normalize(tgt - pos);
	XMVECTOR right = XMVector3Cross(up, dir);

	XMStoreFloat3(&_direction, dir);
	XMStoreFloat3(&_up, up);
	XMStoreFloat3(&_right, right);

	XMMATRIX mat = XMMatrixLookAtLH(pos, tgt, up);
	XMStoreFloat4x4(&_matView, mat);
}

inline void Camera::UpdateProjection()
{
	XMMATRIX matProj = XMMatrixPerspectiveFovLH(_fov,
		System::GetInstance()->GetOptions()._aspectRatio, 
		_near, 
		_far);
	XMStoreFloat4x4(&_matProj, matProj);
}

inline void Camera::MergeMatrices()
{
	XMMATRIX v = XMLoadFloat4x4(&_matView);
	XMMATRIX p = XMLoadFloat4x4(&_matProj);
	XMStoreFloat4x4(&_matViewProj, v * p);
}
