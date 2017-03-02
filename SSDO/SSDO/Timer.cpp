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
	int64_t counts;
	QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&counts));
	QueryPerformanceFrequency(reinterpret_cast<LARGE_INTEGER*>(&_frequency));

	_startTime = static_cast<float>(counts) / static_cast<float>(_frequency);
	_totalTime = 0.0f;
	_prevTotalTime = 0.0f;
	_deltaTime = 0.0f;
}

void Timer::Run()
{
	int64_t counts;
	QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&counts));

	float currentTime = static_cast<float>(counts) / static_cast<float>(_frequency);
	_prevTotalTime = _totalTime;
	_totalTime = currentTime - _startTime;
	_deltaTime = _totalTime - _prevTotalTime;

	//DEBUG(L"TotalTime: " + to_wstring(_totalTime) + L" | DeltaTime: " + to_wstring(_deltaTime));
}

void Timer::Shutdown()
{
}
