#include "SceneRaytracer.h"
#include "Camera.h"
#include "Triangle.h"


SceneRaytracer::SceneRaytracer()
{
}


SceneRaytracer::~SceneRaytracer()
{
}

void SceneRaytracer::InitializeScene()
{
	m_cameras.push_back(new Camera());
	m_primitives.push_back(new Triangle(
		Float3(-0.5f, -0.5f, 0.0f), 
		Float3(0.0f, 0.8f, 0.0f), 
		Float3(0.5f, -0.5f, 0.0f)
	));
}