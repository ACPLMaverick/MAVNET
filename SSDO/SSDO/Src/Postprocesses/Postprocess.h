#pragma once

#include "GlobalDefines.h"
#include <vector>

class Shader;
class Camera;

namespace Postprocesses
{
	class Postprocess
	{
	protected:

		std::vector<Shader*> _shaders;

	public:

		Postprocess();
		~Postprocess();

		virtual void Update() = 0;
		virtual void SetPass(const Camera& camera, int passIndex = 0) const;
		virtual inline int GetPassCount() const { return 1; }
		virtual inline Shader* GetShader(int passIndex = 0) const { return const_cast<Shader*>(_shaders[passIndex]); }
	};
}