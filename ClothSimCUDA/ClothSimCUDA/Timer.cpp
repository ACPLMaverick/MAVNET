#include "Timer.h"

Timer::Timer()
{
	m_deltaTime = 0.0;
	m_fixedDelta = 0.0;
	m_fps = 0.0;
	m_totalTime = 0.0;
	m_ticks = 0;
}

Timer::Timer(const Timer*)
{
}

Timer::~Timer()
{
}



unsigned int Timer::Initialize()
{
	glfwSetTime(0.0);
	return CS_ERR_NONE;
}

unsigned int Timer::Shutdown()
{
	return CS_ERR_NONE;
}

unsigned int Timer::Run()
{
	//high_resolution_clock::time_point point = high_resolution_clock::now();
	//duration<long double> timeSpan = duration_cast<duration<long double>>(point - m_start);
	
	//double newTime = timeSpan.count() * 1000.0;
	double newTime = glfwGetTime() * 1000.0;
	m_deltaTime = newTime - m_totalTime;
	m_totalTime = newTime;

	if (m_ticks < TICKS_TO_UNLOCK_FIXED)
	{
		m_fixedDelta += m_deltaTime;
	}
	else if (m_ticks == TICKS_TO_UNLOCK_FIXED)
	{
		m_deltaTime /= (double)TICKS_TO_UNLOCK_FIXED;
	}

	m_fps = 1.0 / (m_deltaTime / 1000.0);
	++m_ticks;

	//printf("%f, %f, %f\n", m_totalTime, m_deltaTime, m_fps);

	return CS_ERR_NONE;
}


void Timer::AddTimeStamp(unsigned int id)
{
	std::map<unsigned int, double>::iterator it;
	if ((it = m_timeStamps.find(id)) != m_timeStamps.end())
	{
		it->second = GetTotalTime();
	}
	else
	{
		m_timeStamps.emplace(id, GetTotalTime());
	}
}



double Timer::GetTotalTime()
{
	return m_totalTime;
}

double Timer::GetDeltaTime()
{
	return m_deltaTime;
}

double Timer::GetFixedDeltaTime()
{
	if (m_ticks <= TICKS_TO_UNLOCK_FIXED)
		return FIXED_DELTA;
	else
		return m_fixedDelta;
}

double Timer::GetFps()
{
	return m_fps;
}

double Timer::GetTimeStamp(unsigned int id)
{
	return m_timeStamps.at(id);
}

double Timer::GetTimeStampClear(unsigned int id)
{
	double t = GetTimeStamp(id);
	m_timeStamps.erase(id);

	return t;
}

