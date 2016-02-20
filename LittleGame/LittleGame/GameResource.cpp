#include "stdafx.h"
#include "GameResource.h"


GameResource::GameResource()
{
}


GameResource::~GameResource()
{
}

void GameResource::Initialize(const std::string * filePath, const std::string * name)
{
	if (name != nullptr)
		m_name = *name;
	m_filePath = *filePath;

	m_hash = std::hash<std::string>()(m_filePath);
}
