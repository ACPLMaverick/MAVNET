#pragma once
#include "GlobalDefines.h"

namespace Lights
{
	__declspec(align(16))
	class LightAmbient
	{
	protected:

		XMFLOAT4 _color;

	public:
		LightAmbient(const XMFLOAT4 color);
		~LightAmbient();

		inline const XMFLOAT4& GetColor() const { return _color; }
		inline void SetColor(const XMFLOAT4& col) { _color = col; }

		inline LightAmbient& operator=(const LightAmbient& other)
		{
			_color = other._color;
			return *this;
		}
	};

}