#include "stdafx.h"
#include "InputManager.h"
#include "System.h"


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

	DirectInput8Create(System::GetInstance()->GetSystemSettings()->GetHInstance(),
		DIRECTINPUT_VERSION,
		IID_IDirectInput8,
		(void**)&m_directInput, NULL);

	m_directInput->CreateDevice(GUID_SysKeyboard, &m_keyboard, NULL);
	m_keyboard->SetDataFormat(&c_dfDIKeyboard);
	m_keyboard->SetCooperativeLevel(System::GetInstance()->GetSystemSettings()->GetWindowPtr(),
		DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	m_keyboard->Acquire();

	m_directInput->CreateDevice(GUID_SysMouse, &m_mouse, NULL);
	m_mouse->SetDataFormat(&c_dfDIMouse2);
	m_mouse->SetCooperativeLevel(System::GetInstance()->GetSystemSettings()->GetWindowPtr(),
		DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	m_mouse->Acquire();
}

void InputManager::Shutdown()
{
	m_mouse->Unacquire();
	m_mouse->Release();
	m_mouse = nullptr;

	m_keyboard->Unacquire();
	m_keyboard->Release();
	m_keyboard = nullptr;

	m_directInput->Release();
	m_directInput = nullptr;
}

void InputManager::Update()
{
	memcpy_s(&m_prevKeyStates, 256, &m_keyStates, 256);
	memcpy_s(&m_prevMouseState, sizeof(MouseData), &m_mouseState, sizeof(MouseData));

	HRESULT r;

	r = m_keyboard->GetDeviceState(sizeof(m_keyStates), &m_keyStates);
	if (r == DIERR_INPUTLOST || r == DIERR_NOTACQUIRED)
	{
		m_keyboard->Acquire();
		m_keyboard->GetDeviceState(sizeof(m_keyStates), &m_keyStates);
	}

	r = m_mouse->GetDeviceState(sizeof(m_diMouseState), &m_diMouseState);
	if (r == DIERR_INPUTLOST || r == DIERR_NOTACQUIRED)
	{
		m_mouse->Acquire();
		m_mouse->GetDeviceState(sizeof(m_diMouseState), &m_diMouseState);
	}

	m_mouseState.x = (float)m_diMouseState.lX;
	m_mouseState.y = (float)m_diMouseState.lY;
	m_mouseState.scroll = (int)m_diMouseState.lZ;
	for (int i = 0; i < 8; ++i)
	{
		m_mouseState.buttonState[i] = (bool)m_diMouseState.rgbButtons[i];
	}
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

bool InputManager::GetMouseButton(KeyCode kc)
{
	return (m_mouseState.buttonState[min(kc, 15)]);
}

bool InputManager::GetMouseButtonDown(KeyCode kc)
{
	return ((m_mouseState.buttonState[min(kc, 15)]) == true && (m_prevMouseState.buttonState[min(kc, 15)]) == false);
}

bool InputManager::GetMouseButtonUp(KeyCode kc)
{
	return ((m_mouseState.buttonState[min(kc, 15)]) == false && (m_prevMouseState.buttonState[min(kc, 15)]) == true);
}

void InputManager::GetMousePosition(D3DXVECTOR2 * vec)
{
	vec->x = m_mouseState.x;
	vec->y = m_mouseState.y;
}

void InputManager::GetMouseDirection(D3DXVECTOR2 * vec)
{
	vec->x = m_mouseState.x - m_prevMouseState.x;
	vec->y = m_mouseState.y - m_prevMouseState.y;
}

int InputManager::GetMouseScrollAbsolute()
{
	return m_mouseState.scroll;
}

int InputManager::GetMouseScrollRelative()
{
	return m_mouseState.scroll - m_prevMouseState.scroll;
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