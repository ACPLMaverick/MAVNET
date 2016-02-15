#include "InputManager.h"
#include "GUIButton.h"

InputManager::InputManager()
{

}

InputManager::InputManager(const InputManager*)
{

}

InputManager::~InputManager()
{

}


unsigned int InputManager::Initialize()
{
	unsigned int err = CS_ERR_NONE;

	m_tBools.push_back(&m_clickHelperTBool);
	m_isClick = false;
	m_isMove = false;
	m_isHold = false;
	m_isHoldDouble = false;
	m_isPinch = false;

	return err;
}

unsigned int InputManager::Shutdown()
{
	unsigned int err = CS_ERR_NONE;

	m_guiElems.clear();

	return err;
}

unsigned int InputManager::Run()
{
	unsigned int err = CS_ERR_NONE;

#ifdef PLATFORM_WINDOWS

	// here goes data readout from mouse

	double pX, pY;
	glfwGetCursorPos(Renderer::GetInstance()->GetWindowPtr(), &pX, &pY);
	glm::vec2 mPos = glm::vec2((float)pX, (float)pY);
	//Engine* engine = System::GetInstance()->GetEngineData();
	//mPos.x = (mPos.x - ((float)engine->width / 2.0f)) / (float)engine->width * 2.0f;
	//mPos.y = - (mPos.y - ((float)engine->height / 2.0f)) / (float)engine->height * 2.0f;
	//LOGI("%f, %f\n", mPos.x, mPos.y);

	m_touch01Direction = mPos - m_touch01Position;

	if (mPos != m_touch01Position)
		m_isMove = true;
	else
		m_isMove = false;

	m_touch01Position = mPos;
	m_touch02Position = m_touch01Position;
	m_touch02Direction = m_touch01Direction;

	if (glfwGetMouseButton(Renderer::GetInstance()->GetWindowPtr(), GLFW_MOUSE_BUTTON_1))
	{
		m_isHold = true;
		ProcessButtonHolds(&m_touch01Position);
	}
	else
	{
		if (m_isHold)
		{
			m_isClick = true;
			m_clickHelperTBool.SetVal(true);
			ProcessButtonClicks(&m_touch01Position);
		}
		m_isHold = false;
	}

	if (glfwGetMouseButton(Renderer::GetInstance()->GetWindowPtr(), GLFW_MOUSE_BUTTON_2))
	{
		m_isHoldDouble = true;
	}
	else
	{
		m_isHoldDouble = false;
	}

	if (glfwGetMouseButton(Renderer::GetInstance()->GetWindowPtr(), GLFW_MOUSE_BUTTON_3))
	{
		m_isPinch = true;
		m_pinchVal = m_touch02Direction.y;
	}
	else
	{
		m_isPinch = false;
	}

#endif

	for (std::vector<TwoBool*>::iterator it = m_tBools.begin(); it != m_tBools.end(); ++it)
	{
		(*it)->Update();
	}

	if (m_clickHelperTBool.GetVal())
	{
		m_isClick = true;
		//m_pressTBool.SetVal(false);
	}
	else
		m_isClick = false;

	//////////////////////////////////////

	if (m_isHold)
		m_currentlyHeldButtons = ProcessButtonHolds(&m_touch01Position);
	else
		m_currentlyHeldButtons = 0;

	//////////////////////////////////////

#ifndef PLATFORM_WINDOWS

	if (m_isHold && m_isMove)
	{
		if (Timer::GetInstance()->GetCurrentTimeMS() - m_touchEventTime > m_touchEventInterval * 2.0f)
		{
			m_isMove = false;
		}
	}

#endif // PLATFORM_WINDOWS

	return err;
}

bool InputManager::GetPress()
{
	return m_isClick;
}

bool InputManager::GetTouch()
{
	return m_isHold;
}

bool InputManager::GetDoubleTouch()
{
	return m_isHoldDouble;
}

bool InputManager::GetPinch()
{
	return m_isPinch;
}

bool InputManager::GetMove()
{
	return m_isMove;
}

void InputManager::GetTouchPosition(glm::vec2 * vec)
{
	vec->x = m_touch01Position.x;
	vec->y = m_touch01Position.y;
}

void InputManager::GetDoubleTouchPosition(glm::vec2 * vec)
{
	vec->x = (m_touch01Position.x + m_touch02Position.x) / 2.0f;
	vec->y = (m_touch01Position.y + m_touch02Position.y) / 2.0f;
}

