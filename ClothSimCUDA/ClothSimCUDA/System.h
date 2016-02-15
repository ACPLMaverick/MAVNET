#pragma once

//***
// The purpose of this class is to govern all computations that take place within the program, 
// which consist of graphic rendering, CUDA cloth computations and input handling
// main loop is implemented here.
//
// This is a singleton class.
//

#include "Common.h"
#include "Singleton.h"

#include "Renderer.h"
#include "InputManager.h"
#include "InputHandler.h"
#include "PhysicsManager.h"
#include "Scene.h"
#include "SceneTest.h"
#include "SceneSim.h"
#include "Timer.h"

class Renderer;

class System : public Singleton<System>
{
	friend class Singleton<System>;

private:
	System();

	bool m_running;

	Scene* m_scene;
public:
	System(const System*);
	~System();

	unsigned int Initialize();
	unsigned int Shutdown();
	unsigned int Run();
	void Stop();

	Scene* GetCurrentScene();
};

