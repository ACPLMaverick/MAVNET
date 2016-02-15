#pragma once

/*
	An abstract class representing action that can be executed when Action() method of GUIButton is activated.
*/

#include "GUIButton.h"

class GUIButton;

class GUIAction
{
protected:
	GUIButton* m_button;
public:
	GUIAction(GUIButton* b);
	GUIAction(const GUIAction* c);
	~GUIAction();

	virtual unsigned int Initialize();
	virtual unsigned int Action(std::vector<void*>* params, const glm::vec2* clickPos) = 0;

	GUIButton* GetMyButton();
};

