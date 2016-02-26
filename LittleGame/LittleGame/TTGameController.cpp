#include "stdafx.h"
#include "TTGameController.h"
#include "InputManager.h"
#include "System.h"
#include "Timer.h"
#include "TTInputHandler.h"
#include "GameObject.h"
#include "Transform.h"

#include <sstream>

TTGameController::TTGameController()
{
}


TTGameController::~TTGameController()
{
}

void TTGameController::Initialize(uint32_t uid, GameObject* obj, std::string* name)
{
	Component::Initialize(uid, obj, name);
}

void TTGameController::Shutdown()
{
	TTInputHandler::DestroyInstance();
}

void TTGameController::Update()
{
	// controls

	// quit game
	if (TTInputHandler::GetInstance()->ActionDecline())
	{
		System::GetInstance()->Stop();
	}

	// timer test
	if (TTInputHandler::GetInstance()->ActionFlip())
	{
		double ct, dt, fps;
		ct = Timer::GetInstance()->GetActualTime();
		dt = Timer::GetInstance()->GetDeltaTime();
		fps = Timer::GetInstance()->GetFPS();
		std::ostringstream cts, dts, fpss;
		cts << ct;
		dts << dt;
		fpss << fps;

		std::string message = "Current time: " + cts.str() + " s.\nDelta time: " + dts.str() + " s.\nFPS: " + fpss.str();
		std::wstring wmessage = std::wstring(message.begin(), message.end());

		MessageBox(System::GetInstance()->GetSystemSettings()->GetWindowPtr(), wmessage.c_str(), L"Timer test", 0);
	}

	// move test brick
	if (
		TTInputHandler::GetInstance()->ActionLeft() ||
		TTInputHandler::GetInstance()->ActionUp() ||
		TTInputHandler::GetInstance()->ActionRight() ||
		TTInputHandler::GetInstance()->ActionDown()
		)
	{
		double dt = Timer::GetInstance()->GetDeltaTime();
		float speed = 10.0f;
		D3DXVECTOR2 movement = D3DXVECTOR2(0.0f, 0.0f);
		if (TTInputHandler::GetInstance()->ActionLeft())
		{
			movement += D3DXVECTOR2(-1.0f, 0.0f);
		}
		if (TTInputHandler::GetInstance()->ActionUp())
		{
			movement += D3DXVECTOR2(0.0f, 1.0f);
		}
		if (TTInputHandler::GetInstance()->ActionRight())
		{
			movement += D3DXVECTOR2(1.0f, 0.0f);
		}
		if (TTInputHandler::GetInstance()->ActionDown())
		{
			movement += D3DXVECTOR2(0.0f, -1.0f);
		}

		movement *= dt * speed;
		D3DXVECTOR2 nPos = *Controllable->GetTransform()->GetPosition2D() + movement;
		Controllable->GetTransform()->SetPosition2D(&nPos);

		std::wstringstream dupa;
		dupa << nPos.x << " " << nPos.y << "\n";
		OutputDebugString(dupa.str().c_str());
	}
}

void TTGameController::Draw()
{
}
