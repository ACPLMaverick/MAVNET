#include "Mesh.h"


Mesh::Mesh(SimObject* obj) : Component(obj)
{
	m_texID = nullptr;
	m_gloss = 0.0f;
	m_specular = 0.0f;
}

Mesh::Mesh(const Mesh* m) : Component(m)
{
}


Mesh::~Mesh()
{
}



void Mesh::SetTextureID(TextureID* id)
{
	m_texID = id;
}

void Mesh::SetGloss(float gloss)
{
	m_gloss = gloss;
}

void Mesh::SetSpecular(float specular)
{
	m_specular = specular;
}

void Mesh::SetColor(glm::vec4 * col)
{
	m_color = *col;
}

TextureID* Mesh::GetTextureID()
{
	return m_texID;
}

float Mesh::GetGloss()
{
	return m_gloss;
}

float Mesh::GetSpecular()
{
	return m_specular;
}

glm::vec4 * Mesh::GetColor()
{
	return &m_color;
}
