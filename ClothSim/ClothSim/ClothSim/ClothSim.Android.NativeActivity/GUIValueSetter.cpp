#include "GUIValueSetter.h"
#include "GUIText.h"
#include "GUIPicture.h"
#include "GUIButton.h"


GUIValueSetter::GUIValueSetter(const std::string * name, const std::string* label, TextureID* leftTex, TextureID* leftTex_a, TextureID* rightTex, TextureID* rightTex_a, TextureID * fontTex,
	unsigned int states, unsigned int defState, float labelMultiplier, float labelOffset, int labeldigits, float labelposoffset, bool rewind, bool reversed) : GUIElement(name)
{
	m_label = *label;

	m_leftTex = leftTex;
	m_rightTex = rightTex;
	m_leftTex_a = leftTex_a;
	m_rightTex_a = rightTex_a;
	m_fontTex = fontTex;
	m_states = states;
	m_defState = defState;
	m_labelDigits = labeldigits;
	m_labelPosOffset = labelposoffset;
	m_rewind = rewind;
	m_reversed = reversed;
	
	m_labelMultiplier = labelMultiplier;
	m_labelOffset = labelOffset;

	m_isBlockable = true;
	m_isScaled = false;
}

GUIValueSetter::GUIValueSetter(const std::string * name, const std::string* label, TextureID* leftTex, TextureID* leftTex_a, TextureID* rightTex, TextureID* rightTex_a, TextureID * fontTex,
	unsigned int states, unsigned int defState, std::vector<std::string>* labels, float labelposoffset, bool rewind, bool reversed) 
	: GUIValueSetter(name, label, leftTex, leftTex_a, rightTex, rightTex_a, fontTex, states, defState, 0.0f, 0.0f, 0, labelposoffset, rewind, reversed)
{
	size_t size = labels->size();

	if (size != states)
		LOGW("GUIValueSetter: label size is not the same as state count.");

	for (size_t i = 0; i < size && i < (size_t)states; ++i)
	{
		m_valStrings.push_back(labels->at(i));
	}

	if (size < (size_t)states)
	{
		size_t dCount = (size_t)states - size;
		for (size_t i = 0; i < dCount; ++i)
		{
			m_valStrings.push_back("???");
		}
	}

	labelInitialized = true;
}

GUIValueSetter::GUIValueSetter(const GUIValueSetter * c) : GUIElement(c)
{
}

GUIValueSetter::~GUIValueSetter()
{
}

unsigned int GUIValueSetter::Initialize()
{
	unsigned int err = CS_ERR_NONE;

	err = GUIElement::Initialize();
	if (err != CS_ERR_NONE)
		return err;

	if (!labelInitialized)
	{
		for (unsigned int i = 0; i < m_states; ++i)
		{
			float curr = (float)i * m_labelMultiplier + m_labelOffset;
			string str;
			DoubleToStringPrecision(curr, m_labelDigits, &str);
			m_valStrings.push_back(str);
		}
	}
	m_currentState = m_defState;
	float sclFactor = glm::min(m_scale.x, m_scale.y);
	float sclMplier = 2.2f;
	float txtMplier = 0.75f;
	glm::vec2 itemScale = glm::vec2(sclFactor, sclFactor);
	glm::vec2 offsetLbl = glm::vec2(-m_scale.x * 0.45f, m_scale.y);

	string idTl = m_id + "_TextLabel";
	string idTv = m_id + "_TextVal";
	string idBt = m_id + "_BtnLeft";
	string idPc = m_id + "_BtnRight";

	m_textLabel = new GUIText(&idTl, &m_label, m_fontTex);
	m_textLabel->SetScale(itemScale * txtMplier);
	m_textLabel->SetPosition(m_position + offsetLbl);
	m_textLabel->Initialize();
	AddChild(m_textLabel);

	m_textValue = new GUIText(&idTv, &(m_valStrings.at(m_currentState)), m_fontTex);
	m_textValue->SetScale(itemScale * txtMplier);
	m_textValue->SetPosition(m_position - glm::vec2(sclFactor * 2.0f - m_labelPosOffset, sclFactor * 1.5f));
	m_textValue->Initialize();
	AddChild(m_textValue);

	m_btnLeft = new GUIButton(&idBt);
	m_btnLeft->Initialize();
	m_btnLeft->SetPosition(m_position + glm::vec2(-(m_scale.x - sclMplier * sclFactor), 0.0f));
	m_btnLeft->SetScale(itemScale * sclMplier);
	m_btnLeft->SetTextures(m_leftTex, m_leftTex_a);
	AddChild(m_btnLeft);

	m_btnRight = new GUIButton(&idPc);
	m_btnRight->Initialize();
	m_btnRight->SetPosition(m_position + glm::vec2((m_scale.x - sclMplier * sclFactor), 0.0f));
	m_btnRight->SetScale(itemScale * sclMplier);
	m_btnRight->SetTextures(m_rightTex, m_rightTex_a);
	AddChild(m_btnRight);

	for (std::map<string, GUIElement*>::iterator it = m_children.begin(); it != m_children.end(); ++it)
	{
		(*it).second->SetBlockable(true);
		//(*it).second->SetScaled(false);
	}

	return err;
}

