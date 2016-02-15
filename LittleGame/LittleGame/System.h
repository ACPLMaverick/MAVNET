#pragma once

#include "stdafx.h"
#include "Singleton.h"
#include "Resource.h"

class System : public Singleton<System>
{
protected:

#pragma region Constants

	const std::wstring WINDOW_TITLE = L"LittleGame";

#pragma endregion

#pragma region Variables

	HINSTANCE m_hInstance;
	LPWSTR m_lpCmdLine; 
	int m_nCmdShow;
	HWND m_hwnd;

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
};

