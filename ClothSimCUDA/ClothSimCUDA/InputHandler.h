#pragma once

/*
*/

#include "Common.h"
#include "Singleton.h"
#include "InputManager.h"

#include <glm\glm\glm.hpp>

class InputHandler : public Singleton<InputHandler>
{
	friend class Singleton<InputHandler>;

protected:
	InputHandler();
public:
	InputHandler(const InputHandler*);
	~InputHandler();

	bool ExitPressed();
	glm::vec2 GetCursorPosition();
	glm::vec2 GetCursorVector();
	glm::vec3 GetArrowsMovementVector();
	int GetZoomValue();
	bool ActionButtonPressed();
	bool ActionButtonClicked();
	bool CameraRotateButtonPressed();
	bool CameraMoveButtonPressed();
	bool WireframeButtonClicked();
};

