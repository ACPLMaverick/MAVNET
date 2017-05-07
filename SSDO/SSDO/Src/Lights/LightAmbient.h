#pragma once
#include "GlobalDefines.h"

class Camera;

namespace Lights
{
	__declspec(align(16))
	class LightAmbient
	{
	protected:

		XMFLOAT4A _color;

	public:
		LightAmbient(const XMFLOAT4A color);
		~LightAmbient();

		inline const XMFLOAT4A& GetColor() const { return _color; }
		inline void SetColor(const XMFLOAT4A& col) { _color = col; }

		inline LightAmbient& operator=(const LightAmbient& other)
		{
			_color = other._color;
			return *this;
		}

		// This must be called on pointer received from constant buffer.
		void ApplyToShader(const LightAmbient& other, const Camera& camera);
	};

}