#pragma once
#include "GlobalDefines.h"

namespace Lights
{
	__declspec(align(16))
	class LightPoint
	{
	protected:

		XMFLOAT4 _color;
		XMFLOAT3 _position;
		float _range;

	public:

		LightPoint(const XMFLOAT4& color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
			const XMFLOAT3& position = XMFLOAT3(0.0f, 0.0f, 0.0f), float range = 10.0f);
		~LightPoint();

		inline const XMFLOAT4& GetColor() const { return _color; }
		inline void SetColor(const XMFLOAT4& col) { _color = col; }
		inline const XMFLOAT3& GetPosition() const { return _position; }
		inline void SetColor(const XMFLOAT3& pos) { _position = pos; }

		inline float GetRange() const { return _range; }
		inline void SetRange(float rng) { _range = rng; }

		inline LightPoint& operator=(const LightPoint& other)
		{
			_color = other._color;
			_position = other._position;
			_range = other._range;
			return *this;
		}
	};
}