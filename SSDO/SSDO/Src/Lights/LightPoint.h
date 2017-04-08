#pragma once
#include "LightAmbient.h"

namespace Lights
{
	class LightPoint :
		public LightAmbient
	{
	protected:

		XMFLOAT3 _position;
		float _range;

	public:

		LightPoint(const XMFLOAT4& color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
			const XMFLOAT3& position = XMFLOAT3(0.0f, 0.0f, 0.0f), float range = 10.0f);
		~LightPoint();

		inline const XMFLOAT3& GetPosition() const { return _position; }
		inline void SetColor(const XMFLOAT3& pos) { _position = pos; }

		inline float GetRange() const { return _range; }
		inline void SetRange(float rng) { _range = rng; }
	};
}