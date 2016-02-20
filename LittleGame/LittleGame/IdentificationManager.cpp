#include "stdafx.h"
#include "IdentificationManager.h"


IdentificationManager::IdentificationManager()
{
}

IdentificationManager::~IdentificationManager()
{
}

void IdentificationManager::Initialize()
{
}

void IdentificationManager::Shutdown()
{
}

uint32_t IdentificationManager::GetUniqueID()
{
	uint32_t val = 0;

	if (m_greatestUID < UINT32_MAX)
	{
		val = m_greatestUID + 1;
		m_registeredUIDs.push_back(val);
		SetGreatestID(val);
	}
	else if (m_registeredUIDs.size() < UINT32_MAX)
	{
		do
		{
			++val;
		} while (!CheckUniqueID(val));
		m_registeredUIDs.push_back(val);
	}
	else
	{
		val = UINT32_MAX;
	}

	return val;
}

bool IdentificationManager::RegisterUniqueID(uint32_t uid)
{
	if (CheckUniqueID(uid))
	{
		m_registeredUIDs.push_back(uid);
		SetGreatestID(uid);
		return true;
	}
	else return false;
}

bool IdentificationManager::UnregisterUniqueID(uint32_t uid)
{
	for (std::vector<uint32_t>::iterator it = m_registeredUIDs.begin(); it != m_registeredUIDs.end(); ++it)
	{
		if (*it == uid)
		{
			m_registeredUIDs.erase(it);
			if (uid == m_greatestUID)
				m_greatestUID--;
			return true;
		}
	}

	return false;
}

bool IdentificationManager::CheckUniqueID(uint32_t uid)
{
	for (std::vector<uint32_t>::iterator it = m_registeredUIDs.begin(); it != m_registeredUIDs.end(); ++it)
	{
		if (*it == uid)
		{
			return false;
		}
	}
	return true;
}


void IdentificationManager::SetGreatestID(uint32_t uid)
{
	if (uid > m_greatestUID)
	{
		m_greatestUID = uid;
	}
}
