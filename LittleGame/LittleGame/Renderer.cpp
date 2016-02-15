#include "stdafx.h"
#include "Renderer.h"
#include "System.h"
#include "SystemSettings.h"

Renderer::Renderer()
{
}


Renderer::~Renderer()
{
}

void Renderer::Initialize()
{
	SystemSettings* settings = System::GetInstance()->GetSystemSettings();
	D3DPRESENT_PARAMETERS d3dParams;

	ZeroMemory(&d3dParams, sizeof(d3dParams));

	d3dParams.Windowed = !settings->GetWindowFullscreen();
	d3dParams.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dParams.hDeviceWindow = settings->GetWindowPtr();
	
	d3d = Direct3DCreate9(D3D_SDK_VERSION);
	d3d->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		settings->GetWindowPtr(),
		D3DCREATE_SOFTWARE_VERTEXPROCESSING,
		&d3dParams,
		&d3dDevice
		);
}

void Renderer::Shutdown()
{
	d3dDevice->Release();
	d3d->Release();
}

void Renderer::Run()
{
	d3dDevice->Clear(0, NULL, D3DCLEAR_TARGET, C_CLEAR_COLOR, 1.0f, 0);

	d3dDevice->BeginScene();

	// rendering

	d3dDevice->EndScene();

	d3dDevice->Present(NULL, NULL, NULL, NULL);
}
