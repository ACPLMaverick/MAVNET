#pragma once

#include "Mesh.h"

class MeshSprite : public Mesh
{
protected:
	virtual void InitializeVertexData();
public:
	MeshSprite();
	~MeshSprite();
};

