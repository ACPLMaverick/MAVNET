#include "MeshGLPlane.h"

MeshGLPlane::MeshGLPlane(SimObject* obj) : MeshGL(obj)
{
	m_width = 10.0f;
	m_length = 10.0f;
	m_edgesWidth = 9;
	m_edgesLength = 9;
	m_color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	m_vertexDataDual[0] = nullptr;
	m_vertexDataDual[1] = nullptr;
}

MeshGLPlane::MeshGLPlane(SimObject* obj, float width, float length) : MeshGL(obj)
{
	m_width = width;
	m_length = length;
	m_edgesWidth = glm::max<int>((int)width - 1, 0);
	m_edgesLength = glm::max<int>((int)length - 1, 0);
	m_color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	m_vertexDataDual[0] = nullptr;
	m_vertexDataDual[1] = nullptr;
}

MeshGLPlane::MeshGLPlane(SimObject* obj, float width, float length, unsigned int edWidth, unsigned int edLength) : MeshGL(obj)
{
	m_width = width;
	m_length = length;
	m_edgesWidth = edWidth;
	m_edgesLength = edLength;
	m_color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	m_vertexDataDual[0] = nullptr;
	m_vertexDataDual[1] = nullptr;
}

MeshGLPlane::MeshGLPlane(SimObject* obj, float width, float length, unsigned int edWidth, unsigned int edLength, glm::vec4* col) : MeshGL(obj)
{
	m_width = width;
	m_length = length;
	m_edgesWidth = edWidth;
	m_edgesLength = edLength;
	m_color = *col;
	m_vertexDataDual[0] = nullptr;
	m_vertexDataDual[1] = nullptr;
}


MeshGLPlane::MeshGLPlane(const MeshGLPlane * c) : MeshGL(c)
{
}


MeshGLPlane::~MeshGLPlane()
{
}



