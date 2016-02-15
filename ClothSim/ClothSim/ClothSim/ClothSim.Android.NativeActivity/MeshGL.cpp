#include "MeshGL.h"


MeshGL::MeshGL(SimObject* obj) : Mesh(obj)
{
	m_vertexData = nullptr;
}

MeshGL::MeshGL(const MeshGL* m) : Mesh(m)
{
}


MeshGL::~MeshGL()
{
}

unsigned int MeshGL::Initialize()
{
	m_vertexData = new VertexData;
	m_vertexData->data = new VertexDataRaw;
	m_vertexData->ids = new VertexDataID;

	// generate vertex data and vertex array
	GenerateVertexData();
	GenerateBarycentricCoords();

	// setting up buffers

	glGenVertexArrays(1, &m_vertexData->ids->vertexArrayID);
	glBindVertexArray(m_vertexData->ids->vertexArrayID);

	glGenBuffers(1, &m_vertexData->ids->vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexData->ids->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(m_vertexData->data->positionBuffer[0]) * m_vertexData->data->vertexCount, 
		m_vertexData->data->positionBuffer, GL_STATIC_DRAW);

	glGenBuffers(1, &m_vertexData->ids->uvBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexData->ids->uvBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(m_vertexData->data->uvBuffer[0]) * m_vertexData->data->vertexCount,
		m_vertexData->data->uvBuffer, GL_STATIC_DRAW);

	glGenBuffers(1, &m_vertexData->ids->normalBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexData->ids->normalBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(m_vertexData->data->normalBuffer[0]) * m_vertexData->data->vertexCount,
		m_vertexData->data->normalBuffer, GL_STATIC_DRAW);

	glGenBuffers(1, &m_vertexData->ids->colorBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexData->ids->colorBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(m_vertexData->data->colorBuffer[0]) * m_vertexData->data->vertexCount,
		m_vertexData->data->colorBuffer, GL_STATIC_DRAW);

	glGenBuffers(1, &m_vertexData->ids->barycentricBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexData->ids->barycentricBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(m_vertexData->data->barycentricBuffer[0]) * m_vertexData->data->vertexCount,
		m_vertexData->data->barycentricBuffer, GL_STATIC_DRAW);

	glGenBuffers(1, &m_vertexData->ids->indexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vertexData->ids->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(m_vertexData->data->indexBuffer[0]) * m_vertexData->data->indexCount,
		m_vertexData->data->indexBuffer, GL_STATIC_DRAW);

	return CS_ERR_NONE;
}

unsigned int MeshGL::Shutdown()
{
	// cleaning up for openGL

	if (m_vertexData != nullptr)
	{
		glDeleteVertexArrays(1, &m_vertexData->ids->vertexArrayID);
		glDeleteBuffers(1, &m_vertexData->ids->vertexBuffer);
		glDeleteBuffers(1, &m_vertexData->ids->uvBuffer);
		glDeleteBuffers(1, &m_vertexData->ids->normalBuffer);
		glDeleteBuffers(1, &m_vertexData->ids->colorBuffer);
		glDeleteBuffers(1, &m_vertexData->ids->barycentricBuffer);
		glDeleteBuffers(1, &m_vertexData->ids->indexBuffer);
		delete m_vertexData;
	}
		

	return CS_ERR_NONE;
}

