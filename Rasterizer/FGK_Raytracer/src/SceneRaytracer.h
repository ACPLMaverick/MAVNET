#pragma once
#include "Scene.h"

class SceneRaytracer :
	public Scene
{
protected:

#pragma region Functions Protected

	virtual void InitializeScene() override;

#pragma endregion

public:
	SceneRaytracer();
	~SceneRaytracer();
};

