#pragma once

#include "Singleton.h"

#include <XInput.h>
#include <d3dx9math.h>
#include <functional>

#ifdef LE_PLATFORM_WINDOWS

typedef UCHAR KeyCode;

#endif

struct MouseData
{
	bool buttonState[16];
	float x, y;
	int scroll;
};

class InputManager :
	public Singleton<InputManager>
{
	friend class Singleton<InputManager>;
protected:

	BYTE m_keyStates[256];
	BYTE m_prevKeyStates[256];
	MouseData m_mouseData;
	MouseData m_prevMouseData;

	InputManager();

public:

	~InputManager();

	virtual void Initialize();
	virtual void Shutdown();
	virtual void Update();

	virtual bool GetKeyDown(KeyCode kc);
	virtual bool GetKey(KeyCode kc);
	virtual bool GetKeyUp(KeyCode kc);

	virtual void GetMousePosition(D3DXVECTOR2* vec);
	virtual void GetMouseDirection(D3DXVECTOR2* vec);
	virtual int GetMouseScrollAbsolute();
	virtual int GetMouseScrollRelative();

	virtual void GetTouchPosition(D3DXVECTOR2* vec);
	virtual void GetTouchDirection(D3DXVECTOR2* vec);
	virtual bool GetTouchDown(KeyCode kc);
	virtual bool GetTouch(KeyCode kc);
	virtual bool GetTouchUp(KeyCode kc);
	virtual bool GetIsDoubleTouch();
	virtual bool GetIsPinch();
};

