#pragma once
#include "Component.h"
class TestInputController :
	public Component
{
public:
	TestInputController();
	~TestInputController();

	virtual void Update();
	virtual void Draw();
};

