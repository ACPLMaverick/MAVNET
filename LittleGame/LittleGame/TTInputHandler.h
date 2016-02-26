#pragma once

#include "Singleton.h"
#include "InputManager.h"

class TTInputHandler :
	public Singleton<TTInputHandler>
{
	friend class Singleton<TTInputHandler>;
private:
	TTInputHandler();
public:
	~TTInputHandler();

	bool ActionDecline() { return InputManager::GetInstance()->GetKeyUp(LEK_ESCAPE); }
	bool ActionConfirm() { return InputManager::GetInstance()->GetKeyUp(LEK_RETURN); }
	bool ActionLeft() { return InputManager::GetInstance()->GetKey(LEK_LEFT); }
	bool ActionRight() { return InputManager::GetInstance()->GetKey(LEK_RIGHT); }
	bool ActionUp() { return InputManager::GetInstance()->GetKey(LEK_UP); }
	bool ActionDown() { return InputManager::GetInstance()->GetKey(LEK_DOWN); }
	bool ActionFlip() { return InputManager::GetInstance()->GetKeyDown(LEK_SPACE); }
};

