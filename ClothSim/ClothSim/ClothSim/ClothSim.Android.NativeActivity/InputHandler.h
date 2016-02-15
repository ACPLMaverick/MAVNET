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

	bool GetClick();
	bool GetHold();
	bool GetMove();
	bool GetZoom();
	void GetClickPosition(glm::vec2* vec);
	void GetCameraMovementVector(glm::vec2* vec);
	void GetCameraRotationVector(glm::vec2* vec);
	float GetZoomValue();
};

