#include "stdafx.h"
#include "InputManager.h"


InputManager::InputManager()
{
}


InputManager::~InputManager()
{
}

void InputManager::Initialize()
{
	ZeroMemory(&m_keyStates, 256);
	ZeroMemory(&m_prevKeyStates, 256);
}

void InputManager::Shutdown()
{
}

void InputManager::Update()
{
	memcpy_s(&m_prevKeyStates, 256, &m_keyStates, 256);
	GetKeyboardState((PBYTE)&m_keyStates);
}

bool InputManager::GetKeyDown(KeyCode kc)
{
	return ((m_keyStates[kc] >> 7) == 1 && (m_prevKeyStates[kc] >> 7) == 0);
}

bool InputManager::GetKey(KeyCode kc)
{
	return m_keyStates[kc] >> 7;
}

bool InputManager::GetKeyUp(KeyCode kc)
{
	return ((m_keyStates[kc] >> 7) == 0 && (m_prevKeyStates[kc] >> 7) == 1);
}

void InputManager::GetMousePosition(D3DXVECTOR2 * vec)
{
}

void InputManager::GetMouseDirection(D3DXVECTOR2 * vec)
{
}

int InputManager::GetMouseScrollAbsolute()
{
	return 0;
}

int InputManager::GetMouseScrollRelative()
{
	return 0;
}

void InputManager::GetTouchPosition(D3DXVECTOR2 * vec)
{
}

void InputManager::GetTouchDirection(D3DXVECTOR2 * vec)
{
}

bool InputManager::GetTouchDown(KeyCode kc)
{
	return false;
}

bool InputManager::GetTouch(KeyCode kc)
{
	return false;
}

bool InputManager::GetTouchUp(KeyCode kc)
{
	return false;
}

bool InputManager::GetIsDoubleTouch()
{
	return false;
}

bool InputManager::GetIsPinch()
{
	return false;
}
