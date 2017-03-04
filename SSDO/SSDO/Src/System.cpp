#include "stdafx.h"
#include "System.h"
#include "Resource.h"

#include "Renderer.h"
#include "Timer.h"

#include "Scenes/SceneTest.h"

System::System() :
	_options(1280, 720, 60, false)
{
}

System::~System()
{
	Renderer::DestroyInstance();
	Timer::DestroyInstance();
}

void System::Initialize(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	InitInstance(hInstance, nCmdShow);

	Timer::GetInstance()->Initialize();
	Renderer::GetInstance()->Initialize();

	_scene = new Scenes::SceneTest();
	_scene->Initialize();
}

void System::Run()
{
	while (!_bNeedToQuit)
	{
		_scene->Update();
		Renderer::GetInstance()->Run();
		RunMessageLoop();
		Timer::GetInstance()->Run();
	}
}

void System::Shutdown()
{
	if (_scene != nullptr)
	{
		_scene->Shutdown();
		delete _scene;
	}

	Renderer::GetInstance()->Shutdown();
	Timer::GetInstance()->Shutdown();
}

ATOM System::MyRegisterClass(HINSTANCE _hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = _hInstance;
	wcex.hIcon = LoadIcon(_hInstance, MAKEINTRESOURCE(IDI_SSDO));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_SSDO);
	wcex.lpszClassName = _szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}


void System::InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	_hInst = hInstance; // Store instance handle in our global variable

	LoadStringW(_hInst, IDS_APP_TITLE, _szTitle, MAX_LOADSTRING);
	LoadStringW(_hInst, IDC_SSDO, _szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(_hInst);

	_hwnd = CreateWindowW(_szWindowClass, _szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, _options._windowWidth, _options._windowHeight, nullptr, nullptr, hInstance, nullptr);

	ASSERT_D(_hwnd, L"HWND is null");

	ShowWindow(_hwnd, nCmdShow);
	UpdateWindow(_hwnd);
}

void System::RunMessageLoop()
{
	HACCEL hAccelTable = LoadAccelerators(_hInst, MAKEINTRESOURCE(IDC_SSDO));

	MSG msg;

	// Main message loop:
	if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK System::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_EXIT:
			System::GetInstance()->_bNeedToQuit = true;
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code that uses hdc here...
		EndPaint(hWnd, &ps);
	}
	break;
	case WM_DESTROY:
		System::GetInstance()->_bNeedToQuit = true;
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
