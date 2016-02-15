#include "System.h"

System::System()
{
	m_running = false;
	m_exit = false;
	m_scene = nullptr;
}

System::System(const System*)
{

}


System::~System()
{

}



unsigned int System::Initialize()
{
	unsigned int err = CS_ERR_NONE;

	// initializing main singletons

#ifdef PLATFORM_WINDOWS

	m_engine = new Engine();
	m_engine->height = 0;
	m_engine->width = 0;

#endif

	err = Renderer::GetInstance()->Initialize();
	if (err != CS_ERR_NONE) return err;

	err = ResourceManager::GetInstance()->Initialize();
	if (err != CS_ERR_NONE) return err;

	err = InputManager::GetInstance()->Initialize();
	if (err != CS_ERR_NONE) return err;

	err = PhysicsManager::GetInstance()->Initialize();
	if (err != CS_ERR_NONE) return err;

	InputHandler::GetInstance();

	Timer::GetInstance()->Initialize();

	// initializing scene and objects
	m_scene = new SceneTest("SceneTest");
	err = m_scene->Initialize();
	if (err != CS_ERR_NONE) return err;

	Renderer::GetInstance()->Flush();

	m_running = true;

#ifdef _DEBUG
	printf("\nSystem.Initialize has completed successfully.\n");
#endif
	return err;
}

unsigned int System::Shutdown()
{
	unsigned int err = CS_ERR_NONE;

	//if (!m_running)
	//	return CS_ERR_SHUTDOWN_PENDING;

	m_running = false;

	// shutting down scene and objects
	err = m_scene->Shutdown();
	if (err != CS_ERR_NONE) return err;

	// shutting down main singletons

	err = Renderer::GetInstance()->Shutdown();
	if (err != CS_ERR_NONE) return err;

	err = InputManager::GetInstance()->Shutdown();
	if (err != CS_ERR_NONE) return err;

	err = PhysicsManager::GetInstance()->Shutdown();
	if (err != CS_ERR_NONE) return err;

	err = ResourceManager::GetInstance()->Shutdown();
	if (err != CS_ERR_NONE) return err;
	
	Renderer::DestroyInstance();
	InputManager::DestroyInstance();
	InputHandler::DestroyInstance();
	PhysicsManager::DestroyInstance();
	ResourceManager::DestroyInstance();

#ifdef PLATFORM_WINDOWS
	delete m_engine;
#endif

	// shutting down gui

#ifdef _DEBUG
	printf("\nSystem.Shutdown has completed successfully.\n");
#endif
	return err;
}

// main loop runs here
unsigned int System::Run()
{
	unsigned int err = CS_ERR_NONE;

#ifdef PLATFORM_WINDOWS

	while (m_running)
	{
		err = Tick();
		if (err != CS_ERR_NONE)
			return err;
	}

#else

	while (!m_exit)
	{
		while (m_running)
		{
			err = Tick();
		}
		//else
		//{
		//	err = RunAndroid();
		//	if (err != CS_ERR_NONE)
		//		return err;
		//}
		//Shutdown();
		ANativeActivity_finish(m_engine->app->activity);

		while (!m_running && !m_exit)
		{
			err = RunAndroid();
			if (err != CS_ERR_NONE)
				return err;
		}

		ASensorManager_destroyEventQueue(instance->m_engine->sensorManager, instance->m_engine->sensorEventQueue);
		ShutdownAndroid();

	}

#endif

	return err;
}


unsigned int System::Tick()
{
	unsigned int err = CS_ERR_NONE;

	// update timer
	err = Timer::GetInstance()->Run();
	if (err != CS_ERR_NONE)
		return err;

#ifndef PLATFORM_WINDOWS
	// update android-related stuff, mainly events
	err = RunAndroid();
	if (err != CS_ERR_NONE)
	{
		LOGW("DESTROY!");
		return err;
	}
#endif // !PLATFORM_WINDOWS		

	// update input
	err = InputManager::GetInstance()->Run();
	if (err != CS_ERR_NONE)
		return err;

	// update gui

	// update scene
	err = m_scene->Update();
	if (err != CS_ERR_NONE)
		return err;

	// compute collisions
	err = PhysicsManager::GetInstance()->Run();
	if (err != CS_ERR_NONE)
		return err;

	// draw one frame
	err = Renderer::GetInstance()->Run();
	if (err != CS_ERR_NONE)
		return err;

	return err;
}

void System::Stop()
{
	m_running = false;
}


/**
* Initialize android necessary stuff
*/

#ifndef PLATFORM_WINDOWS

