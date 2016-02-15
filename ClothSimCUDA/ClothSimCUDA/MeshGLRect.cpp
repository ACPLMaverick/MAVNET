#include "MeshGLRect.h"

MeshGLRect::MeshGLRect(SimObject* obj) : MeshGL(obj)
{
	m_color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
}

MeshGLRect::MeshGLRect(SimObject* obj, glm::vec4* col) : MeshGL(obj)
{
	m_color = *col;
}

MeshGLRect::MeshGLRect(const MeshGLRect* m) : MeshGL(m)
{
}


MeshGLRect::~MeshGLRect()
{
}



void MeshGLRect::GenerateVertexData()
{
	CreateVertexDataBuffers(4, 6, GL_STATIC_DRAW);

	////////////////

	m_vertexData->data->positionBuffer[0] = glm::vec3(-1.0f, -1.0f, 0.0f);
	m_vertexData->data->positionBuffer[1] = glm::vec3(1.0f, -1.0f, 0.0f);
	m_vertexData->data->positionBuffer[2] = glm::vec3(-1.0f, 1.0f, 0.0f);
	m_vertexData->data->positionBuffer[3] = glm::vec3(1.0f, 1.0f, 0.0f);

	m_vertexData->data->indexBuffer[0] = 0;
	m_vertexData->data->indexBuffer[1] = 1;
	m_vertexData->data->indexBuffer[2] = 2;
	m_vertexData->data->indexBuffer[3] = 2;
	m_vertexData->data->indexBuffer[4] = 1;
	m_vertexData->data->indexBuffer[5] = 3;

	m_vertexData->data->uvBuffer[0] = glm::vec2(0.0f, 0.0f);
	m_vertexData->data->uvBuffer[1] = glm::vec2(1.0f, 0.0f);
	m_vertexData->data->uvBuffer[2] = glm::vec2(0.0f, 1.0f);
	m_vertexData->data->uvBuffer[3] = glm::vec2(1.0f, 1.0f);

	m_vertexData->data->normalBuffer[0] = glm::vec3(0.0f, 0.0f, -1.0f);
	m_vertexData->data->normalBuffer[1] = glm::vec3(0.0f, 0.0f, -1.0f);
	m_vertexData->data->normalBuffer[2] = glm::vec3(0.0f, 0.0f, -1.0f);
	m_vertexData->data->normalBuffer[3] = glm::vec3(0.0f, 0.0f, -1.0f);

	m_vertexData->data->colorBuffer[0] = m_color;
	m_vertexData->data->colorBuffer[1] = m_color;
	m_vertexData->data->colorBuffer[2] = m_color;
	m_vertexData->data->colorBuffer[3] = m_color;
}