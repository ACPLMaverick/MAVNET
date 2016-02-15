#pragma once

#include "MeshGL.h"

class MeshGLRect :
	public MeshGL
{
protected:
	glm::vec4 m_color;

	virtual void GenerateVertexData();
public:
	MeshGLRect(SimObject*);
	MeshGLRect(SimObject*, glm::vec4*);
	MeshGLRect(const MeshGLRect*);
	~MeshGLRect();
};

