#pragma once

/*
	Handles time management. Time is given in SECONDS (as it is standard SI unit).
*/

#include "Singleton.h"

class Timer :
	public Singleton<Timer>
{
	friend class Singleton<Timer>;
protected:

	double m_freq = 1.0;

	double m_startTime = 0.0;
	double m_currentTime = 0.0;
	double m_deltaTime = 0.0;
	double m_fps = 0.0;

	Timer();

	virtual inline double GetCurrentTimeS();
public:

	~Timer();

	virtual void Initialize();
	virtual void Shutdown();
	virtual void Update();

#pragma region Accessors

	double GetStartTime() { return m_startTime; }
	double GetActualTime() { return m_currentTime; }
	double GetDeltaTime() { return m_deltaTime; }
	double GetTotalTimeElapsed() { return m_currentTime - m_startTime; }
	double GetFPS() { return m_fps; }

#pragma endregion

};

