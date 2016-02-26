#include "stdafx.h"
#include "MeshSprite.h"


MeshSprite::MeshSprite()
{
}


MeshSprite::~MeshSprite()
{
}

void MeshSprite::InitializeVertexData()
{
	m_fvf = D3DFVF_SPRITE;

	m_vertexData.m_vertexArray.push_back(VertexSprite(
		D3DXVECTOR3(-0.5f, -0.5f, 0.0f), D3DXVECTOR2(1.0f, 1.0f)));
	m_vertexData.m_vertexArray.push_back(VertexSprite(
		D3DXVECTOR3(-0.5f, 0.5f, 0.0f), D3DXVECTOR2(1.0f, 0.0f)));
	m_vertexData.m_vertexArray.push_back(VertexSprite(
		D3DXVECTOR3(0.5f, 0.5f, 0.0f), D3DXVECTOR2(0.0f, 0.0f)));
	m_vertexData.m_vertexArray.push_back(VertexSprite(
		D3DXVECTOR3(0.5f, -0.5f, 0.0f), D3DXVECTOR2(0.0f, 1.0f)));

	m_vertexData.m_indexArray.push_back(0);
	m_vertexData.m_indexArray.push_back(1);
	m_vertexData.m_indexArray.push_back(3);
	m_vertexData.m_indexArray.push_back(3);
	m_vertexData.m_indexArray.push_back(1);
	m_vertexData.m_indexArray.push_back(2);
}