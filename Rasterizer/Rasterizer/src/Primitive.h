#pragma once

#include "stdafx.h"
#include "Buffer.h"

class Primitive
{
protected:

#pragma region Protected


#pragma endregion

#pragma region Functions Protected

	virtual inline uint16_t ConvertFromScreenToBuffer(float point, uint16_t maxValue);

#pragma endregion

public:

#pragma region Functions Public

	Primitive();
	~Primitive();

	virtual void Update() = 0;
	virtual void Draw(Buffer<int32_t> * const buf, Buffer<float>* const depth) = 0;

#pragma region Accessors

#pragma endregion

#pragma endregion
};

