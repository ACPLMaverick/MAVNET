#pragma once

#include "Primitive.h"
#include "Float3.h"

class Triangle :
	public Primitive
{
protected:

public:
	Float3 m_x, m_y, m_z;

	Triangle(Float3 x, Float3 y, Float3 z);
	~Triangle();

	virtual void Update() override;
	virtual void Draw(Buffer<int32_t> * const buf, Buffer<float>* const depth) override;
};

