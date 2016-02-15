#include "InputHandler.h"

InputHandler::InputHandler()
{

}

InputHandler::InputHandler(const InputHandler*)
{

}

InputHandler::~InputHandler()
{

}

bool InputHandler::GetClick()
{
	return InputManager::GetInstance()->GetPress();
}

bool InputHandler::GetHold()
{
	return InputManager::GetInstance()->GetTouch();
}

bool InputHandler::GetMove()
{
	return InputManager::GetInstance()->GetMove();
}

void InputHandler::GetCameraMovementVector(glm::vec2* vec)
{
	if(InputManager::GetInstance()->GetCurrentlyHeldButtons() == 0)
		InputManager::GetInstance()->GetDoubleTouchDirection(vec);
	else
	{
		vec->x = 0.0f;
		vec->y = 0.0f;
	}
}

void InputHandler::GetCameraRotationVector(glm::vec2* vec)
{
	if (InputManager::GetInstance()->GetCurrentlyHeldButtons() == 0)
		InputManager::GetInstance()->GetTouchDirection(vec);
	else
	{
		vec->x = 0.0f;
		vec->y = 0.0f;
	}
}

float InputHandler::GetZoomValue()
{
	if (InputManager::GetInstance()->GetCurrentlyHeldButtons() == 0)
		return InputManager::GetInstance()->GetPinchValue();
	else
		return 0.0f;
}

bool InputHandler::GetZoom()
{
	return InputManager::GetInstance()->GetPinch();
}

void InputHandler::GetClickPosition(glm::vec2 * vec)
{
	InputManager::GetInstance()->GetTouchPosition(vec);
}
