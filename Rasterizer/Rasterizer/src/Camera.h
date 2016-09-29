#pragma once

#include "stdafx.h"

class Camera
{
protected:

#pragma region Protected

	uint64_t m_id;
	std::string m_name;

#pragma endregion

public:

#pragma region Functions Public

	Camera();
	~Camera();

	void Update();

#pragma region Accessors

	uint64_t GetUID() { return m_id; }
	const std::string* GetName() { return &m_name; }

#pragma endregion

#pragma endregion
};

