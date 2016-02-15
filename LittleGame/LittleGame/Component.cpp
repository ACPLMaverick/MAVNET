#include "stdafx.h"
#include "Component.h"


Component::Component()
{
}


Component::~Component()
{
}

void Component::Initialize(uint32_t uid, GameObject * obj)
{
	m_uID = uid;
	m_obj = obj;
}

void Component::Initialize(uint32_t uid, GameObject * obj, std::string * name)
{
	Initialize(uid, obj);
	m_name = *name;
}

void Component::Shutdown()
{
	m_obj = nullptr;
}
