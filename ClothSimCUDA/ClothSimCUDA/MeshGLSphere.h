#pragma once

/*
This class represents a simple UV sphere.
*/

#include "MeshGL.h"

#define _USE_MATH_DEFINES
#include <math.h>

class MeshGLSphere :
	public MeshGL
{
protected:
	float m_radius;
	unsigned int m_rings;
	unsigned int m_sectors;
	glm::vec4 m_color;

	virtual void GenerateVertexData();
public:
	MeshGLSphere(SimObject* obj);
	MeshGLSphere(SimObject* obj, float radius, unsigned int rings, unsigned int sectors);
	MeshGLSphere(SimObject* obj, float radius, unsigned int rings, unsigned int sectors, glm::vec4* color);
	MeshGLSphere(const MeshGLSphere* c);
	~MeshGLSphere();
};