unsigned int GUIValueSetter::Shutdown()
{
	unsigned int err = CS_ERR_NONE;

	err = GUIElement::Shutdown();
	if (err != CS_ERR_NONE)
		return err;

	m_textLabel->Shutdown();
	m_textValue->Shutdown();
	m_btnLeft->Shutdown();
	m_btnRight->Shutdown();
	delete m_textLabel;
	delete m_textValue;
	delete m_btnLeft;
	delete m_btnRight;

	return err;
}

unsigned int GUIValueSetter::Update()
{
	unsigned int err = CS_ERR_NONE;

	if (m_isEnabled)
	{
		err = GUIElement::Update();
		if (err != CS_ERR_NONE)
			return err;
	}

	return err;
}

unsigned int GUIValueSetter::ExecuteClick(const glm::vec2 * clickPos)
{
	unsigned int ctr = 0;

	if (m_isEnabled)
	{
		for (std::map<std::string, GUIElement*>::iterator it = m_children.begin(); it != m_children.end(); ++it)
		{
			if (InputManager::GetInstance()->GUIElementAreaInClick(it->second, clickPos))
			{
				if (it->second == m_btnRight || it->second == m_btnLeft)
				{
					//ModifyState(m_currentState + 1);
					m_firstHold = true;
					m_currentHoldDeltaMS = M_HOLD_DELTA_MS;
				}
				ctr += (*it).second->ExecuteClick(clickPos);
			}
		}
	}

	if (m_isBlockable)
		++ctr;

	return ctr;
}

unsigned int GUIValueSetter::ExecuteHold(const glm::vec2 * clickPos)
{
	unsigned int ctr = 0;

	GUIButton* down, *up;

	if (m_reversed)
	{
		down = m_btnRight;
		up = m_btnLeft;
	}
	else
	{
		down = m_btnLeft;
		up = m_btnRight;
	}

	if (m_isEnabled)
	{
		for (std::map<std::string, GUIElement*>::iterator it = m_children.begin(); it != m_children.end(); ++it)
		{
			if (InputManager::GetInstance()->GUIElementAreaInClick(it->second, clickPos))
			{
				if (it->second == down)
				{
					ModifyStateOnHold(m_currentState - 1);
				}
				else if (it->second == up)
				{
					ModifyStateOnHold(m_currentState + 1);
				}
				ctr += (*it).second->ExecuteHold(clickPos);
			}
		}
	}

	if (m_isBlockable)
		++ctr;

	return ctr;
}

void GUIValueSetter::CleanupAfterHold()
{
	GUIElement::CleanupAfterHold();
	m_firstHold = false;
	m_currentHoldDeltaMS = M_HOLD_DELTA_MS;
}

unsigned int GUIValueSetter::GetCurrentState()
{
	return m_currentState;
}

void GUIValueSetter::SetCurrentState(unsigned int state)
{
	if(state < m_states)
		m_currentState = state;
	m_textValue->SetText(&m_valStrings.at(m_currentState));
}

float GUIValueSetter::GetLabelMultiplier()
{
	return m_labelMultiplier;
}

float GUIValueSetter::GetLabelOffset()
{
	return m_labelOffset;
}

unsigned int GUIValueSetter::GetStates()
{
	return m_states;
}

void GUIValueSetter::ModifyStateOnHold(unsigned int mVal)
{
	float timeMS = Timer::GetInstance()->GetCurrentTimeMS();
	if (m_firstHold)
	{
		m_timeHelperMS = timeMS;
		m_firstHold = false;
		return;
	}
	if (timeMS - m_timeHelperMS > m_currentHoldDeltaMS)
	{
		m_timeHelperMS = timeMS;
		ModifyState(mVal);
		if(!m_rewind)
			m_currentHoldDeltaMS *= M_HOLD_SPEEDUP_RATE;
	}
}

void GUIValueSetter::ModifyState(unsigned int mVal)
{
	int smVal = (int)mVal;
	if (m_rewind)
	{
		smVal = smVal % m_states;
		if (smVal < 0)
			smVal = m_states - 1;
	}
	else
	{
		if (smVal < 0)
			smVal = 0;
		else if (smVal >= m_states)
			smVal = m_states - 1;
	}
	m_currentState = (unsigned int)smVal;
	m_textValue->SetText(&m_valStrings.at(m_currentState));
	for (std::vector<std::function<void(unsigned int)>>::iterator it = EventStateChanged.begin(); it != EventStateChanged.end(); ++it)
	{
		(*it)(m_currentState);
	}
}