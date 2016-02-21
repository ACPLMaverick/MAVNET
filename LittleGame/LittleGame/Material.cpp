#include "stdafx.h"
#include "Material.h"
#include "Effect.h"
#include "Texture.h"
#include "Mesh.h"
#include "GameObject.h"
#include "Transform.h"
#include "Camera.h"
#include "System.h"
#include "Scene.h"

Material::Material()
{
}


Material::~Material()
{
}

void Material::Initialize(uint32_t manualHash, const std::string * filePath, const std::string * name)
{
	std::string tmp = "NoNameMaterial";
	if (filePath != nullptr)
	{
		tmp = *filePath;

		// LOAD FROM FILE HERE
	}
		
	GameResource::Initialize(&tmp, name);

	if (manualHash != 0 && filePath == nullptr)
		m_hash = manualHash;
}

void Material::Shutdown()
{
	// nothing
}

void Material::Update()
{
}
