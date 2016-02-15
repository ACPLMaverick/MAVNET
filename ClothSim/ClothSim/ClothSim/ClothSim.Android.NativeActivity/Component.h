#pragma once

/*
	Component.
*/

class SimObject;

class Component
{
protected:
	SimObject* m_obj;
	bool m_enabled;
public:
	Component(SimObject* obj);
	Component(const Component*);
	~Component();

	virtual unsigned int Initialize() = 0;
	virtual unsigned int Shutdown() = 0;

	virtual unsigned int Update() = 0;
	virtual unsigned int Draw() = 0;

	virtual void SetEnabled(bool);
	virtual bool GetEnabled();
	
	virtual SimObject* GetMySimObject() final;
};

