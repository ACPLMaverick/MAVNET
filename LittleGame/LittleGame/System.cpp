#include "stdafx.h"
#include "System.h"
#include "Renderer.h"
#include "SceneTest.h"

System::System()
{
}


System::~System()
{
}

void System::Initialize(HINSTANCE hInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	// read settings from file
	// TBA
	m_settings.Initialize();

	m_settings.m_hInstance = hInstance;
	m_settings.m_lpCmdLine = lpCmdLine;
	m_settings.m_nCmdShow = nCmdShow;

	// initialize window in current OS
	InitWindow(hInstance, lpCmdLine, nCmdShow);

	// initialize managers
	Renderer::GetInstance()->Initialize();

	// initialize scenes
	m_scenes.push_back(new SceneTest());
	std::string sName = "SceneTest";
	m_scenes[0]->Initialize(0, &sName);
}

void System::Shutdown()
{
	// shutdown managers
	Renderer::GetInstance()->Shutdown();

	UnregisterClass((m_settings.s_windowTitle.c_str()), m_settings.m_hInstance);
	DestroyWindow(m_settings.m_hwnd);
	PostQuitMessage(0);
}

void System::Run()
{
	while (m_running)
	{
		RunMessages();

		// update scene instances
		m_scenes[m_currentScene]->Update();

		// render one frame
		Renderer::GetInstance()->Run();
	}
}

void System::Pause()
{
}

void System::Stop()
{
	m_running = false;
}

void System::InitWindow(HINSTANCE hInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	WNDCLASS wc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.hCursor = NULL;
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hInstance = hInstance;
	wc.lpfnWndProc = WndProc;
	wc.lpszClassName = (m_settings.s_windowTitle.c_str());
	wc.lpszMenuName = 0;
	wc.style = CS_HREDRAW | CS_VREDRAW;

	RegisterClass(&wc);

	m_settings.m_hwnd = CreateWindow(
		(m_settings.s_windowTitle.c_str()),
		(m_settings.s_windowTitle.c_str()),
		WS_OVERLAPPEDWINDOW,
		10, 10,
		m_settings.s_windowWidth, m_settings.s_windowHeight,
		NULL, NULL,
		hInstance, NULL
		);

	int x = GetLastError();

	ShowWindow(m_settings.m_hwnd, nCmdShow);
	UpdateWindow(m_settings.m_hwnd);
}

inline void System::RunMessages()
{
	MSG msg;

	while (PeekMessageW(&msg, m_settings.m_hwnd, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

LRESULT System::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CREATE:
		break;
	case WM_PAINT:
		break;
	case WM_DESTROY:
		System::GetInstance()->Stop();
		break;
	case WM_SETCURSOR:
		SetCursor(NULL);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
		break;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}