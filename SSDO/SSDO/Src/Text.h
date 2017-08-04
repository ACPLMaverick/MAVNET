#pragma once

#include "GlobalDefines.h"
#include "Mesh.h"
#include "Shader.h"

class Font;

class Text
{
protected:

	Shader::TextConstantBuffer _bufferData;
	Mesh _mesh;
	std::string _text = "Dummy text";
	Font* _font = nullptr;

	inline void InitCommon();

public:
	Text();
	Text(const std::string& text, Font* font);
	~Text();

	// Assuming correct shader is already set.
	void Draw();

	void SetText(const std::string& text);
	void SetFont(Font* font);
	inline void SetColor(const XMFLOAT4A& color) { _bufferData.Color = color; }
	inline void SetPositionScale(const XMFLOAT4A& positionScale) { _bufferData.PositionScale = positionScale; }
	inline const std::string& GetText() { return _text; }
	inline Font* GetFont() { return _font; }
	inline const XMFLOAT4A& GetColor() { return _bufferData.Color; }
	inline const XMFLOAT4A& GetPositionScale() { return _bufferData.PositionScale; }

};

