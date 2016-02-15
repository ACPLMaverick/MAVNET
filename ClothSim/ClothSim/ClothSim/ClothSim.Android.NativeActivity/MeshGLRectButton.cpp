#include "MeshGLRectButton.h"


MeshGLRectButton::MeshGLRectButton(SimObject* obj, GUIElement* guiEl, glm::vec4* col) : MeshGLRect(obj, col)
{
	m_guiEl = guiEl;
}

MeshGLRectButton::MeshGLRectButton(const MeshGLRectButton * c) : MeshGLRect(c)
{
	m_guiEl = c->m_guiEl;
}


MeshGLRectButton::~MeshGLRectButton()
{
}



unsigned int MeshGLRectButton::Initialize()
{
	unsigned int err = MeshGLRect::Initialize();
	if (err != CS_ERR_NONE)
		return err;

	std::string font = "Font";
	m_fontShaderID = ResourceManager::GetInstance()->GetShader(&font);

	return err;
}

unsigned int MeshGLRectButton::Draw()
{
	//if (Renderer::GetInstance()->GetDrawMode() == BASIC)
	ShaderID* ids = Renderer::GetInstance()->GetCurrentShaderID();
	if(ids != m_fontShaderID)
		Renderer::GetInstance()->SetCurrentShader(m_fontShaderID);

	glm::mat4 wvp;
	glm::mat4 rot;
	
	if(m_guiEl != nullptr)
	{
		wvp = *m_guiEl->GetTransformMatrix();
		rot = *m_guiEl->GetRotationMatrix();
	}

	glUniformMatrix4fv(m_fontShaderID->id_worldViewProj, 1, GL_FALSE, &(wvp)[0][0]);
	glUniformMatrix4fv(m_fontShaderID->id_world, 1, GL_FALSE, &(rot)[0][0]);

	// here we will set up texture?
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

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	if (ids != m_fontShaderID)
		Renderer::GetInstance()->SetCurrentShader(ids);

	return CS_ERR_NONE;
}
