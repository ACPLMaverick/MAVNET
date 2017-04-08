#pragma once
#include "GlobalDefines.h"

namespace Lights
{
	class LightAmbient
	{
	protected:

		XMFLOAT4 _color;

	public:
		LightAmbient(const XMFLOAT4 color);
		~LightAmbient();

		virtual void Update() { }

		inline const XMFLOAT4& GetColor() const { return _color; }
		inline void SetColor(const XMFLOAT4& col) { _color = col; }
	};

}