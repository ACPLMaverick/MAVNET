#pragma once

/*
This is an abstract representation of a Mesh rendered by OpenGL 3.3.
*/

#include "Mesh.h"
#include "Renderer.h"
#include "LightAmbient.h"
#include "LightDirectional.h"

#include <glm\glm\gtx\transform.hpp>

////////////

struct VertexDataRaw
{
	glm::vec4* positionBuffer;
	glm::vec2* uvBuffer;
	glm::vec4* normalBuffer;
	glm::vec4* colorBuffer;
	glm::vec4* barycentricBuffer;
	GLuint* indexBuffer;
	unsigned int vertexCount;
	unsigned int indexCount;

	VertexDataRaw()
	{
		positionBuffer = nullptr;
		uvBuffer = nullptr;
		normalBuffer = nullptr;
		colorBuffer = nullptr;
		indexBuffer = nullptr;
		barycentricBuffer = nullptr;
		vertexCount = 0;
		indexCount = 0;
	}

	~VertexDataRaw()
	{
		if (positionBuffer != nullptr)
		{
			delete[] positionBuffer;
			positionBuffer = nullptr;
		}
		if (uvBuffer != nullptr)
		{
			delete[] uvBuffer;
			uvBuffer = nullptr;
		}
		if (normalBuffer != nullptr)
		{
			delete[] normalBuffer;
			normalBuffer = nullptr;
		}
		if (colorBuffer != nullptr)
		{
			delete[] colorBuffer;
			colorBuffer = nullptr;
		}
		if (indexBuffer != nullptr)
		{
			delete[] indexBuffer;
			indexBuffer = nullptr;
		}
		if (barycentricBuffer != nullptr)
		{
			delete[] barycentricBuffer;
			barycentricBuffer = nullptr;
		}
	}
};

struct VertexDataID
{
	GLuint vertexArrayID;
	GLuint vertexBuffer;
	GLuint uvBuffer;
	GLuint normalBuffer;
	GLuint colorBuffer;
	GLuint barycentricBuffer;
	GLuint indexBuffer;
};

struct VertexData
{
	VertexDataRaw* data;
	VertexDataID* ids;

	VertexData()
	{
		data = nullptr;
		ids = nullptr;
	}

	~VertexData()
	{
		if (data != nullptr)
		{
			delete data;
			data = nullptr;
		}
		if (ids != nullptr)
		{
			delete ids;
			ids = nullptr;
		}
	}
};

////////////

class MeshGL :
	public Mesh
{
protected:
	VertexData* m_vertexData;

	virtual void GenerateVertexData() = 0;
	void CreateVertexDataBuffers(unsigned int, unsigned int, GLenum);
	virtual void GenerateBarycentricCoords();
public:
	MeshGL(SimObject*);
	MeshGL(const MeshGL*);
	~MeshGL();

	virtual unsigned int Initialize();
	virtual unsigned int Shutdown();

	virtual unsigned int Update();
	virtual unsigned int Draw();
};

