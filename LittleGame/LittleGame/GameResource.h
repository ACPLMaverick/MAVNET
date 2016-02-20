#pragma once

/*
	An abstraction of game resource.
*/

class GameResource
{
protected:

	std::string m_name = "NoNameResource";
	std::string m_filePath = "";
	uint32_t m_hash = 0;

public:
	GameResource();
	~GameResource();

	virtual void Initialize(const std::string* filePath, const std::string* name = nullptr);
	virtual void Shutdown() = 0;

	const std::string* GetName() { return &m_name; }
	const std::string* GetFilePath() { return &m_name; }
	uint32_t GetHash() { return m_hash; }
};

