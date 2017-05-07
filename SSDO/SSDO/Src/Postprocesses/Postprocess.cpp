#include "stdafx.h"
#include "Postprocess.h"
#include "Shader.h"
#include "Camera.h"

namespace Postprocesses
{
	Postprocess::Postprocess()
	{
	}

	Postprocess::~Postprocess()
	{
	}

	void Postprocess::SetPass(const Camera& camera, int32_t passIndex) const
	{
		_shaders[passIndex]->Set();
		Shader::LightCommonDataPS* cd = reinterpret_cast<Shader::LightCommonDataPS*>(_shaders[passIndex]->MapPsBuffer(0));
		cd->gProjInverse = camera.GetMatProjInverse();
		cd->gViewInverse = camera.GetMatViewInverse();
		cd->gViewPosition = camera.GetPosition();
		_shaders[passIndex]->UnmapPsBuffer(0);
	}

}