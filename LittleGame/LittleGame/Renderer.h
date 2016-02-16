#pragma once

#include "Singleton.h"

#include <d3d9.h>
#include <d3dx9math.h>

class Renderer : public Singleton<Renderer>
{
private:

#pragma region DeviceRelated

	LPDIRECT3D9 d3d;
	LPDIRECT3DDEVICE9 d3dDevice;

#pragma endregion

#pragma region ConstantSettings

	const D3DCOLOR C_CLEAR_COLOR = D3DCOLOR_XRGB(84, 84, 64);

#pragma endregion

public:
	Renderer();
	~Renderer();

	void Initialize();
	void Shutdown();
	void Run();
};