void MeshGLPlane::GenerateVertexData()
{
	unsigned int vertCount = (m_edgesWidth + 2) * (m_edgesLength + 2);
	unsigned int faceCount = (m_edgesWidth + 1) * (m_edgesLength + 1) * 6;
	CreateVertexDataBuffers(vertCount, faceCount, GL_DYNAMIC_DRAW);

	float additionW = m_width / (float)(m_edgesWidth + 1);
	float additionL = m_length / (float)(m_edgesLength + 1);

	int iVert = 0;
	int iInd = 0;
	for (float w = -m_width / 2.0f; w <= m_width / 2.0f + 0.001f; w += additionW)
	{
		for (float l = -m_length / 2.0f; l <= m_length / 2.0f + 0.001f; l += additionL, ++iVert)
		{
			m_vertexData->data->positionBuffer[iVert] = glm::vec4(w, 0.0f, l, 1.0f);
			m_vertexData->data->uvBuffer[iVert] = glm::vec2((w + (m_width / 2.0f)) / m_width, (l + (m_length / 2.0f)) / m_length);
			m_vertexData->data->normalBuffer[iVert] = glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);
			m_vertexData->data->colorBuffer[iVert] = m_color;
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

void MeshGLPlane::GenerateBarycentricCoords()
{
	glm::vec4 bar0 = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
	glm::vec4 bar1 = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
	glm::vec4 bar2 = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);

	glm::vec4 bara[3] = { bar0, bar1, bar2 };
	int barInd = -1;
	int barAdd = 0;

	for (int i = 0; i < m_vertexData->data->vertexCount; ++i)
	{
		if (!(i % (m_edgesLength + 2)))
		{
			barInd = (barInd + 1) % 3;
			barAdd = 0;
		}
		else
		{
			++barAdd;
		}

		m_vertexData->data->barycentricBuffer[i] = bara[(barInd + barAdd) % 3];

		//LOGI("%d: %f, %f, %f\n", i, m_vertexData->data->barycentricBuffer[i].x, m_vertexData->data->barycentricBuffer[i].y, m_vertexData->data->barycentricBuffer[i].z);
	}
}



unsigned int MeshGLPlane::Initialize()
{
	m_vertexData = new VertexData;
	m_vertexData->data = new VertexDataRaw;
	m_vertexData->ids = new VertexDataID;

	m_currentArray = 0;

	m_vertexDataDual[0] = m_vertexData;
	m_vertexDataDual[1] = new VertexData;
	m_vertexDataDual[1]->ids = new VertexDataID;
	m_vertexDataDual[1]->data = new VertexDataRaw;

	// generate vertex data and vertex array

	GenerateVertexData();
	GenerateBarycentricCoords();

	// make a copy
	m_vertexData = m_vertexDataDual[1];
	GenerateVertexData();
	GenerateBarycentricCoords();
	m_vertexData = m_vertexDataDual[0];

	// setting up buffers
	glGenVertexArrays(1, &m_vertexDataDual[0]->ids->vertexArrayID);
	glGenVertexArrays(1, &m_vertexDataDual[1]->ids->vertexArrayID);
	for (int i = 0; i < 2; ++i)
	{
		glBindVertexArray(m_vertexDataDual[i]->ids->vertexArrayID);

		glGenBuffers(1, &m_vertexDataDual[i]->ids->vertexBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, m_vertexDataDual[i]->ids->vertexBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(m_vertexDataDual[i]->data->positionBuffer[0]) * m_vertexDataDual[i]->data->vertexCount,
			m_vertexDataDual[i]->data->positionBuffer, GL_DYNAMIC_COPY);

		glGenBuffers(1, &m_vertexDataDual[i]->ids->uvBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, m_vertexDataDual[i]->ids->uvBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(m_vertexDataDual[i]->data->uvBuffer[0]) * m_vertexDataDual[i]->data->vertexCount,
			m_vertexDataDual[i]->data->uvBuffer, GL_STATIC_DRAW);

		glGenBuffers(1, &m_vertexDataDual[i]->ids->normalBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, m_vertexDataDual[i]->ids->normalBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(m_vertexDataDual[i]->data->normalBuffer[0]) * m_vertexDataDual[i]->data->vertexCount,
			m_vertexDataDual[i]->data->normalBuffer, GL_DYNAMIC_COPY);

		glGenBuffers(1, &m_vertexDataDual[i]->ids->colorBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, m_vertexDataDual[i]->ids->colorBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(m_vertexDataDual[i]->data->colorBuffer[0]) * m_vertexDataDual[i]->data->vertexCount,
			m_vertexDataDual[i]->data->colorBuffer, GL_DYNAMIC_COPY);

		glGenBuffers(1, &m_vertexDataDual[i]->ids->barycentricBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, m_vertexDataDual[i]->ids->barycentricBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(m_vertexDataDual[i]->data->barycentricBuffer[0]) * m_vertexDataDual[i]->data->vertexCount,
			m_vertexDataDual[i]->data->barycentricBuffer, GL_STATIC_DRAW);

		glGenBuffers(1, &m_vertexDataDual[i]->ids->indexBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vertexDataDual[i]->ids->indexBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(m_vertexDataDual[i]->data->indexBuffer[0]) * m_vertexDataDual[i]->data->indexCount,
			m_vertexDataDual[i]->data->indexBuffer, GL_STATIC_DRAW);
	}

	glBindVertexArray(m_vertexDataDual[m_currentArray]->ids->vertexArrayID);

	return CS_ERR_NONE;
}

unsigned int MeshGLPlane::Shutdown()
{
	unsigned int err = CS_ERR_NONE;

	m_vertexDataDual[1]->data = nullptr;

	for (unsigned int id = 0; id < 2; ++id)
	{
		glDeleteVertexArrays(1, &m_vertexDataDual[id]->ids->vertexArrayID);
		glDeleteBuffers(1, &m_vertexDataDual[id]->ids->vertexBuffer);
		glDeleteBuffers(1, &m_vertexDataDual[id]->ids->uvBuffer);
		glDeleteBuffers(1, &m_vertexDataDual[id]->ids->normalBuffer);
		glDeleteBuffers(1, &m_vertexDataDual[id]->ids->colorBuffer);
		glDeleteBuffers(1, &m_vertexDataDual[id]->ids->barycentricBuffer);
		glDeleteBuffers(1, &m_vertexDataDual[id]->ids->indexBuffer);

		if (m_vertexDataDual[id] != nullptr)
			delete m_vertexDataDual[id];
	}
	m_vertexData = nullptr;
	err = MeshGL::Shutdown();
	return err;
}



unsigned int MeshGLPlane::Update()
{
	unsigned int err = MeshGL::Update();
	if (err != CS_ERR_NONE)
		return err;
	/*
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexData->ids->vertexBuffer);
	glBufferSubData(GL_ARRAY_BUFFER, 0,
		sizeof(m_vertexData->data->positionBuffer[0]) * m_vertexData->data->vertexCount, m_vertexData->data->positionBuffer);

	glBindBuffer(GL_ARRAY_BUFFER, m_vertexData->ids->normalBuffer);
	glBufferSubData(GL_ARRAY_BUFFER, 0,
		sizeof(m_vertexData->data->normalBuffer[0]) * m_vertexData->data->vertexCount, m_vertexData->data->normalBuffer);

	glBindBuffer(GL_ARRAY_BUFFER, m_vertexData->ids->colorBuffer);
	glBufferSubData(GL_ARRAY_BUFFER, 0,
		sizeof(m_vertexData->data->colorBuffer[0]) * (m_vertexData->data->vertexCount), m_vertexData->data->colorBuffer);
	*/
	return CS_ERR_NONE;
}



unsigned int MeshGLPlane::SwapDataPtrs()
{
	if (m_currentArray == 0)
		m_currentArray = 1;
	else
		m_currentArray = 0;

	m_vertexData = m_vertexDataDual[m_currentArray];

	return m_currentArray;
}

VertexData* MeshGLPlane::GetVertexDataPtr()
{
	return m_vertexData;
}

VertexData ** MeshGLPlane::GetVertexDataDualPtr()
{
	return m_vertexDataDual;
}

unsigned int MeshGLPlane::GetEdgesWidth()
{
	return m_edgesWidth;
}

unsigned int MeshGLPlane::GetEdgesLength()
{
	return m_edgesLength;
}