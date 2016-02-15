#pragma once

/*
	This is a mesh used to represent 2D text to draw on the screen.
*/

#define CHAR_HEIGHT 16
#define CHAR_WIDTH 16
#define FIELD_WIDTH 10
#define FIELD_HEIGHT 20
#define START_CHAR 32
#define CHAR_COUNT 256
#define SIZE_MULTIPLIER 0.1f
#define SPACE_BETWEEN_LETTERS 0.35f
#define Y_COMPENSATION 2.5f

#include "MeshGL.h"
#include "GUIText.h"

#include <string>

class GUIText;

class MeshGLText :
	public MeshGL
{
protected:
	GUIText* m_guiText;
	const string* m_text;
	unsigned long m_textLetterCount;

	ShaderID* m_fontShaderID;

	virtual void GenerateVertexData();
	void BindVertexData();
	void UpdateVertexDataUV();
public:
	MeshGLText(SimObject*, const string*);
	MeshGLText(GUIText*, const string*);
	MeshGLText(const MeshGLText*);
	~MeshGLText();

	virtual unsigned int Initialize();
	virtual unsigned int Draw();

	const string* GetText();
	void SetText(const string* text);
};

