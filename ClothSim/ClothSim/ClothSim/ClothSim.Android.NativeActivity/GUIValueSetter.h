#pragma once
#include "GUIElement.h"
#include <vector>
#ifdef PLATFORM_WINDOWS
#include <functional>
#endif

class GUIPicture;
class GUIButton;
class GUIText;

class GUIValueSetter :
	public GUIElement
{
protected:
	std::vector<string> m_valStrings;
	string m_label;
	
	GUIText* m_textLabel;
	GUIText* m_textValue;
	GUIButton* m_btnLeft;
	GUIButton* m_btnRight;

	TextureID* m_leftTex;
	TextureID* m_rightTex;
	TextureID* m_leftTex_a;
	TextureID* m_rightTex_a;
	TextureID* m_fontTex;
	
	float m_labelMultiplier;
	float m_labelOffset;
	int m_labelDigits;
	float m_labelPosOffset;
	unsigned int m_states;
	unsigned int m_defState;
	unsigned int m_currentState;
	bool m_rewind;
	bool m_reversed;

	float m_timeHelperMS = 0.0f;
	bool m_firstHold = false;
	const float M_HOLD_DELTA_MS = 250.0f;
	const float M_HOLD_SPEEDUP_RATE = 0.75f;
	float m_currentHoldDeltaMS = M_HOLD_DELTA_MS;

	bool labelInitialized = false;

	inline void ModifyStateOnHold(unsigned int mVal);
	inline void ModifyState(unsigned int mVal);
public:
	std::vector<std::function<void(unsigned int)>> EventStateChanged;

	GUIValueSetter(const std::string* name, const std::string* label, TextureID* leftTex, TextureID* leftTex_a, TextureID* rightTex, TextureID* rightTex_a,
		TextureID* fontTex, unsigned int states, unsigned int defState, float labelMultiplier, float labelOffset, int labelDigits, float labelposoffset, bool rewind, bool reversed);
	GUIValueSetter(const std::string* name, const std::string* label, TextureID* leftTex, TextureID* leftTex_a, TextureID* rightTex, TextureID* rightTex_a,
		TextureID* fontTex, unsigned int states, unsigned int defState, std::vector<string>* labels, float labelposoffset, bool rewind, bool reversed);
	GUIValueSetter(const GUIValueSetter* c);
	~GUIValueSetter();

	virtual unsigned int Initialize();
	virtual unsigned int Shutdown();

	virtual unsigned int Update();

	virtual unsigned int ExecuteClick(const glm::vec2* clickPos);
	virtual unsigned int ExecuteHold(const glm::vec2* clickPos);
	virtual void CleanupAfterHold();

	unsigned int GetCurrentState();
	void SetCurrentState(unsigned int state);

	float GetLabelMultiplier();
	float GetLabelOffset();
	unsigned int GetStates();
};

