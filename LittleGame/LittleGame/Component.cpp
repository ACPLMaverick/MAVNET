#include "stdafx.h"
#include "Component.h"


Component::Component()
{
}


Component::~Component()
{
}

void Component::Initialize(uint32_t uid, GameObject * obj, std::string* name)
{
	m_uID = uid;
	m_obj = obj;
	if(name != nullptr)
		m_name = *name;
}

void Component::Shutdown()
{
	m_obj = nullptr;
}
