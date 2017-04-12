#include "stdafx.h"
#include "Postprocess.h"
#include "Shader.h"

namespace Postprocesses
{
	Postprocess::Postprocess()
	{
	}

	Postprocess::~Postprocess()
	{
	}

	void Postprocess::SetShader() const
	{
		_shader->Set();
	}

}