#include "MeshGLText.h"


MeshGLText::MeshGLText(SimObject* obj, const string* text) : MeshGL(obj)
{
	m_text = text;
	m_textLetterCount = text->length();
}

MeshGLText::MeshGLText(GUIText* obj, const string* text) : MeshGL((SimObject*)nullptr)
{
	m_guiText = obj;
	m_text = text;
	m_textLetterCount = text->length();
}

MeshGLText::MeshGLText(const MeshGLText* m) : MeshGL(m)
{
}


MeshGLText::~MeshGLText()
{
	// we assume we do not need to delete the text as it is deleted by arbitrary class
}

unsigned int MeshGLText::Initialize()
{
	m_vertexData = new VertexData;
	m_vertexData->data = new VertexDataRaw;
	m_vertexData->ids = new VertexDataID;

	// generate vertex data and vertex array
	

	// setting up buffers

	glGenVertexArrays(1, &m_vertexData->ids->vertexArrayID);
	glBindVertexArray(m_vertexData->ids->vertexArrayID);

	glGenBuffers(1, &m_vertexData->ids->vertexBuffer);
	glGenBuffers(1, &m_vertexData->ids->uvBuffer);
	glGenBuffers(1, &m_vertexData->ids->normalBuffer);
	glGenBuffers(1, &m_vertexData->ids->colorBuffer);
	glGenBuffers(1, &m_vertexData->ids->indexBuffer);
	GenerateVertexData();

	// assign the font shader from resourceManager.
	std::string font = "Font";
	m_fontShaderID = ResourceManager::GetInstance()->GetShader(&font);

	BindVertexData();
	UpdateVertexDataUV();

	return CS_ERR_NONE;
}

unsigned int MeshGLText::Draw()
{
	//if (Renderer::GetInstance()->GetDrawMode() == BASIC)
	ShaderID* ids = Renderer::GetInstance()->GetCurrentShaderID();
	Renderer::GetInstance()->SetCurrentShader(m_fontShaderID);

	glm::mat4* wvp;
	if (m_guiText == nullptr)
	{
		wvp = &((*System::GetInstance()->GetCurrentScene()->GetCamera()->GetViewProjMatrix()) *
			(*m_obj->GetTransform()->GetWorldMatrix()));
	}
	else
	{
		wvp = m_guiText->GetTransformMatrix();
	}

	glUniformMatrix4fv(m_fontShaderID->id_worldViewProj, 1, GL_FALSE, &(*wvp)[0][0]);

	// here we will set up texture?
	if (m_texID != nullptr)
		glBindTexture(GL_TEXTURE_2D, m_texID->id);
	else
		glBindTexture(GL_TEXTURE_2D, NULL);

	//////////////////////////////////////////

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);

	glBindBuffer(GL_ARRAY_BUFFER, m_vertexData->ids->vertexBuffer);
	glVertexAttribPointer(
		0,
		3,
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
		3,
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

	Renderer::GetInstance()->SetCurrentShader(ids);

	return CS_ERR_NONE;
}


void MeshGLText::GenerateVertexData()
{
	// for changing number of letters in runtime, we need to update buffers (their lengths) accordingly.
	CreateVertexDataBuffers(m_textLetterCount * 4, m_textLetterCount * 6, GL_DYNAMIC_DRAW);

	float j = 0.0f;
	for (int i = 0; i < m_textLetterCount; ++i, j += ((float)FIELD_WIDTH * SIZE_MULTIPLIER))
	{
		m_vertexData->data->positionBuffer[4 * i] = glm::vec3(0.0f + j, 0.0f, 0.0f);
		m_vertexData->data->positionBuffer[4 * i + 1] = glm::vec3((float)FIELD_WIDTH * SIZE_MULTIPLIER + j, 0.0f, 0.0f);
		m_vertexData->data->positionBuffer[4 * i + 2] = glm::vec3(0.0f + j, (float)FIELD_HEIGHT * SIZE_MULTIPLIER * Y_COMPENSATION, 0.0f);
		m_vertexData->data->positionBuffer[4 * i + 3] = glm::vec3((float)FIELD_WIDTH * SIZE_MULTIPLIER + j, (float)FIELD_HEIGHT * SIZE_MULTIPLIER * Y_COMPENSATION, 0.0f);

		m_vertexData->data->indexBuffer[6 * i] = 4 * i;
		m_vertexData->data->indexBuffer[6 * i + 1] = 4 * i + 1;
		m_vertexData->data->indexBuffer[6 * i + 2] = 4 * i + 2;
		m_vertexData->data->indexBuffer[6 * i + 3] = 4 * i + 2;
		m_vertexData->data->indexBuffer[6 * i + 4] = 4 * i + 1;
		m_vertexData->data->indexBuffer[6 * i + 5] = 4 * i + 3;

		m_vertexData->data->uvBuffer[4 * i] = glm::vec2(0.0f, 0.0f);
		m_vertexData->data->uvBuffer[4 * i + 1] = glm::vec2(1.0f, 0.0f);
		m_vertexData->data->uvBuffer[4 * i + 2] = glm::vec2(0.0f, 1.0f);
		m_vertexData->data->uvBuffer[4 * i + 3] = glm::vec2(1.0f, 1.0f);

		m_vertexData->data->normalBuffer[4 * i] = glm::vec3(0.0f, 0.0f, -1.0f);
		m_vertexData->data->normalBuffer[4 * i + 1] = glm::vec3(0.0f, 0.0f, -1.0f);
		m_vertexData->data->normalBuffer[4 * i + 2] = glm::vec3(0.0f, 0.0f, -1.0f);
		m_vertexData->data->normalBuffer[4 * i + 3] = glm::vec3(0.0f, 0.0f, -1.0f);

		m_vertexData->data->colorBuffer[4 * i] = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
		m_vertexData->data->colorBuffer[4 * i + 1] = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
		m_vertexData->data->colorBuffer[4 * i + 2] = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
		m_vertexData->data->colorBuffer[4 * i + 3] = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	}
}

