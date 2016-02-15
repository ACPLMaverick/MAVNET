#include "Scene.h"
#include "Renderer.h"


Scene::Scene(string n)
{
	m_name = n;

	m_lAmbient = nullptr;

	m_currentObjectID = 0;
	m_currentCameraID = 0;
}

Scene::Scene(const Scene*)
{
}


Scene::~Scene()
{
}

unsigned int Scene::Shutdown()
{
	unsigned int err = CS_ERR_NONE;

	delete m_lAmbient;

	for (map<string, GUIElement*>::iterator it = m_guiElements.begin(); it != m_guiElements.end(); ++it)
	{
		delete it->second;
	}
	m_guiElements.clear();

	for (vector<LightDirectional*>::iterator it = m_lDirectionals.begin(); it != m_lDirectionals.end(); ++it)
	{
		delete (*it);
	}
	m_lDirectionals.clear();

	for (vector<SimObject*>::iterator it = m_objects.begin(); it != m_objects.end(); ++it)
	{
		err = (*it)->Shutdown();

		if (err != CS_ERR_NONE)
			return err;

		delete (*it);
	}
	m_objects.clear();

	for (vector<Camera*>::iterator it = m_cameras.begin(); it != m_cameras.end(); ++it)
	{
		err = (*it)->Shutdown();

		if (err != CS_ERR_NONE)
			return err;

		delete (*it);
	}
	m_cameras.clear();

	return CS_ERR_NONE;
}

unsigned int Scene::Update()
{
	unsigned int err = CS_ERR_NONE;

	for (map<string, GUIElement*>::iterator it = m_guiElements.begin(); it != m_guiElements.end(); ++it)
	{
		err = it->second->Update();

		if (err != CS_ERR_NONE)
			return err;
	}

	for (vector<SimObject*>::iterator it = m_objects.begin(); it != m_objects.end(); ++it)
	{
		err = (*it)->Update();

		if (err != CS_ERR_NONE)
			return err;
	}

	for (vector<Camera*>::iterator it = m_cameras.begin(); it != m_cameras.end(); ++it)
	{
		err = (*it)->Update();

		if (err != CS_ERR_NONE)
			return err;
	}

	return CS_ERR_NONE;
}

unsigned int Scene::Draw()
{
	unsigned int err;

	for (vector<SimObject*>::iterator it = m_objects.begin(); it != m_objects.end(); ++it)
	{
		err = (*it)->Draw();

		if (err != CS_ERR_NONE)
			return err;
	}

	if (Renderer::GetInstance()->GetDrawMode() != WIREFRAME)
	{
		for (map<string, GUIElement*>::iterator it = m_guiElements.begin(); it != m_guiElements.end(); ++it)
		{
			err = it->second->Draw();

			if (err != CS_ERR_NONE)
				return err;
		}
	}

	return CS_ERR_NONE;
}



unsigned int Scene::GetCurrentObjectID()
{
	return m_currentObjectID;
}

unsigned int Scene::GetCurrentCameraID()
{
	return m_currentCameraID;
}

void Scene::SetAmbientLight(LightAmbient* amb)
{
	m_lAmbient = amb;
}

void Scene::SetAmbientLightDestroyCurrent(LightAmbient* amb)
{
	if (m_lAmbient != nullptr)
	{
		delete m_lAmbient;
	}
	m_lAmbient = amb;
}

void Scene::AddDirectionalLight(LightDirectional* dir)
{
	m_lDirectionals.push_back(dir);
}

void Scene::AddObject(SimObject* obj)
{
	m_objects.push_back(obj);
}

void Scene::AddCamera(Camera* cam)
{
	m_cameras.push_back(cam);
}

void Scene::AddGUIElement(GUIElement* gui)
{
	m_guiElements.emplace(*gui->GetID(), gui);
}


void Scene::RemoveAmbientLight()
{
	delete m_lAmbient;
	m_lAmbient = nullptr;
}

void Scene::RemoveDirectionalLight(LightDirectional* ptr)
{
	for (vector<LightDirectional*>::iterator it = m_lDirectionals.begin(); it != m_lDirectionals.end(); ++it)
	{
		if (ptr == (*it))
		{
			m_lDirectionals.erase(it);
			break;
		}
	}
}

void Scene::RemoveDirectionalLight(unsigned int which)
{
	unsigned int ctr = 0;
	for (vector<LightDirectional*>::iterator it = m_lDirectionals.begin(); it != m_lDirectionals.end(); ++it, ++ctr)
	{
		if (ctr == which)
		{
			m_lDirectionals.erase(it);
			break;
		}
	}
}

void Scene::RemoveObject(SimObject* ptr)
{
	for (vector<SimObject*>::iterator it = m_objects.begin(); it != m_objects.end(); ++it)
	{
		if (ptr == (*it))
		{
			m_objects.erase(it);
			break;
		}
	}
}

void Scene::RemoveCamera(Camera* ptr)
{
	for (vector<Camera*>::iterator it = m_cameras.begin(); it != m_cameras.end(); ++it)
	{
		if (ptr == (*it))
		{
			m_cameras.erase(it);
			break;
		}
	}
}

void Scene::RemoveObject(unsigned int which)
{
	int ctr = 0;
	for (vector<SimObject*>::iterator it = m_objects.begin(); it != m_objects.end(); ++it, ++ctr)
	{
		if (ctr == which)
		{
			m_objects.erase(it);
			break;
		}
	}
}

void Scene::RemoveCamera(unsigned int which)
{
	int ctr = 0;
	for (vector<Camera*>::iterator it = m_cameras.begin(); it != m_cameras.end(); ++it, ++ctr)
	{
		if (ctr == which)
		{
			m_cameras.erase(it);
			break;
		}
	}
}

void Scene::RemoveGUIElement(GUIElement* gui)
{
	m_guiElements.erase(*gui->GetID());
}


LightAmbient* Scene::GetAmbientLight()
{
	return m_lAmbient;
}

LightDirectional* Scene::GetLightDirectional(unsigned int which)
{
	if (which < m_lDirectionals.size())
		return m_lDirectionals.at(which);
	else
		return nullptr;
}

unsigned int Scene::GetLightDirectionalCount()
{
	return m_lDirectionals.size();
}

SimObject* Scene::GetObject()
{
	return m_objects.at(m_currentObjectID);
}

Camera* Scene::GetCamera()
{
	return m_cameras.at(m_currentCameraID);
}

SimObject* Scene::GetObject(unsigned int which)
{
	if (which < m_objects.size())
		return m_objects.at(which);
	else
		return nullptr;
}

Camera* Scene::GetCamera(unsigned int which)
{
	if (which < m_cameras.size())
		return m_cameras.at(which);
	else
		return nullptr;
}

GUIElement* Scene::GetGUIElement(std::string* id)
{
	return m_guiElements.at(*id);
}
