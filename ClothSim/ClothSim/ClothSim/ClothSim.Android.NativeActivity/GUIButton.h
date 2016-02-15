#pragma once

/*
	This class is a simple clickable button, that an action can be assigned to via function pointer.
*/

#include "GUIElement.h"
#include "GUIAction.h"
#include "InputHandler.h"
#include "InputManager.h"
#include "MeshGLRectButton.h"

#include <vector>
#ifdef PLATFORM_WINDOWS
#include <functional>
#endif

class GUIAction;
class MeshGLRectButton;

class GUIButton :
	public GUIElement
{
protected:
	glm::vec4 m_color;


	TextureID* m_textureClicked;

	std::vector<void*> m_paramsClick;
	std::vector<void*> m_paramsHold;
	
	virtual void GenerateTransformMatrix();
	void ComputeScaleFactors(glm::vec2* factors);

public:
	std::vector<std::function<void(std::vector<void*>* params, const glm::vec2* clickPos)>> EventClick;
	std::vector<std::function<void(std::vector<void*>* params, const glm::vec2* clickPos)>> EventHold;

	GUIButton(const std::string* id);
	GUIButton(const GUIButton*);
	~GUIButton();

	virtual unsigned int Initialize();
	virtual unsigned int Shutdown();

	void SetTextures(TextureID* texIdle, TextureID* texClicked);
	void RemoveTextures();

	void SetParamsClick(void* params);
	void SetParamsHold(void* params);
	std::vector<void*>* GetParamsClick();
	std::vector<void*>* GetParamsHold();

	virtual unsigned int ExecuteClick(const glm::vec2* clickPos);
	virtual unsigned int ExecuteHold(const glm::vec2* clickPos);
};

