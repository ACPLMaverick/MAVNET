#include "InputManager.h"

InputManager::InputManager()
{

}

InputManager::InputManager(const InputManager*)
{

}

InputManager::~InputManager()
{

}


unsigned int InputManager::Initialize()
{
	unsigned int err = CS_ERR_NONE;

	glfwSetInputMode(Renderer::GetInstance()->GetWindow(), GLFW_STICKY_KEYS, GL_TRUE);
	glfwSetInputMode(Renderer::GetInstance()->GetWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	glfwSetInputMode(Renderer::GetInstance()->GetWindow(), GLFW_STICKY_MOUSE_BUTTONS, GL_TRUE);

	glfwSetCursorPos(Renderer::GetInstance()->GetWindow(), CSSET_WINDOW_WIDTH / 2.0, CSSET_WINDOW_HEIGHT / 2.0);

	glfwSetScrollCallback(Renderer::GetInstance()->GetWindow(), MouseScrollCallback);

	return err;
}

unsigned int InputManager::Shutdown()
{
	unsigned int err = CS_ERR_NONE;

	return err;
}

unsigned int InputManager::Run()
{
	unsigned int err = CS_ERR_NONE;

	// update pressed buttons removing
	bool finished;
	do
	{
		finished = true;
		for (vector<int>::iterator it = m_pressed.begin(); it != m_pressed.end(); ++it)
		{
			if (!GetKey(*it))
			{
				m_pressed.erase(it);
				finished = false;
				break;
			}
		}
	} while (!finished);

	// update mouse
	double mx, my;
	glfwGetCursorPos(Renderer::GetInstance()->GetWindow(), &mx, &my);
	mx /= (double)CSSET_WINDOW_WIDTH;
	my /= (double)CSSET_WINDOW_HEIGHT;
	m_mouseData.mouseXRelative = mx - m_mouseData.mouseX;
	m_mouseData.mouseYRelative = my - m_mouseData.mouseY;
	m_mouseData.mouseX = mx;
	m_mouseData.mouseY = my;

	if (m_mouseData.scroll != m_scrollHelper)
	{
		if (m_mouseData.scroll > m_scrollHelper)
		{
			m_mouseData.scrollRelative = 1;
		}
		else
		{
			m_mouseData.scrollRelative = -1;
		}
		m_scrollHelper = m_mouseData.scroll;
	}
	else
	{
		m_mouseData.scrollRelative = 0;
	}

	return err;
}

bool InputManager::IsPressed(int code)
{
	for (vector<int>::iterator it = m_pressed.begin(); it != m_pressed.end(); ++it)
	{
		if (code == (*it))
			return true;
	}
	return false;
}

bool InputManager::GetKey(int code)
{
	if (code > 7 )
		return glfwGetKey(Renderer::GetInstance()->GetWindow(), code);
	else
		return glfwGetMouseButton(Renderer::GetInstance()->GetWindow(), code);
}

bool InputManager::GetKeyDown(int code)
{
	if (GetKey(code) && !IsPressed(code))
	{
		m_pressed.push_back(code);
		return true;
	}
	return false;
}

bool InputManager::GetKeyUp(int code)
{
	if (!GetKey(code) && IsPressed(code))
	{
		return true;
	}
	return false;
}



MouseData* InputManager::GetMouseData()
{
	return &m_mouseData;
}



void InputManager::MouseScrollCallback(GLFWwindow* window, double x, double y)
{
	int value = (int)y;
	instance->m_mouseData.scroll += value;
}