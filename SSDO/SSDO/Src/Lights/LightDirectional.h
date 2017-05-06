#pragma once
#include "GlobalDefines.h"

namespace Lights
{
	__declspec(align(16))
	class LightDirectional
	{
	protected:

		XMFLOAT4A _color;
		XMFLOAT3A _direction;

	public:

		LightDirectional(const XMFLOAT4A& color = XMFLOAT4A(1.0f, 1.0f, 1.0f, 1.0f),
			const XMFLOAT3A& direction = XMFLOAT3A(0.0f, 0.0f, 1.0f));
		~LightDirectional();

		inline const XMFLOAT4A& GetColor() const { return _color; }
		inline void SetColor(const XMFLOAT4A& col) { _color = col; }
		inline const XMFLOAT3A& GetDirection() const { return _direction; }
		inline void SetDirection(const XMFLOAT3A& dir) 
		{ 
			XMVECTOR vDir = XMLoadFloat3(&dir);
			vDir *= -1.0f;
			vDir = XMVector3Normalize(vDir);
			XMStoreFloat3(&_direction, vDir);
		}

		inline LightDirectional& operator=(const LightDirectional& other)
		{
			_color = other._color;
			_direction = other._direction;
			return *this;
		}
	};
}