unsigned int MeshGL::Draw()
{
	glm::mat4 wvp = (*System::GetInstance()->GetCurrentScene()->GetCamera()->GetViewProjMatrix()) * 
		(* m_obj->GetTransform()->GetWorldMatrix());

	ShaderID* ids = Renderer::GetInstance()->GetCurrentShaderID();

	glUniformMatrix4fv(ids->id_worldViewProj, 1, GL_FALSE, &wvp[0][0]);
	glUniformMatrix4fv(ids->id_world, 1, GL_FALSE, &(*m_obj->GetTransform()->GetWorldMatrix())[0][0]);
	glUniformMatrix4fv(ids->id_worldInvTrans, 1, GL_FALSE, &(*m_obj->GetTransform()->GetWorldInverseTransposeMatrix())[0][0]);

	glm::vec3* tempEye = System::GetInstance()->GetCurrentScene()->GetCamera()->GetPosition();
	glUniform4f(ids->id_eyeVector, tempEye->x, tempEye->y, tempEye->z, 1.0f);

	// here we will set up light from global lighting in the scene
	if (System::GetInstance()->GetCurrentScene()->GetAmbientLight() != nullptr)
	{
		LightAmbient* amb = System::GetInstance()->GetCurrentScene()->GetAmbientLight();
		glm::vec3* diff = amb->GetDiffuseColor();
		glUniform4f(ids->id_lightAmb, diff->x, diff->y, diff->z, 1.0f);
	}

	if (System::GetInstance()->GetCurrentScene()->GetLightDirectionalCount() > 0 )
	{
		LightDirectional* firstDiff = System::GetInstance()->GetCurrentScene()->GetLightDirectional(0);
		if (firstDiff != nullptr)
		{
			glm::vec3 *d, *s;
			d = firstDiff->GetDiffuseColor();
			s = firstDiff->GetSpecularColor();
			glm::vec3* dir = firstDiff->GetDirection();
			glUniform4f(ids->id_lightDiff, d->x, d->y, d->z, 1.0f);
			glUniform4f(ids->id_lightSpec, s->x, s->y, s->z, 1.0f);
			glUniform4f(ids->id_lightDir, dir->x, dir->y, dir->z, 1.0f);
		}
		
	}
	// here we will set up highlight?
	// here we will set up glossiness
	glUniform1f(ids->id_gloss, m_gloss);
	glUniform1f(ids->id_specular, m_specular);

	// here we will set up texture?
	glActiveTexture(GL_TEXTURE0);
	if (m_texID != nullptr)
		glBindTexture(GL_TEXTURE_2D, m_texID->id);
	else
		glBindTexture(GL_TEXTURE_2D, NULL);

	//////////////////////////////////////////

	glBindVertexArray(m_vertexData->ids->vertexArrayID);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);
	glEnableVertexAttribArray(4);

	glBindBuffer(GL_ARRAY_BUFFER, m_vertexData->ids->vertexBuffer);
	glVertexAttribPointer(
		0,
		4,
		GL_FLOAT,
		GL_FALSE,
		0,
		(void*)0
		);

	glBindBuffer(GL_ARRAY_BUFFER, m_vertexData->ids->uvBuffer);
	glVertexAttribPointer(
		1,
		2,
		GL_FLOAT,
		GL_FALSE,
		0,
		(void*)0
		);

	glBindBuffer(GL_ARRAY_BUFFER, m_vertexData->ids->normalBuffer);
	glVertexAttribPointer(
		2,
		4,
		GL_FLOAT,
		GL_FALSE,
		0,
		(void*)0
		);

	glBindBuffer(GL_ARRAY_BUFFER, m_vertexData->ids->colorBuffer);
	glVertexAttribPointer(
		3,
		4,
		GL_FLOAT,
		GL_FALSE,
		0,
		(void*)0
		);

	glBindBuffer(GL_ARRAY_BUFFER, m_vertexData->ids->barycentricBuffer);
	glVertexAttribPointer(
		4,
		4,
		GL_FLOAT,
		GL_FALSE,
		0,
		(void*)0
		);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vertexData->ids->indexBuffer);


	glDrawElements(
		GL_TRIANGLES,
		m_vertexData->data->indexCount,
		GL_UNSIGNED_INT,
		(void*)0
		);


	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(3);
	glDisableVertexAttribArray(4);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	return CS_ERR_NONE;
}

unsigned int MeshGL::Update()
{
	return CS_ERR_NONE;
}

