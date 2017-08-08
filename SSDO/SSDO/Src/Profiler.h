#pragma once

#include "GlobalDefines.h"

namespace Scenes
{
	class Scene;
}

class Timer;
class Text;

class AveragedTime
{
public:

	typedef float (Timer::*UpdateFuncPtr)() const;

protected:

	Timer* _timer;
	UpdateFuncPtr _funcPtr;
	float _averaged;
	float _averagePeriod;
	float _averageTemp;
	float _averageAccumulator;
	float _averageDivisor;

	inline void UpdateInternal(float dt);

public:

	AveragedTime(Timer* timer, UpdateFuncPtr funcPtr, float averagePeriod = 0.5f);
	~AveragedTime();

	void Update();
	void UpdatePeriodBegin();
	void UpdatePeriodEnd();

	inline float GetAveragedValue() { return _averaged; }
};

class Profiler
{
protected:

	static const size_t TEXT_COUNT = 6;

	AveragedTime _baseTime;
	AveragedTime _postprocessTime;

	Scenes::Scene* _scene;

	Text* _allTexts[TEXT_COUNT];
	Text* _tLblFPS = nullptr;
	Text* _tValFPS = nullptr;
	Text* _tLblMsGlobal = nullptr;
	Text* _tValMsGlobal = nullptr;
	Text* _tLblMsPostprocess = nullptr;
	Text* _tValMsPostprocess = nullptr;

public:
	Profiler(Scenes::Scene* scene);
	~Profiler();

	virtual void Initialize();
	virtual void Update();
	virtual void UpdatePostprocessBegin();
	virtual void UpdatePostprocessEnd();
	virtual void Shutdown();
};

