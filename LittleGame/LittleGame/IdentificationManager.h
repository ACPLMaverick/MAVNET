#pragma once

#include "Singleton.h"

#include <vector>

class IdentificationManager :
	public Singleton<IdentificationManager>
{
	friend class Singleton<IdentificationManager>;
private:
	std::vector<uint32_t> m_registeredUIDs;
	uint32_t m_greatestUID = 0;

	IdentificationManager();

	inline void SetGreatestID(uint32_t uid);
public:
	~IdentificationManager();

	void Initialize();
	void Shutdown();

	uint32_t GetUniqueID();
	bool RegisterUniqueID(uint32_t uid);
	bool UnregisterUniqueID(uint32_t uid);
	bool CheckUniqueID(uint32_t uid);
};

