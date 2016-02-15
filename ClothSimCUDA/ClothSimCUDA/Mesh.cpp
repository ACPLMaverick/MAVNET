#include "Mesh.h"


Mesh::Mesh(SimObject* obj) : Component(obj)
{
	m_texID = nullptr;
	m_gloss = FLT_MAX;
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

TextureID* Mesh::GetTextureID()
{
	return m_texID;
}

float Mesh::GetGloss()
{
	return m_gloss;
}