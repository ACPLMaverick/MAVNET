#include "stdafx.h"
#include "System.h"
#include "Scene.h"
#include "SceneRaytracer.h"

System::System() :
	m_BufferColor(800, 600),
	m_BufferDepth(800, 600)
{
}


System::~System()
{
}

void System::Initialize(HINSTANCE hInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	m_settings.m_hInstance = hInstance;
	m_settings.m_lpCmdLine = lpCmdLine;
	m_settings.m_nCmdShow = nCmdShow;

	// initialize window in current OS
	InitWindow(hInstance, lpCmdLine, nCmdShow);

	// initialize DIB
	RECT r;
	GetClientRect(m_settings.m_hwnd, &r);
	
	ZeroMemory(&m_bitmapScreenBufferInfo, sizeof(BITMAPINFO));
	m_bitmapScreenBufferInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	m_bitmapScreenBufferInfo.bmiHeader.biWidth = r.right - r.left;
	m_bitmapScreenBufferInfo.bmiHeader.biHeight = r.bottom - r.top;
	m_bitmapScreenBufferInfo.bmiHeader.biPlanes = 1;
	m_bitmapScreenBufferInfo.bmiHeader.biBitCount = 32;
	m_bitmapScreenBufferInfo.bmiHeader.biSizeImage = m_settings.GetWindowWidth() * m_settings.GetWindowHeight();
	m_bitmapScreenBufferInfo.bmiHeader.biCompression = BI_RGB;
	m_bitmapScreenBufferInfo.bmiHeader.biSizeImage = 0;

	HDC dc = GetWindowDC(m_settings.m_hwnd);
	m_bitmapScreenBuffer = CreateDIBSection(dc, &m_bitmapScreenBufferInfo, DIB_RGB_COLORS, &m_bitmapScreenBufferDataPtr, NULL, NULL);

	// initialize managers

	// initialize scenes
	m_scenes.push_back(new SceneRaytracer());
	std::string sName = "SceneRaytracer";
	m_scenes[0]->Initialize(0, &sName);
}

void System::Shutdown()
{
	// shutdown managers

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

		m_BufferColor.Fill(0x00FF0000);
		m_BufferDepth.Fill(0.0f);

		// draw scene to buffer
		m_scenes[m_currentScene]->Draw(&m_BufferColor, &m_BufferDepth);

		// draw buffer to window
		RECT r;
		GetClientRect(m_settings.m_hwnd, &r);
		InvalidateRect(m_settings.m_hwnd, &r, false);
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

inline void System::ResizeWindowBitmap()
{
	if (m_bitmapScreenBuffer == nullptr || m_bitmapScreenBufferDataPtr == nullptr)
		return;

}

inline void System::DrawColorBuffer()
{
	if (m_bitmapScreenBuffer == nullptr || m_bitmapScreenBufferDataPtr == nullptr)
		return;

	PAINTSTRUCT ps;
	BITMAP nbm;
	HDC hdc = BeginPaint(m_settings.m_hwnd, &ps);

	// fill bitmap with color buffer data
	size_t tj = m_bitmapScreenBufferInfo.bmiHeader.biWidth;
	size_t ti = m_bitmapScreenBufferInfo.bmiHeader.biHeight;
	for (size_t i = 0; i < ti; ++i)
	{
		for (size_t j = 0; j < tj; ++j)
		{
			((int32_t*)m_bitmapScreenBufferDataPtr)[i * tj + j] = (int32_t)m_BufferColor.GetPixelScaled((uint16_t)j, (uint16_t)i, tj, ti);
		}
	}

	HDC hdcMem = CreateCompatibleDC(hdc);
	HBITMAP oldBitmap = (HBITMAP)SelectObject(hdcMem, (HBITMAP)m_bitmapScreenBuffer);
	GetObject((HBITMAP)m_bitmapScreenBuffer, sizeof(BITMAP), &nbm);
	BitBlt(hdc, 0, 0, ps.rcPaint.right - ps.rcPaint.left, ps.rcPaint.bottom - ps.rcPaint.top,
		hdcMem, 0, 0, SRCCOPY);

	SelectObject(hdcMem, oldBitmap);
	DeleteDC(hdcMem);

	EndPaint(m_settings.m_hwnd, &ps);
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
	case WM_SIZE:
		{
			System::GetInstance()->ResizeWindowBitmap();
		}

		break;
	case WM_PAINT:
		{
			System::GetInstance()->DrawColorBuffer();
		}
		
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