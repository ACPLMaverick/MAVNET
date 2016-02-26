#pragma once

#include "Component.h"

class GameObject;

class TTGameController :
	public Component
{
public:
#pragma region Variables

	GameObject* Controllable = nullptr;

#pragma endregion

	TTGameController();
	~TTGameController();

	virtual void Initialize(uint32_t uid, GameObject* obj, std::string* name = nullptr);
	virtual void Shutdown();
	virtual void Update();
	virtual void Draw();
};

