#pragma once
#include "LightAmbient.h"

namespace Lights
{
	class LightDirectional :
		public LightAmbient
	{
	protected:

		XMFLOAT3 _direction;

	public:

		LightDirectional(const XMFLOAT4& color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
			const XMFLOAT3& direction = XMFLOAT3(0.0f, 0.0f, 1.0f));
		~LightDirectional();

		inline const XMFLOAT3& GetDirection() const { return _direction; }
		inline void SetColor(const XMFLOAT3& dir) { _direction = dir; }
	};
}