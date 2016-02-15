#include "MeshGLSphere.h"

MeshGLSphere::MeshGLSphere(SimObject* obj) : MeshGL(obj)
{
	m_radius = 1.0f;
	m_rings = 16;
	m_sectors = 16;
	m_color = (glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
}

MeshGLSphere::MeshGLSphere(SimObject* obj, float radius, unsigned int rings, unsigned int sectors) : MeshGL(obj)
{
	m_radius = radius;
	m_rings = rings;
	m_sectors = sectors;
	m_color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
}

MeshGLSphere::MeshGLSphere(SimObject* obj, float radius, unsigned int rings, unsigned int sectors, glm::vec4* color) : MeshGLSphere(obj, radius, rings, sectors)
{
	m_color = *color;
}

MeshGLSphere::~MeshGLSphere()
{
}



void MeshGLSphere::GenerateVertexData()
{
	unsigned int vertCount = m_rings * m_sectors;
	unsigned int indexCount = m_rings * m_sectors * 6;
	CreateVertexDataBuffers(vertCount, indexCount, GL_STATIC_DRAW);

	float const R = 1.0f / (float)(m_rings - 1);
	float const S = 1.0f / (float)(m_sectors - 1);
	
	int i = 0;
	for (int r = 0; r < m_rings; ++r)
	{
		for (int s = 0; s < m_sectors; ++s, ++i)
		{
			float const y = sin(-M_PI_2 + M_PI * r * R);
			float const x = cos(2 * M_PI * s * S) * sin(M_PI * r * R);
			float const z = sin(2 * M_PI * s * S) * sin(M_PI * r * R);

			m_vertexData->data->positionBuffer[i] = glm::vec4(x * m_radius, y * m_radius, z * m_radius, 1.0f);
			m_vertexData->data->normalBuffer[i] = glm::vec4(x, y, -z, 0.0f);
			m_vertexData->data->uvBuffer[i] = glm::vec2(s * S, r * R);
			m_vertexData->data->colorBuffer[i] = m_color;
		}
	}

	i = 0;
	for (int i = 0; i < indexCount; ++i)
	{
		m_vertexData->data->indexBuffer[i] = 0;
	}

	for (int r = 0; r < m_rings - 1; ++r)
	{
		for (int s = 0; s < m_sectors - 1; ++s, i+=6)
		{
			m_vertexData->data->indexBuffer[i] = r * m_sectors + s;
			m_vertexData->data->indexBuffer[i + 1] = r * m_sectors + s + 1;
			m_vertexData->data->indexBuffer[i + 2] = (r + 1) * m_sectors + s + 1;

			m_vertexData->data->indexBuffer[i + 3] = r * m_sectors + s;
			m_vertexData->data->indexBuffer[i + 4] = (r + 1) * m_sectors + s + 1;
			m_vertexData->data->indexBuffer[i + 5] = (r + 1) * m_sectors + s;
		}
	}
	//for (i = 0; i < m_vertexData->data->indexCount; i += 3)
	//{
	//	LOGI("%f %f %f \n", m_vertexData->data->colorBuffer[m_vertexData->data->indexBuffer[i]].x, m_vertexData->data->colorBuffer[m_vertexData->data->indexBuffer[i]].y,
	//		m_vertexData->data->colorBuffer[m_vertexData->data->indexBuffer[i]].z);
	//	LOGI("%f %f %f \n", m_vertexData->data->colorBuffer[m_vertexData->data->indexBuffer[i + 1]].x, m_vertexData->data->colorBuffer[m_vertexData->data->indexBuffer[i + 1]].y,
	//		m_vertexData->data->colorBuffer[m_vertexData->data->indexBuffer[i + 1]].z);
	//	LOGI("%f %f %f \n\n", m_vertexData->data->colorBuffer[m_vertexData->data->indexBuffer[i + 1]].x, m_vertexData->data->colorBuffer[m_vertexData->data->indexBuffer[i + 1]].y,
	//		m_vertexData->data->colorBuffer[m_vertexData->data->indexBuffer[i + 1]].z);
	//}
	//for (i = 0; i < m_vertexData->data->vertexCount; ++i)
	//{
	//	LOGI("%f %f %f \n", m_vertexData->data->colorBuffer[i].x, m_vertexData->data->colorBuffer[i].y,
	//		m_vertexData->data->colorBuffer[i].z);
	//}
	
}