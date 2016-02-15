#pragma once
#include "MeshGL.h"
class MeshGLBox :
	public MeshGL
{
protected:
	float m_width, m_length, m_height;

	virtual void GenerateVertexData();
public:
	MeshGLBox(SimObject*);
	MeshGLBox(SimObject*, float, float, float);
	MeshGLBox(SimObject*, float, float, float, glm::vec4*);
	MeshGLBox(const MeshGLBox*);
	~MeshGLBox();
};

