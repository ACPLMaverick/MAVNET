#pragma once

/*
	This component is the main controller of the whole simulation. Mainly responds to user input.
*/

#include "Common.h"
#include "Component.h"
#include "InputHandler.h"

#include <sstream>
#include <iomanip>

class GUIText;

class GUIController :
	public Component
{
private:
	bool cursorHideHelper = false;
	double infoTimeDisplayHelper = 0.0;

	const float BOX_SPEED = 0.005f;
	const float INFO_UPDATE_RATE = 120.0f;

	GUIText* m_fpsText;
	GUIText* m_dtText;
	GUIText* m_ttText;

	void DoubleToStringPrecision(double, int, std::string*);
public:
	GUIController(SimObject* obj);
	GUIController(const GUIController*);
	~GUIController();

	virtual unsigned int Initialize();
	virtual unsigned int Shutdown();

	virtual unsigned int Update();
	virtual unsigned int Draw();
};