void MeshGLText::BindVertexData()
{
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexData->ids->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(m_vertexData->data->positionBuffer[0]) * m_vertexData->data->vertexCount,
		m_vertexData->data->positionBuffer, GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, m_vertexData->ids->uvBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(m_vertexData->data->uvBuffer[0]) * m_vertexData->data->vertexCount,
		m_vertexData->data->uvBuffer, GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, m_vertexData->ids->normalBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(m_vertexData->data->normalBuffer[0]) * m_vertexData->data->vertexCount,
		m_vertexData->data->normalBuffer, GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, m_vertexData->ids->colorBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(m_vertexData->data->colorBuffer[0]) * m_vertexData->data->vertexCount,
		m_vertexData->data->colorBuffer, GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vertexData->ids->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(m_vertexData->data->indexBuffer[0]) * m_vertexData->data->indexCount,
		m_vertexData->data->indexBuffer, GL_DYNAMIC_DRAW);
}

void MeshGLText::UpdateVertexDataUV()
{
	int ctr = 0;
	char temp = m_text->at(ctr);
	float uvX, uvY;
	float one = 1.0f / 16.0f;
	float oneX = one * SPACE_BETWEEN_LETTERS;
	
	//glBindBuffer(GL_ARRAY_BUFFER, m_vertexData->ids->uvBuffer);
	//float* mapped = (float*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
	//glBindBuffer(GL_ARRAY_BUFFER, m_vertexData->ids->uvBuffer);
	while (ctr < m_textLetterCount && temp != '\0')
	{
		if (temp < START_CHAR)
			temp += 255;

		temp -= START_CHAR;
		uvX = (temp % 16) / 16.0f + 0.002f;
		uvY = ((255-temp) / 16) / 16.0f;

		m_vertexData->data->uvBuffer[4 * ctr] = glm::vec2(uvX, uvY);
		m_vertexData->data->uvBuffer[4 * ctr + 1] = glm::vec2(uvX + oneX, uvY);
		m_vertexData->data->uvBuffer[4 * ctr + 2] = glm::vec2(uvX, uvY + one);
		m_vertexData->data->uvBuffer[4 * ctr + 3] = glm::vec2(uvX + oneX, uvY + one);

		//write to mapped buffer
		//mapped[ctr] = m_vertexData->data->uvBuffer[4 * ctr].y;
		//mapped[4 * ctr + 1] = m_vertexData->data->uvBuffer[4 * ctr + 1];
		//mapped[4 * ctr + 2] = m_vertexData->data->uvBuffer[4 * ctr + 2];
		//mapped[4 * ctr + 3] = m_vertexData->data->uvBuffer[4 * ctr + 3];

		// load next char
		++ctr;
		if (ctr < m_textLetterCount)
			temp = m_text->at(ctr);
	}
	//glUnmapBuffer(GL_ARRAY_BUFFER);

	glBindBuffer(GL_ARRAY_BUFFER, m_vertexData->ids->uvBuffer);
	glBufferSubData(GL_ARRAY_BUFFER, 0,
		sizeof(m_vertexData->data->uvBuffer[0]) * m_vertexData->data->vertexCount, m_vertexData->data->uvBuffer);

	//glBufferData(GL_ARRAY_BUFFER, sizeof(m_vertexData->data->uvBuffer[0]) * m_vertexData->data->vertexCount,
	//	m_vertexData->data->uvBuffer, GL_DYNAMIC_DRAW);
	//const GLubyte* b = glewGetErrorString(glGetError());
}

const string* MeshGLText::GetText()
{
	return m_text;
}

void MeshGLText::SetText(const string* text)
{
	int newLength = text->length();

	m_text = text;
	
	if (m_textLetterCount != newLength)
	{
		m_textLetterCount = newLength;
		GenerateVertexData();
		BindVertexData();
	}
	if (newLength <= m_textLetterCount)
	{
		m_textLetterCount = newLength;
	}
	UpdateVertexDataUV();
}
