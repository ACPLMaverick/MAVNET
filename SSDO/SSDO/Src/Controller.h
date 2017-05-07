#pragma once

#include "GlobalDefines.h"

namespace Scenes
{
	class Scene;
}

namespace Postprocesses
{
	class Postprocess;
}

class Controller
{
protected:

	Scenes::Scene* _scene;
	Postprocesses::Postprocess* _ssao;

	float _cameraSpeed;
	float _cameraBoost;
	float _cameraRotateSpeedX;
	float _cameraRotateSpeedY;

public:
	Controller(Scenes::Scene* scene);
	~Controller();

	virtual void Initialize();
	virtual void Update();
	virtual void Shutdown();
};

