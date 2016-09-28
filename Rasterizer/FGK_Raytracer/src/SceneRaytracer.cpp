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
}