#pragma once

#include "GlobalDefines.h"
#include "Buffer.h"

class Texture
{
protected:

	Buffer<uint8_t> _rawData;

	int32_t _width = 0;
	int32_t _height = 0;
	int32_t _bpp = 32;
	DXGI_FORMAT _format = DXGI_FORMAT_R8G8B8A8_UINT;
	bool _bMipmapped = false;
	bool _bReadOnly = false;
	bool _bIsRenderTarget = false;

	ID3D11Texture2D* _fTexture = nullptr;
	ID3D11SamplerState* _fSampler = nullptr;
	ID3D11ShaderResourceView* _fSRV = nullptr;
	ID3D11RenderTargetView* _fRTV = nullptr;

	void Shutdown();

public:
	Texture();
	Texture(const std::string& fileName, bool bMakeReadOnly = false);
	inline ~Texture() { Shutdown(); }

	inline Buffer<uint8_t>& GetRawData() { return _rawData; }

	void InitResources(bool bMakeReadOnly = false, bool bIsRenderTarget = false);

	void Set(int32_t slot);
	void SetAsRenderTarget(int32_t slot);

	inline bool IsReadOnly() { return _bReadOnly; }

	inline void MakeReadOnly()
	{
		if (IsReadOnly())
			return;

		_bReadOnly = true;
		if(_rawData.IsAllocated())
			_rawData.Destroy();
	}

	inline void SetWidth(int32_t width)
	{
		if (IsReadOnly())
			return;

		_width = width;
	}

	inline void SetHeight(int32_t height)
	{
		if (IsReadOnly())
			return;

		_height = height;
	}

	inline void SetBPP(int32_t bpp)
	{
		if (IsReadOnly())
			return;

		_bpp = bpp;
	}

	inline void SetFormat(DXGI_FORMAT format)
	{
		if (IsReadOnly())
			return;

		_format = format;
	}

	inline void SetMipmapped(bool mipmapped)
	{
		if (IsReadOnly())
			return;

		_bMipmapped = mipmapped;
	}

	inline int32_t GetWidth() const { return _width; }
	inline int32_t GetHeight() const { return _height; }
	inline int32_t GetBPP() const { return _bpp; }
	inline DXGI_FORMAT GetFormat() const { return _format; }
	inline bool GetMipmapped() const { return _bMipmapped; }


	static void ClearTextureSlot(int32_t slot);
};

