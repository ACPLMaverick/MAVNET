#include "stdafx.h"
#include "System.h"
#include "Renderer.h"
#include "PhysicsManager.h"
#include "ResourceManager.h"
#include "IdentificationManager.h"
#include "SceneTest.h"
#include "InputManager.h"
#include "Timer.h"
#include "TTSceneGame.h"

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
	Timer::GetInstance()->Initialize();
	Renderer::GetInstance()->Initialize();
	IdentificationManager::GetInstance()->Initialize();
	ResourceManager::GetInstance()->Initialize();
	InputManager::GetInstance()->Initialize();
	PhysicsManager::GetInstance()->Initialize();

	// initialize scenes
	m_scenes.push_back(new TTSceneGame());
	std::string sName = "TTSceneGame";
	m_scenes[0]->Initialize(IdentificationManager::GetInstance()->GetUniqueID(), &sName);
}

void System::Shutdown()
{
	// shutdown managers
	PhysicsManager::GetInstance()->Shutdown();
	InputManager::GetInstance()->Shutdown();
	ResourceManager::GetInstance()->Shutdown();
	IdentificationManager::GetInstance()->Shutdown();
	Renderer::GetInstance()->Shutdown();
	Timer::GetInstance()->Shutdown();

	InputManager::DestroyInstance();
	ResourceManager::DestroyInstance();
	IdentificationManager::DestroyInstance();
	Renderer::DestroyInstance();
	Timer::DestroyInstance();

	UnregisterClass((m_settings.s_windowTitle.c_str()), m_settings.m_hInstance);
	DestroyWindow(m_settings.m_hwnd);
	PostQuitMessage(0);
}

void System::Run()
{
	while (m_running)
	{
		RunMessages();

		// update timer
		Timer::GetInstance()->Update();

		// update input
		InputManager::GetInstance()->Update();

		// update scene instances
		m_scenes[m_currentScene]->Update();

		PhysicsManager::GetInstance()->Run();

		m_scenes[m_currentScene]->LateUpdate();

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

void System::AddEventHandlerMessage(std::function<void(UINT, WPARAM, LPARAM)>* func)
{
	m_eventsMessage.push_back(func);
}

bool System::RemoveEventHandlerMessage(std::function<void(UINT, WPARAM, LPARAM)>* func)
{
	for (std::vector<std::function<void(UINT, WPARAM, LPARAM)>*>::iterator it = m_eventsMessage.begin(); it != m_eventsMessage.end(); ++it)
	{
		if (*it == func)
		{
			m_eventsMessage.erase(it);
			return true;
		}
	}

	return false;
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
	// pass message to event handlers
	for (std::vector<std::function<void(UINT, WPARAM, LPARAM)>*>::iterator it = instance->m_eventsMessage.begin(); it != instance->m_eventsMessage.end(); ++it)
	{
		(*(*it))(message, wParam, lParam);
	}

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