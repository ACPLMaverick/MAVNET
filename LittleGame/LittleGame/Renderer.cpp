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
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dParams,
		&m_d3dDevice
		);

	m_d3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_d3dDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_d3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_d3dDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DBLEND_SRCALPHA);
	m_d3dDevice->SetRenderState(D3DRS_ZENABLE, D3DZBUFFERTYPE::D3DZB_TRUE);
	m_d3dDevice->SetRenderState(D3DRS_ZWRITEENABLE, D3DZBUFFERTYPE::D3DZB_TRUE);

	m_d3dDevice->SetRenderState(D3DRS_SHADEMODE, D3DSHADEMODE::D3DSHADE_FLAT);
	m_d3dDevice->SetRenderState(D3DRS_SPECULARENABLE, FALSE);
	m_d3dDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	m_d3dDevice->SetRenderState(D3DRS_AMBIENT, FALSE);
	m_d3dDevice->SetRenderState(D3DRS_COLORVERTEX, FALSE);
	m_d3dDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	m_d3dDevice->SetRenderState(D3DRS_LOCALVIEWER, FALSE);
	m_d3dDevice->SetRenderState(D3DRS_NORMALIZENORMALS, FALSE);

	m_d3dDevice->LightEnable(0, FALSE);

	m_d3dDevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_MODULATE);
	m_d3dDevice->SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_CURRENT);
	m_d3dDevice->SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_TEXTURE);

	m_d3dDevice->SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	m_d3dDevice->SetTextureStageState(1, D3DTSS_ALPHAARG1, D3DTA_CURRENT);
	m_d3dDevice->SetTextureStageState(1, D3DTSS_ALPHAARG2, D3DTA_TEXTURE);

	// CHECKING FOR SUPPORTED SHADER VERSION
	/*
	std::string vprof = D3DXGetVertexShaderProfile(m_d3dDevice) != nullptr ? D3DXGetVertexShaderProfile(m_d3dDevice) : "DUPA";
	std::string pProg = D3DXGetPixelShaderProfile(m_d3dDevice) != nullptr ? D3DXGetPixelShaderProfile(m_d3dDevice) : "DUPA";
	std::string mess = "Supported Vertex Shader Profile: " + vprof + "\n" +
		"Supported Pixel Shader Profile: " + pProg;
	std::wstring wmess(mess.begin(), mess.end());

	MessageBox(settings->GetWindowPtr(), wmess.c_str(), L"Information", 0);
	*/
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