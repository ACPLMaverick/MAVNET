#pragma once

#include "stdafx.h"
#include "Singleton.h"

class Timer : public Singleton<Timer>
{
protected:

#pragma region Protected

	int64_t _frequency = 0;
	float _startTime = 0.0f;
	float _totalTime = 0.0f;
	float _prevTotalTime = 0.0f;
	float _deltaTime = 0.0f;

#pragma endregion

#pragma region Functions Protected

#pragma endregion

public:

#pragma region Functions Public

	Timer();
	~Timer();

	void Initialize();
	void Run();
	void Shutdown();

	float GetTotalTime() const { return _totalTime; }
	float GetDeltaTime() const { return _deltaTime; }
	float GetFPS() const { return 1.0f / _deltaTime; }

#pragma endregion
};

