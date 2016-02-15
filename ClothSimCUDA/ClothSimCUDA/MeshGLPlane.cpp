#include "MeshGLPlane.h"

MeshGLPlane::MeshGLPlane(SimObject* obj) : MeshGL(obj)
{
	m_width = 10.0f;
	m_length = 10.0f;
	m_edgesWidth = 9;
	m_edgesLength = 9;
}

MeshGLPlane::MeshGLPlane(SimObject* obj, float width, float length) : MeshGL(obj)
{
	m_width = width;
	m_length = length;
	m_edgesWidth = glm::max<int>((int)width - 1, 0);
	m_edgesLength = glm::max<int>((int)length - 1, 0);
}

MeshGLPlane::MeshGLPlane(SimObject* obj, float width, float length, unsigned int edWidth, unsigned int edLength) : MeshGL(obj)
{
	m_width = width;
	m_length = length;
	m_edgesWidth = edWidth;
	m_edgesLength = edLength;
}


MeshGLPlane::~MeshGLPlane()
{
}



void MeshGLPlane::GenerateVertexData()
{
	unsigned int vertCount = (m_edgesWidth + 2) * (m_edgesLength + 2);
	unsigned int faceCount = (m_edgesWidth + 1) * (m_edgesLength + 1);
	CreateVertexDataBuffers(vertCount, faceCount * 6, GL_DYNAMIC_DRAW);

	float additionW = m_width / (float)(m_edgesWidth + 1);
	float additionL = m_length / (float)(m_edgesLength + 1);

	int iVert = 0;
	int iInd = 0;
	for (float w = -m_width / 2.0f; w <= m_width / 2.0f + 0.001f; w += additionW)
	{
		for (float l = -m_length / 2.0f; l <= m_length / 2.0f + 0.001f; l += additionL, ++iVert)
		{
			m_vertexData->data->positionBuffer[iVert] = glm::vec3(w, 0.0f, l);
			m_vertexData->data->uvBuffer[iVert] = glm::vec2((w + (m_width / 2.0f)) / m_width, (l + (m_length / 2.0f)) / m_length);
			m_vertexData->data->normalBuffer[iVert] = glm::vec3(0.0f, 1.0f, 0.0f);
			m_vertexData->data->colorBuffer[iVert] = glm::vec4(1.0f, 0.5f, 0.7f, 1.0f);
		}
	}

	int cond = (m_edgesLength + 2) * (m_edgesWidth + 1);
	for (int i = 0; i < cond ; ++i)
	{
		if ((i + 1) % (m_edgesLength + 2) == 0)
		{
			continue;
		}

		m_vertexData->data->indexBuffer[iInd] = i;
		m_vertexData->data->indexBuffer[iInd + 1] = i + 1;
		m_vertexData->data->indexBuffer[iInd + 2] = i + m_edgesLength + 3;
		m_vertexData->data->indexBuffer[iInd + 3] = i + m_edgesLength + 3;
		m_vertexData->data->indexBuffer[iInd + 4] = i + m_edgesLength + 2;
		m_vertexData->data->indexBuffer[iInd + 5] = i;

		iInd += 6;
	}
}



unsigned int MeshGLPlane::Initialize()
{
	m_vertexData = new VertexData;
	m_vertexData->data = new VertexDataRaw;
	m_vertexData->ids = new VertexDataID;

	// generate vertex data and vertex array

	GenerateVertexData();

	// setting up buffers
	glGenVertexArrays(1, &m_vertexData->ids->vertexArrayID);
	glBindVertexArray(m_vertexData->ids->vertexArrayID);

	glGenBuffers(1, &m_vertexData->ids->vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexData->ids->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(m_vertexData->data->positionBuffer[0]) * m_vertexData->data->vertexCount,
		m_vertexData->data->positionBuffer, GL_DYNAMIC_DRAW);

	glGenBuffers(1, &m_vertexData->ids->uvBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexData->ids->uvBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(m_vertexData->data->uvBuffer[0]) * m_vertexData->data->vertexCount,
		m_vertexData->data->uvBuffer, GL_STATIC_DRAW);

	glGenBuffers(1, &m_vertexData->ids->normalBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexData->ids->normalBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(m_vertexData->data->normalBuffer[0]) * m_vertexData->data->vertexCount,
		m_vertexData->data->normalBuffer, GL_DYNAMIC_DRAW);

	glGenBuffers(1, &m_vertexData->ids->colorBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexData->ids->colorBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(m_vertexData->data->colorBuffer[0]) * m_vertexData->data->vertexCount,
		m_vertexData->data->colorBuffer, GL_DYNAMIC_DRAW);

	glGenBuffers(1, &m_vertexData->ids->indexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vertexData->ids->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(m_vertexData->data->indexBuffer[0]) * m_vertexData->data->indexCount,
		m_vertexData->data->indexBuffer, GL_STATIC_DRAW);


	return CS_ERR_NONE;
}



unsigned int MeshGLPlane::Update()
{
	unsigned int err = MeshGL::Update();
	if (err != CS_ERR_NONE)
		return err;

	glBindBuffer(GL_ARRAY_BUFFER, m_vertexData->ids->vertexBuffer);
	glBufferSubData(GL_ARRAY_BUFFER, 0,
		sizeof(m_vertexData->data->positionBuffer[0]) * m_vertexData->data->vertexCount, m_vertexData->data->positionBuffer);

	glBindBuffer(GL_ARRAY_BUFFER, m_vertexData->ids->normalBuffer);
	glBufferSubData(GL_ARRAY_BUFFER, 0,
		sizeof(m_vertexData->data->normalBuffer[0]) * m_vertexData->data->vertexCount, m_vertexData->data->normalBuffer);

	glBindBuffer(GL_ARRAY_BUFFER, m_vertexData->ids->colorBuffer);
	glBufferSubData(GL_ARRAY_BUFFER, 0,
		sizeof(m_vertexData->data->colorBuffer[0]) * (m_vertexData->data->vertexCount), m_vertexData->data->colorBuffer);

	return CS_ERR_NONE;
}



VertexData* MeshGLPlane::GetVertexDataPtr()
{
	return m_vertexData;
}

unsigned int MeshGLPlane::GetEdgesWidth()
{
	return m_edgesWidth;
}

unsigned int MeshGLPlane::GetEdgesLength()
{
	return m_edgesLength;
}