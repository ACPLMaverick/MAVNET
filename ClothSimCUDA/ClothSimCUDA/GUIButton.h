#pragma once

/*
	This class is a simple clickable button, that an action can be assigned to via function pointer.
*/

#include "GUIElement.h"

class GUIButton :
	public GUIElement
{
public:
	GUIButton(const std::string*);
	GUIButton(const GUIButton*);
	~GUIButton();

	virtual unsigned int Initialize();
	virtual unsigned int Shutdown();

	virtual unsigned int Update();
	virtual unsigned int Draw();

	void OnClick();
	void OnHold();
};

