#pragma once
#include "Scene.h"
class TTSceneGame :
	public Scene
{
protected:
	virtual void InitializeScene();
public:
	TTSceneGame();
	~TTSceneGame();
};

