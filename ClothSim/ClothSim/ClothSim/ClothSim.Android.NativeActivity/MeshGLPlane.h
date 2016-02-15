#pragma once

/*
	This class represents a plane of a varying mesh detail, which will be used as a cloth base.
	It also contains data structure that allow simulation to access each vertex's neighbours and modify them.
*/

#include "MeshGL.h"
class MeshGLPlane :
	public MeshGL
{
protected:
	float m_width;
	float m_length;
	unsigned int m_edgesWidth;
	unsigned int m_edgesLength;
	unsigned int m_currentArray;
	VertexData* m_vertexDataDual[2];

	virtual void GenerateVertexData();
	virtual void GenerateBarycentricCoords();
public:
	MeshGLPlane(SimObject*);
	MeshGLPlane(SimObject*, float, float);
	MeshGLPlane(SimObject*, float, float, unsigned int, unsigned int);
	MeshGLPlane(SimObject*, float, float, unsigned int, unsigned int, glm::vec4* col);
	MeshGLPlane(const MeshGLPlane*);
	~MeshGLPlane();

	virtual unsigned int Initialize();
	virtual unsigned int Shutdown();
	virtual unsigned int Update();

	unsigned int SwapDataPtrs();
	VertexData* GetVertexDataPtr();
	VertexData** GetVertexDataDualPtr();
	unsigned int GetEdgesWidth();
	unsigned int GetEdgesLength();
};

