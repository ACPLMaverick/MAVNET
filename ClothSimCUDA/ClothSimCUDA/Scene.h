#pragma once

/*
	This class is basically a container for all logical objects that take part in the simulation.
	It is an abstract representation. Method Initialize specifies what objects will we load.
*/

#include "Common.h"
#include "SimObject.h"
#include "Camera.h"
#include "LightAmbient.h"
#include "LightDirectional.h"
#include "GUIElement.h"

#include <string>
#include <vector>
#include <map>

using namespace std;

class Scene
{
protected:
	string m_name;

	LightAmbient* m_lAmbient;
	vector<LightDirectional*> m_lDirectionals;

	vector<SimObject*> m_objects;
	vector<Camera*> m_cameras;
	unsigned int m_currentObjectID;
	unsigned int m_currentCameraID;

	map<string, GUIElement*> m_guiElements;
public:
	Scene(string);
	Scene(const Scene*);
	~Scene();

	virtual unsigned int Initialize() = 0;
	unsigned int Shutdown();

	unsigned int Update();
	unsigned int Draw();

	unsigned int GetCurrentObjectID();
	unsigned int GetCurrentCameraID();

	void SetAmbientLight(LightAmbient*);
	void SetAmbientLightDestroyCurrent(LightAmbient*);
	void AddDirectionalLight(LightDirectional*);
	void AddObject(SimObject*);
	void AddCamera(Camera*);
	void AddGUIElement(GUIElement*);

	void RemoveAmbientLight();
	void RemoveDirectionalLight(LightDirectional*);
	void RemoveDirectionalLight(unsigned int);
	void RemoveObject(SimObject*);
	void RemoveCamera(Camera*);
	void RemoveObject(unsigned int);
	void RemoveCamera(unsigned int);
	void RemoveGUIElement(GUIElement*);

	LightAmbient* GetAmbientLight();
	LightDirectional* GetLightDirectional(unsigned int);
	unsigned int GetLightDirectionalCount();
	SimObject* GetObject();
	Camera* GetCamera();
	SimObject* GetObject(unsigned int);
	Camera* GetCamera(unsigned int);
	GUIElement* GetGUIElement(std::string*);
};

