#pragma once

#include "stdafx.h"
#include "Singleton.h"
#include "Resource.h"
#include "SystemSettings.h"

class System : public Singleton<System>
{
protected:

#pragma region SettingsSystem

	SystemSettings m_settings;

#pragma endregion

#pragma region Variables

	bool m_active = true;
	bool m_running = true;

#pragma endregion


	inline void InitWindow(
		_In_ HINSTANCE hInstance,
		_In_ LPWSTR    lpCmdLine,
		_In_ int       nCmdShow);
	inline void RunMessages();

	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

public:

	System();
	~System();

	void Initialize(
		_In_ HINSTANCE hInstance,
		_In_ LPWSTR    lpCmdLine,
		_In_ int       nCmdShow);
	void Shutdown();
	void Run();
	void Pause();
	void Stop();

	SystemSettings* GetSystemSettings();
};

