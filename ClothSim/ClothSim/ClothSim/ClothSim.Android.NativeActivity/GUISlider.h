#pragma once
#include "GUIElement.h"
#include <vector>

#ifdef PLATFORM_WINDOWS
#include <functional>
#endif

class GUIPicture;
class GUIButton;
class GUIText;

class GUISlider :
	public GUIElement
{
protected:
	std::vector<string> m_valStrings;
	string m_label;
	
	GUIText* m_textLabel;
	GUIText* m_textValue;
	GUIButton* m_sliderHead;
	GUIPicture* m_sliderBar;

	TextureID* m_headTex;
	TextureID* m_barTex;
	TextureID* m_fontTex;
	
	float m_startPoint;
	float m_step;
	float m_length;
	float m_labelMultiplier;
	float m_labelOffset;
	unsigned int m_states;
	unsigned int m_defState;
	unsigned int m_currentState;

	bool labelInitialized = false;


	inline void MoveSliderHead(const glm::vec2* clickPos);
	inline void SetSliderHead(const glm::vec2* clickPos);
public:
	std::vector<std::function<void(unsigned int)>> EventStateChanged;

	GUISlider(const std::string* name, const std::string* label, TextureID* headTex, TextureID* barTex, TextureID* fontTex, unsigned int states, unsigned int defState, float labelMultiplier, float labelOffset);
	GUISlider(const std::string* name, const std::string* label, TextureID* headTex, TextureID* barTex, TextureID* fontTex, unsigned int states, unsigned int defState, std::vector<string>* labels);
	GUISlider(const GUISlider* c);
	~GUISlider();

	virtual unsigned int Initialize();
	virtual unsigned int Shutdown();

	virtual unsigned int Update();

	virtual unsigned int ExecuteClick(const glm::vec2* clickPos);
	virtual unsigned int ExecuteHold(const glm::vec2* clickPos);
};

