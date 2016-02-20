#include "stdafx.h"
#include "Renderer.h"
#include "System.h"
#include "SystemSettings.h"
#include "Scene.h"

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
	
	m_d3d = Direct3DCreate9(D3D_SDK_VERSION);
	m_d3d->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		settings->GetWindowPtr(),
		D3DCREATE_SOFTWARE_VERTEXPROCESSING,
		&d3dParams,
		&m_d3dDevice
		);

	m_d3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
}

void Renderer::Shutdown()
{
	m_d3dDevice->Release();
	m_d3d->Release();
}

void Renderer::Run()
{
	m_d3dDevice->Clear(0, NULL, D3DCLEAR_TARGET, C_CLEAR_COLOR, 1.0f, 0);

	m_d3dDevice->BeginScene();

	// rendering
	System::GetInstance()->GetCurrentScene()->Draw();

	m_d3dDevice->EndScene();

	m_d3dDevice->Present(NULL, NULL, NULL, NULL);
}