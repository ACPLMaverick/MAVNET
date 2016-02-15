#pragma once

#include "MeshGL.h"

class MeshGLRect :
	public MeshGL
{
protected:
	virtual void GenerateVertexData();
public:
	MeshGLRect(SimObject*);
	MeshGLRect(SimObject*, glm::vec4*);
	MeshGLRect(const MeshGLRect*);
	~MeshGLRect();
};

