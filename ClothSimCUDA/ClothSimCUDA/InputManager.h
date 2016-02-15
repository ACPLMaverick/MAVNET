#pragma once

/*
	This class controls all input devices and provides an interface for InputHandler to read input device status.
*/

#include "Common.h"
#include "Singleton.h"
#include "Renderer.h"

#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <vector>

struct MouseData
{
	double mouseX, mouseY, mouseXRelative, mouseYRelative;
	int scroll, scrollRelative;

	MouseData()
	{
		mouseX = mouseY = mouseXRelative = mouseYRelative = 0.0;
		scroll = scrollRelative = 0;
	}
};

class InputManager : public Singleton<InputManager>
{
	friend class Singleton<InputManager>;

protected:
	InputManager();

	std::vector<int> m_pressed;	// this vector stores pressed buttons WHICH WE HAVE QUERIED
	MouseData m_mouseData;
	int m_scrollHelper = 0;

	static void MouseScrollCallback(GLFWwindow*, double, double);
	bool IsPressed(int);
public:
	InputManager(const InputManager*);
	~InputManager();

	unsigned int Initialize();
	unsigned int Shutdown();
	unsigned int Run();

	bool GetKey(int);
	bool GetKeyDown(int);
	bool GetKeyUp(int);

	MouseData* GetMouseData();
};

