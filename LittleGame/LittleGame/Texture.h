#pragma once

#include "GameResource.h"

#include <d3dx9.h>

class Texture : public GameResource
{
protected:

	IDirect3DTexture9* m_texture;
	uint32_t m_width;
	uint32_t m_height;
	uint32_t m_mipLevels;
	uint32_t m_format;

	inline void GetInfoFromTexture();
	static VOID WINAPI ColorFill(D3DXVECTOR4* pOut, const D3DXVECTOR2* pTexCoord,
		const D3DXVECTOR2* pTexelSize, LPVOID pData);
public:
	Texture();
	~Texture();

	virtual void Initialize(const std::string* filePath, const std::string* name = nullptr);
	virtual void Initialize(const std::string* name, uint32_t width, uint32_t height, D3DXCOLOR* color);
	virtual void Shutdown();

	IDirect3DTexture9* const GetDX9Texture() const { return m_texture; }
};

