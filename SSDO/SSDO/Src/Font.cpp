#include "stdafx.h"
#include "Font.h"
#include "System.h"

#include <ft2build.h>
#include FT_FREETYPE_H

#define Lib() System::GetInstance()->GetFreeTypeLibrary()

void Font::LoadFont(std::wstring fileName)
{
	const int32_t aTabSize = 128;
	_alignmentTab.Allocate(aTabSize);
	_alignmentTab.Resize(aTabSize);

	// R8_UNORM format
	const int32_t rawDataSize = 2 * aTabSize * _charSize * _charSize * sizeof(uint8_t);
	const int32_t rawDataPitch = static_cast<int32_t>(sqrt(rawDataSize) + 0.1);
	const float rawDataPitchRec = 1.0f / ((float)rawDataPitch);

	_texture.GetRawData().Allocate(rawDataSize);
	_texture.GetRawData().Resize(rawDataSize);
	ZEROM(_texture.GetRawData().GetDataPtr(), _texture.GetRawData().GetSize());
	ZEROM(_alignmentTab.GetDataPtr(), _alignmentTab.GetSizeBytes());

	FT_Face face;
	FT_Error error;
	std::string pathComplete(fileName.begin(), fileName.end());
	pathComplete = "./Resources/Fonts/" + pathComplete + ".ttf";
	error = FT_New_Face(Lib(), pathComplete.c_str(), 0, &face);
	ASSERT(!error);

	_name = face->family_name;
	_name += " ";
	_name += face->style_name;

	// set correct face size
	error = FT_Set_Pixel_Sizes(face, 0, _charSize);
	ASSERT(!error);

	// calculate min max variables
	uint32_t maxWidth = 0, maxHeight = 0;
	int32_t minLeft = INT32_MAX, minTop = INT32_MAX;
	for (int32_t c = 0; c < aTabSize; ++c)
	{
		if (FT_Load_Char(face, static_cast<FT_ULong>(c), FT_LOAD_RENDER))
		{
			continue;
		}

		if (face->glyph->bitmap_left < minLeft)
		{
			minLeft = face->glyph->bitmap_left;
		}
		if (face->glyph->bitmap_top < minTop)
		{
			minTop = face->glyph->bitmap_top;
		}
		if (face->glyph->bitmap.width > maxWidth)
		{
			maxWidth = face->glyph->bitmap.width;
		}
		if (face->glyph->bitmap.rows > maxHeight)
		{
			maxHeight = face->glyph->bitmap.rows;
		}
	}


	int32_t advX = 0, advY = 0;
	int32_t pxOffset = 3;
	// load glyphs to raw data (R8_UNORM format)
	for (int32_t c = 0; c < aTabSize; ++c)
	{
		if (FT_Load_Char(face, static_cast<FT_ULong>(c), FT_LOAD_RENDER))
		{
			_alignmentTab[c] = CharAlignment();
			continue;
		}

		FT_GlyphSlot g = face->glyph;

		int32_t baseX = advX + g->bitmap_left - minLeft;
		int32_t baseY = advY + (_charSize - (g->bitmap_top + minTop));
		int32_t lastX = (baseX + g->bitmap.width);
		int32_t lastY = (baseY + g->bitmap.rows);
		ASSERT(baseX >= 0 && baseY >= 0 && baseY < static_cast<int32_t>(rawDataPitch));

		if (lastX >= static_cast<int32_t>(rawDataPitch) || baseX >= static_cast<int32_t>(rawDataPitch))
		{
			advX = CLAMP(static_cast<int32_t>(advX - rawDataPitch), 0, static_cast<int32_t>(rawDataPitch));
			advY += _charSize;

			baseX = advX + g->bitmap_left - minLeft;
			baseY = advY + (_charSize - (g->bitmap_top + minTop));
			lastX = min((baseX + static_cast<int32_t>(g->bitmap.width)), rawDataPitch);
			lastY = min((baseY + static_cast<int32_t>(g->bitmap.rows)), rawDataPitch);
		}

		if (lastY > rawDataPitch)
			lastY = rawDataPitch;

		// copy from bitmap buffer to raw data at correct position
		for (int32_t i = baseY, ib = 0; i < lastY; ++i, ++ib)
		{
			for (int32_t j = baseX, jb = 0; j < lastX; ++j, ++jb)
			{
				// convert j and i to 1D index
				int32_t indexRawData = j + (rawDataPitch - i) * rawDataPitch;
				int32_t indexBitmap = jb + ib * g->bitmap.width;
				ASSERT(indexRawData > 0 && indexRawData < static_cast<int32_t>(_texture.GetRawData().GetSize()) && indexBitmap < static_cast<int32_t>(g->bitmap.width * g->bitmap.rows));

				_texture.GetRawData()[indexRawData] = g->bitmap.buffer[indexBitmap];
			}
		}

		// load character alignment for this char

		int32_t currentAdvX = (g->advance.x >> 6);
		int32_t currentAdvY = (g->advance.y >> 6);

		_alignmentTab[c] = CharAlignment
		(
			(float)baseX * rawDataPitchRec,
			(float)baseY * rawDataPitchRec,
			(float)(max((int32_t)currentAdvX, (int32_t)g->bitmap.width)) * rawDataPitchRec,
			(float)(g->bitmap.rows) * rawDataPitchRec,
			(float)(g->bitmap_top - static_cast<FT_Int>(g->bitmap.rows)) * rawDataPitchRec
		);

		advX += currentAdvX + pxOffset;
		advY += currentAdvY;

		if (advX >= static_cast<int32_t>(rawDataPitch))
		{
			advX -= rawDataPitch;
			advY += _charSize;
		}
	}

	// Create texture resource
	_texture.SetWidth(rawDataPitch);
	_texture.SetMipmapped(true);
	_texture.SetBPP(8);
	_texture.SetFormat(DXGI_FORMAT::DXGI_FORMAT_R8_UNORM);

	/*size_t siz = _texture.GetRawData().GetSize();
	for (size_t i = 0; i < siz; ++i)
	{
		uint8_t data = _texture.GetRawData()[i];
		std::wstring ds(to_wstring(data) + L"\n");
		OutputDebugString(ds.c_str());
	}*/

	_texture.InitResources(true);
}

void Font::Shutdown()
{
}