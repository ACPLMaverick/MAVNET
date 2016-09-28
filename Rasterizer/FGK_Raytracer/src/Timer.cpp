#include "stdafx.h"
#include "Timer.h"


Timer::Timer()
{
}


Timer::~Timer()
{
}

void Timer::Initialize()
{
	LARGE_INTEGER li;
	QueryPerformanceFrequency(&li);
	m_freq = double(li.QuadPart);

	m_startTime = GetCurrentTimeS();
	m_currentTime = m_startTime;
}

void Timer::Shutdown()
{
}

void Timer::Update()
{
	double time = GetCurrentTimeS();
	m_deltaTime = time - m_currentTime;
	m_currentTime = time;

	m_fps = 1.0f / max(m_deltaTime, 0.00000000001);
}

double Timer::GetCurrentTimeS()
{
	double t;
	LARGE_INTEGER li;
	QueryPerformanceCounter(&li);
	t = double(li.QuadPart) / m_freq - m_startTime;
	return t;
}