void InputManager::GetTouchDirection(glm::vec2 * vec)
{
	vec->x = m_touch01Direction.x;
	vec->y = m_touch01Direction.y;
}

void InputManager::GetDoubleTouchDirection(glm::vec2 * vec)
{
	vec->x = (m_touch01Direction.x + m_touch02Direction.x) / 2.0f;
	vec->y = (m_touch01Direction.y + m_touch02Direction.y) / 2.0f;
}

void InputManager::GetAcceleration(glm::vec3 * vec)
{
	*vec = m_acceleration;
}

void InputManager::GetAccelerationDelta(glm::vec3 * vec)
{
	*vec = m_accelerationDelta;
}

float InputManager::GetPinchValue()
{
	return m_pinchVal;
}

unsigned int InputManager::GetCurrentlyHeldButtons()
{
	return m_currentlyHeldButtons;
}

void InputManager::AddGUIElement(GUIElement * button)
{
	m_guiElems.push_back(button);
}

void InputManager::RemoveGUIElement(GUIElement * button)
{
	int ctr = 0;
	for (std::vector<GUIElement*>::iterator it = m_guiElems.begin(); it != m_guiElems.end(); ++it, ++ctr)
	{
		if ((*it) == button)
		{
			m_guiElems.erase(it);
			return;
		}
	}
}

void InputManager::ComputeScaleFactors(glm::vec2 * factors)
{
	Engine* engine = System::GetInstance()->GetEngineData();
	float scrWidth = engine->width;
	float scrHeight = engine->height;
	float bBias = 0.6f;
	float factorX = (scrHeight / scrWidth);
	float factorY = (scrWidth / scrHeight);
	float hsFactor = 1.0f / Renderer::GetInstance()->GetScreenRatio();
	if (scrWidth > scrHeight)
	{
		factorY = 1.0f / factorY * hsFactor;
		factorX *= hsFactor;
	}

	factorX *= (1.0f / hsFactor);

	factors->x = factorX;
	factors->y = factorY;
}

bool InputManager::GUIElementAreaInClick(GUIElement * button, const glm::vec2 * clickPos)
{
	// we have an event here, so we calculate current finger position
	glm::vec2 cp;

	Engine* e = System::GetInstance()->GetEngineData();
	float w = e->width;
	float h = e->height;

	cp.x = clickPos->x / w * 2.0f - 1.0f;
	cp.y = -(clickPos->y / h * 2.0f - 1.0f);

	glm::vec2 nScl = button->GetScale();
	glm::vec2 pos = button->GetPosition();
	glm::vec2 scaleFactors = glm::vec2(1.0f, 1.0f);
	if (button->GetScaled())
		ComputeScaleFactors(&scaleFactors);
	nScl.x *= scaleFactors.x;
	nScl.y *= scaleFactors.y;

	// check if click is within boundaries of this button
	if (
		cp.x >= pos.x - nScl.x &&
		cp.x <= pos.x + nScl.x &&
		cp.y >= pos.y - nScl.y &&
		cp.y <= pos.y + nScl.y
		)
	{
		return true;
	}

	return false;
}

void InputManager::GetClickPosInScreenCoords(const glm::vec2 * clPos, glm::vec2 * retPos)
{
	Engine* engine = System::GetInstance()->GetEngineData();
	float width = (float)engine->width;
	float height = (float)engine->height;
	retPos->x = (clPos->x / width) * 2.0f - 1.0f;
	retPos->y = (clPos->y / height) * 2.0f - 1.0f;
}

unsigned int InputManager::ProcessButtonClicks(const glm::vec2 * clickPos)
{
	unsigned int ctr = 0;
	for (std::vector<GUIElement*>::iterator it = m_guiElems.begin(); it != m_guiElems.end(); ++it)
	{
		if (GUIElementAreaInClick(*it, clickPos))
		{
			ctr += (*it)->ExecuteClick(clickPos);
			(*it)->CleanupAfterHold();
		}
	}

	return ctr;
}

unsigned int InputManager::ProcessButtonHolds(const glm::vec2 * clickPos)
{
	unsigned int ctr = 0;
	for (std::vector<GUIElement*>::iterator it = m_guiElems.begin(); it != m_guiElems.end(); ++it)
	{
		if (GUIElementAreaInClick(*it, clickPos))
		{
			ctr += (*it)->ExecuteHold(clickPos);
		}
		else if ((*it)->GetHoldInProgress())
		{
			(*it)->CleanupAfterHold();
		}
	}
	return ctr;
}

