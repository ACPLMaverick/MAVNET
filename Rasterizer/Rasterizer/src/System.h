#pragma once

#include "stdafx.h"
#include "Singleton.h"
#include "Buffer.h"

#include <vector>
#include <functional>

class Scene;

class System : public Singleton<System>
{
	friend class Singleton<System>;
protected:

#pragma region SettingsSystem

	SystemSettings m_settings;

#pragma endregion

#pragma region Draw Related

	Buffer<int32_t> m_BufferColor;
	Buffer<float> m_BufferDepth;

	HBITMAP m_bitmapScreenBuffer;
	BITMAPINFO m_bitmapScreenBufferInfo;
	void* m_bitmapScreenBufferDataPtr;

#pragma endregion

#pragma region Variables

	bool m_active = true;
	bool m_running = true;

#pragma endregion

#pragma region Collections

	std::vector<std::function<void(UINT, WPARAM, LPARAM)>*> m_eventsMessage;
	std::vector<Scene*> m_scenes;
	uint32_t m_currentScene = 0;

#pragma endregion

	System();

	inline void InitWindow(
		_In_ HINSTANCE hInstance,
		_In_ LPWSTR    lpCmdLine,
		_In_ int       nCmdShow);
	inline void RunMessages();
	inline void ResizeWindowBitmap();
	inline void DrawColorBuffer();

	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

public:

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

	void AddEventHandlerMessage(std::function<void(UINT, WPARAM, LPARAM)>* func);
	bool RemoveEventHandlerMessage(std::function<void(UINT, WPARAM, LPARAM)>* func);

#pragma endregion
};

