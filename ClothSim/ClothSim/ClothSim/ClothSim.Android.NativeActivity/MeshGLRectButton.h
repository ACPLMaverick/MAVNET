#pragma once
#include "MeshGLRect.h"
#include "GUIElement.h"

class GUIElement;

class MeshGLRectButton :
	public MeshGLRect
{
protected:
	GUIElement* m_guiEl;
	ShaderID* m_fontShaderID;

public:
	MeshGLRectButton(SimObject* obj, GUIElement* guiEl, glm::vec4* col);
	MeshGLRectButton(const MeshGLRectButton* c);
	~MeshGLRectButton();

	virtual unsigned int Initialize();
	virtual unsigned int Draw();
};

