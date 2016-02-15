#pragma once

/*
	This component is the main controller of the whole simulation. Mainly responds to user input.
*/

#include "Common.h"
#include "Component.h"
#include "InputHandler.h"
#include "Settings.h"

class GUIText;
class GUIButton;
class ClothSimulator;
class GUISettingsScreen;

#define BOX_SPEED 0.005f

class GUIController :
	public Component
{
private:
	enum MovementDirection
	{
		FORWARD = 1,
		BACKWARD,
		LEFT,
		RIGHT,
		UP,
		DOWN
	};

	enum InputMode
	{
		ARROWS = 1,
		TOUCH
	};

	double infoTimeDisplayHelper = 0.0;

	const float INFO_UPDATE_RATE = 1000.0f;

	InputMode m_inputMode = InputMode::ARROWS;

	GUIText* m_fpsText;
	GUIText* m_dtText;
	GUIText* m_ttText;
	GUIText* m_stText;
	GUIText* m_dText;

	std::vector<GUIElement*> m_otherGroups;
	GUISettingsScreen* m_sScreen;

	GUIButton* m_btnMvArrows;
	GUIButton* m_btnMvTouch;
	int m_modeChangeHelper = -1;

	GUIElement* m_btnArrowsGroup;

	ClothSimulator* m_cSimulator;
	bool m_firstRun = true;

	static void ActionExitProgram(std::vector<void*>* params, const glm::vec2* clickPos);
	static void ActionMoveActiveObject(std::vector<void*>* params, const glm::vec2* clickPos);
	static void ActionSetDisplayMode(std::vector<void*>* params, const glm::vec2* clickPos);
	static void ActionShowPreferences(std::vector<void*>* params, const glm::vec2* clickPos);
	static void ActionApplyPreferences(std::vector<void*>* params, const glm::vec2* clickPos);
	static void ActionCancelPreferences(std::vector<void*>* params, const glm::vec2* clickPos);
	static void ActionSwitchInputMode(std::vector<void*>* params, const glm::vec2* clickPos);
public:
	GUIController(SimObject* obj);
	GUIController(const GUIController*);
	~GUIController();

	virtual unsigned int Initialize();
	virtual unsigned int Shutdown();

	virtual unsigned int Update();
	virtual unsigned int Draw();
};

