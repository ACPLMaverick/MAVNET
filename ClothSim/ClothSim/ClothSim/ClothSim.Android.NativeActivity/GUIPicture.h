#pragma once
#include "GUIElement.h"
class GUIPicture :
	public GUIElement
{
public:
	GUIPicture(const std::string* s, TextureID* tex);
	GUIPicture(const GUIPicture* c);
	~GUIPicture();

	virtual unsigned int Initialize();
	virtual unsigned int Shutdown();
};

