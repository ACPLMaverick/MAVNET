#pragma once

/*
	This GUI element is a simple static text that is drawn over the screen.
*/

#include "GUIElement.h"
#include "Common.h"
#include "MeshGLText.h"

class MeshGLText;

class GUIText :
	public GUIElement
{
protected:
	std::string m_text;
public:
	GUIText(const std::string*, const std::string*, TextureID*);
	GUIText(const GUIText*);
	~GUIText();

	virtual unsigned int Initialize();
	virtual unsigned int Shutdown();

	void SetText(const std::string*);
	void SetFontTextureID(TextureID*);
		
	std::string* GetText();
	TextureID* GetFontTextureID();
};