unsigned int System::InitAndroid(android_app* app)
{
	m_engine = new Engine();
	app->userData = m_engine;
	app->onAppCmd = System::AHandleCmd;
	app->onInputEvent = InputManager::AHandleInput;
	app->activity->callbacks->onNativeWindowRedrawNeeded = Renderer::AHandleResize;
	m_engine->app = app;

	// Prepare to monitor accelerometer
	m_engine->sensorManager = ASensorManager_getInstance();
	m_engine->accelerometerSensor = ASensorManager_getDefaultSensor(m_engine->sensorManager,
		ASENSOR_TYPE_ACCELEROMETER);
	m_engine->sensorEventQueue = ASensorManager_createEventQueue(m_engine->sensorManager,
		m_engine->app->looper, LOOPER_ID_USER, NULL, NULL);

	if (m_engine->app->savedState != NULL) {
		// We are starting with a previous saved state; restore from it.
		m_engine->state = *(struct saved_state*)app->savedState;
	}

	m_engine->animating = 1;
	/*
	public void onWindowFocusChanged(boolean hasFocus) {
	super.onWindowFocusChanged(hasFocus);
	if (hasFocus) {
	decorView.setSystemUiVisibility(
	View.SYSTEM_UI_FLAG_LAYOUT_STABLE
	| View.SYSTEM_UI_FLAG_LAYOUT_HIDE_NAVIGATION
	| View.SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN
	| View.SYSTEM_UI_FLAG_HIDE_NAVIGATION
	| View.SYSTEM_UI_FLAG_FULLSCREEN
	| View.SYSTEM_UI_FLAG_IMMERSIVE_STICKY);
	}
	}
	*/
	// loop waiting for recieving context from android

	while (true)
	{
		int ident;
		int events;
		android_poll_source* source;

		// loop until all events are read, then continue
		// this is necessary to recieve ANativeActivity pointer
		while (
			ident = ALooper_pollAll(m_engine->animating ? 0 : -1, NULL, &events, (void**)&source) >= 0
			)
		{
			// process this event
			if (source != NULL)
			{
				source->process(m_engine->app, source);
			}

			// originally sensor data processing was here

			// check if we are exiting
			if (m_engine->app->destroyRequested != 0)
			{
				return CS_ANDROID_ERROR;
			}
		}

		if (m_engine->app->window != NULL)
			break;
	}

	return CS_ERR_NONE;
}

/**
* Destroy android stuff
*/
unsigned int System::ShutdownAndroid()
{
	delete m_engine;

	return CS_ERR_NONE;
}

/**
* Run android stuff, mainly events
*/
unsigned int System::RunAndroid()
{
	// Read all pending events.
	int ident;
	int events;
	struct android_poll_source* source;

	// If not animating, we will block forever waiting for events.
	// If animating, we loop until all events are read, then continue
	// to draw the next frame of animation.
	while ((ident = ALooper_pollAll(m_engine->animating ? 0 : -1, NULL, &events,
		(void**)&source)) >= 0)
	{

		// Process this event.
		if (source != NULL) {
			source->process(m_engine->app, source);
		}

		// If a sensor has data, process it now.
		if (ident == LOOPER_ID_USER) {
			// Checking for accelrometer data is commented out.

			if (m_engine->accelerometerSensor != NULL) {
				ASensorEvent event;
				while (ASensorEventQueue_getEvents(m_engine->sensorEventQueue,
					&event, 1) > 0) {

					//LOGI("accelerometer: x=%f y=%f z=%f",
					//event.acceleration.x, event.acceleration.y,
					//event.acceleration.z);
					InputManager::GetInstance()->UpdateAcceleration(&event.acceleration);
				}
			}

		}

		// Check if we are exiting.
		if (m_engine->app->destroyRequested != 0) 
		{
			m_engine->app->activity->vm->DetachCurrentThread();
			return CS_ERR_SHUTDOWN_PENDING;
		}
	}

	return CS_ERR_NONE;
}

/**
* Process the next main command.
*/
void System::AHandleCmd(struct android_app* app, int32_t cmd)
{
	struct Engine* engine = (struct Engine*)app->userData;
	switch (cmd) {
	case APP_CMD_SAVE_STATE:

		// The system has asked us to save our current state.  Do so.
		engine->app->savedState = malloc(sizeof(struct saved_state));
		*((struct saved_state*)engine->app->savedState) = engine->state;
		engine->app->savedStateSize = sizeof(struct saved_state);

		break;

	case APP_CMD_INIT_WINDOW:
		// Initalize everything.
		if (engine->app->window != NULL)
		{
			System::GetInstance()->Initialize();

			Timer::GetInstance()->Run();

			InputManager::GetInstance()->Run();

			System::GetInstance()->m_scene->Update();

			PhysicsManager::GetInstance()->Run();

			Renderer::GetInstance()->Run();
		}
		else
		{
			LOGW("System: Window is NULL!");
		}

		break;

	case APP_CMD_TERM_WINDOW:

		// Shutdown and clean up everything.
		System::GetInstance()->Shutdown();

		break;

	case APP_CMD_DESTROY:

		instance->m_exit = true;

		break;

	case APP_CMD_GAINED_FOCUS:

		// When our app gains focus, we start monitoring the accelerometer.
		engine->animating = 1;
		if (engine->accelerometerSensor != NULL) {
			ASensorEventQueue_enableSensor(engine->sensorEventQueue,
				engine->accelerometerSensor);
			// We'd like to get 60 events per second (in us).
			ASensorEventQueue_setEventRate(engine->sensorEventQueue,
				engine->accelerometerSensor, (1000L / 60) * 1000);
		}

		break;

	case APP_CMD_LOST_FOCUS:

		// When our app loses focus, we stop monitoring the accelerometer.
		// This is to avoid consuming battery while not being used.
		if (engine->accelerometerSensor != NULL) {
			ASensorEventQueue_disableSensor(engine->sensorEventQueue,
				engine->accelerometerSensor);
		}

		// Also stop animating.
		engine->animating = 0;
		Timer::GetInstance()->Run();

		InputManager::GetInstance()->Run();

		System::GetInstance()->m_scene->Update();

		PhysicsManager::GetInstance()->Run();

		Renderer::GetInstance()->Run();

		break;
	}
}


#endif // !PLATFORM_WINDOWS

Engine* System::GetEngineData()
{
	return m_engine;
}

bool System::GetRunning()
{
	return m_running;
}

Scene* System::GetCurrentScene()
{
	return m_scene;
}