#pragma once

#include "Singleton.h"

#include <d3d9.h>
#include <d3dx9math.h>

class Renderer : public Singleton<Renderer>
{
	friend class Singleton<Renderer>;
private:

#pragma region DeviceRelated

	LPDIRECT3D9 m_d3d;
	LPDIRECT3DDEVICE9 m_d3dDevice;

#pragma endregion

#pragma region ConstantSettings

	const D3DCOLOR C_CLEAR_COLOR = D3DCOLOR_XRGB(84, 84, 64);

#pragma endregion

	Renderer();

public:
	~Renderer();

	void Initialize();
	void Shutdown();
	void Run();

#pragma region Accessors

	LPDIRECT3D9 const GetDirect3D() { return m_d3d; }
	LPDIRECT3DDEVICE9 const GetDirect3DDevice() { return m_d3dDevice; }

#pragma endregion
};

