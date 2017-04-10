#pragma once
#include "GlobalDefines.h"

namespace Lights
{
	__declspec(align(16))
	class LightDirectional
	{
	protected:

		XMFLOAT4 _color;
		XMFLOAT3 _direction;

	public:

		LightDirectional(const XMFLOAT4& color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
			const XMFLOAT3& direction = XMFLOAT3(0.0f, 0.0f, 1.0f));
		~LightDirectional();

		inline const XMFLOAT4& GetColor() const { return _color; }
		inline void SetColor(const XMFLOAT4& col) { _color = col; }
		inline const XMFLOAT3& GetDirection() const { return _direction; }
		inline void SetDirection(const XMFLOAT3& dir) 
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