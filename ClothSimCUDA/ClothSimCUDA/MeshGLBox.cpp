#include "MeshGLBox.h"

MeshGLBox::MeshGLBox(SimObject* obj) : MeshGL(obj)
{
	m_width = 1.0f;
	m_height = 1.0f;
	m_length = 1.0f;
	m_color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
}

MeshGLBox::MeshGLBox(SimObject* obj, float w, float h, float l) : MeshGL(obj)
{
	m_width = w;
	m_height = h;
	m_length = l;
	m_color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
}

MeshGLBox::MeshGLBox(SimObject* obj, float w, float h, float l, glm::vec4* col) : MeshGLBox(obj, w, h, l)
{
	m_color = *col;
}

MeshGLBox::MeshGLBox(const MeshGLBox* c) : MeshGL(c)
{
	m_width = c->m_width;
	m_height = c->m_height;
	m_length = c->m_height;
	m_color = c->m_color;
}


MeshGLBox::~MeshGLBox()
{
}



void MeshGLBox::GenerateVertexData()
{
	CreateVertexDataBuffers(24, 36, GL_STATIC_DRAW);

	////////////////

	int face = 0;
	int index = 0;
	// front face
	m_vertexData->data->positionBuffer[face + 0] = glm::vec3(- m_width / 2.0f, - m_height / 2.0f, m_length / 2.0f);
	m_vertexData->data->positionBuffer[face + 1] = glm::vec3(m_width / 2.0f, -m_height / 2.0f, m_length / 2.0f);
	m_vertexData->data->positionBuffer[face + 2] = glm::vec3(m_width / 2.0f, m_height / 2.0f, m_length / 2.0f);
	m_vertexData->data->positionBuffer[face + 3] = glm::vec3(-m_width / 2.0f, m_height / 2.0f, m_length / 2.0f);

	m_vertexData->data->indexBuffer[index + 0] = face + 0;
	m_vertexData->data->indexBuffer[index + 1] = face + 1;
	m_vertexData->data->indexBuffer[index + 2] = face + 2;
	m_vertexData->data->indexBuffer[index + 3] = face + 0;
	m_vertexData->data->indexBuffer[index + 4] = face + 2;
	m_vertexData->data->indexBuffer[index + 5] = face + 3;

	m_vertexData->data->uvBuffer[face + 0] = glm::vec2(0.0f, 0.0f);
	m_vertexData->data->uvBuffer[face + 1] = glm::vec2(1.0f, 0.0f);
	m_vertexData->data->uvBuffer[face + 2] = glm::vec2(0.0f, 1.0f);
	m_vertexData->data->uvBuffer[face + 3] = glm::vec2(1.0f, 1.0f);

	m_vertexData->data->normalBuffer[face + 0] = glm::vec3(0.0f, 0.0f, -1.0f);
	m_vertexData->data->normalBuffer[face + 1] = glm::vec3(0.0f, 0.0f, -1.0f);
	m_vertexData->data->normalBuffer[face + 2] = glm::vec3(0.0f, 0.0f, -1.0f);
	m_vertexData->data->normalBuffer[face + 3] = glm::vec3(0.0f, 0.0f, -1.0f);

	m_vertexData->data->colorBuffer[face + 0] = m_color;
	m_vertexData->data->colorBuffer[face + 1] = m_color;
	m_vertexData->data->colorBuffer[face + 2] = m_color;
	m_vertexData->data->colorBuffer[face + 3] = m_color;

	// rear face
	face += 4;
	index += 6;
	m_vertexData->data->positionBuffer[face + 0] = glm::vec3(-m_width / 2.0f, -m_height / 2.0f, -m_length / 2.0f);
	m_vertexData->data->positionBuffer[face + 1] = glm::vec3(m_width / 2.0f, -m_height / 2.0f, -m_length / 2.0f);
	m_vertexData->data->positionBuffer[face + 2] = glm::vec3(m_width / 2.0f, m_height / 2.0f, -m_length / 2.0f);
	m_vertexData->data->positionBuffer[face + 3] = glm::vec3(-m_width / 2.0f, m_height / 2.0f, -m_length / 2.0f);

	m_vertexData->data->indexBuffer[index + 0] = face + 2;
	m_vertexData->data->indexBuffer[index + 1] = face + 1;
	m_vertexData->data->indexBuffer[index + 2] = face + 0;
	m_vertexData->data->indexBuffer[index + 3] = face + 3;
	m_vertexData->data->indexBuffer[index + 4] = face + 2;
	m_vertexData->data->indexBuffer[index + 5] = face + 0;

	m_vertexData->data->uvBuffer[face + 0] = glm::vec2(0.0f, 0.0f);
	m_vertexData->data->uvBuffer[face + 1] = glm::vec2(1.0f, 0.0f);
	m_vertexData->data->uvBuffer[face + 2] = glm::vec2(0.0f, 1.0f);
	m_vertexData->data->uvBuffer[face + 3] = glm::vec2(1.0f, 1.0f);

	m_vertexData->data->normalBuffer[face + 0] = glm::vec3(0.0f, 0.0f, 1.0f);
	m_vertexData->data->normalBuffer[face + 1] = glm::vec3(0.0f, 0.0f, 1.0f);
	m_vertexData->data->normalBuffer[face + 2] = glm::vec3(0.0f, 0.0f, 1.0f);
	m_vertexData->data->normalBuffer[face + 3] = glm::vec3(0.0f, 0.0f, 1.0f);

	m_vertexData->data->colorBuffer[face + 0] = m_color;
	m_vertexData->data->colorBuffer[face + 1] = m_color;
	m_vertexData->data->colorBuffer[face + 2] = m_color;
	m_vertexData->data->colorBuffer[face + 3] = m_color;

	// left face
	face += 4;
	index += 6;
	m_vertexData->data->positionBuffer[face + 0] = glm::vec3(-m_width / 2.0f, -m_height / 2.0f, m_length / 2.0f);
	m_vertexData->data->positionBuffer[face + 1] = glm::vec3(-m_width / 2.0f, -m_height / 2.0f, -m_length / 2.0f);
	m_vertexData->data->positionBuffer[face + 2] = glm::vec3(-m_width / 2.0f, m_height / 2.0f, -m_length / 2.0f);
	m_vertexData->data->positionBuffer[face + 3] = glm::vec3(-m_width / 2.0f, m_height / 2.0f, m_length / 2.0f);

	m_vertexData->data->indexBuffer[index + 0] = face + 2;
	m_vertexData->data->indexBuffer[index + 1] = face + 1;
	m_vertexData->data->indexBuffer[index + 2] = face + 0;
	m_vertexData->data->indexBuffer[index + 3] = face + 3;
	m_vertexData->data->indexBuffer[index + 4] = face + 2;
	m_vertexData->data->indexBuffer[index + 5] = face + 0;

	m_vertexData->data->uvBuffer[face + 0] = glm::vec2(0.0f, 0.0f);
	m_vertexData->data->uvBuffer[face + 1] = glm::vec2(1.0f, 0.0f);
	m_vertexData->data->uvBuffer[face + 2] = glm::vec2(0.0f, 1.0f);
	m_vertexData->data->uvBuffer[face + 3] = glm::vec2(1.0f, 1.0f);

	m_vertexData->data->normalBuffer[face + 0] = glm::vec3(-1.0f, 0.0f, 0.0f);
	m_vertexData->data->normalBuffer[face + 1] = glm::vec3(-1.0f, 0.0f, 0.0f);
	m_vertexData->data->normalBuffer[face + 2] = glm::vec3(-1.0f, 0.0f, 0.0f);
	m_vertexData->data->normalBuffer[face + 3] = glm::vec3(-1.0f, 0.0f, 0.0f);

	m_vertexData->data->colorBuffer[face + 0] = m_color;
	m_vertexData->data->colorBuffer[face + 1] = m_color;
	m_vertexData->data->colorBuffer[face + 2] = m_color;
	m_vertexData->data->colorBuffer[face + 3] = m_color;

	// right face
	face += 4;
	index += 6;
	m_vertexData->data->positionBuffer[face + 0] = glm::vec3(m_width / 2.0f, -m_height / 2.0f, m_length / 2.0f);
	m_vertexData->data->positionBuffer[face + 1] = glm::vec3(m_width / 2.0f, -m_height / 2.0f, -m_length / 2.0f);
	m_vertexData->data->positionBuffer[face + 2] = glm::vec3(m_width / 2.0f, m_height / 2.0f, -m_length / 2.0f);
	m_vertexData->data->positionBuffer[face + 3] = glm::vec3(m_width / 2.0f, m_height / 2.0f, m_length / 2.0f);

	m_vertexData->data->indexBuffer[index + 0] = face + 0;
	m_vertexData->data->indexBuffer[index + 1] = face + 1;
	m_vertexData->data->indexBuffer[index + 2] = face + 2;
	m_vertexData->data->indexBuffer[index + 3] = face + 0;
	m_vertexData->data->indexBuffer[index + 4] = face + 2;
	m_vertexData->data->indexBuffer[index + 5] = face + 3;

	m_vertexData->data->uvBuffer[face + 0] = glm::vec2(0.0f, 0.0f);
	m_vertexData->data->uvBuffer[face + 1] = glm::vec2(1.0f, 0.0f);
	m_vertexData->data->uvBuffer[face + 2] = glm::vec2(0.0f, 1.0f);
	m_vertexData->data->uvBuffer[face + 3] = glm::vec2(1.0f, 1.0f);

	m_vertexData->data->normalBuffer[face + 0] = glm::vec3(1.0f, 0.0f, 0.0f);
	m_vertexData->data->normalBuffer[face + 1] = glm::vec3(1.0f, 0.0f, 0.0f);
	m_vertexData->data->normalBuffer[face + 2] = glm::vec3(1.0f, 0.0f, 0.0f);
	m_vertexData->data->normalBuffer[face + 3] = glm::vec3(1.0f, 0.0f, 0.0f);

	m_vertexData->data->colorBuffer[face + 0] = m_color;
	m_vertexData->data->colorBuffer[face + 1] = m_color;
	m_vertexData->data->colorBuffer[face + 2] = m_color;
	m_vertexData->data->colorBuffer[face + 3] = m_color;

	// down face
	face += 4;
	index += 6;
	m_vertexData->data->positionBuffer[face + 0] = glm::vec3(-m_width / 2.0f, -m_height / 2.0f, -m_length / 2.0f);
	m_vertexData->data->positionBuffer[face + 1] = glm::vec3(m_width / 2.0f, -m_height / 2.0f, -m_length / 2.0f);
	m_vertexData->data->positionBuffer[face + 2] = glm::vec3(m_width / 2.0f, -m_height / 2.0f, m_length / 2.0f);
	m_vertexData->data->positionBuffer[face + 3] = glm::vec3(-m_width / 2.0f, -m_height / 2.0f, m_length / 2.0f);

	m_vertexData->data->indexBuffer[index + 0] = face + 0;
	m_vertexData->data->indexBuffer[index + 1] = face + 1;
	m_vertexData->data->indexBuffer[index + 2] = face + 2;
	m_vertexData->data->indexBuffer[index + 3] = face + 0;
	m_vertexData->data->indexBuffer[index + 4] = face + 2;
	m_vertexData->data->indexBuffer[index + 5] = face + 3;

	m_vertexData->data->uvBuffer[face + 0] = glm::vec2(0.0f, 0.0f);
	m_vertexData->data->uvBuffer[face + 1] = glm::vec2(1.0f, 0.0f);
	m_vertexData->data->uvBuffer[face + 2] = glm::vec2(0.0f, 1.0f);
	m_vertexData->data->uvBuffer[face + 3] = glm::vec2(1.0f, 1.0f);

	m_vertexData->data->normalBuffer[face + 0] = glm::vec3(0.0f, -1.0f, 0.0f);
	m_vertexData->data->normalBuffer[face + 1] = glm::vec3(0.0f, -1.0f, 0.0f);
	m_vertexData->data->normalBuffer[face + 2] = glm::vec3(0.0f, -1.0f, 0.0f);
	m_vertexData->data->normalBuffer[face + 3] = glm::vec3(0.0f, -1.0f, 0.0f);

	m_vertexData->data->colorBuffer[face + 0] = m_color;
	m_vertexData->data->colorBuffer[face + 1] = m_color;
	m_vertexData->data->colorBuffer[face + 2] = m_color;
	m_vertexData->data->colorBuffer[face + 3] = m_color;

	// up face
	face += 4;
	index += 6;
	m_vertexData->data->positionBuffer[face + 0] = glm::vec3(-m_width / 2.0f, m_height / 2.0f, -m_length / 2.0f);
	m_vertexData->data->positionBuffer[face + 1] = glm::vec3(m_width / 2.0f, m_height / 2.0f, -m_length / 2.0f);
	m_vertexData->data->positionBuffer[face + 2] = glm::vec3(m_width / 2.0f, m_height / 2.0f, m_length / 2.0f);
	m_vertexData->data->positionBuffer[face + 3] = glm::vec3(-m_width / 2.0f, m_height / 2.0f, m_length / 2.0f);

	m_vertexData->data->indexBuffer[index + 0] = face + 2;
	m_vertexData->data->indexBuffer[index + 1] = face + 1;
	m_vertexData->data->indexBuffer[index + 2] = face + 0;
	m_vertexData->data->indexBuffer[index + 3] = face + 3;
	m_vertexData->data->indexBuffer[index + 4] = face + 2;
	m_vertexData->data->indexBuffer[index + 5] = face + 0;

	m_vertexData->data->uvBuffer[face + 0] = glm::vec2(0.0f, 0.0f);
	m_vertexData->data->uvBuffer[face + 1] = glm::vec2(1.0f, 0.0f);
	m_vertexData->data->uvBuffer[face + 2] = glm::vec2(0.0f, 1.0f);
	m_vertexData->data->uvBuffer[face + 3] = glm::vec2(1.0f, 1.0f);

	m_vertexData->data->normalBuffer[face + 0] = glm::vec3(0.0f, 1.0f, 0.0f);
	m_vertexData->data->normalBuffer[face + 1] = glm::vec3(0.0f, 1.0f, 0.0f);
	m_vertexData->data->normalBuffer[face + 2] = glm::vec3(0.0f, 1.0f, 0.0f);
	m_vertexData->data->normalBuffer[face + 3] = glm::vec3(0.0f, 1.0f, 0.0f);

	m_vertexData->data->colorBuffer[face + 0] = m_color;
	m_vertexData->data->colorBuffer[face + 1] = m_color;
	m_vertexData->data->colorBuffer[face + 2] = m_color;
	m_vertexData->data->colorBuffer[face + 3] = m_color;
}