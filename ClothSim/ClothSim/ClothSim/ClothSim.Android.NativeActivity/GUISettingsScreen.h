#pragma once
#include "GUIElement.h"
#include "ClothSimulator.h"

#ifdef PLATFORM_WINDOWS
#include <functional>
#endif

class GUIPicture;
class GUIButton;
class GUIText;
class GUIValueSetter;
class ClothSimulator;

class GUISettingsScreen :
	public GUIElement
{
protected:
	const float M_BTN_X_OFFSET = 0.3f;

	GUIPicture* m_bg;
	GUIButton* m_btnApply;
	GUIButton* m_btnCancel;
	GUIText* m_lblSettings;
	GUIValueSetter* m_vsSimType;		// 0 - MS_GPU, 1 - MS_CPU, 2 - MS_CPUx4, 3 - PB_GPU, 4 - PB_CPU, 5 - PB_CPUx4
	GUIValueSetter* m_vsObjType;		// sphere, box
	GUIValueSetter* m_vsGravity;		// 0.1f - 10.0f
	GUIValueSetter* m_vsWidth;			// 1.0f - 50.0f
	GUIValueSetter* m_vsLength;			// 1.0f - 50.0f
	GUIValueSetter* m_vsEdgesWidth;		// 0 - 126
	GUIValueSetter* m_vsEdgesLength;	// 0 - 126
	GUIValueSetter* m_vsElasticity;		// 0.0f - 100.0f
	GUIValueSetter* m_vsMass;			// 1.0f - 100.0f
	GUIValueSetter* m_vsAirDamp;		// 0.0f - 1.0f
	GUIValueSetter* m_vsElDamp;			// 0.0f - 100.0f

	std::vector<GUIValueSetter*> m_vsVector;
	std::vector<unsigned int> m_vsStatesVector;

	std::vector<void*> m_paramsApply;
	std::vector<void*> m_paramsCancel;

	SimParams m_params;

	static void ApplyButtonClick(std::vector<void*>* params, const glm::vec2* clickPos);
	static void CancelButtonClick(std::vector<void*>* params, const glm::vec2* clickPos);

	inline void UpdateParams();
	inline void ResetStates();
public:
	std::vector<std::function<void(std::vector<void*>* params, const glm::vec2* clickPos)>> EventApply;
	std::vector<std::function<void(std::vector<void*>* params, const glm::vec2* clickPos)>> EventCancel;

	GUISettingsScreen(const std::string* id);
	GUISettingsScreen(const GUISettingsScreen* c);
	~GUISettingsScreen();

	virtual unsigned int Initialize();
	virtual unsigned int Shutdown();

	SimParams* GetSimParams();

	void SetParamsApply(void* params);
	void SetParamsCancel(void* params);
	std::vector<void*>* GetParamsApply();
	std::vector<void*>* GetParamsCancel();

	void SetCancelEnabled(bool enabled);
};

