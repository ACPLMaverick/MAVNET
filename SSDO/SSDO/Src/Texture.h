#pragma once

#include "GlobalDefines.h"
#include "Buffer.h"

class Texture
{
protected:

	Buffer<uint8_t> _rawData;

	int32_t _width = 0;
	int32_t _bpp = 32;
	DXGI_FORMAT _format = DXGI_FORMAT_R8G8B8A8_UINT;
	bool _bMipmapped = false;

	ID3D11Texture2D* _fTexture = nullptr;
	ID3D11SamplerState* _fSampler = nullptr;
	ID3D11ShaderResourceView* _fSRV = nullptr;

	void Shutdown();

public:
	Texture();
	Texture(const std::string& fileName, bool bMakeReadOnly = false);
	inline ~Texture() { Shutdown(); }

	inline Buffer<uint8_t>& GetRawData() { return _rawData; }

	void InitResources(bool bMakeReadOnly = false);

	void Set(int32_t slot);

	inline bool IsReadOnly() { return !_rawData.IsAllocated(); }

	inline void MakeReadOnly()
	{
		if (IsReadOnly())
			return;

		_rawData.Destroy();
	}

	inline void SetWidth(int32_t width)
	{
		if (IsReadOnly())
			return;

		_width = width;
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
	inline int32_t GetBPP() const { return _bpp; }
	inline DXGI_FORMAT GetFormat() const { return _format; }
	inline bool GetMipmapped() const { return _bMipmapped; }
};

