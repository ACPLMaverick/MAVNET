#pragma once
#include "Scene.h"
class SceneSim :
	public Scene
{
public:
	SceneSim(string);
	SceneSim(const SceneSim*);
	~SceneSim();

	virtual unsigned int Initialize();
};

