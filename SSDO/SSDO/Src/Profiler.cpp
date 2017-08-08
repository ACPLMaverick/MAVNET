#include "stdafx.h"
#include "Profiler.h"
#include "System.h"
#include "Timer.h"
#include "Scenes\Scene.h"
#include "Text.h"

Profiler::Profiler(Scenes::Scene* scene) : 
	_scene(scene),
	_baseTime(Timer::GetInstance(), &Timer::GetDeltaTime),
	_postprocessTime(Timer::GetInstance(), &Timer::GetDeltaTime, 0.01f)
{
}


Profiler::~Profiler()
{
}

void Profiler::Initialize()
{
	_tLblFPS = new Text("FPS: ");
	_tLblMsGlobal = new Text("Global DT: ");
	_tLblMsPostprocess = new Text("SSDO DT: ");
	_tValFPS = new Text("Value", false);
	_tValMsGlobal = new Text("Value", false);
	_tValMsPostprocess = new Text("Value", false);

	_allTexts[0] = _tLblFPS;
	_allTexts[1] = _tLblMsGlobal;
	_allTexts[2] = _tLblMsPostprocess;
	_allTexts[3] = _tValFPS;
	_allTexts[4] = _tValMsGlobal;
	_allTexts[5] = _tValMsPostprocess;

	const float xBasePos(-0.9f);
	const float yBasePos(0.9f);
	const float xAddition(0.25f);
	const float yAddition(-0.06f);
	const size_t textCountDiv2(TEXT_COUNT / 2);

	for (size_t i = 0; i < TEXT_COUNT; ++i)
	{
		_allTexts[i]->SetColor(XMFLOAT4A(0.1f, 0.8f, 0.2f, 1.0f));
		_allTexts[i]->SetScale(0.2f);

		_scene->AddText(_allTexts[i]);

		float currentXAddition(i >= textCountDiv2 ? xAddition : 0.0f);
		float currentYAddition(yAddition * (float)(i % textCountDiv2));

		const XMFLOAT2A currentPos(xBasePos + currentXAddition, yBasePos + currentYAddition);
		_allTexts[i]->SetPosition(currentPos);
	}


	_tLblFPS->SetPosition(XMFLOAT2A(_tLblFPS->GetPosition().x - 0.032f, _tLblFPS->GetPosition().y));
}

void Profiler::Update()
{
	_baseTime.Update();
	// ssdo time is updated externally

	float fps(1.0f / _baseTime.GetAveragedValue());
	_tValFPS->SetText(to_string(fps));
	_tValMsGlobal->SetText(to_string(_baseTime.GetAveragedValue()));
	_tValMsPostprocess->SetText(to_string(_postprocessTime.GetAveragedValue()));
	//OutputDebugString((L"Dupsko " + to_wstring(_postprocessTime.GetAveragedValue()) + L"\n").c_str());
}

void Profiler::UpdatePostprocessBegin()
{
	_postprocessTime.UpdatePeriodBegin();
}

void Profiler::UpdatePostprocessEnd()
{
	_postprocessTime.UpdatePeriodEnd();
}

void Profiler::Shutdown()
{
}

AveragedTime::AveragedTime(Timer * timer, UpdateFuncPtr funcPtr, float averagePeriod) :
	_timer(timer),
	_funcPtr(funcPtr),
	_averagePeriod(averagePeriod),
	_averaged(0.0f),
	_averageTemp(0.0f),
	_averageAccumulator(0.0f),
	_averageDivisor(0.0f)
{
}

AveragedTime::~AveragedTime()
{
}

void AveragedTime::Update()
{
	UpdateInternal(_timer->GetDeltaTime());
}

void AveragedTime::UpdatePeriodBegin()
{
	_averageTemp = _timer->GetNowTime();
}

void AveragedTime::UpdatePeriodEnd()
{
	float dt = _timer->GetNowTime() - _averageTemp;
	UpdateInternal(dt);
}

inline void AveragedTime::UpdateInternal(float dt)
{
	_averageAccumulator += dt;
	_averageDivisor += 1.0f;

	if (_averageAccumulator >= _averagePeriod)
	{
		ASSERT(_averageDivisor != 0.0f);
		_averaged = _averageAccumulator / _averageDivisor;
		_averageAccumulator = 0.0f;
		_averageDivisor = 0.0f;
	}
}