#ifndef PLATFORM_WINDOWS

void InputManager::UpdateAcceleration(const ASensorVector* sVec)
{
	glm::vec3 newAcc = glm::vec3(sVec->x, sVec->y, sVec->z);
	m_accelerationDelta = newAcc - m_acceleration;
	m_acceleration = newAcc;
}
/**
* Process the next input event.
*/
int32_t InputManager::AHandleInput(struct android_app* app, AInputEvent* event) 
{
	Engine* engine = System::GetInstance()->GetEngineData();

	if (AInputEvent_getType(event) == AINPUT_EVENT_TYPE_MOTION &&
		AInputEvent_getSource(event) == AINPUT_SOURCE_TOUCHSCREEN
		) 
	{
		//LOGI("InputManager: TOUCHSCREEN! Count: %d", AMotionEvent_getPointerCount(event));
		unsigned int pCount = AMotionEvent_getPointerCount(event);
		InputManager* im = InputManager::GetInstance();

		if (AMotionEvent_getAction(event) == AMOTION_EVENT_ACTION_MOVE)
		{
			im->m_isMove = true;
			double cTime = Timer::GetInstance()->GetCurrentTimeMS();
			im->m_touchEventInterval = glm::min(cTime - im->m_touchEventTime, 1000.0);
			im->m_touchEventTime = cTime;
		}

		if (pCount == 1)
		{
			glm::vec2 tVec;
			
			tVec.x = AMotionEvent_getX(event, 0);
			tVec.y = AMotionEvent_getY(event, 0);

			im->m_touch01Direction = tVec - im->m_touch01Position;
			im->m_touch01Position = tVec;


			if (AMotionEvent_getAction(event) == AMOTION_EVENT_ACTION_UP)
			{
				im->ProcessButtonClicks(&tVec);
				im->m_isHold = false;
			}
			else if(AMotionEvent_getAction(event) == AMOTION_EVENT_ACTION_DOWN)
			{
				im->m_isHold = true;
			}
		}

		else if (pCount == 2)
		{
			glm::vec2 tVec1, tVec2;

			tVec1.x = AMotionEvent_getX(event, 0);
			tVec1.y = AMotionEvent_getY(event, 0);
			tVec2.x = AMotionEvent_getX(event, 1);
			tVec2.y = AMotionEvent_getY(event, 1);

			im->m_touch01Direction = tVec1 - im->m_touch01Position;
			im->m_touch02Direction = tVec2 - im->m_touch02Position;
			im->m_touch01Position = tVec1;
			im->m_touch02Position = tVec2;

			if (AMotionEvent_getAction(event) == AMOTION_EVENT_ACTION_MOVE)
			{
				im->m_isHold = false;
				float dot = glm::dot(im->m_touch01Direction, im->m_touch02Direction);
				if (dot < 0.7f && im->m_isHoldDouble)
				{
					float fl = glm::length(im->m_touch01Direction);
					float sl = glm::length(im->m_touch02Direction);
					float mp = 1.0f;
					float diff = glm::length(im->m_touch01Position - im->m_touch02Position);
					if (diff - im->m_diffPinch < 0)
						mp *= -1.0f;
					im->m_diffPinch = diff;
					im->m_pinchVal = (fl + sl) / 2.0f * mp;

					im->m_isHoldDouble = false;
					im->m_isPinch = true;
				}
				else
				{
					im->m_isHoldDouble = true;
					im->m_isPinch = false;
				}
			}
		}

		// cleanup
		if (AMotionEvent_getAction(event) == AMOTION_EVENT_ACTION_UP)
		{
			im->m_isHold = false;
			im->m_isHoldDouble = false;
			im->m_isPinch = false;
			im->m_isMove = false;
			im->m_touch01Position = glm::vec2(0.0f);
			im->m_touch01Direction = glm::vec2(0.0f);
			im->m_touch02Position = glm::vec2(0.0f);
			im->m_touch02Direction = glm::vec2(0.0f);
		}
		return 1;
	}
	if (AInputEvent_getType(event) == AINPUT_EVENT_TYPE_KEY)
	{
		// do nothing atm
		return 1;
	}
	
	return 0;
}

#endif