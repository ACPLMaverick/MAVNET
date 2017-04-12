#pragma once

#include "GlobalDefines.h"

class Shader;

namespace Postprocesses
{
	class Postprocess
	{
	protected:

		Shader* _shader;

	public:

		Postprocess();
		~Postprocess();

		virtual void Update() = 0;
		void SetShader() const;
	};
}