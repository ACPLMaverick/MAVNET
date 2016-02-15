#include "GUIText.h"


GUIText::GUIText(const std::string* id, const std::string* text, TextureID* tid) : GUIElement(id)
{
	m_text = *text;
	m_textureIdle = tid;
	m_mesh = nullptr;
}

GUIText::GUIText(const GUIText* c) : GUIElement(c)
{
}

GUIText::~GUIText()
{
}



unsigned int GUIText::Initialize()
{
	unsigned int err;
	m_mesh = new MeshGLText(this, &m_text);
	m_mesh->SetTextureID(m_textureIdle);
	err = m_mesh->Initialize();
	if (err != CS_ERR_NONE)
	{
		delete m_mesh;
		return err;
	}

	return CS_ERR_NONE;
}

unsigned int GUIText::Shutdown()
{
	m_mesh->Shutdown();
	delete m_mesh;
	return CS_ERR_NONE;
}


void GUIText::SetText(const std::string* text)
{
	m_text = *text;
	((MeshGLText*)m_mesh)->SetText(&m_text);
}

void GUIText::SetFontTextureID(TextureID* id)
{
	m_textureIdle = id;
	((MeshGLText*)m_mesh)->SetTextureID(m_textureIdle);
}



std::string* GUIText::GetText()
{
	return &m_text;
}

TextureID* GUIText::GetFontTextureID()
{
	return m_textureIdle;
}