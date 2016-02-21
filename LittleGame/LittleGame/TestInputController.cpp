#include "stdafx.h"
#include "TestInputController.h"
#include "InputManager.h"
#include "System.h"

TestInputController::TestInputController()
{
}


TestInputController::~TestInputController()
{
}

void TestInputController::Update()
{
	if (InputManager::GetInstance()->GetKeyUp(VK_ESCAPE))
	{
		System::GetInstance()->Stop();
	}
}

void TestInputController::Draw()
{
}
