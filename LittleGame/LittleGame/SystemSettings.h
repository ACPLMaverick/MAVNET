#pragma once

/*
	System-wide defines
*/


// Platform selection defines

#define LG_PLATFORM_WINDOWS
//#define LG_PLATFORM_LINUX
//#define LG_PLATFORM_ANDROID

#ifdef LG_PLATFORM_WINDOWS

#define LG_RENDERER_DX9
//#define LG_RENDERER_DX11
//#define LG_RENDERER_OGL_WIN

#define LG_LIB_SYSTEM #include <windows.h>
#define LG_System_SPECIFIC SystemWin

#elif LG_PLATFORM_LINUX

#define LG_RENDERER_OGL_LNX

#define LG_LIB_SYSTEM
#define LG_System_SPECIFIC SystemLnx

#elif LG_PLATFORM_ANDROID

#define LG_RENDERER_OGL_ES

#define LG_LIB_SYSTEM
#define LG_System_SPECIFIC SystemAdr

#endif

//////////////////

// Other defines

//////////////////////////////////////
//////////////////////////////////////

class System;

class SystemSettings
{
	friend class System;

private:

#pragma region Constants

	const std::string C_LOADPATH = "Settings.ini";

#pragma endregion


#pragma region Settings

	std::wstring s_windowTitle = L"LittleGame";
	int32_t s_windowWidth = 800;
	int32_t s_windowHeight = 600;
	bool s_windowVsync = false;
	bool s_windowFullscreen = false;
	bool s_soundEnabled = true;

	HINSTANCE m_hInstance = NULL;
	LPWSTR m_lpCmdLine = NULL;
	int m_nCmdShow = 0;
	HWND m_hwnd = NULL;

	int32_t m_displayWidth = 0;
	int32_t m_displayHeight = 0;

#pragma endregion

public:

	SystemSettings();
	~SystemSettings();

	void Initialize();

#pragma region Accessors

	std::wstring* GetWindowTitle() { return &s_windowTitle; }
	int32_t GetWindowWidth() { return s_windowWidth; }
	int32_t GetWindowHeight() { return s_windowHeight; }
	bool GetWindowVsync() { return s_windowVsync; }
	bool GetWindowFullscreen() { return s_windowFullscreen; }
	bool GetSoundEnabled() { return s_soundEnabled; }

	HINSTANCE GetHInstance() { return m_hInstance; }
	LPWSTR* GetLpCmdLine() { return &m_lpCmdLine; }
	int GetCmdShow() { return m_nCmdShow; }
	HWND GetWindowPtr() { return m_hwnd; }

	int32_t GetDisplayWidth() { return m_displayWidth; }
	int32_t GetDisplayHeight() { return m_displayHeight; }

#pragma endregion
};

