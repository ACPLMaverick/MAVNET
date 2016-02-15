#include "Component.h"


Component::Component(SimObject* obj)
{
	m_enabled = true;
	m_obj = obj;
}

Component::Component(const Component*)
{

}

Component::~Component()
{
}

SimObject* Component::GetMySimObject()
{
	return m_obj;
}



void Component::SetEnabled(bool en)
{
	m_enabled = en;
}



bool Component::GetEnabled()
{
	return m_enabled;
}