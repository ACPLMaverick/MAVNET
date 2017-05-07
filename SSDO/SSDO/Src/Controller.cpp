#include "stdafx.h"
#include "Controller.h"
#include "System.h"
#include "Scenes/Scene.h"
#include "Input.h"
#include "Camera.h"
#include "Timer.h"
#include "Postprocesses/SimpleSSAO.h"

using namespace Scenes;
using namespace Postprocesses;

Controller::Controller(Scene* scene) : _scene(scene), _cameraSpeed(1.0f), _cameraBoost(30.0f), _cameraRotateSpeedX(1.0f), _cameraRotateSpeedY(1.0f)
{
}

Controller::~Controller()
{
}

void Controller::Initialize()
{
	for (auto it = _scene->_postprocesses.begin(); it != _scene->_postprocesses.end(); ++it)
	{
		if ((_ssao = dynamic_cast<SimpleSSAO*>(*it)) != nullptr)
		{
			break;
		}
	}
}

void Controller::Update()
{
	// Base controls
	if (Input::GetInstance()->GetKey(VK_ESCAPE))
	{
		System::GetInstance()->Exit();
	}

	// Camera controls
	Camera* cam = _scene->_mainCamera;

	if (cam != nullptr)
	{
		XMFLOAT3A pos = cam->GetPosition();
		XMFLOAT3A tgt = cam->GetTarget();
		XMFLOAT3A up = cam->GetUp();
		XMFLOAT3A dir = cam->GetDirection();

		XMVECTOR vPos = XMLoadFloat3A(&pos), vTgt = XMLoadFloat3A(&tgt), vUp = XMLoadFloat3A(&up), vDir = XMLoadFloat3A(&dir);
		XMVECTOR vRight = XMVector3Normalize(XMVector3Cross(vUp, vDir));

		if (Input::GetInstance()->GetKey(VK_RBUTTON))
		{
			XMFLOAT2A mouseMovement = Input::GetInstance()->GetMouseRelativePosition();
			mouseMovement.x *= _cameraRotateSpeedX * Timer::GetInstance()->GetDeltaTime();
			mouseMovement.y *= _cameraRotateSpeedY * Timer::GetInstance()->GetDeltaTime();

			XMVECTOR rotYaw = XMQuaternionRotationAxis(vUp, mouseMovement.x);
			XMVECTOR rotPitch = XMQuaternionRotationAxis(vRight, mouseMovement.y);

			vDir = XMVector3Rotate(XMVector3Rotate(vDir, rotYaw), rotPitch);
			cam->SetDirection(vDir);
		}

		float currentBoost = 1.0f;
		if (Input::GetInstance()->GetKey(VK_SHIFT))
		{
			currentBoost = _cameraBoost;
		}

		if (Input::GetInstance()->GetKey('Q'))	// up
		{
			vPos = vPos + vUp * currentBoost * _cameraSpeed * Timer::GetInstance()->GetDeltaTime();
			cam->SetPosition(vPos);
			cam->SetDirection(vDir);
		}
		if (Input::GetInstance()->GetKey('E')) // down
		{
			vPos = vPos - vUp * currentBoost * _cameraSpeed * Timer::GetInstance()->GetDeltaTime();
			cam->SetPosition(vPos);
			cam->SetDirection(vDir);
		}
		if (Input::GetInstance()->GetKey('A')) // left
		{
			vPos = vPos - vRight * currentBoost * _cameraSpeed * Timer::GetInstance()->GetDeltaTime();
			cam->SetPosition(vPos);
			cam->SetDirection(vDir);
		}
		if (Input::GetInstance()->GetKey('D')) // right
		{
			vPos = vPos + vRight * currentBoost * _cameraSpeed * Timer::GetInstance()->GetDeltaTime();
			cam->SetPosition(vPos);
			cam->SetDirection(vDir);
		}
		if (Input::GetInstance()->GetKey('W')) // forwards
		{
			vPos = vPos + vDir * currentBoost * _cameraSpeed * Timer::GetInstance()->GetDeltaTime();
			cam->SetPosition(vPos);
			cam->SetDirection(vDir);
		}
		if (Input::GetInstance()->GetKey('S')) // backwards
		{
			vPos = vPos - vDir * currentBoost * _cameraSpeed * Timer::GetInstance()->GetDeltaTime();
			cam->SetPosition(vPos);
			cam->SetDirection(vDir);
		}
	}

	// Component controls
	if (_ssao != nullptr)
	{
		if (Input::GetInstance()->GetKeyDown('V')) // backwards
		{
			_ssao->ToggleEnabled();
		}
	}
}

void Controller::Shutdown()
{
}
