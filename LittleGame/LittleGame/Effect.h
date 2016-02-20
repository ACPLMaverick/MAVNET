#pragma once

/*
	A simple DX9 effect class. It encapsulates all effect actions, settings and such.
*/

#include <d3dx9.h>

#include "GameResource.h"

class Effect : public GameResource
{
protected:

	ID3DXEffect* m_effect;

public:
	Effect();
	~Effect();

	virtual void Initialize(const std::string* filePath, const std::string* name = nullptr);
	virtual void Shutdown();

#pragma region Accessors

	ID3DXEffect* const GetDX9Effect() { return m_effect; }
	const std::string* GetName() { return &m_name; }
	const std::string* GetFilePath() { return &m_name; }

#pragma endregion
};

