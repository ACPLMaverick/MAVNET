#include "stdafx.h"
#include "Texture.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include "System.h"

#include <DxErr.h>
#pragma comment(lib, "dxerr.lib")
#pragma comment(lib, "legacy_stdio_definitions.lib")

Texture::Texture()
{
}


Texture::~Texture()
{
}

void Texture::Initialize(const std::string * filePath, const std::string * name)
{
	GameResource::Initialize(filePath, name);

	char* data = nullptr;
	uint32_t dataSize = 0;
	/*
	ResourceManager::GetInstance()->LoadFileFromAssets(&data, &dataSize, &m_filePath, ResourceManager::FileMode::BINARY);

	HRESULT h = D3DXCreateTextureFromFileInMemory(Renderer::GetInstance()->GetDirect3DDevice(), data, dataSize, &m_texture);
	if (h != S_OK)
	{
		std::wstring err = DXGetErrorDescription(h);
		MessageBox(System::GetInstance()->GetSystemSettings()->GetWindowPtr(), err.c_str(), L"Texture loading error!", 0);
	}
	delete data; // !!!!!!!!!!!
	*/

	std::wstring wFilePath(filePath->begin(), filePath->end());
	D3DXCreateTextureFromFile(Renderer::GetInstance()->GetDirect3DDevice(), wFilePath.c_str(), &m_texture);

	GetInfoFromTexture();
}

void Texture::Initialize(const std::string * name, uint32_t width, uint32_t height, D3DXCOLOR* color)
{
	GameResource::Initialize(name, name);

	HRESULT h = D3DXCreateTexture(Renderer::GetInstance()->GetDirect3DDevice(), width, height, 0, NULL, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &m_texture);

	D3DXFillTexture(m_texture, Texture::ColorFill, color);

	GetInfoFromTexture();
}

void Texture::Shutdown()
{
	m_texture->Release();
}

inline void Texture::GetInfoFromTexture()
{
	D3DSURFACE_DESC desc;
	m_texture->GetLevelDesc(0, &desc);

	m_width = desc.Width;
	m_height = desc.Height;
	m_format = (uint32_t)desc.Format;
	m_mipLevels = m_texture->GetLevelCount();
}

VOID Texture::ColorFill(D3DXVECTOR4 * pOut, const D3DXVECTOR2 * pTexCoord, const D3DXVECTOR2 * pTexelSize, LPVOID pData)
{
	D3DXCOLOR col = *(D3DXCOLOR*)pData;
	*pOut = D3DXVECTOR4(col.r, col.g, col.b, col.a);
}