void MeshGL::CreateVertexDataBuffers(unsigned int vCount, unsigned int iCount, GLenum target)
{
	m_vertexData->data->vertexCount = vCount;
	m_vertexData->data->indexCount = iCount;
	/*
	bool ifPos, ifInd, ifUv, ifNrm, ifCol, ifBar;
	ifPos = ifInd = ifUv = ifNrm = ifCol = ifBar = false;

	if (m_vertexData->data->positionBuffer != nullptr)
	{
		ifPos = true;
		glDeleteBuffers(1, &m_vertexData->ids->vertexBuffer);
		
		delete[] m_vertexData->data->positionBuffer;
		m_vertexData->data->positionBuffer = nullptr; 
	}
	if (m_vertexData->data->indexBuffer != nullptr)
	{
		ifInd = true;
		glDeleteBuffers(1, &m_vertexData->ids->indexBuffer);

		delete[] m_vertexData->data->indexBuffer;
		m_vertexData->data->indexBuffer = nullptr;
	}
	if (m_vertexData->data->uvBuffer != nullptr)
	{
		ifUv = true;
		glDeleteBuffers(1, &m_vertexData->ids->uvBuffer);
		
		delete[] m_vertexData->data->uvBuffer;
		m_vertexData->data->uvBuffer = nullptr;
	}
	if (m_vertexData->data->normalBuffer != nullptr)
	{
		ifNrm = true;
		glDeleteBuffers(1, &m_vertexData->ids->normalBuffer);
		
		delete[] m_vertexData->data->normalBuffer;
		m_vertexData->data->normalBuffer = nullptr;
	}
	if (m_vertexData->data->colorBuffer != nullptr)
	{
		ifCol = true;
		glDeleteBuffers(1, &m_vertexData->ids->colorBuffer);

		delete[] m_vertexData->data->colorBuffer;
		m_vertexData->data->colorBuffer = nullptr;
	}
	if (m_vertexData->data->barycentricBuffer != nullptr)
	{
		ifBar = true;
		glDeleteBuffers(1, &m_vertexData->ids->barycentricBuffer);

		delete[] m_vertexData->data->barycentricBuffer;
		m_vertexData->data->barycentricBuffer = nullptr;
	}
	*/

	if (m_vertexData->data->positionBuffer != nullptr)
	{
		delete[] m_vertexData->data->positionBuffer;
		m_vertexData->data->positionBuffer = nullptr;
	}
	if (m_vertexData->data->indexBuffer != nullptr)
	{
		delete[] m_vertexData->data->indexBuffer;
		m_vertexData->data->indexBuffer = nullptr;
	}
	if (m_vertexData->data->uvBuffer != nullptr)
	{
		delete[] m_vertexData->data->uvBuffer;
		m_vertexData->data->uvBuffer = nullptr;
	}
	if (m_vertexData->data->normalBuffer != nullptr)
	{
		delete[] m_vertexData->data->normalBuffer;
		m_vertexData->data->normalBuffer = nullptr;
	}
	if (m_vertexData->data->colorBuffer != nullptr)
	{
		delete[] m_vertexData->data->colorBuffer;
		m_vertexData->data->colorBuffer = nullptr;
	}
	if (m_vertexData->data->barycentricBuffer != nullptr)
	{
		delete[] m_vertexData->data->barycentricBuffer;
		m_vertexData->data->barycentricBuffer = nullptr;
	}

	m_vertexData->data->positionBuffer = new glm::vec4[m_vertexData->data->vertexCount];
	m_vertexData->data->indexBuffer = new unsigned int[m_vertexData->data->indexCount];	
	m_vertexData->data->uvBuffer = new glm::vec2[m_vertexData->data->vertexCount];
	m_vertexData->data->normalBuffer = new glm::vec4[m_vertexData->data->vertexCount];
	m_vertexData->data->colorBuffer = new glm::vec4[m_vertexData->data->vertexCount];
	m_vertexData->data->barycentricBuffer = new glm::vec4[m_vertexData->data->vertexCount];
	/*
	if (ifPos)
	{
		glGenBuffers(1, &m_vertexData->ids->vertexBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, m_vertexData->ids->vertexBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(m_vertexData->data->positionBuffer[0]) * m_vertexData->data->vertexCount,
			m_vertexData->data->positionBuffer, target);
	}
	if (ifUv)
	{
		glGenBuffers(1, &m_vertexData->ids->uvBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, m_vertexData->ids->uvBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(m_vertexData->data->uvBuffer[0]) * m_vertexData->data->vertexCount,
			m_vertexData->data->uvBuffer, target);
	}
	if (ifNrm)
	{

		glGenBuffers(1, &m_vertexData->ids->normalBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, m_vertexData->ids->normalBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(m_vertexData->data->normalBuffer[0]) * m_vertexData->data->vertexCount,
			m_vertexData->data->normalBuffer, target);
	}
	if (ifCol)
	{

		glGenBuffers(1, &m_vertexData->ids->colorBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, m_vertexData->ids->colorBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(m_vertexData->data->colorBuffer[0]) * m_vertexData->data->vertexCount,
			m_vertexData->data->colorBuffer, target);
	}
	if (ifInd)
	{
		glGenBuffers(1, &m_vertexData->ids->indexBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vertexData->ids->indexBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(m_vertexData->data->indexBuffer[0]) * m_vertexData->data->indexCount,
			m_vertexData->data->indexBuffer, target);
	}
	if (ifBar)
	{

		glGenBuffers(1, &m_vertexData->ids->barycentricBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, m_vertexData->ids->barycentricBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(m_vertexData->data->barycentricBuffer[0]) * m_vertexData->data->vertexCount,
			m_vertexData->data->barycentricBuffer, target);
	}
	*/
}

void MeshGL::GenerateBarycentricCoords()
{
	glm::vec4 bar0 = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
	glm::vec4 bar1 = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
	glm::vec4 bar2 = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
	glm::vec4 bara[3] = { bar0, bar1, bar2 };

	for (int i = 0; i < m_vertexData->data->vertexCount; ++i)
	{
		m_vertexData->data->barycentricBuffer[i] = bara[i % 3];
	}
}
