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



bool InputHandler::ExitPressed()
{
	if (InputManager::GetInstance()->GetKey(GLFW_KEY_ESCAPE))
	{
		return true;
	}
	else return false;
}

glm::vec2 InputHandler::GetCursorPosition()
{
	glm::vec2 pos;
	MouseData* m = InputManager::GetInstance()->GetMouseData();

	pos.x = m->mouseX;
	pos.y = m->mouseY;

	return pos;
}

glm::vec2 InputHandler::GetCursorVector()
{
	glm::vec2 pos;
	MouseData* m = InputManager::GetInstance()->GetMouseData();

	pos.x = m->mouseXRelative;
	pos.y = m->mouseYRelative;

	return pos;
}

glm::vec3 InputHandler::GetArrowsMovementVector()
{
	glm::vec3 vec = glm::vec3(0.0f, 0.0f, 0.0f);

	if (InputManager::GetInstance()->GetKey(GLFW_KEY_W))
		vec.z = -1.0f;
	if (InputManager::GetInstance()->GetKey(GLFW_KEY_S))
		vec.z = 1.0f;
	if (InputManager::GetInstance()->GetKey(GLFW_KEY_A))
		vec.x = -1.0f;
	if (InputManager::GetInstance()->GetKey(GLFW_KEY_D))
		vec.x = 1.0f;
	if (InputManager::GetInstance()->GetKey(GLFW_KEY_Q))
		vec.y = 1.0f;
	if (InputManager::GetInstance()->GetKey(GLFW_KEY_Z))
		vec.y = -1.0f;

	if (vec.x != 0.0f || vec.y != 0.0f || vec.z != 0.0f)
	vec = glm::normalize(vec);

	return vec;
}

int InputHandler::GetZoomValue()
{
	MouseData* m = InputManager::GetInstance()->GetMouseData();
	return m->scrollRelative;
}

bool InputHandler::ActionButtonPressed()
{
	return InputManager::GetInstance()->GetKey(GLFW_MOUSE_BUTTON_LEFT);
}

bool InputHandler::ActionButtonClicked()
{
	return false;
}

bool InputHandler::CameraRotateButtonPressed()
{
	return InputManager::GetInstance()->GetKey(GLFW_MOUSE_BUTTON_RIGHT);
}

bool InputHandler::CameraMoveButtonPressed()
{
	return InputManager::GetInstance()->GetKey(GLFW_MOUSE_BUTTON_MIDDLE);
}

bool InputHandler::WireframeButtonClicked()
{
	return InputManager::GetInstance()->GetKeyDown(GLFW_KEY_V);
}
