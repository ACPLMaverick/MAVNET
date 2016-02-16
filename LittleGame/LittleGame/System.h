#pragma once

#include "stdafx.h"
#include "Singleton.h"
#include "Resource.h"

#include <vector>

class Scene;

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

#pragma region Collections

	std::vector<Scene*> m_scenes;
	uint32_t m_currentScene = 0;

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

#pragma region Accessors

	SystemSettings* GetSystemSettings() { return &m_settings; }
	Scene* GetCurrentScene() { return m_scenes[m_currentScene]; }
	std::vector<Scene*>* const GetSceneCollection() { return &m_scenes; }

#pragma endregion
};

